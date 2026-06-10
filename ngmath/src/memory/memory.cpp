#include "ngmpch.h"
#include "../context/context.h"
#include "block.h"
#include "memory.h"
#include "allocater.h"
#include "../log/logger.h"
namespace ngmath {

	void* memory::s_Memory = nullptr;
	//ngm_block<context> memory::s_Contexts;

	
	void allocater::_allocate_data(void*& data) {

		data = malloc(m_Size + m_Reserve + m_IDOffset);
		if (!data) {
			throw NGM_MALLOC_FAILURE;
			return;
		}

		size_t idoffset = 0;
		size_t offset = m_IDOffset;

		for (allocater_indexer& indexer : m_Indexers) {

			indexer.offset = offset;
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

	void memory::_init_fundamentals()
	{
			allocater _allocater;
			//Push initializers & allocate
			_allocater._push_initializer<memory_info>();
			allocater_obj<logger>& log = _allocater._push_initializer<logger>();
			allocater_obj<block_id>& blockIDs = _allocater._push_initializer<block_id, NGM_MEMORY_MAX_BLOCKS>();
			_allocater._reserve_block<context>();
			_allocater._allocate_data(s_Memory);		
			//Initialize variables
			logger::s_Instance = log.obj;
			NGM_LOG_DEBUG("Initialized logger");
			memory_info& info = _get_info();
			info.empty = _allocater.m_Reserve;
			info.filled = _allocater.m_Size;
			info.blockIDs = blockIDs.obj;
			info.blockIDSlots = NGM_MEMORY_MAX_BLOCKS;
			info.totalBlocks = 0;
			info.totalObjIDs = _allocater.m_Indexers.size();
			NGM_LOG_DEBUG("Initialized memory");
			_allocater._clear_objs();

			//Initialize context
	}

	void memory::_create_context_block()
	{
		size_t& slots = _get_info().blockIDSlots;
		if (slots == 0) {
			throw NGM_BLOCK_LIMIT;
			return;
		}
		size_t& avail = _get_info().empty;
		if (avail < ngm_block<context>::reserve_size)
			_expand_reserve(ngm_block<context>::reserve_size - avail);
		void* loc = _get_next_empty_loc();
	}

	void memory::_expand_reserve(size_t size)
	{
		size_t objIds = _get_info().totalObjIDs;
		size_t newSize = size + _total_bytes_used();
		void* memory = _ngm_malloc(newSize);
		size_t idoffset = 0;
		for (size_t i = 0; i < objIds; ++i) {
			obj_type_id* objid = (obj_type_id*)((uint8_t*)s_Memory + idoffset);
			objid->move_data(memory, s_Memory);
			idoffset += objid->obj_type_id_size();
		}
	}

	void memory::init()
	{
		if (s_Memory)
			throw NGM_ERROR_MEMORY_REINITIATION;
		_init_fundamentals();
		_create_context_block();


	}
}
