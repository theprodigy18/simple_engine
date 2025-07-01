#pragma once

#include "common_header.hpp"

namespace drop::id
{
    using idType = u32;

    namespace
    {
        constexpr idType generationBits {10};
        constexpr idType indexBits {sizeof(idType) * 8 - generationBits};

        constexpr idType generationMask {(idType {1} << generationBits) - 1};
        constexpr idType indexMask {(idType {1} << indexBits) - 1};
    } // namespace anonymous

    constexpr idType invalidID {idType(-1)};
    constexpr u32    minDeletedElements {1024}; // Min deleted elements before we can use older index instead of new.

    using generationType = std::conditional_t<generationBits <= 16, std::conditional_t<generationBits <= 8, u8, u16>, u32>;

    constexpr bool isValid(idType id)
    {
        return id != invalidID;
    }

    constexpr idType getIndex(idType id)
    {
        return id & indexMask;
    }

    constexpr idType getGeneration(idType id)
    {
        return (id >> indexBits) & generationMask;
    }

    constexpr idType newGeneration(idType id)
    {
        idType generation {getGeneration(id) + 1};
        SM_ASSERT(generation < (((u64) 1 << generationBits) - 1), "Generation overflow.");
        return (getIndex(id)) | (generation << indexBits);
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

#define DEFINE_TYPED_ID(name)                                \
    struct name final : IDBase                               \
    {                                                        \
        constexpr explicit name(idType id) : IDBase {id} { } \
        constexpr name() : IDBase {0} { }                    \
    };

#else
#define DEFINE_TYPED_ID(name) using name = idType;
#endif // _DEBUG

} // namespace drop::id
