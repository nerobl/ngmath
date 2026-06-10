#pragma once
#include "errors.h"
#include "ngmobj.h"
//memory 
#define NGM_MEMORY_MAX_BLOCKS                      32
#define NGM_CONTEXT_TO_RESERVE                     (4 * (sizeof(context) + ngm_obj_memory_block_reserve_size<argument_set>::size))

#define NGM_ARGUMENT_MEMORY_BLOCK_RESERVE_SIZE     1024
#define NGM_ARGUMENT_SET_MEMORY_BLOCK_RESERVE_SIZE (sizeof(argument_set) + ngm_obj_memory_block_reserve_size<argument>::size)
#define NGM_CONTEXT_MEMORY_BLOCK_RESERVE_SIZE      (4 * (sizeof(context) + ngm_obj_memory_block_reserve_size<argument_set>::size))

#define NGM_ARGUMENT_MEMORY_BLOCK_RESERVE_SIZE     1024

#define NGM_MAX_MEMORY                         10000000 
#define NGM_MEMORY_INFO_VAR_TYPE               size_t

#define NGM_DEFINE_NGM_OBJ_RESERVE_SIZE(type, _size)\
template<> struct ngmath::ngm_obj_memory_block_reserve_size<##type> {static constexpr size_t size = _size;}

class block;
template<typename T> class ngm_block;
template<typename T> class ngm_nested_block;
struct obj_type_id;

//logger
#define NGM_LOGGER_BUFFER_SIZE                 1024
#define NGM_LOG_DEBUG(mssg)                    log_debug(mssg)

//FORWARD DECLARATIONS
//main
class ngdata;

//contexts
class context;

//arguments
class argument;

#include "errors.h"

