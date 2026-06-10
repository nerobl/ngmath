#pragma once
#include "../core.h"
namespace ngmath {

	struct block_id { 
		size_t m_ByteLoc = 0;
		block* m_Location = nullptr; 
	};

	class block {
	protected:
		size_t m_Filled = 0;
		size_t m_Empty = 0;
	private:
		virtual void _init_stats() = 0;
	public:
		virtual ~block() {}
	};

	template<ngmobj_type T> class ngm_block : public block {
	public:
		static constexpr size_t reserve_size = ngm_obj_memory_block_reserve_size<T>::size;
		friend class memory;
	private:
		T* m_Begin = nullptr;
		T* m_Current = nullptr;
		void _init_stats() override {}
	public:
		ngm_block() {}
	};

	template<ngmobj_type T> class ngm_nested_block : public block {
	public:
		static constexpr size_t reserve_size = ngm_obj_memory_block_reserve_size<T>::size;
		friend class memory;
	private:
		T* m_Begin = nullptr;
		T* m_Current = nullptr;
		void _init_stats() override {}
	public:
		ngm_nested_block() {

		}
	};



}