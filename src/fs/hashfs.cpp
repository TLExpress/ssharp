// Copyright (C) 2025 TLExpress.
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "hashfs.hpp"

#include "util/exceptions.hpp"

namespace ssharp::fs::hashfs
{
using namespace ssharp::types;
namespace ssexcept = ssharp::exceptions;

ssharpfs_t parse(const span_t& span)
{
    if (span.size() < sizeof(header_t))
    {
        throw ssexcept::parse_error(
            "not a valid hashfs, invalid size, expected at least " +
            std::to_string(sizeof(header_t)) + " bytes, got " +
            std::to_string(span.size()));
    }

    auto header_buff = span.get(span_attr_t{0, sizeof(header_t)});
    const auto header = reinterpret_cast<const header_t*>(header_buff.data());

    if (header->signature != expected_signature)
    {
        throw ssexcept::parse_error(
            "not a valid hashfs, invalid signature, expected 0x23534353, got " +
            std::to_string(header->signature));
    }
    if (header->version != expected_version)
    {
        throw ssexcept::parse_error(
            "not a valid hashfs, invalid version, expected 0x01, got " +
            std::to_string(header->version));
    }
    if (header->method != expected_method)
    {
        throw ssexcept::parse_error(
            "not a valid hashfs, invalid method, expected 0x59544943, got " +
            std::to_string(header->method));
    }

    ssharpfs_t fs;
    fs.salt = header->salt;
    auto entries_offset = header->offset;
    for (uint32_t i = 0; i < header->entries_count; i++)
    {
        auto entry_offset = entries_offset + (i * sizeof(entry_t));
        auto entry_buff = span.get(span_attr_t{entry_offset, sizeof(entry_t)});
        const auto entry = reinterpret_cast<const entry_t*>(entry_buff.data());
        std::shared_ptr<ssharpfs::entry_t> entry_ptr;
        if (entry->flags.is_directory())
        {
            entry_ptr = std::make_shared<directory_entry_t>();
        }
        else
        {
            entry_ptr = std::make_shared<generic_entry_t>();
        }
        entry_ptr->is_encrypted = entry->flags.is_encrypted()
                                            ? is_encrypted_t::encrypted
                                            : is_encrypted_t::decrypted;
        entry_ptr->data = span_t{span, {entry->offset, entry->compressed_size}};
        entry_ptr->compress_attr =
            entry->flags.is_compressed()
                ? std::make_optional(compress_attr_t{
                      compress_type_t::zlib, entry->uncompressed_size})
                : std::nullopt;
        fs.insert({hash_attr_t{entry->hash, fs.salt}, entry_ptr});
    }
}
} // namespace ssharp::fs::hashfs_v1
