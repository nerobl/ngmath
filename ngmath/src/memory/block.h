#pragma once
#include "../core.h"
#include <stdio.h>
#include <typeinfo>
#include "../log/formatter.h"
namespace ngmath {

	struct block_id { block* loc = nullptr; };

	class block {
	protected:
		size_t m_MemOffset = 0;
		size_t m_Filled = 0;
		size_t m_Empty = 0;
		void* m_Begin = nullptr;
		void* m_Current = nullptr;
		void _increment_fill_amount(size_t amount) {
			m_Filled += amount;
			m_Empty -= amount;
			m_Current = NGM_INCREMENT_BYTE_LOC(m_Current, amount);
		}
		bool _check_fill_err(size_t amount);
		inline void _set_obj_block(ngmobj* obj) { obj->_set_block(this); }

	private:
		virtual void _init_stats() = 0;
	public:
		virtual ~block() {}
		inline size_t bytes_filled() const { return m_Filled; }
		inline size_t bytes_empty() const { return m_Empty; }
		inline size_t bytes_used() const { return m_Filled + m_Empty; }
		inline size_t memory_byte_offset() const { return m_MemOffset; }
		inline size_t cur_memory_offset() const { return m_MemOffset + ((size_t)m_Current - (size_t)m_Begin); }
		virtual size_t get_reserve_size() const = 0;
		virtual bool block_is_for_type(const std::type_info& _type) const = 0;
		virtual void* get_objs() const = 0;
		void reg_nested_block(block* b);

	};



	template<ngmobj_type T> class ngm_block : public block {
	public:
		static constexpr size_t reserve_size = sizeof(block) + ngm_obj_memory_block_reserve_size<T>::size;
		friend class memory;
	private:
		void _init_stats() override {}
		ngm_block() {}
		ngm_block(void* begin, size_t loc) {
			m_MemOffset = loc;
			m_Empty = reserve_size;
			m_Begin = begin;
			m_Current = begin;
			_increment_fill_amount(sizeof(ngm_block<T>));
		}
	public:
		T* make_obj() {
			if (m_Empty < sizeof(T))
				throw NGM_MEMORY_OVERFLOW;
			T* obj = new (m_Current) T();
			_increment_fill_amount(sizeof(T));
			_set_obj_block(obj);
			return obj;
		}
		inline size_t get_reserve_size() const override { return reserve_size; }
		inline bool block_is_for_type(const std::type_info& _type) const override { return typeid(T) == _type; }
		inline void* get_objs() const override { return NGM_INCREMENT_BYTE_LOC(m_Begin, sizeof(ngm_block<T>)); }
	};

	template<ngmobj_type T> class ngm_nested_block : public block {
	public:
		static constexpr size_t reserve_size = sizeof(block) + ngm_obj_memory_block_reserve_size<T>::size;
	private:
		void _init_stats() override {}

	public:
		ngm_nested_block(block* parent) { parent->reg_nested_block(this); }
		ngm_nested_block() {}
		T* make_obj() {
			if (_check_fill_err(sizeof(T)))
				return nullptr;
			T* obj = new (m_Current) T();
			_increment_fill_amount(sizeof(T));
			_set_obj_block(obj);
			return obj;
		}
		inline size_t get_reserve_size() const override { return reserve_size; }
		inline bool block_is_for_type(const std::type_info& _type) const override { return typeid(T) == _type; }
		inline void* get_objs() const override { return NGM_INCREMENT_BYTE_LOC(m_Begin, sizeof(ngm_nested_block<T>)); }

	};

	template<> struct type_formatter<block> {
		size_t m_Filled = 0;
		size_t m_Empty = 0;
		void* m_Begin = nullptr;
		void* m_Current = nullptr;
		static constexpr const char* str =
			"Block Info:"
			"\n\tUsed:       %zu Bytes"
			"\n\tFilled:     %zu Bytes"
			"\n\tLocation:   %zu";
		static void fmt(char* buffer, const block& block) {
			snprintf(buffer, NGM_FORMAT_TEMP_BUFFER_SIZE, str, block.bytes_used(), block.bytes_filled(), block.memory_byte_offset());
		}
	};

	template<typename T> struct is_ngmath_mem_block : std::is_base_of<block, T> {};
	template<typename T> inline constexpr bool is_ngmath_mem_block_v = is_ngmath_mem_block<T>::value;
	template<typename T> concept ngmath_mem_block_type = is_ngmath_mem_block_v<T>;

}