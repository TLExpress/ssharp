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

#include "directory.hpp"

#include "util/exceptions.hpp"

namespace ssharp::parser::directory
{

namespace ssexcept = ssharp::exceptions;

parsed_paths_t parse(const buff_t& buff, std::optional<hash_attr_t> hash)
{
    size_t bom_offset = 0;
    if (buff.size() >= 3 && buff[0] == '\xEF' && buff[1] == '\xBB' && buff[2] == '\xBF')
    {
        bom_offset = 3;
    }
    std::istringstream stream{std::string(buff.begin() + bom_offset, buff.end())};
    parsed_paths_t set;
    std::string path;
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
        if (path.find_first_of("<>:\"/\\|?") != std::string::npos)
        {
            throw ssexcept::parse_error(
                "Path contains invalid characters: " + path);
        }
        is_directory_t is_directory = is_directory_t::file;
        if (path.front() == '*')
        {
            is_directory = is_directory_t::directory;
            path.erase(0, 1);
        }

        set.insert({path_t(path), is_absolute_path_t::relative, is_directory, hash});
    }
    return set;
}

} // namespace ssharp::parser::directory
