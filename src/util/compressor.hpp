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
#include "util/exceptions.hpp"

namespace ssharp::util
{
using namespace ssharp::types;

buff_t compress(const buff_t& data,
                compress_type_t type);
buff_t decompress(const buff_t& data,
                  compress_type_t type,
                  std::optional<size_t> fixed_output_size = std::nullopt,
                  std::optional<size_t> peek_size = std::nullopt);
void remove_zlib_attr(buff_t& data);
void remove_gzip_attr(buff_t& data);

} // namespace ssharp::util
