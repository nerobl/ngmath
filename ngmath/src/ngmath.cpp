#include "ngmpch.h"
#include "ngmath.h"
#include "memory/memory.h"
#include "log/logger.h"
#include "argument/argument_set.h"
#include <stdio.h>

namespace ngmath {



	void init() {

		printf("Initializing ngmath...\n");
		memory::init();
		printf("Finished initializing ngmath.\n");

	}

	void shutdown()
	{

	}

	void sandbox()
	{
		NumericalHash hash = generate_numerical_data_type_hash<int>();
		NumericalHash hash2 = generate_numerical_data_type_hash<double>();
		NumericalHash hash3 = get_numerical_type_hash<real<int>>::NumericalHash;
		NumericalHash hash4 = get_numerical_type_hash<complex<double>>::NumericalHash;
		NumericalHash size0 = get_numerical_type_hash<complex<double>>::bytes;
		NumericalHash size1 = get_numerical_type_hash<complex<double>>::bytes;

		numerical_type_info info;
		size_t x = sizeof(double);
		get_numerical_info_from_hash(hash3, info);
		get_numerical_info_from_hash(hash4, info);
	}
}