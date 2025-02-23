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

#include "pmd.hpp"

#include "util/exceptions.hpp"

namespace ssharp::parser::pmd
{

namespace ssexcept = ssharp::exceptions;

parsed_paths_t find_paths(const buff_t& buff, std::optional<hash_attr_t> hash)
{
    // Check if the buffer is large enough to contain the header
    if (buff.size() < sizeof(header_t))
    {
        throw ssexcept::parse_error(
            "not a valid pmd, invalid size, expected at least " +
            std::to_string(sizeof(header_t)) + " bytes, got " +
            std::to_string(buff.size()));
    }

    const auto header = reinterpret_cast<const header_t*>(buff.data());
    // Check if the version is correct
    if (header->version != expected_version)
    {
        throw ssexcept::parse_error(
            "not a valid pmd, invalid version, expected 0x04, got " +
            std::to_string(header->version));
    }

    parsed_paths_t set;
    auto data = buff.data();
    const auto size = buff.size();
    uint32_t path_count = header->material_count * header->look_count;
    for (uint32_t c = 0; c < path_count; c++)
    {
        // get the offset of the path
        auto offset = header->material_offset + (c * sizeof(uint32_t));
        auto d_offset = *reinterpret_cast<const uint32_t*>(data + offset);
        if (d_offset >= size)
            throw ssexcept::parse_error("not a valid pmd, out of bounds");

        // get the path
        path_t path;
        for (auto path_char = d_offset;; path_char++)
        {
            if (path_char >= size)
                throw ssexcept::parse_error(
                    "not a valid pmd, unexpected end of file");
            if (data[path_char] == '\0')
                break;
            path += data[path_char];
        }
        is_absolute_path_t is_absolute = is_absolute_path_t::relative;
        if(path.string().front() == '/')
        {
            is_absolute = is_absolute_path_t::absolute;
            path = path.string().substr(1);
        }
        set.emplace(parsed_path_t{path, is_absolute, is_directory_t::file, hash});
    }
    return set;
}

} // namespace ssharp::parser::pmd
