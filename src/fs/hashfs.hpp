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

namespace ssharp::fs::hashfs_v1
{
using namespace ssharp::types;
using namespace ssharpfs;

constexpr uint32_t expected_signature = 0x23534353U;
constexpr uint32_t expected_method = 0x59544943U;
constexpr uint16_t expected_version = 0x01;

#pragma pack(push, 1)

struct flags_t : std::bitset<32>
{
    using std::bitset<32>::bitset;
    bool is_directory() const
    {
        return test(0);
    }
    bool is_compressed() const
    {
        return test(1);
    }
    bool need_varify() const
    {
        return test(2);
    }
    bool is_encrypted() const
    {
        return test(3);
    }

};

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

struct entry_t
{
    hash_t hash;
    uint64_t offset;
    flags_t flags;
    crc32_t crc32;
    uint32_t uncompressed_size;
    uint32_t compressed_size;
};

#pragma pack(pop)

ssharpfs_t parse(std::istream& stream);

} // namespace ssharp::fs::hashfs
