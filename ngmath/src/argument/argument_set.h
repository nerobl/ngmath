#pragma once
#include "argument.h"
namespace ngmath {

	class argument_id {
		const char* id = "?";
	};

	class argument_set : public ngmobj {
	private:
		char m_Dictionary[NGM_ARGUMENT_DICTIONARY_SIZE];
		ngm_nested_block<argument> m_Arguments;
		void _set_block(block* _block);
		void _reg_argument_id(argument_id& id);
	public:
		argument_set();
	};

	NGM_DEFINE_NGM_OBJ_RESERVE_SIZE(argument_set, sizeof(argument_set) + ngm_obj_memory_block_reserve_size<argument>::size);
}