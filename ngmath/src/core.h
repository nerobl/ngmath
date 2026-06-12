#pragma once
#include "errors.h"
//memory 
#define NGM_MEMORY_MAX_BLOCKS                      128
#define NGM_CONTEXTS_TO_RESERVE                    1

#define NGM_ARGUMENT_MEMORY_BLOCK_RESERVE_SIZE     1024
#define NGM_CONTEXT_ROOT_MEMORY_BLOCK_RESERVE_SIZE 65536

#define NGM_ARGUMENT_SET_MEMORY_BLOCK_RESERVE_SIZE (sizeof(argument_set) + ngm_obj_memory_block_reserve_size<argument>::size)
#define NGM_CONTEXT_MEMORY_BLOCK_RESERVE_SIZE      (sizeof(context) + ngm_obj_memory_block_reserve_size<argument_set>::size)

#define NGM_ARGUMENT_MEMORY_BLOCK_RESERVE_SIZE     1024

#define NGM_MAX_MEMORY                         10000000 
#define NGM_MEMORY_INFO_VAR_TYPE               size_t

#define NGM_INCREMENT_BYTE_LOC(data, bytes) ((uint8_t*)data + bytes)

#define NGM_DEFINE_NGM_OBJ_RESERVE_SIZE(type, _size)\
template<> struct ngmath::ngm_obj_memory_block_reserve_size<##type> {static constexpr size_t size = _size;}

//logger
#define NGM_FORMAT_ANUM_BUFFER_SIZE            16
#define NGM_FORMAT_MAP_SIZE                    1024
#define NGM_FORMAT_TEMP_BUFFER_SIZE             256
#define NGM_LOGGER_BUFFER_SIZE                 1024
#define NGM_LOG_DEBUG(mssg, ...)               log_debug(mssg, __VA_ARGS__)
#define NGM_DEFINE_TYPE_LOG_FORMAT(type, _str)  template<> struct type_formatter<##type> { static constexpr const char* str = _str; };
using log_size_t = size_t;

//arguments
#define NGM_ARGUMENT_DICTIONARY_SIZE           1024
//forward declarations
namespace ngmath {
	class block;
	struct obj_type_id;
	class context;
	class argument;
	class allocater;
	template<typename T> struct allocater_obj;
	template<typename T> struct obj_id;
}

#include "ngmobj.h"

