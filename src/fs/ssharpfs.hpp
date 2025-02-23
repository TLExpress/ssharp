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

#include "util/span.hpp"
#include "util/types.hpp"
#include "parser/directory.hpp"
#include "parser/pmd.hpp"
#include "parser/soundref.hpp"

#include <map>

namespace ssharp::fs::ssharpfs
{
using namespace ssharp::types;
using span_t = ssharp::util::span_t;


struct entry_t 
{
    is_encrypted_t is_encrypted;
    span_t data;
    parsed_paths_t parsed_paths;
    std::optional<compress_attr_t> compress_attr;
    virtual file_type_t file_type() const = 0;
    virtual void parse() {};
    virtual void compress(compress_type_t type);
    virtual void decompress(compress_type_t type);
};

struct generic_entry_t : entry_t
{
    file_type_t file_type() const override
    {
        return file_type_t::generic;
    }
};

struct sii_entry_t : entry_t
{
    sii_status_t sii_status;    
    file_type_t file_type() const override
    {
        return file_type_t::sii;
    }
    void parse() override
    {
        // TODO: Implement
    }
};

struct directory_entry_t : public entry_t
{
    file_type_t file_type() const override
    {
        return file_type_t::directory;
    }
    void parse() override
    {
        parsed_paths = parser::directory::parse(data.get());
    }
};

struct mat_entry_t : entry_t
{
    file_type_t file_type() const override
    {
        return file_type_t::mat;
    }
    void parse() override
    {
        // TODO: Implement
    }
};

struct pmd_entry_t : entry_t
{
    file_type_t file_type() const override
    {
        return file_type_t::pmd;
    }
    void parse() override
    {
        parsed_paths = parser::pmd::find_paths(data.get());
    }
};

struct tobj_entry_t : entry_t
{
    file_type_t file_type() const override
    {
        return file_type_t::tobj;
    }
    void parse() override
    {
        // TODO: Implement
    }
};

struct soundref_entry_t : entry_t
{
    file_type_t file_type() const override
    {
        return file_type_t::soundref;
    }
    void parse() override
    {
        parsed_paths = parser::soundref::parse(data.get());
    }
};

struct ssharpfs_t : std::map<entry_key_t, std::shared_ptr<entry_t>>
{
    using std::map<entry_key_t, std::shared_ptr<entry_t>>::map;
    void rebuild_directories();
    void prune_directories(path_t root = path_t(""));
    parsed_paths_t get_parsed_paths() const;
    void apply_dictionary(const dictionary_t& dictionary);
    bool entries_all_resolved() const;
    salt_t salt = 0;
};

} // namespace ssharp::fs::ssharpfs