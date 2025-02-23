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

#include <exception>
#include <string>

namespace ssharp::exceptions
{

class exception : public std::exception
{
  public:
    exception(const std::string& msg) : msg(msg) {}
    const char* what() const noexcept override
    {
        return msg.c_str();
    }

  private:
    std::string msg;
};

class parse_error : public exception
{
  public:
    parse_error(const std::string& msg) : exception("parse_error: " + msg), msg(msg) {}

    const char* what() const noexcept override
    {
        return msg.c_str();
    }

  private:
    std::string msg;
};

class span_error : public exception
{
  public:
    span_error(const std::string& msg) : exception("span_error: " + msg), msg(msg) {}

    const char* what() const noexcept override
    {
        return msg.c_str();
    }

  private:
    std::string msg;
};

} // namespace ssharp::exceptions
