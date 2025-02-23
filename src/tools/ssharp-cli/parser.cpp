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

#include "parser/pmd.hpp"
#include "parser/sii.hpp"
#include "parser/font.hpp"
#include "parser/tobj.hpp"
#include "parser/soundref.hpp"
#include "ssharp-cli.hpp"
#include "util/span.hpp"

#include <functional>

namespace ssharp::cli
{
namespace parser
{
void parse(const std::vector<std::string>& paths, const std::string& type)
{
    namespace pmd = ssharp::parser::pmd;
    using span_t = ssharp::util::span_t;
    using namespace ssharp::types;
    parsed_paths_t parsed_paths;
    if (type == "pmd")
    {
        for (const auto& path : paths)
        {
            auto result = pmd::find_paths(*span_t{path});
            parsed_paths.insert(result.begin(), result.end());
        }
    }
    else if (type == "soundref")
    {
        for (const auto& path : paths)
        {
            auto data = *span_t{path};
            auto result = ssharp::parser::soundref::find_paths(data);
            parsed_paths.insert(result.begin(), result.end());
        }
    }
    else if (type == "font")
    {
        for (const auto& path : paths)
        {
            auto data = *span_t{path};
            auto result = ssharp::parser::font::find_paths(data);
            parsed_paths.insert(result.begin(), result.end());
        }
    }
    else if (type == "tobj")
    {
        for (const auto& path : paths)
        {
            auto data = *span_t{path};
            auto result = ssharp::parser::tobj::find_paths(data);
            parsed_paths.insert(result.begin(), result.end());
        }
    }
    else if (type == "sii")
    {
        for (const auto& path : paths)
        {
            auto data = *span_t{path};
            auto result = ssharp::parser::sii::find_paths(data);
            parsed_paths.insert(result.begin(), result.end());
        }
    }
    else
    {
        std::cerr << "Unknown type: " << type << std::endl;
        return;
    }
    for (const auto& p : parsed_paths)
    {
        auto [path, is_absolute_path, is_directory, _] = p;
        if (is_absolute_path == is_absolute_path_t::absolute)
        {
            path = path_t{"/" + path.string()};
        }
        if (is_directory == is_directory_t::directory)
        {
            path /= "";
        }
        std::cout << path.string() << std::endl;
    }
    return;
}
} // namespace parser

void add_parser_sub_command(CLI::App& app, std::vector<std::string>& paths,
                            std::string& type)
{
    auto parser = app.add_subcommand("parse", "Parse a file");
    parser
        ->add_option("--type,-t", type, "Specify the type of file (e.g., pmd)")
        ->required();
    parser->add_option("file", paths, "The file to parse")->required();
    parser->callback([&paths, &type]() { parser::parse(paths, type); });
}
} // namespace ssharp::cli
