#pragma once
#include "../errors.h"
#include "numerical_types.h"

namespace ngmath {
#define NUMERICAL_HASH_SETTER_DEF(name)\
inline NumericalHash set_numerical_##name##_comp(uint32_t hash, NumericalHashComponent type)\
{ return (hash & NumericalHashZeroer_##name) | (type << NumericalHashOffset_##name);}\
inline NumericalHash get_numerical_##name##_comp(uint32_t hash)\
{ return ((hash & ~NumericalHashZeroer_##name) >> NumericalHashOffset_##name);}

	NUMERICAL_HASH_SETTER_DEF(type);
	NUMERICAL_HASH_SETTER_DEF(sign);
	NUMERICAL_HASH_SETTER_DEF(rep);
	NUMERICAL_HASH_SETTER_DEF(bytes);

	template<floating_point_data_type T>
	inline NumericalHash generate_floating_point_type_hash() {
		NumericalHash hash = set_numerical_rep_comp(0, NumericalHashComp_fp);
		hash = set_numerical_sign_comp(hash, NumericalHashComp_signed);
		hash = set_numerical_bytes_comp(hash, sizeof(T));
		return hash;
	}

	template<integral_data_type T>
	inline NumericalHash generate_integral_type_hash() {
		NumericalHash hash = set_numerical_rep_comp(0, NumericalHashComp_integ);
		if constexpr (std::is_signed_v<T>)
			hash = set_numerical_sign_comp(hash, NumericalHashComp_signed);
		else
			hash = set_numerical_sign_comp(hash, NumericalHashComp_unsigned);
		hash = set_numerical_bytes_comp(hash, sizeof(T));
		return hash;
	}

	template<numerical_data_type T>
	inline NumericalHash generate_numerical_data_type_hash() {
		if constexpr (std::is_integral_v<T>)
			return generate_integral_type_hash<T>();
		else if constexpr (std::is_floating_point_v<T>)
			return generate_floating_point_type_hash<T>();
		else
			throw NGM_ERROR_UNKNOWN_NUMERICAL_DATA_TYPE;
		return 0;
	}

	template<numerical_type T>
	inline NumericalHash generate_numerical_type_hash() {
		NumericalHash hash = set_numerical_type_comp(0, numerical_type_hash<T>::val);
		hash |= generate_numerical_data_type_hash<underlying_numerical_data_type_t<T>>();
		return hash;
	}


	template<typename T>  struct get_numerical_data_type_representation {
		static constexpr NumericalHashComponent val = (std::is_integral_v<T>) ? NumericalHashComp_integ : NumericalHashComp_fp;
	};
	template<typename T>  struct get_numerical_data_type_sign {
		static constexpr NumericalHashComponent val = (std::is_integral_v<T>) ? NumericalHashComp_integ : NumericalHashComp_fp;
	};

	template<numerical_type T> struct get_numerical_type_hash {
		static constexpr NumericalHashComponent sign = (std::is_signed_v<T>) ? NumericalHashComp_signed : NumericalHashComp_unsigned;
		static constexpr NumericalHashComponent rep = (std::is_integral_v<T>) ? NumericalHashComp_integ : NumericalHashComp_fp;
		static constexpr NumericalHashComponent bytes = sizeof(underlying_numerical_data_type_t<T>);
		static constexpr NumericalHashComponent type = numerical_type_hash<T>::val;
		static constexpr NumericalHashComponent NumericalHash = (
			(type << NumericalHashOffset_type) |
			(sign << NumericalHashOffset_sign) | 
			(rep << NumericalHashOffset_rep) | 
			(bytes << NumericalHashOffset_bytes)
			);
	};

	inline void get_numerical_info_from_hash(NumericalHash hash, numerical_type_info& info) {
		info.type = get_numerical_type_comp(hash);
		info.rep = get_numerical_rep_comp(hash);
		info.sign = get_numerical_sign_comp(hash);
		info.bytes = get_numerical_bytes_comp(hash);
	}
}


