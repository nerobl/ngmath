#pragma once
#include "../argument/argument.h"

namespace ngmath {
	class variable : public argument {
	private:
	public:
		size_t bytes() const override { return sizeof(variable); }
	};
}