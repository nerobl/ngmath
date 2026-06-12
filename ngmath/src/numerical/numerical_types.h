#pragma once
#include "numerical.h"

namespace ngmath {


	class numerical_real : public numerical {
	public:
		virtual ~numerical_real() {}
		virtual size_t bytes() const = 0;
	};

	class numerical_complex : public numerical {
	public:
		virtual ~numerical_complex() {}
		virtual size_t bytes() const = 0;
	};

	class numerical_quaternion : public numerical {
	public:
		virtual ~numerical_quaternion() {}
		virtual size_t bytes() const = 0;
	};

	template<numerical_data_type T>
	class real : public numerical_real {
	public:
		T v = 0;
		real() {}
		real(T _v) : v(_v) {}
		size_t bytes() const override { return sizeof(real<T>); }
	};

	template<numerical_data_type T>
	class complex : public numerical_complex {
	public:
		T r = 0;
		T i = 0;
		complex() {}
		complex(T _r, T _i) : r(_r), i(_i) {}
		size_t bytes() const override { return sizeof(complex<T>); }

	};

	template<numerical_data_type T>
	class quaternion : public numerical_quaternion {
	public:
		T s = 0;
		T i = 0;
		T j = 0;
		T k = 0;
		quaternion() {}
		quaternion(T _s, T _i, T _j, T _k) : s(_s), i(_i), j(_j), k(_k) {}
		size_t bytes() const override { return sizeof(quaternion<T>); }


	};


	template<numerical_type T> struct underlying_numerical_data_type;
	template<numerical_data_type D> struct underlying_numerical_data_type<real<D>> { using type = D; };
	template<numerical_data_type D> struct underlying_numerical_data_type<complex<D>> { using type = D; };
	template<numerical_data_type D> struct underlying_numerical_data_type<quaternion<D>> { using type = D; };
	template<numerical_type T> using underlying_numerical_data_type_t = typename underlying_numerical_data_type<T>::type;

	template<numerical_type T> struct numerical_type_hash;
	template<> struct numerical_type_hash<numerical_real> {
		static constexpr NumericalHashComponent val = 1;
	};
	template<> struct numerical_type_hash<numerical_complex> {
		static constexpr NumericalHashComponent val = 2;
	};
	template<> struct numerical_type_hash<numerical_quaternion> {
		static constexpr NumericalHashComponent val = 3;
	};
	template<numerical_data_type D> struct numerical_type_hash<real<D>> {
		static constexpr NumericalHashComponent val = 1;
	};
	template<numerical_data_type D> struct numerical_type_hash<complex<D>> {
		static constexpr NumericalHashComponent val = 2;
	};
	template<numerical_data_type D> struct numerical_type_hash<quaternion<D>> {
		static constexpr NumericalHashComponent val = 3;
	};

	//NGM_DEFINE_TYPE_LOG_FORMAT(real<double>, "%f");
	
}




