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

#include "util/types.hpp"

namespace ssharp::tobjtools
{

using namespace ssharp::types;

enum class tobj_type_t : uint8_t
{
    generic = 2,
    cubic = 5,
};

enum class mag_filter_t : uint8_t
{
    nearest = 0,
    linear = 1,
    default_v = 3
};

enum class min_filter_t : uint8_t
{
    nearest = 0,
    linear = 1,
    default_v = 3
};

enum class mip_filter_t : uint8_t
{
    nearest = 0,
    trilinear = 1,
    nomips = 2,
    default_v = 3
};

enum class tobj_addr_t : uint8_t
{
    repeat = 0,
    clamp = 1,
    clamp_to_edge = 2,
    clamp_to_border = 3,
    mirror = 4,
    mirror_clamp = 5,
    mirror_clamp_to_edge = 6,
};

enum class tobj_color_space_t : uint8_t
{
    srgb = 0,
    linear = 1,
};

enum class tobj_compress_t : uint8_t
{
    yes = 0,
    no = 1,
};

enum class tobj_anisotropic_t : uint8_t
{
    yes = 0,
    no = 1,
};

enum class format_t : uint8_t // surface format
{
    unknown = 0,
    r32g32b32a32_typeless = 1,
    r32g32b32a32_float = 2,
    r32g32b32a32_uint = 3,
    r32g32b32a32_sint = 4,
    r32g32b32_typeless = 5,
    r32g32b32_float = 6,
    r32g32b32_uint = 7,
    r32g32b32_sint = 8,
    r16g16b16a16_typeless = 9,
    r16g16b16a16_float = 10,
    r16g16b16a16_unorm = 11,
    r16g16b16a16_uint = 12,
    r16g16b16a16_snorm = 13,
    r16g16b16a16_sint = 14,
    r32g32_typeless = 15,
    r32g32_float = 16,
    r32g32_uint = 17,
    r32g32_sint = 18,
    r32g8x24_typeless = 19,
    d32_float_s8x24_uint = 20,
    r32_float_x8x24_typeless = 21,
    x32_typeless_g8x24_uint = 22,
    r10g10b10a2_typeless = 23,
    r10g10b10a2_unorm = 24,
    r10g10b10a2_uint = 25,
    r11g11b10_float = 26,
    r8g8b8a8_typeless = 27,
    r8g8b8a8_unorm = 28,
    r8g8b8a8_unorm_srgb = 29,
    r8g8b8a8_uint = 30,
    r8g8b8a8_snorm = 31,
    r8g8b8a8_sint = 32,
    r16g16_typeless = 33,
    r16g16_float = 34,
    r16g16_unorm = 35,
    r16g16_uint = 36,
    r16g16_snorm = 37,
    r16g16_sint = 38,
    r32_typeless = 39,
    d32_float = 40,
    r32_float = 41,
    r32_uint = 42,
    r32_sint = 43,
    r24g8_typeless = 44,
    d24_unorm_s8_uint = 45,
    r24_unorm_x8_typeless = 46,
    x24_typeless_g8_uint = 47,
    r8g8_typeless = 48,
    r8g8_unorm = 49,
    r8g8_uint = 50,
    r8g8_snorm = 51,
    r8g8_sint = 52,
    r16_typeless = 53,
    r16_float = 54,
    d16_unorm = 55,
    r16_unorm = 56,
    r16_uint = 57,
    r16_snorm = 58,
    r16_sint = 59,
    r8_typeless = 60,
    r8_unorm = 61,
    r8_uint = 62,
    r8_snorm = 63,
    r8_sint = 64,
    a8_unorm = 65,
    r1_unorm = 66,
    r9g9b9e5_sharedexp = 67,
    r8g8_b8g8_unorm = 68,
    g8r8_g8b8_unorm = 69,
    bc1_typeless = 70,
    bc1_unorm = 71,
    bc1_unorm_srgb = 72,
    bc2_typeless = 73,
    bc2_unorm = 74,
    bc2_unorm_srgb = 75,
    bc3_typeless = 76,
    bc3_unorm = 77,
    bc3_unorm_srgb = 78,
    bc4_typeless = 79,
    bc4_unorm = 80,
    bc4_snorm = 81,
    bc5_typeless = 82,
    bc5_unorm = 83,
    bc5_snorm = 84,
    b5g6r5_unorm = 85,
    b5g5r5a1_unorm = 86,
    b8g8r8a8_unorm = 87,
    b8g8r8x8_unorm = 88,
    r10g10b10_xr_bias_a2_unorm = 89,
    b8g8r8a8_typeless = 90,
    b8g8r8a8_unorm_srgb = 91,
    b8g8r8x8_typeless = 92,
    b8g8r8x8_unorm_srgb = 93,
    bc6h_typeless = 94,
    bc6h_uf16 = 95,
    bc6h_sf16 = 96,
    bc7_typeless = 97,
    bc7_unorm = 98,
    bc7_unorm_srgb = 99,
    ayuv = 100,
    y410 = 101,
    y416 = 102,
    nv12 = 103,
    p010 = 104,
    p016 = 105,
    _420_opaque = 106,
    yuy2 = 107,
    y210 = 108,
    y216 = 109,
    nv11 = 110,
    ai44 = 111,
    ia44 = 112,
    p8 = 113,
    a8p8 = 114,
    b4g4r4a4_unorm = 115,
    p208 = 130,
    v208 = 131,
    v408 = 132,

