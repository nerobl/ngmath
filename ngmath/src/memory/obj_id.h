#pragma once
#include <cstdint>
namespace ngmath {
	struct obj_type_id {
		size_t elements = 0;
		size_t offset = 0;
		void* get(void* data) const { return ((uint8_t*)data + offset); }
		virtual void move_data(void* to, void* from) const = 0;
		virtual size_t obj_type_id_size() const = 0;
	};

	template<typename T> struct obj_id : public obj_type_id {
		void move_data(void* to, void* from) const override {
			T* _from = (T*)get(from);
			T* _to = (T*)get(to);
			for (size_t i = 0; i < elements; ++i) {
				new (&_to[i]) T(std::move(_from[i]));
				_from[i].~T();
			}
		}

		size_t obj_type_id_size() const override { return sizeof(obj_id<T>); }
	};
}