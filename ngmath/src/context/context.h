#pragma once
#include "../argument/argument_set.h"

namespace ngmath {

}
namespace ngmath {

	class context : public ngmobj {
	protected:
		ngm_nested_block<argument_set> m_ArgSet;
		void _set_block(block* _block) override {}
	public:
		context() {}
		virtual ~context() {}
	};

	NGM_DEFINE_NGM_OBJ_RESERVE_SIZE(context, sizeof(context) + ngm_obj_memory_block_reserve_size<argument_set>::size);


	class context_root : public context {
	private:
		static context_root* s_Main;
		ngm_nested_block<context> m_Children;
	protected:
		void _set_block(block* _block) override;
	public:
		context_root() {}
		context_root(const context_root& other) = delete;
		context_root& operator=(const context_root& other) = delete;
		context_root(context_root&& other) noexcept;
		context_root& operator=(context_root&& other) noexcept;
		~context_root() {}

		//static context_root* GetInstance() { return s_Main; }
		static void init();
	};
	//NGM_DEFINE_NGM_OBJ_RESERVE_SIZE(context_root, sizeof(context_root) + NGM_CONTEXTS_TO_RESERVE * ngm_obj_memory_block_reserve_size<context>::size);
	NGM_DEFINE_NGM_OBJ_RESERVE_SIZE(context_root, NGM_CONTEXT_ROOT_MEMORY_BLOCK_RESERVE_SIZE);

	void initialize_ngmath_contexts();
//	context_root* get_main_context();
}
