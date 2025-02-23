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

#include "compressor.hpp"
#include <bitset>

#include "zlib-ng.h"
#include "zbuild.h"
#include "zutil.h"
#ifdef S390_DFLTCC_DEFLATE
#include "arch/s390/dfltcc_common.h"
#else
/* Returns the upper bound on compressed data length based on uncompressed data
 * length, assuming default settings. Zero means that arch-specific deflation
 * code behaves identically to the regular zlib-ng algorithms. */
#define DEFLATE_BOUND_COMPLEN(source_len) 0
#endif

namespace ssharp::util
{
using namespace ssharp::types;
namespace ssexcept = ssharp::exceptions;

#pragma pack(push, 1)
struct zlib_header_t
{
    uint8_t cmf;
    uint8_t flg;
};

struct gzip_flg_t : std::bitset<8>
{
    using std::bitset<8>::bitset;
    bool ftext() const { return test(0); }
    bool fhcrc() const { return test(1); }
    bool fextra() const { return test(2); }
    bool fname() const { return test(3); }
    bool fcomment() const { return test(4); }
};

struct gzip_header_t
{
    uint16_t id;
    uint8_t cm;
    gzip_flg_t flg;
    uint8_t mtime;
    uint8_t xfl;
    uint8_t os;
};
#pragma pack(pop)

size_t inline adler32(const buff_t& data)
{
    return PREFIX(adler32)(0, data.data(), data.size());
}

int32_t inline wbits(compress_type_t type)
{
    switch (type)
    {
        case compress_type_t::zlib:
            return DEF_WBITS;
        case compress_type_t::gzip:
            return DEF_WBITS | 16;
        case compress_type_t::raw:
            return -DEF_WBITS;
        default:
            break;
    }
    throw ssexcept::exception("Invalid compression type");
}

size_t inline compress_bound(size_t source_len, compress_type_t type)
{
    size_t wraplen = 0;
    switch (type)
    {
        case compress_type_t::zlib:
            wraplen = ZLIB_WRAPLEN;
            break;
        case compress_type_t::gzip:
            wraplen = GZIP_WRAPLEN;
            break;
        case compress_type_t::raw:
        default:
            break;
    }

    z_uintmax_t complen = DEFLATE_BOUND_COMPLEN(source_len);

    if (complen > 0)
        return complen;

#ifndef NO_QUICK_STRATEGY
    return source_len               /* The source size itself */
           + (source_len == 0 ? 1
                              : 0)  /* Always at least one byte for any input */
           +
           (source_len < 9 ? 1 : 0) /* One extra byte for lengths less than 9 */
           + DEFLATE_QUICK_OVERHEAD(source_len) /* Source encoding overhead,
                                                   padded to next full byte */
           + DEFLATE_BLOCK_OVERHEAD /* Deflate block overhead bytes */
           + wraplen;               /* none, zlib or gzip wrapper */

#else
    return source_len + (source_len >> 4) + 7 + wraplen;
#endif
}

buff_t compress(const buff_t& data, compress_type_t type)
{
    buff_t compressed;
    PREFIX3(stream) stream;
    int err;
    const unsigned int max = (unsigned int)-1;
    z_size_t left;

    left = compress_bound(data.size(), type);
    compressed.resize(left);

    stream.zalloc = NULL;
    stream.zfree = NULL;
    stream.opaque = NULL;

    err = PREFIX(deflateInit2)(&stream, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                               wbits(type), DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY);
    if (err != Z_OK)
        throw ssexcept::exception("Failed to initialize compression stream");

    stream.next_out = compressed.data();
    stream.avail_out = 0;
    stream.next_in = (z_const unsigned char*)data.data();
    stream.avail_in = 0;
    auto source_remaining = data.size();

    do
    {
        if (stream.avail_out == 0)
        {
            stream.avail_out =
                left > (unsigned long)max ? max : (unsigned int)left;
            left -= stream.avail_out;
        }
        if (stream.avail_in == 0)
        {
            stream.avail_in = data.size() > (unsigned long)max
                                  ? max
                                  : (unsigned int)data.size();
            source_remaining -= stream.avail_in;
        }
        err =
            PREFIX(deflate)(&stream, source_remaining ? Z_NO_FLUSH : Z_FINISH);
    } while (err == Z_OK);

    if (err != Z_STREAM_END)
    {
        PREFIX(deflateEnd)(&stream);
        return buff_t();
    }

    compressed.resize(stream.total_out);
    PREFIX(deflateEnd)(&stream);
    return compressed;
}

buff_t decompress(const buff_t& data, compress_type_t type,
                  std::optional<size_t> fixed_output_size,
                  std::optional<size_t> peek_size)
{
    buff_t decompressed;

    PREFIX3(stream) stream;
    int err;
    const unsigned int max = (unsigned int)-1;
    z_size_t left;

    auto source_remaining = data.size();
    auto reserving = data.size() * 2;

    do
    {
        if (fixed_output_size)
        {
            left = fixed_output_size.value();
        }
        else
        {
            left = reserving;
        }
        decompressed.resize(left);

        stream.next_in = (z_const unsigned char*)data.data();
        stream.avail_in = 0;
        stream.zalloc = NULL;
        stream.zfree = NULL;
        stream.opaque = NULL;

        err = PREFIX(inflateInit2)(&stream, wbits(type));
        if (err != Z_OK)
            throw ssexcept::exception(
                "Failed to initialize decompression stream");

        stream.next_out = decompressed.data();
        stream.avail_out = 0;

        do
        {
            if (stream.avail_out == 0)
            {
                stream.avail_out =
                    left > (unsigned long)max ? max : (unsigned int)left;
                left -= stream.avail_out;
            }
            if (stream.avail_in == 0)
            {
                stream.avail_in = data.size() > (unsigned long)max
                                      ? max
                                      : (unsigned int)data.size();
                source_remaining -= stream.avail_in;
            }
            err = PREFIX(inflate)(&stream,
                                  source_remaining ? Z_NO_FLUSH : Z_FINISH);
            if (err == Z_OK && peek_size &&
                stream.total_out >= peek_size.value())
            {
                err = Z_STREAM_END;
            }
            if (err == Z_BUF_ERROR)
            {
                if(fixed_output_size)
                {
                    PREFIX(inflateEnd)(&stream);
                    throw ssexcept::exception("Failed to decompress data");
                }
                reserving <<= 1;
            }
        } while (err == Z_OK);
    } while (err == Z_BUF_ERROR);

    if (err != Z_STREAM_END)
    {
        PREFIX(inflateEnd)(&stream);
        throw ssexcept::exception("Failed to decompress data");
    }

    decompressed.resize(stream.total_out);
    PREFIX(inflateEnd)(&stream);
    return decompressed;
}

void remove_zlib_attr(buff_t& data)
{
    auto constexpr minimum_zlib_size = 6;
    if (data.size() <= minimum_zlib_size)
        throw ssexcept::exception("Invalid zlib data");
    data.erase(data.begin(), data.begin() + 2);
    data.erase(data.end() - 4, data.end());
}

void remove_gzip_attr(buff_t& data)
{
    if (data.size() <= sizeof(gzip_header_t) + 8)
        throw ssexcept::exception("Invalid gzip data");
    gzip_header_t* header = reinterpret_cast<gzip_header_t*>(data.data());
    auto& flg = header->flg;
    size_t offset = 10;
    if (flg.fextra())
    {
        offset += data[offset] + data[offset + 1] * 256;
    }
    if (flg.fname())
    {
        while (data[offset] != 0)
            offset++;
        offset++;
    }
    if (flg.fcomment())
    {
        while (data[offset] != 0)
            offset++;
        offset++;
    }
    if(flg.fhcrc())
    {
        offset += 2;
    }
    if(data.size() <= offset + 8)
        throw ssexcept::exception("Invalid gzip data");
    data.erase(data.begin(), data.begin() + offset);
    data.erase(data.end() - 8, data.end());
}

} // namespace ssharp::util
