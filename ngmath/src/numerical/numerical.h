#pragma once
#include "../argument/argument.h"
#include "numerical_defs.h"

namespace ngmath {
	class numerical : public argument {
	public:
		virtual ~numerical() {}
		virtual size_t bytes() const = 0;
	};

	template<typename T> struct is_numerical_type : std::is_base_of<numerical, T> {};
	template<typename T> inline constexpr bool is_numerical_type_v = is_numerical_type<T>::value;
	template<typename T> concept numerical_type = is_numerical_type_v<T>;
}

