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

#include "ssharpfs.hpp"

namespace ssharp::fs::ssharpfs
{

parsed_paths_t ssharpfs_t::get_parsed_paths() const
{
    parsed_paths_t paths;
    for (const auto& [key, entry] : *this)
    {
        if (entry->parsed_paths.empty())
        {
            continue;
        }
        paths.insert(entry->parsed_paths.begin(), entry->parsed_paths.end());
    }
}

bool ssharpfs_t::entries_all_resolved() const
{
    for (const auto& [key, _] : *this)
    {
        if(std::holds_alternative<hash_attr_t>(key))
        {
            return false;
        }
    }
    return true;
}

} // namespace ssharp::fs::ssharpfs