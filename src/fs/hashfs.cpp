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

namespace ssharp::fs::hashfs_v1
{
using namespace ssharp::types;
namespace ssexcept = ssharp::exceptions;

ssharpfs_t parse(const std::istream& stream)
{
    auto buff = *span;
    if (buff.size() < sizeof(header_t))
    {
        throw ssexcept::invalid_input(
            "not a valid hashfs, invalid size, expected at least " +
            std::to_string(sizeof(header_t)) + " bytes, got " +
            std::to_string(buff.size()));
    }

    const auto header = reinterpret_cast<const header_t*>(buff.data());
    if (header->signature != expected_signature)
    {
        throw ssexcept::invalid_input(
            "not a valid hashfs, invalid signature, expected 0x23534353, got " +
            std::to_string(header->signature));
    }

    if (header->version != expected_version)
    {
        throw ssexcept::invalid_input(
            "not a valid hashfs, invalid version, expected 0x01, got " +
            std::to_string(header->version));
    }

    ssharpfs_t fs;
    auto data = buff.data();
    const auto size = buff.size();
    for (uint32_t c = 0; c < header->entries_count; c++)
    {
        auto offset = header->offset + (c * sizeof(entry_t));
        if (offset >= size)
            throw ssexcept::invalid_input("not a valid hashfs, out of bounds");

        auto entry = reinterpret_cast<const entry_t*>(data + offset);
        if (entry->flags.is_encrypted())
        {
            throw ssexcept::invalid_input("not a valid hashfs, encrypted entry");
        }

        if (entry->flags.need_varify())
        {
            throw ssexcept::invalid_input("not a valid hashfs, entry needs varify");
        }

        if (entry->flags.is_directory())
        {
            throw ssexcept::invalid_input("not a valid hashfs, directory entry");
        }

        if (entry->flags.is_compressed())
        {
            throw ssexcept::invalid_input("not a valid hashfs, compressed entry");
        }

        auto entry_data = span_t{buff_t(data + entry->data.offset, entry->data.size)};
        fs[entry->key] = std::make_shared<generic_entry_t>(entry_data);
    }

    return fs;
}
} // namespace ssharp::fs::hashfs_v1