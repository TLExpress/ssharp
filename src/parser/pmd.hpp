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

#pragma once

#include "util/types.hpp"

namespace ssharp::parser::pmd
{

using namespace ssharp::types;

constexpr uint32_t expected_version = 0x04;

#pragma pack(push, 1)
struct attrib_link_t
{
    int32_t from; // +0x0
    int32_t to;   // +0x4
};

struct attrib_def_t
{
    uint64_t name;  // +0x0
    int32_t type;   // +0x8
    int32_t offset; // +0xC
};

struct attrib_value_t
{
    union
    {
        int int_value;
        float float_value;
    };
};

struct header_t
{
    uint32_t version;              // Position: 0x0

    uint32_t material_count;       // Position: 0x4
    uint32_t look_count;           // Position: 0x8
    uint32_t piece_count;          // Position: 0xC
    uint32_t variant_count;        // Position: 0x10
    uint32_t part_count;           // Position: 0x14
    uint32_t attribs_count;        // Position: 0x18

    uint32_t attribs_values_size;  // Position: 0x1C
    uint32_t material_block_size;  // Position: 0x20

    uint32_t look_offset;          // Position: 0x24
    uint32_t variant_offset;       // Position: 0x28
    uint32_t part_attribs_offset;  // Position: 0x2C
    uint32_t attribs_value_offset; // Position: 0x30
    uint32_t attribs_offset;       // Position: 0x34
    uint32_t material_offset;      // Position: 0x38
    uint32_t material_data_offset; // Position: 0x3C
                                   /*
                                       uint64_t looks[look_count];
                                       uint64_t variants[variant_count];
                                       attrib_link_t attribs_link[part_count];
                                       attrib_def_t attribs_def[attribs_count];
                                       uint8_t attribs_value_block[variant_count * attribs_values_size];
                                       uint32_t materials_offset[look_count * material_count];
                                       uint8_t materials_data[material_block_size];
                                   */
};
#pragma pack(pop)

/**
 * @brief Parse a buffer containing a PMD file
 * @param buff The buffer containing the PMD file
 * @param hash The hash attribute of the file
 * @return A set of paths
 * @throws parse_error if the file is invalid
 */
parsed_paths_t find_paths(const buff_t& buff,
                          std::optional<hash_attr_t> hash = std::nullopt);

} // namespace ssharp::parser::pmd
