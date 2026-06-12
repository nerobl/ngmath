#pragma once
#include <stdlib.h>
#include "../log/formatter.h"
namespace ngmath {

	struct memory_info {
		size_t totalObjIDs = 0;
		size_t empty = 0;
		size_t filled = 0;
		size_t totalBlocks = 0;
		size_t blockIDSlots = 0;
		//offsets
		size_t blockIDOffset = 0;
		size_t infoOffset = 0;
		inline double used_kb() const { return (double)(empty + filled) / 1000.0; }
		inline double filled_kb() const { return ((double)(filled)) / 1000.0; }
		inline const block_id* get_block_ids(const void* mem) {
			return (const block_id*)NGM_INCREMENT_BYTE_LOC(mem, blockIDOffset);
		}

	};

	class memory {
	private:

		static void* s_Memory;
		static block_id* s_BlockIDs;
		static block* s_Contexts;
		static memory_info* s_Info;

		static void* _ngm_malloc(size_t size);

		/*
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
		*/

		static void _init_info(const allocater& _allocater, const allocater_obj<memory_info>& info);
		static void _reset_vars_after_reallocation(void* memory, const memory_info& info, size_t newSize);
		static obj_type_id* _get_obj_id(obj_type_id* last);
		static memory_info& _get_info() { return *s_Info; }
		static size_t _total_bytes_used() { return _get_info().empty + _get_info().filled; }
		static void* _get_next_empty_loc() { 
			return (_get_info().empty) ? ((void*)((uint8_t*)s_Memory + _get_info().filled)) : nullptr;
		}
		static void _init_fundamentals();
		static void _create_main_context_block();
		static void _reallocate_memory(size_t size);
		static block_id* _reg_next_empty_block_id() { 
			block_id* id = (block_id*)(s_BlockIDs + s_Info->totalBlocks);
			--s_Info->blockIDSlots;
			++s_Info->totalBlocks;
			return id;
		}

		template<ngmobj_type T> static ngm_block<T>* _make_new_block() {
			if (s_Info->empty < ngm_block<T>::reserve_size)
				throw NGM_MEMORY_OVERFLOW;
			else if (s_Info->blockIDSlots == 0)
				throw NGM_BLOCK_OVERFLOW;
			block_id* id = _reg_next_empty_block_id();
			void* where = _get_next_empty_loc();
			ngm_block<T>* obj = new (where) ngm_block<T>(where, s_Info->filled);
			id->loc = obj;
			s_Info->empty -= ngm_block<T>::reserve_size;
			s_Info->filled += ngm_block<T>::reserve_size;
			return obj;
		}
		template<typename T> static T* _make_obj_at_next_avail() {
			if (s_Info->empty < sizeof(T))
				throw NGM_MEMORY_OVERFLOW;
			T* obj = new (_get_next_empty_loc()) T();
			s_Info->empty -= sizeof(T);
			s_Info->filled += sizeof(T);

		}
	public:
		static bool init();
		template<typename T> static const block_id* get_block_id_for_type() {
			const block_id* blockids = s_Info->get_block_ids(s_Memory);
			for (size_t i = 0; i < s_Info->totalBlocks; ++i)
				if (blockids[i].loc->block_is_for_type(typeid(T)))
					return &blockids[i];
			return nullptr;
		}
	};

	template<> struct type_formatter<memory_info> {
		static constexpr const char* str =
			"Memory Info:"
			"\n\tUsed:       %.2f KB"
			"\n\tFilled:     %.2f KB"
			"\n\tBlocks:     %zu"
			"\n\tMax Blocks: %zu";
		static void fmt(char* buffer, const memory_info& info) {
			snprintf(buffer, NGM_FORMAT_TEMP_BUFFER_SIZE, str, info.used_kb(), info.filled_kb(), info.totalBlocks, info.blockIDSlots);
		}
	};

	bool initialize_ngmath_memory();

}