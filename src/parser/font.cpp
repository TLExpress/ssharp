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

#include "font.hpp"

#include "util/exceptions.hpp"

#include <regex>

namespace ssharp::parser::font
{

namespace ssexcept = ssharp::exceptions;

parsed_paths_t find_paths(const buff_t& buff, std::optional<hash_attr_t> hash)
{
    size_t bom_offset = 0;
    if (buff.size() >= 3 && buff[0] == '\xEF' && buff[1] == '\xBB' &&
        buff[2] == '\xBF')
    {
        bom_offset = 3;
    }
    std::istringstream stream{
        std::string(buff.begin() + bom_offset, buff.end())};
    parsed_paths_t set;
    std::string path;
    std::regex re(R"(image:([^,]+))");
    while (std::getline(stream, path))
    {
        while (!path.empty() && path.back() == '\r')
        {
            path.pop_back();
        }
        if (path.empty())
        {
            continue;
        }
        std::smatch match;
        if (std::regex_search(path, match, re))
        {
            path = match[1];
        }
        else
        {
            continue;
        }
        is_absolute_path_t is_absolute_path_t = is_absolute_path_t::relative;
        if (path.front() == '/')
        {
            is_absolute_path_t = is_absolute_path_t::absolute;
            path.erase(0, 1);
        }

        set.insert(
            {path_t(path), is_absolute_path_t, is_directory_t::file, hash});
    }
    return set;
}

} // namespace ssharp::parser::font
