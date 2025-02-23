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

#include "util/span.hpp"
#include "util/types.hpp"
#include "fs/ssharpfs.hpp"

#include <map>

#pragma pack(push, 1)

namespace ssharp::fs::zipfs
{
using namespace ssharp::types;
using namespace ssharpfs;

#pragma once

enum class compression : uint16_t
{
    none = 0,
    deflate = 8
};

struct local_file_header_t
{
    uint32_t signature;
    uint16_t version_needed_to_extract;
    uint16_t general_purpose_bit_flag;
    uint16_t compression_method;
    uint16_t last_mod_file_time;
    uint16_t last_mod_file_date;
    uint32_t crc32;
    uint32_t compressed_size;
    uint32_t uncompressed_size;
    uint16_t file_name_length;
    uint16_t extra_field_length;
    // file_name (variable size)
    // extra_field (variable size)
};
static_assert(sizeof(local_file_header_t) == 30, "local_file_header_t size mismatch");

struct central_directory_file_header_t
{
    uint32_t signature;
    uint16_t version_made_by;
    uint16_t version_needed_to_extract;
    uint16_t general_purpose_bit_flag;
    uint16_t compression_method;
    uint16_t last_mod_file_time;
    uint16_t last_mod_file_date;
    uint32_t crc32;
    uint32_t compressed_size;
    uint32_t uncompressed_size;
    uint16_t file_name_length;
    uint16_t extra_field_length;
    uint16_t file_comment_length;
    uint16_t disk_number_start;
    uint16_t internal_file_attributes;
    uint32_t external_file_attributes;
    uint32_t relative_offset_of_local_header;
    // file_name (variable size)
    // extra_field (variable size)
    // file_comment (variable size)
};
static_assert(sizeof(central_directory_file_header_t) == 46, "central_directory_file_header_t size mismatch");

struct end_of_central_directory_record_t
{
    uint32_t signature;
    uint16_t number_of_this_disk;
    uint16_t number_of_disk_with_start_of_central_directory;
    uint16_t total_number_of_entries_in_central_directory_on_this_disk;
    uint16_t total_number_of_entries_in_central_directory;
    uint32_t size_of_central_directory;
    uint32_t offset_of_start_of_central_directory;
    uint16_t zip_file_comment_length;
    // zip_file_comment (variable size)
};
static_assert(sizeof(end_of_central_directory_record_t) == 22, "end_of_central_directory_record_t size mismatch");

#pragma pack(pop)

ssharpfs_t parse(const span_t& span);
void export_to(const ssharpfs_t& fs, path_t& output_file_path);

} // namespace ssharp::fs::zipfs
