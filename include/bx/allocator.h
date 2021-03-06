/*
 * Copyright 2010-2017 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bx#license-bsd-2-clause
 */

#ifndef BX_ALLOCATOR_H_HEADER_GUARD
#define BX_ALLOCATOR_H_HEADER_GUARD

#include "bx.h"

#include <new>

#if BX_CONFIG_ALLOCATOR_DEBUG
#	define BX_ALLOC(_allocator, _size)                         bx::alloc(_allocator, _size, 0, __FILE__, __LINE__)
#	define BX_REALLOC(_allocator, _ptr, _size)                 bx::realloc(_allocator, _ptr, _size, 0, __FILE__, __LINE__)
#	define BX_FREE(_allocator, _ptr)                           bx::free(_allocator, _ptr, 0, __FILE__, __LINE__)
#	define BX_ALIGNED_ALLOC(_allocator, _size, _align)         bx::alloc(_allocator, _size, _align, __FILE__, __LINE__)
#	define BX_ALIGNED_REALLOC(_allocator, _ptr, _size, _align) bx::realloc(_allocator, _ptr, _size, _align, __FILE__, __LINE__)
#	define BX_ALIGNED_FREE(_allocator, _ptr, _align)           bx::free(_allocator, _ptr, _align, __FILE__, __LINE__)
#	define BX_NEW(_allocator, _type)                           ::new(BX_ALLOC(_allocator, sizeof(_type) ) ) _type
#	define BX_DELETE(_allocator, _ptr)                         bx::deleteObject(_allocator, _ptr, 0, __FILE__, __LINE__)
#	define BX_ALIGNED_NEW(_allocator, _type, _align)           ::new(BX_ALIGNED_ALLOC(_allocator, sizeof(_type), _align) ) _type
#	define BX_ALIGNED_DELETE(_allocator, _ptr, _align)         bx::deleteObject(_allocator, _ptr, _align, __FILE__, __LINE__)
#else
#	define BX_ALLOC(_allocator, _size)                         bx::alloc(_allocator, _size, 0)
#	define BX_REALLOC(_allocator, _ptr, _size)                 bx::realloc(_allocator, _ptr, _size, 0)
#	define BX_FREE(_allocator, _ptr)                           bx::free(_allocator, _ptr, 0)
#	define BX_ALIGNED_ALLOC(_allocator, _size, _align)         bx::alloc(_allocator, _size, _align)
#	define BX_ALIGNED_REALLOC(_allocator, _ptr, _size, _align) bx::realloc(_allocator, _ptr, _size, _align)
#	define BX_ALIGNED_FREE(_allocator, _ptr, _align)           bx::free(_allocator, _ptr, _align)
#	define BX_NEW(_allocator, _type)                           ::new(BX_ALLOC(_allocator, sizeof(_type) ) ) _type
#	define BX_DELETE(_allocator, _ptr)                         bx::deleteObject(_allocator, _ptr, 0)
#	define BX_ALIGNED_NEW(_allocator, _type, _align)           ::new(BX_ALIGNED_ALLOC(_allocator, sizeof(_type), _align) ) _type
#	define BX_ALIGNED_DELETE(_allocator, _ptr, _align)         bx::deleteObject(_allocator, _ptr, _align)
#endif // BX_CONFIG_DEBUG_ALLOC

#ifndef BX_CONFIG_ALLOCATOR_NATURAL_ALIGNMENT
#	define BX_CONFIG_ALLOCATOR_NATURAL_ALIGNMENT 8
#endif // BX_CONFIG_ALLOCATOR_NATURAL_ALIGNMENT

namespace bx
{
	struct BX_NO_VTABLE AllocatorI
	{
		virtual ~AllocatorI() = 0;

		/// Allocated, resizes memory block or frees memory.
		///
		/// @param[in] _ptr If _ptr is NULL new block will be allocated.
		/// @param[in] _size If _ptr is set, and _size is 0, memory will be freed.
		/// @param[in] _align Alignment.
		/// @param[in] _file Debug file path info.
		/// @param[in] _line Debug file line info.
		virtual void* realloc(void* _ptr, size_t _size, size_t _align, const char* _file, uint32_t _line) = 0;
	};

	/// Check if pointer is aligned. _align must be power of two.
	bool isAligned(const void* _ptr, size_t _align);

	/// Aligns pointer to nearest next aligned address. _align must be power of two.
	void* alignPtr(
		  void* _ptr
		, size_t _extra
		, size_t _align = BX_CONFIG_ALLOCATOR_NATURAL_ALIGNMENT
		);

	///
	void* alloc(
		  AllocatorI* _allocator
		, size_t _size
		, size_t _align = 0
		, const char* _file = NULL
		, uint32_t _line = 0
		);

	///
	void free(
		  AllocatorI* _allocator
		, void* _ptr
		, size_t _align = 0
		, const char* _file = NULL
		, uint32_t _line = 0
		);

	///
	void* realloc(
		  AllocatorI* _allocator
		, void* _ptr
		, size_t _size
		, size_t _align = 0
		, const char* _file = NULL
		, uint32_t _line = 0
		);

	///
	void* alignedAlloc(
		  AllocatorI* _allocator
		, size_t _size
		, size_t _align
		, const char* _file = NULL
		, uint32_t _line = 0
		);

	///
	void alignedFree(
		  AllocatorI* _allocator
		, void* _ptr
		, size_t /*_align*/
		, const char* _file = NULL
		, uint32_t _line = 0
		);

	///
	void* alignedRealloc(
		  AllocatorI* _allocator
		, void* _ptr
		, size_t _size
		, size_t _align
		, const char* _file = NULL
		, uint32_t _line = 0
		);

	///
	template <typename ObjectT>
	void deleteObject(
		  AllocatorI* _allocator
		, ObjectT* _object
		, size_t _align = 0
		, const char* _file = NULL
		, uint32_t _line = 0
		);

} // namespace bx

#include "allocator.inl"

#endif // BX_ALLOCATOR_H_HEADER_GUARD
