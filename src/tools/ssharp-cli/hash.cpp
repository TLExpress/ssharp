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
#include "cityhash/city.hpp"

#include <functional>

namespace ssharp::cli
{
namespace hash
{
using namespace ssharp::types;
void hash(const std::vector<std::string>& strs, uint16_t salt, bool verbose)
{
    for (auto str : strs)
    {
        hash_t hash;
        if(salt)
        {
            str = std::to_string(salt) + str;
        }
        hash = cityhash::CityHash64(str);
        if (verbose)
        {
            std::cout << "Length: " << str.size() << std::endl;
            std::cout << "Hash of \"" << str << "\": " << std::endl;
        }
        std::cout << std::hex << hash << std::endl;
    }
}
} // namespace hash
void add_hash_sub_command(CLI::App& app, std::vector<std::string>& strs,
                            uint8_t& salt, bool& verbose)
{
    auto hash = app.add_subcommand("hash", "Hash a string");
    hash->add_option("--salt,-s", salt, "Specify the salt to use");
    hash->add_flag("--verbose,-v", verbose, "Print the string along with the hash");
    hash->add_option("string", strs, "The string to hash")->required();
    hash->callback([&strs, &salt, &verbose]() { hash::hash(strs, salt, verbose); });
}
} // namespace ssharp::cli
