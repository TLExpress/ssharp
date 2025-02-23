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

#include "util/compressor.hpp"

#include "ssharp-cli.hpp"
#include "util/span.hpp"

namespace ssharp::cli
{
namespace compressor
{
using namespace ssharp::types;
void compress(const std::vector<std::string>& paths, const std::string& type)
{
    compress_type_t ctype;
    if (type == "zlib")
        ctype = compress_type_t::zlib;
    else if (type == "gzip")
        ctype = compress_type_t::gzip;
    else if (type == "raw")
        ctype = compress_type_t::raw;
    else
    {
        std::cerr << "Invalid compression type" << std::endl;
        return;
    }
    for (const auto& path : paths)
    {
        auto data = *util::span_t(path);
        auto compressed = util::compress(data, ctype);
        std::ofstream ofs(path + ".compressed", std::ios::binary);
        ofs.write(reinterpret_cast<const char*>(compressed.data()), compressed.size());
    }
}
void decompress(const std::vector<std::string>& paths, const std::string& type)
{
    compress_type_t ctype;
    if (type == "zlib")
        ctype = compress_type_t::zlib;
    else if (type == "gzip")
        ctype = compress_type_t::gzip;
    else if (type == "raw")
        ctype = compress_type_t::raw;
    else
    {
        std::cerr << "Invalid compression type" << std::endl;
        return;
    }
    for (const auto& path : paths)
    {
        auto data = *util::span_t(path);
        auto decompressed = util::decompress(data, ctype);
        std::ofstream ofs(path + ".decompressed", std::ios::binary);
        ofs.write(reinterpret_cast<const char*>(decompressed.data()), decompressed.size());
    }
}
} // namespace compressor
void add_compress_sub_command(CLI::App& app, std::vector<std::string>& paths,
                              std::string& type)
{
    auto compress = app.add_subcommand("compress", "Compress files");
    compress->add_option("paths", paths, "Paths to compress")
        ->required()
        ->type_name("PATHS");
    compress->add_option("-t,--type", type, "Compression type")
        ->required()
        ->type_name("TYPE");
    compress->callback([&]() { compressor::compress(paths, type); });
}
void add_decompress_sub_command(CLI::App& app, std::vector<std::string>& paths,
                                std::string& type)
{
    auto decompress = app.add_subcommand("decompress", "Decompress files");
    decompress->add_option("paths", paths, "Paths to decompress")
        ->required()
        ->type_name("PATHS");
    decompress->add_option("-t,--type", type, "Compression type")
        ->required()
        ->type_name("TYPE");
    decompress->callback([&]() { compressor::decompress(paths, type); });
}
} // namespace ssharp::cli
