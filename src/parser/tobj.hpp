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
#include "tobj-util/types.hpp"

namespace ssharp::parser::tobj
{

using namespace ssharp::types;
using namespace ssharp::tobjtools;

/**
 * @brief Parse a buffer containing a tobj file
 * @param buff The buffer containing the tobj file
 * @param hash The hash attribute of the file
 * @return A set of paths
 * @throws parse_error if the file is invalid
 */
parsed_paths_t find_paths(const buff_t& buff,
                     std::optional<hash_attr_t> hash = std::nullopt);
} // namespace ssharp::parser::tobj
