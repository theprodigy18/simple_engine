#pragma once

#include "common_header.hpp"

// Unsigned types.
using u8  = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

// Signed types;
using i8  = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

// Floating point types.
using f32 = float;

constexpr u32 u32InvalidID {u32(-1)}; // Invalid id for u32.

// Char types.
using WChar = wchar_t*;
using Char  = char*;