    r8g8b8_unorm = 200,
    b8g8r8_unorm = 201,
    r8g8b8_unorm_srgb = 202,
    b8g8r8_unorm_srgb = 203,
    r10g10b10a2_snorm = 204,
    r10g10b10x2_snorm = 205,
    l8_unorm = 206,
    l8a8_unorm = 207,
    d24_unorm = 208,
    b10g10r10a2_unorm = 209,
    unknown_210 = 210,
    unknown_211 = 211,
    pvrtc_2bpp_rgb = 212,
    pvrtc_2bpp_rgba = 213,
    pvrtc_4bpp_rgb = 214,
    pvrtc_4bpp_rgba = 215,
    r4g4b4a4_unorm = 216,
    r5g6b5_unorm = 217,
    b5g5r5x1_unorm = 218,
    r8g8b8x8_unorm = 219,
    r8g8b8x8_unorm_srgb = 220,
    d24_unorm_x8_uint = 221
};

/*
    default:
     - tga (32 bits) -> DXT5
     - tga (24 bits) -> DXT1

    usage tsnormal:
     - tga -> ATI2
     - png (nocompress) -> R16G16    # png is required

    nomips:
     - tga -> DXT5/DXT1

    color_space linear:
     - tga -> DXT5/DXT1

    nocompress:
     - tga (32 bits) -> A8R8G8B8
     - tga (24 bits) -> R8G8B8

    usage ui = nomips + color_space linear + nocompress

    usage ui    # (nomips + color_space linear + nocompress)
     - tga (32 bits) -> A8R8G8B8
     - tga (24 bits) -> R8G8B8
*/

#pragma pack(push, 1)

constexpr uint32_t expected_signature = 0x70b10a01;

struct header_t
{
    uint32_t signature;          // Position: 0
    uint8_t placeholder_1[18]; // Position: 4
    uint8_t bias;              // Position: 22
    uint8_t placeholder_2;     // Position: 23
    tobj_type_t type;          // Position: 24
    uint8_t placeholder_3;     // Position: 25 2 or 0
    mag_filter_t
        mag_filter; // Position: 26 Magnification filter: How the texture is
                    // sampled when a texel covers more than one pixel.
    min_filter_t
        min_filter; // Position: 27 Minification filter: How the texture is
                    // sampled when a texel covers less than one pixel.
    mip_filter_t mip_filter;        // Position: 28
    uint8_t placeholder_4;          // Position: 29 always 0?
    tobj_addr_t addr_u;             // Position: 30
    tobj_addr_t addr_v;             // Position: 31
    tobj_addr_t addr_w;             // Position: 32
    tobj_compress_t compress;       // Position: 33
    uint8_t placeholder_5;          // Position: 34
    tobj_anisotropic_t anisotropic; // Position: 35
    uint16_t placeholder_6;         // Position: 36
    tobj_color_space_t color_space; // Position: 38
    uint8_t placeholder_7;          // Position: 39
};
static_assert(sizeof(header_t) == 40, "tobj_header_t size mismatch");

struct texture_attr_t
{
    uint32_t length;
    uint32_t placeholder;
};
static_assert(sizeof(texture_attr_t) == 8, "tobj_texture_t size mismatch");

#pragma pack(pop)
} // namespace ssharp::tobjtools
