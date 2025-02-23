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

#include "util/exceptions.hpp"
#include "util/types.hpp"

#include <concepts>

namespace ssharp::util
{

using namespace ssharp::types;
namespace ssexcept = ssharp::exceptions;

template <typename T>
concept span_source_template =
    std::same_as<T, buff_t> || std::same_as<T, path_t>;

class span_t
{
  public:
    template <span_source_template Source>
    span_t(Source&& source, std::optional<span_attr_t> attr = std::nullopt) :
        source(std::forward<Source>(source)), attr(attr)
    {}
    span_t(span_source_t&& source,
           std::optional<span_attr_t> attr = std::nullopt) :
        source(std::move(source)), attr(attr)
    {}
    span_t(span_t&&) = default;
    span_t& operator=(const span_t&) = default;
    span_t& operator=(span_t&&) = default;
    buff_t operator*() const
    {
        return get();
    }
    buff_t get(std::optional<span_attr_t> part = std::nullopt) const
    {
        pos_t eventual_pos = 0;
        std::optional<size_t> eventual_size;
        if (attr)
        {
            auto [pos, size] = *attr;
            eventual_pos += pos;
            eventual_size = size;
        }
        if (part)
        {
            auto [pos, size] = *part;
            eventual_pos += pos;
            eventual_size = size;
        }
        if (std::holds_alternative<buff_t>(source))
        {
            auto& buff = std::get<buff_t>(source);
            return buff_t(buff.begin() + eventual_pos,
                          eventual_size ? buff.begin() + eventual_pos + *eventual_size : buff.end());
        }
        else if (std::holds_alternative<path_t>(source))
        {
            const auto& path = std::get<path_t>(source);
            std::ifstream file(path, std::ios::binary);
            if (!file.is_open())
            {
                throw std::ios::failure(
                    "Failed to open file: " + path.string());
            }
            file.seekg(0, std::ios::end);
            auto total_size = file.tellg();
            file.seekg(eventual_pos);
            buff_t buff(eventual_size ? *eventual_size : total_size - eventual_pos);
            file.read(reinterpret_cast<char*>(buff.data()), buff.size());
            return buff;
        }
        throw ssexcept::exception("Invalid span source");
    }
    size_t size() const
    {
        if(!attr)
        {
            attr = std::make_pair(0, get().size());
        }
        return attr->second;
    }

  private:
    span_source_t source;
    mutable std::optional<span_attr_t> attr;
};

} // namespace ssharp::util
