#pragma once
#include "argument.h"
namespace ngmath {

	class argument_set : public ngmobj {
	private:
		ngm_nested_block<argument> m_Arguments;
	public:

		friend class memory;
	};

	NGM_DEFINE_NGM_OBJ_RESERVE_SIZE(argument_set, NGM_ARGUMENT_SET_MEMORY_BLOCK_RESERVE_SIZE);
}