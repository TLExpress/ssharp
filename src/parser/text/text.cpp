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

#include "util/types.hpp"
#include "text.hpp"

#include <regex>

namespace ssharp::text
{

using namespace ssharp::types;

#ifdef EXCLUDED

float inline ieee754_hex_to_float(const std::string& hexStr)
{
    if (hexStr.size() < 2 || hexStr[0] != '&') {
        throw std::invalid_argument("Invalid IEEE 754 hex format");
    }

    uint32_t intValue;
    std::stringstream ss;
    ss << std::hex << hexStr.substr(1);
    ss >> intValue;

    return std::bit_cast<float>(intValue);
}

std::string inline float_to_ieee754_hex(float value)
{
    uint32_t intValue = std::bit_cast<uint32_t>(value);
    std::stringstream ss;
    ss << "&" << std::hex << intValue;
    return ss.str();
}

single_t inline string_to_single(const std::string& value)
{
    if (value.front() == '&')
    {
        return ieee754_hex_to_float(value);
    }
    return std::stof(value);
}

static constexpr int token_base = 38;
static constexpr auto token_char_table = "0123456789abcdefghijklmnopqrstuvwxyz_";
static constexpr size_t token_max_length = 12;
static constexpr uint64_t token_max_encoded_value = (1ULL << 63) - 1;

std::string encoded_string_t::decode(uint64_t encoded)
{
    std::string result;
    while (encoded > 0) {
        size_t index = (encoded % token_base) - 1;
        result = token_char_table[index] + result;
        encoded /= token_base;
    }
    return result;
}

uint64_t encoded_string_t::encode(const std::string& identifier)
{
    if (identifier.length() > token_max_length)
    {
        throw std::invalid_argument("Identifier exceeds maximum length of 12 characters");
    }
    
    uint64_t result = 0;
    for (char ch : identifier) {
        size_t index = std::string(token_char_table).find(ch);
        if (index == std::string::npos) {
            throw std::invalid_argument("Invalid character in identifier");
        }
        if (result > (token_max_encoded_value / token_base)) {
            throw std::overflow_error("Encoded value exceeds 63-bit limit");
        }
        result = result * token_base + index + 1;
    }
    return result;
}

std::expected<value_t, std::string> parse_value(const std::string& value, std::optional<try_type_t> try_type)
{
    if (try_type.has_value())
    {
        switch (try_type.value().index())
        {
        case 0:
            return parse_value(value, get_number_type(value));
        case 1:
            switch (std::get<1>(try_type.value()))
            {
            case number_type_t::string:
                if(value.front()=='\"' && value.back()=='\"' && value.size() > 1)
                {
                    return value.substr(1, value.size() - 2);
                }
                return std::unexpected("Invalid string");

            case number_type_t::encoded_string:
                try
                {
                    return std::stoull(value, nullptr, 16);
                }
                catch (const std::invalid_argument& e)
                {
                    return std::unexpected("Invalid encoded string");
                }

            case number_type_t::single:
                try
                {
                    return string_to_single(value);
                }
                catch (const std::invalid_argument& e)
                {
                    return std::unexpected("Invalid single");
                }

            case number_type_t::vec2s:
                // format: {1.4,2.5}
                if (value.front() == '{' && value.back() == '}')
                {
                    std::regex re{R"(\{([-+]?[0-9]+(?:\.[0-9]*)?),([-+]?[0-9]+(?:\.[0-9]*)?)\})"};
                    std::smatch match;
                    if (std::regex_match(value, match, re))
                    {
                        return vec2s{string_to_single(match[1]), string_to_single(match[2])};
                    }
                }
                return std::unexpected("Invalid vec2s");
                
            case number_type_t::vec3s:
                // format: {1.4,2.5,3.6}
                if (value.front() == '{' && value.back() == '}')
                {
                    std::regex re{R"(\{([-+]?[0-9]+(?:\.[0-9]*)?),([-+]?[0-9]+(?:\.[0-9]*)?),([-+]?[0-9]+(?:\.[0-9]*)?)\})"};
                    std::smatch match;
                    if (std::regex_match(value, match, re))
                    {
                        return vec3s{std::stof(match[1]), std::stof(match[2]), std::stof(match[3])};
                    }
                }
                return std::unexpected("Invalid vec3s");
            case number_type_t::vec3i:
                // format: {1,2,3}
                if (value.front() == '{' && value.back() == '}')
                {
                    std::regex re{R"(\{([-+]?[0-9]+),([-+]?[0-9]+),([-+]?[0-9]+)\})"};
                    std::smatch match;
                    if (std::regex_match(value, match, re))
                    {
                        return vec3i{std::stoi(match[1]), std::stoi(match[2]), std::stoi(match[3])};
                    }
                }
                return std::unexpected("Invalid vec3i");
            case number_type_t::vec4s:
                // format: {1.4,2.5,3.6,4.7}
                if (value.front() == '{' && value.back() == '}')
                {
                    std::regex re{R"(\{
                        ([-+]?[0-9]+(?:\.[0-9]*)?),
                        ([-+]?[0-9]+(?:\.[0-9]*)?),
                        ([-+]?[0-9]+(?:\.[0-9]*)?),
                        ([-+]?[0-9]+(?:\.[0-9]*)?)
                    \})"};
                    std::smatch match;
                    if (std::regex_match(value, match, re))
                    {
                        return vec4s{std::stof(match[1]), std::stof(match[2]), std::stof(match[3]), std::stof(match[4])};
                    }
                }
                return std::unexpected("Invalid vec4s");
            case number_type_t::vec4s_array:
                return vec4s_array{vec4s{std::stof(value)}};
            case number_type_t::vec7s:
                return vec7s{std::stof(value)};
            case number_type_t::vec7s_array:
                return vec7s_array{vec7s{std::stof(value)}};
            case number_type_t::vec8s:
                return vec8s{std::stof(value)};
            case number_type_t::vec8s_array:
                return vec8s_array{vec8s{std::stof(value)}};
            case number_type_t::int32:
                return std::stoi(value);
            case number_type_t::int32_array:
                return int32_t_array{std::stoi(value)};
            case number_type_t::uint32:
                return std::stoul(value);
            case number_type_t::uint32_array:
                return uint32_t_array{std::stoul(value)};
            case number_type_t::int16:
                return std::stoi(value);
            case number_type_t::int16_array:
                return int16_t_array{std::stoi(value)};
            case number_type_t::uint16:
                return std::stoul(value);
            case number_type_t::uint16_array:
                return uint16_t_array{std::stoul(value)};
            case number_type_t::int64:
                return std::stoll(value);
            case number_type_t::int64_array:
                return int64_t_array{std::stoll(value)};
            case number_type_t::uint64:
                return std::stoull(value);
            case number_type_t::uint64_array:
                return uint64_t_array{std::stoull(value)};
            case number_type_t::boolean:
                return value == "true";
            case number_type_t::boolean_array:
                return bool_array{value == "true"};
            case number_type_t::ordinal_string:
                return ordinal_string{0, value};
            case number_type_t::ordinal_string_array:
                return ordinal_string_array{ordinal_string{0, value}};
            case number_type_t::id:
                return id{0, {std::stoull(value)}};
            case number_type_t::id_array:
                return id_array{{0, {std::stoull(value)}}};
            }
        }
    }
    return value;
}
#endif

} // namespace ssharp::text