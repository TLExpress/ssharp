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

#include "ssharp-cli.hpp"

int main(int argc, char** argv)
{
    namespace cli = ssharp::cli;
    CLI::App app{"ssharp-cli"};
    app.description("A command line interface for ssharp.");
    app.footer("Usage: ssharp-cli [OPTIONS] SUBCOMMAND");
    std::vector<std::string> paths;
    std::string type;
    cli::add_parser_sub_command(app, paths, type);
    std::vector<std::string> strs;
    uint8_t salt = 0;
    bool verbose = false;
    cli::add_hash_sub_command(app, strs, salt, verbose);
    cli::add_compress_sub_command(app, paths, type);
    cli::add_decompress_sub_command(app, paths, type);
    CLI11_PARSE(app, argc, argv);
    return 0;
}
