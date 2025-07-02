#pragma once

#include "common_header.hpp"

namespace drop::id
{
    using idType = u32; // We can actually use u64 if we need higher lifetime count.

    namespace
    {
		// This two are bits separation for index and generation.
        constexpr idType generationBits {10}; 
        constexpr idType indexBits {sizeof(idType) * 8 - generationBits};

		// And this is the masking.
        constexpr idType generationMask {(idType {1} << generationBits) - 1}; // For example gen = 10bit -> 0x000003FF.
        constexpr idType indexMask {(idType {1} << indexBits) - 1}; // for example idx = 22bit -> 0x003FFFFF.
    } // namespace anonymous

    constexpr idType invalidID {idType(-1)}; // Invalid id value = -1(0xFFFFFFFF).
    constexpr u32    minDeletedElements {1024}; // Min deleted elements before we can use older index instead of new.

    using generationType = std::conditional_t<generationBits <= 16, std::conditional_t<generationBits <= 8, u8, u16>, u32>; // Just clarify size of our generation type.

	// Make sure is not out of range.
    constexpr bool IsValid(idType id)
    {
        return id != invalidID;
    }

    constexpr idType GetIndex(idType id)
    {
        return id & indexMask;
    }

    constexpr idType GetGeneration(idType id)
    {
        return (id >> indexBits) & generationMask;
    }

	// Increase generation by one.
    constexpr idType NewGeneration(idType id)
    {
        idType generation {GetGeneration(id) + 1};
        SM_ASSERT(generation < (((u64) 1 << generationBits) - 1), "Generation overflow.");
        return (GetIndex(id)) | (generation << indexBits);
    }

#ifdef _DEBUG
    namespace
    {
        struct IDBase
        {
            constexpr explicit IDBase(idType id) : _id {id} { }
            constexpr operator idType() const { return _id; }

        private:
            idType _id {invalidID};
        };
    } // namespace anonymous

#define DEFINE_TYPED_ID(name)                                    \
    struct name final : id::IDBase                               \
    {                                                            \
        constexpr explicit name(id::idType id) : IDBase {id} { } \
        constexpr name() : id::IDBase {0} { }                    \
    };

#else
#define DEFINE_TYPED_ID(name) using name = id::idType;
#endif // _DEBUG

} // namespace drop::id
