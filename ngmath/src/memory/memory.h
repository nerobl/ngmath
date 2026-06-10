#pragma once
#include <stdlib.h>
//#include "../core.h"

namespace ngmath {

	class memory {
	private:
		struct memory_info {
			block_id* blockIDs = nullptr;
			size_t totalObjIDs = 0;
			size_t empty = 0;
			size_t filled = 0;
			size_t totalBlocks = 0;
			size_t blockIDSlots = 0;
		};

		static void* s_Memory;
		static block* s_Contexts;

		static void* _ngm_malloc(size_t size) {
			void* obj = nullptr;
			obj = malloc(size);
			if (!obj)
				throw NGM_MALLOC_FAILURE;
			return obj;
		}
		template<typename T>
		static T* _ngm_malloc() {
			T* obj = nullptr;
			obj = (T*)malloc(sizeof(T));
			if (!obj)
				throw NGM_MALLOC_FAILURE;
			return obj;
		}
		template<typename T>
		static void _ngm_dealloc(T* var) {
			if (!var) {
				throw NGM_DEALLOC_FAILURE;
				return;
			}
			var->~T();
			free(var);
		}

		template<typename T>
		static T* _ngm_malloca(size_t size) {
			T* obj = nullptr;
			obj = (T*)malloc(sizeof(T) * size);
			if (!obj)
				throw NGM_MALLOC_FAILURE;
			return obj;
		}

		template<typename T>
		static void _ngm_dealloca(T* var, size_t size) {
			if (!var)
				throw NGM_DEALLOC_FAILURE;
			for (size_t i = 0; i < size; ++i)
				var[i].~T();
			free(var);
		}

		static memory_info& _get_info() { return *(memory_info*)s_Memory; }
		static size_t _total_bytes_used() { return _get_info().empty + _get_info().filled; }
		static void* _get_next_empty_loc() { 
			return (_get_info().empty) ? ((void*)((uint8_t*)s_Memory + _get_info().filled)) : nullptr;
		}
		static void _init_fundamentals();
		static void _create_context_block();
		static void _expand_reserve(size_t size);

	public:
		static void init();
		template<ngmobj_type T> static void create() {

		}
	};


}