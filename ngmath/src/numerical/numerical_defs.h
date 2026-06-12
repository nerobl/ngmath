#pragma once
#include <cstdint>
#include <type_traits>

namespace ngmath {
	typedef uint32_t NumericalHashComponent;
	typedef NumericalHashComponent NumericalHash;
	constexpr NumericalHashComponent NumericalHashComponentBits = 8;
	constexpr NumericalHashComponent NumericalHashOffset_type = NumericalHashComponentBits * 3;
	constexpr NumericalHashComponent NumericalHashOffset_sign = NumericalHashComponentBits * 2;
	constexpr NumericalHashComponent NumericalHashOffset_rep = NumericalHashComponentBits * 1;
	constexpr NumericalHashComponent NumericalHashOffset_bytes = 0;

	constexpr NumericalHashComponent NumericalHashZeroer_type = 0x00FFFFFF;
	constexpr NumericalHashComponent NumericalHashZeroer_sign = 0xFF00FFFF;
	constexpr NumericalHashComponent NumericalHashZeroer_rep = 0xFFFF00FF;
	constexpr NumericalHashComponent NumericalHashZeroer_bytes = 0xFFFFFF00;

	constexpr NumericalHashComponent NumericalHashComp_unsigned = 1;
	constexpr NumericalHashComponent NumericalHashComp_signed = 2;

	constexpr NumericalHashComponent NumericalHashComp_integ = 1;
	constexpr NumericalHashComponent NumericalHashComp_fp = 2;



	template<typename T> concept fund_numerical_type = (std::is_integral_v<T> || std::is_floating_point_v<T>);
	template<typename T> concept integral_data_type = std::is_integral_v<T>;
	template<typename T> concept size_t_data_type = std::is_same_v<T, size_t>;
	template<typename T> concept signed_integral_data_type = (std::is_integral_v<T> && std::is_signed_v<T>);
	template<typename T> concept unsigned_integral_data_type = (std::is_integral_v<T> && !std::is_signed_v<T>);
	template<typename T> concept floating_point_data_type = std::is_floating_point_v<T>;

	template<typename T> struct is_signed_integral_data_type : std::false_type {};
	template<signed_integral_data_type T> struct is_signed_integral_data_type<T> : std::true_type {};
	template<typename T> inline constexpr bool is_signed_integral_data_type_v = is_signed_integral_data_type<T>::value;

	template<typename T> struct is_unsigned_integral_data_type : std::false_type {};
	template<unsigned_integral_data_type T> struct is_unsigned_integral_data_type<T> : std::true_type {};
	template<typename T> inline constexpr bool is_unsigned_integral_data_type_v = is_unsigned_integral_data_type<T>::value;

	template<typename T> struct is_floating_point_data_type : std::false_type {};
	template<floating_point_data_type T> struct is_floating_point_data_type<T> : std::true_type {};
	template<typename T> inline constexpr bool is_floating_point_data_type_v = is_floating_point_data_type<T>::value;

	template<typename T> struct is_size_t_data_type : std::false_type {};
	template<size_t_data_type T> struct is_size_t_data_type<T> : std::true_type {};
	template<typename T> inline constexpr bool is_size_t_data_type_v = is_size_t_data_type<T>::value;


	template<typename T> struct is_numerical_data_type : std::false_type {};
	template<integral_data_type T> struct is_numerical_data_type<T> : std::true_type {};
	template<floating_point_data_type T> struct is_numerical_data_type<T> : std::true_type {};
	template<typename T> inline constexpr bool is_numerical_data_type_v = is_numerical_data_type<T>::value;
	template<typename T> concept numerical_data_type = is_numerical_data_type_v<T>;


	struct numerical_type_info {
		NumericalHashComponent type = 0;
		NumericalHashComponent rep = 0;
		NumericalHashComponent sign = 0;
		NumericalHashComponent bytes = 0;
	};

}
