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

#include "ssharpfs.hpp"

#include <bitset>
#include <expected>

namespace ssharp::fs::hashfs
{
using namespace ssharp::types;
using namespace ssharpfs;

constexpr uint32_t expected_signature = 0x23534353U;
constexpr uint32_t expected_method = 0x59544943U;
constexpr uint16_t expected_version = 0x01;

#pragma pack(push, 1)

struct flags_t
{
    bool is_directory() const
    {
        return flags & 0b1;
    }
    bool is_compressed() const
    {
        return flags & 0b10;
    }
    bool need_varify() const
    {
        return flags & 0b100;
    }
    bool is_encrypted() const
    {
        return flags & 0b1000;
    }
private:
    uint32_t flags;
};
static_assert(sizeof(flags_t) == 4, "flags_t size mismatch");

struct header_t
{
    uint32_t signature;
    uint16_t version;
    salt_t salt;
    uint32_t method;
    uint32_t entries_count;
    uint64_t offset;
    uint64_t auth_offset;
};
static_assert(sizeof(v1_header_t) == 32, "header_t size mismatch");

struct entry_t
{
    hash_t hash;
    uint64_t offset;
    flags_t flags;
    crc32_t crc32;
    uint32_t uncompressed_size;
    uint32_t compressed_size;
};
static_assert(sizeof(v1_entry_t) == 32, "entry_t size mismatch");

#pragma pack(pop)

ssharpfs_t parse(const span_t& span);
void export_to(const ssharpfs_t& fs, path_t& output_file_path);

} // namespace ssharp::fs::hashfs
