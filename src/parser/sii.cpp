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

#include "sii.hpp"

#include "util/exceptions.hpp"

#include <regex>

namespace ssharp::parser::sii
{
    
namespace ssexcept = ssharp::exceptions;

buff_t crlf_to_lf(const buff_t& buff)
{
    try
    {
        static const std::regex re{R"(\r\n)"};
        std::string buff_str{buff.begin(), buff.end()};
        std::string result;
        std::regex_replace(std::back_inserter(result), buff_str.begin(), buff_str.end(), re, "\n");
        return buff_t{result.begin(), result.end()};
    }
    catch (const std::exception& e)
    {
        throw ssexcept::parse_error(e.what());
    }
}

buff_t remove_comments(const buff_t& buff)
{
    // commend start with '#' and end with '\n'
    static const std::regex re{R"(^\s*#.*)"};
    std::string buff_str{buff.begin(), buff.end()};
    std::string result;
    std::regex_replace(std::back_inserter(result), buff_str.begin(), buff_str.end(), re, "");
    return buff_t{result.begin(), result.end()};
}

buff_t remove_empty_lines(const buff_t& buff)
{
    static const std::regex re{R"(^\s*$\n?)"};
    std::string buff_str{buff.begin(), buff.end()};
    std::string result;
    std::regex_replace(std::back_inserter(result), buff_str.begin(), buff_str.end(), re, "\n");
    return buff_t{result.begin(), result.end()};
}

buff_t remove_newline_behind_colon(const buff_t& buff)
{
    static const std::regex re{R"(:\s*\n\s*)"};
    std::string buff_str{buff.begin(), buff.end()};
    std::string result;
    std::regex_replace(std::back_inserter(result), buff_str.begin(), buff_str.end(), re, "");
    return buff_t{result.begin(), result.end()};
}

buff_t remove_whitespace(const buff_t& buff)
{
    static const std::regex re{R"([ \t\f\v]+)"};
    std::string buff_str{buff.begin(), buff.end()};
    std::string result;
    std::regex_replace(std::back_inserter(result), buff_str.begin(), buff_str.end(), re, "");
    return buff_t{result.begin(), result.end()};
}

parsed_paths_t find_paths(const buff_t& buff, std::optional<hash_attr_t> hash)
{
    buff_t result = crlf_to_lf(buff);
    result = remove_comments(result);
    result = remove_empty_lines(result);
    result = remove_newline_behind_colon(result);
    result = remove_whitespace(result);
    // handle utf8 bom
    size_t bom_offset = 0;
    if (result.size() >= 3 && result[0] == '\xEF' && result[1] == '\xBB' && result[2] == '\xBF')
    {
        bom_offset = 3;
    }
    std::istringstream stream{std::string(result.begin() + bom_offset, result.end())};
    parsed_paths_t set;
    std::string record;
    while (std::getline(stream, record))
    {
        // record would look like:
        // key:value
        std::string key;
        std::string value;
        size_t colon_pos = record.find(':');
        // should not be in the end of the line
        if (colon_pos != std::string::npos && colon_pos != record.size() - 1)
        {
            key = record.substr(0, colon_pos);
            value = record.substr(colon_pos + 1);
            // only string should be stored
            // others can be ignored
            // string will appears in quotes
            if (value.front() == '\"' && value.back() == '\"')
            {
                value = value.substr(1, value.size() - 2);
            }
            else
            {
                continue;
            }
            // if key is "icon", add prefix "/material/ui/accessory/" to value
            if (key == "icon")
            {
                value = std::string{"/material/ui/accessory/"} + value + ".mat";
            }
            // value would have additional attributes that we dont wanted
            // the pattern would be like:
            // path/to/file.sii|hash
            // we only need the path
            size_t pipe_pos = value.find('|');
            if (pipe_pos != std::string::npos)
            {
                value = value.substr(0, pipe_pos);
            }
            if (value.empty())
            {
                continue;
            }
            // distinguish relative and absolute path with the first character
            is_absolute_path_t is_absolute = is_absolute_path_t::relative;
            if (value.front() == '/')
            {
                is_absolute = is_absolute_path_t::absolute;
                value = value.substr(1);
            }
            set.insert({path_t(value), is_absolute, is_directory_t::file, hash});
        }
        // sii would have include file
        // the pattern is like:
        // @include path/to/file
        else if (record.starts_with("@include") && record.size() > 8)
        {
            value = record.substr(8);
            // remove head quote
            if (value.front() == '\"')
            {
                value = value.substr(1);
            }
            //remove tail quote
            if (value.back() == '\"')
            {
                value.pop_back();
            }
            // distinguish relative and absolute path with the first character
            is_absolute_path_t is_absolute = is_absolute_path_t::relative;
    
            if (value.front() == '/')
            {
                is_absolute = is_absolute_path_t::absolute;
                value = value.substr(1);
            }
            set.insert({path_t(value), is_absolute, is_directory_t::file, hash});
        }
    }
    return set;
}

} // namespace ssharp::parser::sii