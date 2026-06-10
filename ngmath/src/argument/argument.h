#pragma once
#include "../memory/block.h"
namespace ngmath {

	class argument : public ngmobj {
	public:
		virtual ~argument() {}
		virtual size_t bytes() const = 0;
	};

	template<typename T> struct is_argument_type : std::is_base_of<argument, std::remove_cvref_t<T>> {};
	template<typename T> inline constexpr bool is_is_argument_type_v = is_argument_type<T>::value;
	NGM_DEFINE_NGM_OBJ_RESERVE_SIZE(argument, NGM_ARGUMENT_MEMORY_BLOCK_RESERVE_SIZE);

}
