#include "ngmpch.h"
#include "../context/context.h"
#include "block.h"
#include "allocater.h"
#include "memory.h"
#include "../log/logger.h"
namespace ngmath {

	void* memory::s_Memory = nullptr;
	memory_info* memory::s_Info = nullptr;
	block_id* memory::s_BlockIDs = nullptr;

	
	void allocater::_allocate_data(void*& data) {

		data = malloc(m_Size + m_Reserve + m_IDOffset);
		if (!data) {
			throw NGM_MALLOC_FAILURE;
			return;
		}

		size_t idoffset = 0;
		size_t offset = m_IDOffset;

		for (allocater_indexer& indexer : m_Indexers) {

			indexer.obj->set_offset(offset);
			indexer.trigger(data, idoffset);
			offset += indexer.size;
			idoffset += indexer.obj->get_id_size();
		}

	}

	void allocater::_clear_objs()
	{
		for (_allocater_obj* obj : m_Objs)
			free(obj);
		m_Objs.clear();
	}

	void* memory::_ngm_malloc(size_t size)
	{
		void* obj = nullptr;
		obj = malloc(size);
		if (!obj)
			throw NGM_MALLOC_FAILURE;
		return obj;
	}

	void memory::_init_info(const allocater& _allocater, const allocater_obj<memory_info>& info)
	{
		s_Info = info.obj;
		s_Info->empty = _allocater.m_Reserve;
		s_Info->filled = _allocater.m_Size + _allocater.m_IDOffset;
		s_Info->blockIDSlots = NGM_MEMORY_MAX_BLOCKS;
		s_Info->totalBlocks = 0;
		s_Info->infoOffset = info.get_offset();
		s_Info->totalObjIDs = _allocater.m_Indexers.size();
	}

	void memory::_reset_vars_after_reallocation(void* memory, const memory_info& info, size_t newSize)
	{
		s_Memory = memory;
		size_t offset = 0;
		s_Info = (memory_info*)NGM_INCREMENT_BYTE_LOC(s_Memory, info.infoOffset);
		s_BlockIDs = (block_id*)NGM_INCREMENT_BYTE_LOC(s_Memory, info.blockIDOffset);
		s_Info->empty = newSize - s_Info->filled;
	}

	obj_type_id* memory::_get_obj_id(obj_type_id* last)
	{
		return (!last) ? (obj_type_id*)s_Memory : (obj_type_id*)((uint8_t*)last + last->obj_type_id_size());
	}

	void memory::_init_fundamentals()
	{
			allocater _allocater;
			//Push initializers & allocate
			allocater_obj<memory_info>& info = _allocater._push_initializer<memory_info>();
			allocater_obj<logger>& log = _allocater._push_initializer<logger>();
			allocater_obj<block_id>& blockIDs = _allocater._push_initializer<block_id, NGM_MEMORY_MAX_BLOCKS>();
			_allocater._reserve_block<context_root>();
			_allocater._allocate_data(s_Memory);
			logger::s_Instance = log.obj;
			NGM_LOG_DEBUG("Initialized logger");
			//Initialize variables
			//run_logger_test();
			_init_info(_allocater, info);
			s_Info->blockIDOffset = blockIDs.get_offset();
			s_BlockIDs = blockIDs.obj;
			NGM_LOG_DEBUG("Initialized memory");
			NGM_LOG_DEBUG("{0}", *s_Info);
			_allocater._clear_objs();

			//Initialize context
	}

	void memory::_create_main_context_block()
	{
		NGM_LOG_DEBUG("Creating context block");
		size_t& slots = _get_info().blockIDSlots;
		if (slots == 0) {
			throw NGM_BLOCK_LIMIT;
			return;
		}
		size_t& avail = _get_info().empty;
		if (avail < ngm_block<context_root>::reserve_size)
			_reallocate_memory(ngm_block<context>::reserve_size - avail);
		void* loc = _get_next_empty_loc();
		ngm_block<context_root>* block = _make_new_block<context_root>();
		context_root* context = block->make_obj();
	}

	void memory::_reallocate_memory(size_t size)
	{
		NGM_LOG_DEBUG("Reallocating memory...");
		size_t objIds = _get_info().totalObjIDs;
		size_t newSize = size + _total_bytes_used();
		void* memory = _ngm_malloc(newSize);
		size_t idoffset = 0;
		for (size_t i = 0; i < objIds; ++i) {
			obj_type_id* objid = (obj_type_id*)((uint8_t*)s_Memory + idoffset);
			obj_mover mover = objid->get_mover();
			idoffset += mover(memory, s_Memory, objid->elements, objid->offset, idoffset);
		}
		memory_info info = _get_info();
		free(s_Memory);
		_reset_vars_after_reallocation(memory, info, newSize);
		NGM_LOG_DEBUG("Finished reallocating memory.");
	}

	bool memory::init()
	{
		if (s_Memory) {
			throw NGM_ERROR_MEMORY_REINITIATION;
			return false;
		}
		_init_fundamentals();
		_create_main_context_block();
		return true;

	}

	bool initialize_ngmath_memory()
	{
		if (!memory::init()) {
			throw NGM_MEMORY_INIT_ERROR;
			return false;
		}
	}
}
