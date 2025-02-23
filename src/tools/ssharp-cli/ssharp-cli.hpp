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

#include "CLI/CLI.hpp"
#include "util/types.hpp"

#include <optional>

namespace ssharp::cli
{
namespace parser
{
void parse(const std::vector<std::string>& paths, const std::string& type);
} // namespace parser
namespace hash
{
void hash(const std::vector<std::string>& strs, uint16_t salt, bool verbose);
} // namespace hash
namespace compressor
{
void compress(const std::vector<std::string>& paths, const std::string& type);
void decompress(const std::vector<std::string>& paths, const std::string& type);
} // namespace compress
void add_parser_sub_command(CLI::App& app, std::vector<std::string>& paths,
                            std::string& type);
void add_hash_sub_command(CLI::App& app, std::vector<std::string>& strs,
                            uint8_t& salt, bool& verbose);
void add_compress_sub_command(CLI::App& app, std::vector<std::string>& paths,
                            std::string& type);
void add_decompress_sub_command(CLI::App& app, std::vector<std::string>& paths,
                            std::string& type);
} // namespace ssharp::cli
