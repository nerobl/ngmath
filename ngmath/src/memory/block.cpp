#include "ngmpch.h"
#include "block.h"
#include "../log/logger.h"
namespace ngmath {
	bool block::_check_fill_err(size_t amount)
	{
		if (m_Empty < amount) {
			throw NGM_BLOCK_OVERFLOW;
			return true;
		}
		return false;
	}
	void block::reg_nested_block(block* b)
	{
		NGM_LOG_DEBUG("Registering nested block...");
		NGM_LOG_DEBUG("Recipient block info:\n{0}", *this);
		size_t amount = b->get_reserve_size() - sizeof(block);
		NGM_LOG_DEBUG("Reserving {0} bytes", amount);
		if (_check_fill_err(amount))
			return;
		b->m_MemOffset = cur_memory_offset();
		b->m_Empty = amount;
		b->m_Begin = m_Current;
		b->m_Current = m_Current;
		_increment_fill_amount(amount);
		NGM_LOG_DEBUG("After registering nested block:\n{0}", *this);

	}
}
