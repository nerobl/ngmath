#pragma once
#include "../errors.h"
#include <vector>
#include "obj_id.h"
namespace ngmath {

	using intializer_func = void*(*)(size_t, void*);



	struct _allocater_obj {
		virtual ~_allocater_obj() {}
		virtual void set_offset(size_t offset) = 0;
		virtual void set(void* _obj) = 0;
		virtual void make_id(void* data, size_t idoffset) const = 0;
		virtual size_t get_id_size() const = 0;
		virtual size_t get_offset() const = 0;
		virtual obj_type_id* get_type_id() = 0;
		virtual obj_type_id* get_next(size_t& offset) const = 0;
	};

	template<typename T>
	struct allocater_obj : public _allocater_obj {
		obj_id<T> id;
		T* obj = nullptr;
		inline void set_offset(size_t offset) override { id.offset = offset; }
		inline void set(void* _obj) override { obj = (T*)_obj; }
		inline void make_id(void* data, size_t idoffset) const override {
			obj_id<T>* _id = (obj_id<T>*)((uint8_t*)data + idoffset);
			new (_id) obj_id<T>();
			_id->elements = id.elements;
			_id->offset = id.offset;
		}
		inline size_t get_id_size() const override { return sizeof(obj_id<T>); }
		inline size_t get_offset() const override { return id.offset; }
		inline obj_type_id* get_type_id() override { return &id; }
		obj_type_id* get_next(size_t& offset) const override {
			offset += sizeof(obj_id<T>);
			return (obj_type_id*)((uint8_t*)this + sizeof(obj_id<T>));
		}

		inline T* operator->() { return obj; }
		inline T& operator*() { return *obj; }
	};

	/*
	template<typename _T>
	struct allocater_obj<ngm_block<_T>> : public _allocater_obj {
		ngm_block<_T>* obj = nullptr;
		inline void set(void* _obj) override { obj = (ngm_block<_T>*)_obj; }
		inline ngm_block<_T>* operator->() { return obj; }
		inline ngm_block<_T>& operator*() { return *obj; }
	};
	*/

	class allocater {
	private:

		struct allocater_indexer {
			_allocater_obj* obj = nullptr;
			size_t offset = 0;
			size_t size = 0;
			intializer_func func;
			inline void trigger(void* data, size_t idoffset) { 
				obj->set(func(obj->get_offset(), data));
				obj->make_id(data, idoffset);
			}
		};



		std::vector<_allocater_obj*> m_Objs;
		std::vector<allocater_indexer> m_Indexers;
		size_t m_IDOffset = 0;
		size_t m_Size = 0;
		size_t m_Reserve = 0;

		template<typename T, size_t size> static void* _obj_initializer(size_t offset, void* data) {
			T* var = (T*)((uint8_t*)data + offset);
			for (size_t i = 0; i < size; ++i)
				new (&var[i]) T();
			return (void*)var;
		}



		template<typename T, size_t size> static T& _obj_getter(size_t offset, void* data) {
			T* var = (T*)((uint8_t*)data + offset);
			for (size_t i = 0; i < size; ++i)
				new (&var[i]) T();
		}

		template<typename T, size_t N>
		allocater_obj<T>& _push_initializer(intializer_func func) {
			m_IDOffset += sizeof(obj_id<T>);
			m_Size += sizeof(T) * N;
			void* rawobj = malloc(sizeof(allocater_obj<T>));
			if (!rawobj)
				throw NGM_MALLOC_FAILURE;
			allocater_obj<T>* obj = new (rawobj) allocater_obj<T>();
			allocater_indexer& indexer = m_Indexers.emplace_back();
			indexer.obj = obj;
			indexer.size = sizeof(T) * N;
			indexer.func = func;
			m_Objs.emplace_back(obj);
			obj->id.elements = N;
			return *obj;
		}

		template<typename T, size_t N>
		allocater_obj<T>& _push_initializer() { return _push_initializer<T, N>(_obj_initializer<T,N>); }

		template<typename T>
		allocater_obj<T>& _push_initializer() { return _push_initializer<T, 1>(_obj_initializer<T,1>); }

		template<typename T>
		void _reserve_block() { m_Reserve += ngm_block<T>::reserve_size;}
		/*
		template<typename T> allocater_obj<ngm_block<T>>& _push_block_initializer() {
			return _push_initializer<ngm_block<T>>(_obj_initializer<ngm_block<T>, 1>, ngm_block<T>::reserve_size);
		}
		*/

		void _allocate_data(void*& data);
		void _clear_objs();

	public:
		allocater() {}
		friend class memory;

	};
}