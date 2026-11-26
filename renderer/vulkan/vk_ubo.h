#ifndef _VULKAN_VKUBO_H
#define _VULKAN_VKUBO_H

#include <stddef.h>

#define VKOFFSET_OF(T, name) offsetof(T, name)
#define VKOFFSET_N_OF(T, name, index) VKOFFSET_OF(T, name[index]) // index is immediate value
#define VKOFFSET_VAR_N_OF(T, name, index) (VKOFFSET_OF(T, name) + sizeof(((T *)0)->name[0]) * (index)) // index ius variable

/**
 * Global uniform variables
 */
typedef struct VKGlobalUniformObject_s
{
#define UBO_DEF(T, x) T x
#define UBO_MAT4(x) UBO_DEF(mat4, x)
#define UBO_VEC4(x) UBO_DEF(vec4, x)
#define UBO_FLOAT(x) UBO_DEF(float, x)
#define UBO_INT(x) UBO_DEF(int, x)
#define UBO_BOOL(x) UBO_DEF(bool32, x)
#define UBO_VEC2(x) UBO_DEF(vec2, x)
#define UBO_MAT4_N(x, n) UBO_MAT4(x)[n]
#define UBO_VEC4_N(x, n) UBO_VEC4(x)[n]
#define UBO_INT_N(x, n) UBO_INT(x)[n]
#define UBO_BOOL_N(x, n) UBO_BOOL(x)[n]

#include "ubo_prop_global.inc.h"
#undef UBO_DEF
} VKGlobalUniformObject_t;

static_assert(sizeof(VKGlobalUniformObject_t) % 4 == 0, "");

/**
 * Global uniform dirty state
 * align with ubo
 */
typedef struct VKGlobalUniformObjectState_s
{
#define UBO_DEF(x) int x
#define UBO_MAT4(x) imat4 x
#define UBO_VEC4(x) ivec4 x
#define UBO_FLOAT UBO_DEF
#define UBO_INT UBO_DEF
#define UBO_BOOL UBO_DEF
#define UBO_VEC2(x) ivec2 x
#define UBO_MAT4_N(x, n) UBO_MAT4(x)[n]
#define UBO_VEC4_N(x, n) UBO_VEC4(x)[n]
#define UBO_INT_N(x, n) UBO_INT(x)[n]
#define UBO_BOOL_N(x, n) UBO_BOOL(x)[n]

#include "ubo_prop_global.inc.h"
#undef UBO_DEF
} VKGlobalUniformObjectState_t;

static_assert(sizeof(VKGlobalUniformObjectState_t) % 4 == 0, "");

static_assert(sizeof(VKGlobalUniformObjectState_t) == sizeof(VKGlobalUniformObject_t), "");

/**
 * Global uniform offset location
 */
typedef VKGlobalUniformObjectState_t VKGlobalUniformObjectLocation_t;


/**
 * static assert uniform offset
 */
#define UBO_DEF(x) static_assert(offsetof(VKGlobalUniformObject_t, x) == offsetof(VKGlobalUniformObjectState_t, x), "")
#define UBO_MAT4 UBO_DEF
#define UBO_VEC4 UBO_DEF
#define UBO_FLOAT UBO_DEF
#define UBO_INT UBO_DEF
#define UBO_BOOL UBO_DEF
#define UBO_VEC2 UBO_DEF
#define UBO_MAT4_N(x, n) UBO_DEF(x)
#define UBO_VEC4_N(x, n) UBO_DEF(x)
#define UBO_INT_N(x, n) UBO_DEF(x)
#define UBO_BOOL_N(x, n) UBO_DEF(x)

#include "ubo_prop_global.inc.h"
#undef UBO_DEF


#define VKSTATE_PARM_OFFSET(p) offsetof(VKGlobalUniformObjectState_t, p)
#define VKSTATE_PARMS_OFFSET(p, n) (offsetof(VKGlobalUniformObjectState_t, p) + (n) * sizeof(((VKGlobalUniformObjectState_t *)0)->p[0]))

#define VKGLOBAL_PARM_OFFSET(p) offsetof(VKGlobalUniformObject_t, p)
#define VKGLOBAL_PARMS_OFFSET(p, n) (offsetof(VKGlobalUniformObject_t, p) + (n) * sizeof(((VKGlobalUniformObject_t *)0)->p[0]))


