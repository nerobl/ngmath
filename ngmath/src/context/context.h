#pragma once
#include "../argument/argument_set.h"
namespace ngmath {

	class context : public ngmobj {
	private:
		static block* s_Block;
		static context* s_Main;
		const context* m_Parent;

		ngm_nested_block<argument_set> m_Arguments;
	public:
		context();
		context(const context& other) = delete;
		context& operator=(const context& other) = delete;
		context(context&& other) noexcept;
		context& operator=(context&& other) noexcept;
		~context();

		friend class memory;
	};
	NGM_DEFINE_NGM_OBJ_RESERVE_SIZE(context, NGM_CONTEXT_MEMORY_BLOCK_RESERVE_SIZE);
}
