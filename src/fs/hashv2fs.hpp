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

namespace ssharp::fs::hashv2fs
{
using namespace ssharp::types;
using namespace ssharpfs;

constexpr uint32_t expected_signature = 0x23534353U;
constexpr uint32_t expected_method = 0x59544943U;
constexpr uint16_t expected_version = 0x02;

#pragma pack(push, 1)

struct flags_t
{
    bool is_directory() const
    {
        return flags & 0b1;
    }
private:
    uint8_t flags;
};
static_assert(sizeof(flags_t) == 1, "flags_t size mismatch");

enum class platform_t : uint8_t
{
    pc = 0,
    xb = 1,
    xs = 2,
    ps4 = 3,
    ps5 = 4,
};

struct header_t
{
    uint32_t signature;
    uint16_t version;
    salt_t salt;
    uint32_t method;
    uint32_t entries_count;
    uint32_t entries_compressed_size;
    uint32_t metadata_count;
    uint32_t metadata_compressed_size;
    uint64_t entries_offset;
    uint64_t metadata_offset;
    uint64_t auth_offset;
    platform_t platform;
};
static_assert(sizeof(header_t) == 53, "header_t size mismatch");

struct entry_t
{
    hash_t hash;
    uint32_t meta_index;
    uint16_t meta_count;
    uint8_t flags;
    uint8_t placeholder;
};
static_assert(sizeof(entry_t) == 16, "v2_entry_t size mismatch");

struct meta_plain_t
{
    size_t get_compressed_size() const
    {
        return compressed_size & 0x0FFFFFFF;
    }
    compress_type_t get_compress_type() const
    {
        return static_cast<compress_type_t>(compress_type & 0xF);
    }
    size_t get_uncompressed_size() const
    {
        return uncompressed_size & 0x0FFFFFFF;
    }
    size_t get_offset_rs4() const
    {
        return static_cast<size_t>(offset_rs4) << 4;
    }
private:
    uint32_t compressed_size : 28;
    uint32_t compress_type : 4;
    uint32_t uncompressed_size : 28;
    uint32_t flags: 4;
    uint32_t placeholder;
    uint32_t offset_rs4;
};
static_assert(sizeof(meta_plain_t) == 16, "meta_plain_t size mismatch");

struct meta_img_t
{

private:
    uint32_t width : 16;
    uint32_t height : 16;
    uint32_t mipmap_count : 4;
    uint32_t format : 8;
    uint32_t is_cube : 2;
    uint32_t count : 6;
    uint32_t pitch_alighment : 4;
    uint32_t image_alignment : 4;
};
static_assert(sizeof(meta_img_t) == 8, "meta_img_t size mismatch");

struct meta_sample_t
{
    uint32_t mag_filter : 1;
    uint32_t min_filter : 1;
    uint32_t mip_filter : 2;
    uint32_t addr_u : 3;
    uint32_t addr_v : 3;
    uint32_t addr_w : 3;
    uint32_t placeholder : 19;
};
static_assert(sizeof(meta_sample_t) == 4, "meta_sample_t size mismatch");

#pragma pack(pop)

ssharpfs_t parse(const span_t& span);
void export_to(const ssharpfs_t& fs, path_t& output_file_path);

} // namespace ssharp::fs::hashfs