void R_VK_InitGlobalUBO();
int R_VK_GetGlobalUBOOffset(const char *name, int index);


// Uniform structs of shaders
#define UBO_DEFINE(stage, type) stage##UniformBufferObject_##type##_t
#define VERTEX_UBO_DEFINE(type) UBO_DEFINE(vertex, type)
#define FRAGMENT_UBO_DEFINE(type) UBO_DEFINE(fragment, type)

#include "ubo_struct.inc.h"


// Function of copy global uniforms to shaders
#define UBOX_TRANSFORM_NAME(stage, T) R_VK_TransformUniform0_##stage##_##T
#define UBO0_TRANSFORM_NAME(T) UBOX_TRANSFORM_NAME(vertex, T)
#define UBO1_TRANSFORM_NAME(T) UBOX_TRANSFORM_NAME(fragment, T)

#define UBOX_TRANSFORM_DEFINE(stage, T) void UBOX_TRANSFORM_NAME(stage, T)()
#define UBO0_TRANSFORM_DEFINE(T) UBOX_TRANSFORM_DEFINE(vertex, T)
#define UBO1_TRANSFORM_DEFINE(T) UBOX_TRANSFORM_DEFINE(fragment, T)

#include "ubo_copy_func.inc.h"


// Function of clear shaders uniform dirty state
#define UBOX_CLEAR_NAME(stage, T) R_VK_ClearUniformState_##stage##_##T
#define UBO0_CLEAR_NAME(T) UBOX_CLEAR_NAME(vertex, T)
#define UBO1_CLEAR_NAME(T) UBOX_CLEAR_NAME(fragment, T)

#define UBOX_CLEAR_DEFINE(stage, T) void UBOX_CLEAR_NAME(stage, T)(void)
#define UBO0_CLEAR_DEFINE(T) UBOX_CLEAR_DEFINE(vertex, T)
#define UBO1_CLEAR_DEFINE(T) UBOX_CLEAR_DEFINE(fragment, T)

#include "ubo_clear_func.inc.h"


// Function of get shader uniform variables offset location
#define UBOX_LOCATION_NAME(stage, T) R_VK_GetUniformLocation_##stage##_##T
#define UBO0_LOCATION_NAME(T) UBOX_LOCATION_NAME(vertex, T)
#define UBO1_LOCATION_NAME(T) UBOX_LOCATION_NAME(fragment, T)

#define UBOX_LOCATION_DEFINE(stage, T) void UBOX_LOCATION_NAME(stage, T)(VKGlobalUniformObjectLocation_t *ret)
#define UBO0_LOCATION_DEFINE(T) UBOX_LOCATION_DEFINE(vertex, T)
#define UBO1_LOCATION_DEFINE(T) UBOX_LOCATION_DEFINE(fragment, T)

#include "ubo_loc_func.inc.h"


// Function of check shaders uniform dirty state
#define UBOX_CHECK_NAME(stage, T) R_VK_CheckUniformState_##stage##_##T
#define UBO0_CHECK_NAME(T) UBOX_CHECK_NAME(vertex, T)
#define UBO1_CHECK_NAME(T) UBOX_CHECK_NAME(fragment, T)

#define UBOX_CHECK_DEFINE(stage, T) void UBOX_CHECK_NAME(stage, T)(void)
#define UBO0_CHECK_DEFINE(T) UBOX_CHECK_DEFINE(vertex, T)
#define UBO1_CHECK_DEFINE(T) UBOX_CHECK_DEFINE(fragment, T)

#include "ubo_check_func.inc.h"


// Function of get uniform offset by name
#define UBOX_LOC_NAME_NAME(stage, T) R_VK_GetUniformOffsetByName_##stage##_##T
#define UBO0_LOC_NAME_NAME(T) UBOX_LOC_NAME_NAME(vertex, T)
#define UBO1_LOC_NAME_NAME(T) UBOX_LOC_NAME_NAME(fragment, T)

#define UBOX_LOC_NAME_DEFINE(stage, T) int UBOX_LOC_NAME_NAME(stage, T)(const char *name, int index)
#define UBO0_LOC_NAME_DEFINE(T) UBOX_LOC_NAME_DEFINE(vertex, T)
#define UBO1_LOC_NAME_DEFINE(T) UBOX_LOC_NAME_DEFINE(fragment, T)

#include "ubo_locname_func.inc.h"


#endif //_VULKAN_VKUBO_H
