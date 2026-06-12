#pragma once
#include <typeinfo>
#include <cstdint>
namespace ngmath {

	using obj_mover = size_t(*)(void*, void*, size_t, size_t, size_t);

	struct obj_type_id {
		size_t elements = 0;
		size_t offset = 0;
		static void* get(void* data, size_t _offset) { return ((uint8_t*)data + _offset); }
		//void* get(void* data) const { return ((uint8_t*)data + offset); }
		//virtual void move_data(void* to, void* from) const = 0;
		virtual obj_mover get_mover() = 0;
		virtual size_t obj_type_id_size() const = 0;
		virtual bool is_type(const std::type_info& _type) const = 0;
	};

	template<typename T> struct obj_id : public obj_type_id {
		static obj_id* get_id_obj(void* data, size_t offset) {
			return (obj_id*)((uint8_t*)data + offset);
		}
		static size_t move_data(void* to, void* from, size_t elems, size_t offset, size_t idoffset) {
			T* _from = (T*)get(from, offset);
			T* _to = (T*)get(to, offset);
			for (size_t i = 0; i < elems; ++i) {
				new (&_to[i]) T(std::move(_from[i]));
				_from[i].~T();
			}
			obj_id* old = get_id_obj(from, idoffset);
			obj_id* _new = get_id_obj(to, idoffset);
			new (_new) obj_id(std::move(*old));
			(*old).~obj_id<T>();
			return _new->obj_type_id_size();
		}

		inline obj_mover get_mover() override { return move_data; }
		inline size_t obj_type_id_size() const override { return sizeof(obj_id<T>); }
		inline bool is_type(const std::type_info& _type) const override { return _type == typeid(T); }
	};
}