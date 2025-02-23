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

#include "tobj.hpp"

#include "util/exceptions.hpp"

namespace ssharp::parser::tobj
{

namespace ssexcept = ssharp::exceptions;

parsed_paths_t find_paths(const buff_t& buff, std::optional<hash_attr_t> hash)
{
    if (buff.size() < sizeof(header_t) + sizeof(texture_attr_t))
    {
        throw ssexcept::parse_error(
            "not a valid tobj, invalid size, expected at least " +
            std::to_string(sizeof(header_t)) + " bytes, got " +
            std::to_string(buff.size()));
    }
    auto header = reinterpret_cast<const header_t*>(buff.data());
    auto texture_attr =
        reinterpret_cast<const texture_attr_t*>(buff.data() + sizeof(header_t));
    if (header->signature != expected_signature)
    {
        throw ssexcept::parse_error(
            "not a valid tobj, invalid signature, got " +
            std::to_string(header->signature));
    }
    auto path_size = texture_attr->length;
    if (buff.size() < sizeof(header_t) + sizeof(texture_attr_t) + path_size)
    {
        throw ssexcept::parse_error(
            "not a valid tobj, invalid size, expected at least " +
            std::to_string(
                sizeof(header_t) + sizeof(texture_attr_t) + path_size) +
            " bytes, got " + std::to_string(buff.size()));
    }
    auto path = std::string{
        reinterpret_cast<const char*>(
            buff.data() + sizeof(header_t) + sizeof(texture_attr_t)),
        path_size};

    is_absolute_path_t is_absolute = is_absolute_path_t::relative;
    if (path.front() == '/')
    {
        is_absolute = is_absolute_path_t::absolute;
        path = path.substr(1);
    }
    return {{path_t(path), is_absolute, is_directory_t::file, hash}};
}
} // namespace ssharp::parser::tobj
