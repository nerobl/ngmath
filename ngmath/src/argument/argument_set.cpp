#include "ngmpch.h"
#include "argument_set.h"

namespace ngmath {
	void argument_set::_set_block(block* _block)
	{
		_block->reg_nested_block(&m_Arguments);
	}
	void argument_set::_reg_argument_id(argument_id& id)
	{
	}
	argument_set::argument_set()
	{
		memset(&m_Dictionary, 0, NGM_ARGUMENT_DICTIONARY_SIZE);
	}
}
