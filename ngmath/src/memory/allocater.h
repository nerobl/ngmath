#pragma once
#include "../errors.h"
#include <vector>
#include "obj_id.h"
namespace ngmath {

	using intializer_func = void*(*)(size_t, void*);



	struct _allocater_obj {
		virtual ~_allocater_obj() {}
		virtual void set(void* _obj) = 0;
		virtual obj_type_id* make_id(void* _obj) = 0;
		virtual size_t get_id_size() = 0;
		virtual obj_type_id* get_type_id() = 0;
	};

	template<typename T>
	struct allocater_obj : public _allocater_obj {
		obj_id<T> id;
		T* obj = nullptr;
		inline void set(void* _obj) override { obj = (T*)_obj; }
		obj_type_id* make_id(void* _obj) override {
			obj_id<T>* _id = new (_obj) obj_id<T>();
			return _id;
		}
		inline size_t get_id_size() override { return sizeof(obj_id<T>); }
		inline obj_type_id* get_type_id() override { return &id; }

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
				obj->set(func(offset, data)); 
				void* idloc = (void*)((uint8_t*)data + idoffset);
				obj_type_id* id = obj->make_id(idloc);
				id->offset = offset;
				id->elements = obj->get_type_id()->elements;
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