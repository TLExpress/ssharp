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

namespace ssharp::parser::sii
{

using namespace ssharp::types;

buff_t crlf_to_lf(const buff_t& buff);
buff_t remove_comments(const buff_t& buff);
buff_t remove_empty_lines(const buff_t& buff);
buff_t remove_newline_behind_colon(const buff_t& buff);
buff_t remove_whitespace(const buff_t& buff);

/**
 * @brief Parse a buffer containing a sii file
 * @param buff The buffer containing the sii file
 * @param hash The hash attribute of the file
 * @return A set of paths
 * @throws parse_error if the file is invalid
 */
parsed_paths_t find_paths(const buff_t& buff,
                          std::optional<hash_attr_t> hash = std::nullopt);

} // namespace ssharp::parser::sii