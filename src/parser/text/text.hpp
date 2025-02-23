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

#include <expected>
#include <map>
#include <variant>

namespace ssharp::text
{

using namespace ssharp::types;

#ifdef EXCLUDED
struct encoded_string_t
{
    uint64_t encoded;
    std::string decode(uint64_t encoded);
    static uint64_t encode(const std::string& identifier);
};
static_assert(sizeof(encoded_string_t) == 8, "encoded_string_t size mismatch");

using int16_t_array = std::vector<int16_t>;
using uint16_t_array = std::vector<uint16_t>;
using int32_t_array = std::vector<int32_t>;
using uint32_t_array = std::vector<uint32_t>;
using int64_t_array = std::vector<int64_t>;
using uint64_t_array = std::vector<uint64_t>;
using single_t = float;
using single_array = std::vector<float>;
using bool_array = std::vector<bool>;
using string_array = std::vector<std::string>;
using encoded_string_array = std::vector<encoded_string_t>;
using vec2s = std::array<float, 2>;
using vec3s = std::array<float, 3>;
using vec3i = std::array<int32_t, 3>;
using vec4s = std::array<float, 4>;
using vec7s = std::array<float, 7>;
using vec8s = std::array<float, 8>;
using vec2s_array = std::vector<vec2s>;
using vec3s_array = std::vector<vec3s>;
using vec3i_array = std::vector<vec3i>;
using vec4s_array = std::vector<vec4s>;
using vec7s_array = std::vector<vec7s>;
using vec8s_array = std::vector<vec8s>;
using ordinal_string = std::pair<uint32_t, std::string>;
using ordinal_string_array = std::vector<ordinal_string>;
using id = std::pair<uint32_t, std::vector<uint64_t>>;
using id_array = std::vector<id>;

using value_t = std::variant<
    std::monostate, std::string, string_array, encoded_string_t,
    encoded_string_array, single_t, single_array, vec2s, vec2s_array, vec3s,
    vec3s_array, vec3i, vec3i_array, vec4s, vec4s_array, vec7s, vec7s_array,
    vec8s, vec8s_array, int32_t, int32_t_array, uint32_t, uint32_t_array,
    int16_t, int16_t_array, uint16_t, uint16_t_array, int64_t, int64_t_array,
    uint64_t, uint64_t_array, bool, bool_array, ordinal_string,
    ordinal_string_array, id, id_array>;

using record_t = std::pair<std::string, value_t>;

enum class number_type_t : uint32_t
{
    invalid = 0x00000000,
    string = 0x00000001,
    string_array = 0x00000002,
    encoded_string = 0x00000003,
    encoded_string_array = 0x00000004,
    single = 0x00000005,
    single_array = 0x00000006,
    vec2s = 0x00000007,
    vec2s_array = 0x00000008,
    vec3s = 0x00000009,
    vec3s_array = 0x0000000A,
    vec3i = 0x00000011,
    vec3i_array = 0x00000012,
    vec4s = 0x00000017,
    vec4s_array = 0x00000018,
    vec7s = 0x00000019,
    vec7s_array = 0x0000001A,
    int32 = 0x00000025,
    int32_array = 0x00000026,
    uint32 = 0x00000027,
    uint32_array = 0x00000028,
    int16 = 0x00000029,
    int16_array = 0x0000002A,
    uint16 = 0x0000002B,
    uint16_array = 0x0000002C,
    int64 = 0x00000031,
    int64_array = 0x00000032,
    uint64 = 0x00000033,
    uint64_array = 0x00000034,
    bytebool = 0x00000035,
    bytebool_array = 0x00000036,
    ordinal_string = 0x00000037,
    ordinal_string_array = 0x00000038,
    id = 0x00000039,
    id_array = 0x0000003A
};

using try_type_t = std::variant<std::string, number_type_t>;
std::optional<number_type_t> get_number_type(const std::string& type);
std::expected<value_t, std::string> parse_value(
    const std::string& value,
    std::optional<try_type_t> try_type = std::nullopt);
std::expected<record_t, std::string> parse_record(const std::string& record);

class struct_t : public std::map<std::string, value_t>
{
  public:
    using std::map<std::string, value_t>::map;
};
#endif

} // namespace ssharp::text
