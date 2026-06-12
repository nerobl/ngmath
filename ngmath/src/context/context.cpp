#include "ngmpch.h"
#include "context.h"
#include "../memory/memory.h"
#include "../log/logger.h"
namespace ngmath {
	context_root* context_root::s_Main = nullptr;
	void context_root::_set_block(block* _block)
	{
		if (s_Main) {
			throw NGM_MAIN_CONTEXT_REINITIATION;
			return;
		}
		_block->reg_nested_block(&m_ArgSet);
		_block->reg_nested_block(&m_Children);
		argument_set* mainset = m_ArgSet.make_obj();
		s_Main = this;
	}

	void context_root::init()
	{
	}

}
