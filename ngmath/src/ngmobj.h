#pragma once
#include <type_traits>

namespace ngmath {
	class ngmobj {
	private:
	public:
		virtual ~ngmobj() {}
	};

	template<typename T> struct is_ngmath_obj : std::is_base_of<ngmobj, T> {};
	template<typename T> inline constexpr bool is_ngmath_obj_v = is_ngmath_obj<T>::value;
	template<typename T> concept ngmobj_type = is_ngmath_obj_v<T>;

	template<ngmobj_type T> struct ngm_obj_memory_block_reserve_size;
}