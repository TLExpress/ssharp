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

#include <filesystem>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <variant>
#include <vector>

namespace ssharp::types
{

using buff_t = std::vector<uint8_t>;
using path_t = std::filesystem::path;
using paths_t = std::set<path_t>;
using pos_t = std::streampos;
using span_source_t = std::variant<buff_t, path_t>;
using span_attr_t = std::pair<pos_t, size_t>;
using is_absolute_path_t = bool;
using is_directory_t = bool;
using parsed_path_t = std::tuple<path_t, is_absolute_path_t, is_directory_t, std::optional<path_t>>;
using is_compressed_file_t = bool;
using parsed_paths_t = std::set<parsed_path_t>;

} // namespace ssharp::types
