#ifndef _VULKAN_VKUBO_H
#define _VULKAN_VKUBO_H

#include <stddef.h>

typedef float vec2[2];
typedef float vec3[3];
typedef float vec4[4];
typedef float mat4[16];
typedef unsigned int bool32;
typedef unsigned char byte;
typedef int ivec4[4];
typedef int imat4[16];

typedef struct globalUniformBufferObject_s
{
#define UBO_DEF(T, x) T x
#define UBO_MAT4(x) UBO_DEF(mat4, x)
#define UBO_VEC4(x) UBO_DEF(vec4, x)
#define UBO_FLOAT(x) UBO_DEF(float, x)
#define UBO_INT(x) UBO_DEF(int, x)
#define UBO_BOOL(x) UBO_DEF(bool32, x)

#include "ubo_prop.inc.h"
#undef UBO_DEF
} globalUniformBufferObject_t;

static_assert(sizeof(globalUniformBufferObject_t) % 4 == 0);

// align with ubo
typedef struct globalUniformBufferObjectState_s
{
#define UBO_DEF(x) int x
#define UBO_MAT4(x) imat4 x
#define UBO_VEC4(x) ivec4 x
#define UBO_FLOAT UBO_DEF
#define UBO_INT UBO_DEF
#define UBO_BOOL UBO_DEF

#include "ubo_prop.inc.h"
#undef UBO_DEF
} globalUniformBufferObjectState_t;

static_assert(sizeof(globalUniformBufferObjectState_t) % 4 == 0);

static_assert(sizeof(globalUniformBufferObjectState_t) == sizeof(globalUniformBufferObject_t));

#define UBO_DEF(x) static_assert(offsetof(globalUniformBufferObject_t, x) == offsetof(globalUniformBufferObjectState_t, x), "")
#define UBO_MAT4 UBO_DEF
#define UBO_VEC4 UBO_DEF
#define UBO_FLOAT UBO_DEF
#define UBO_INT UBO_DEF
#define UBO_BOOL UBO_DEF

#include "ubo_prop.inc.h"
#undef UBO_DEF

typedef void (*copyUniform_f)();


#define UBO_DECLARATION(stage, type) typedef struct stage##UniformBufferObject_##type {
#define END_UBO(stage, type) } stage##UniformBufferObject_##type##_t;
#define UBO_DEFINE(stage, type) stage##UniformBufferObject_##type##_t
#define UBO_ALIAS_DEFINE(stage, alias, type) typedef stage##UniformBufferObject_##type##_t stage##UniformBufferObject_##alias##_t;

#define VERTEX_UBO_DECLARATION(type) UBO_DECLARATION(vertex, type)
#define END_VERTEX_UBO(type) END_UBO(vertex, type)
#define VERTEX_UBO_DEFINE(type) UBO_DEFINE(vertex, type)
#define VERTEX_UBO_ALIAS_DEFINE(alias, type) UBO_ALIAS_DEFINE(vertex, alias, type)

#define FRAGMENT_UBO_DECLARATION(type) UBO_DECLARATION(fragment, type)
#define END_FRAGMENT_UBO(type) END_UBO(fragment, type)
#define FRAGMENT_UBO_DEFINE(type) UBO_DEFINE(fragment, type)
#define FRAGMENT_UBO_ALIAS_DEFINE(alias, type) UBO_ALIAS_DEFINE(fragment, alias, type)

#include "ubo_struct.inc.h"


#define VKSTATE_PARM_OFFSET(p) offsetof(globalUniformBufferObjectState_t, p)
#define VKSTATE_PARMS_OFFSET(p, n) (offsetof(globalUniformBufferObjectState_t, p) + (n) * sizeof(((globalUniformBufferObjectState_t *)0)->p[0]))

#define VKGLOBAL_PARM_OFFSET(p) offsetof(globalUniformBufferObject_t, p)
#define VKGLOBAL_PARMS_OFFSET(p, n) (offsetof(globalUniformBufferObject_t, p) + (n) * sizeof(((globalUniformBufferObject_t *)0)->p[0]))

#define SHADER_PARM_ADDR2(p) VKGLOBAL_PARM_OFFSET(p), VKSTATE_PARM_OFFSET(p)
#define SHADER_PARMS_ADDR2(p, n) VKGLOBAL_PARMS_OFFSET(p, n), VKSTATE_PARMS_OFFSET(p, n)


#define SHADER_PARM_OFFSET(stage, T, p) offsetof(UBO_DEFINE(stage, T), p)
#define SHADER_PARMS_N_OFFSET(stage, T, p, n) (offsetof(UBO_DEFINE(stage, T), p) + (n) * sizeof(((UBO_DEFINE(stage, T) *)0)->p[0]))

#define UBOX_TRANSFORM_NAME(stage, T) R_VK_TransformUniform0_##stage##_##T
#define UBO0_TRANSFORM_NAME(T) UBOX_TRANSFORM_NAME(vertex, T)
#define UBO1_TRANSFORM_NAME(T) UBOX_TRANSFORM_NAME(fragment, T)

#define UBOX_TRANSFORM_DEFINE(stage, T) void UBOX_TRANSFORM_NAME(stage, T)()
#define UBO0_TRANSFORM_DEFINE(T) UBOX_TRANSFORM_DEFINE(vertex, T)
#define UBO1_TRANSFORM_DEFINE(T) UBOX_TRANSFORM_DEFINE(fragment, T)


#define UBOX_CPYS(T, stage, DT, name) R_VK_TransformUniform(SHADER_PARM_OFFSET(stage, T, name), VKGLOBAL_PARM_OFFSET(name), vk_backend.currentProgram->ubo_##stage, sizeof(DT))
#define UBOX_CPY1F(T, stage, name) R_VK_TransformUniform1f(SHADER_PARM_OFFSET(stage, T, name), VKGLOBAL_PARM_OFFSET(name), vk_backend.currentProgram->ubo_##stage)
#define UBOX_CPY4F(T, stage, name) R_VK_TransformUniform4f(SHADER_PARM_OFFSET(stage, T, name), VKGLOBAL_PARM_OFFSET(name), vk_backend.currentProgram->ubo_##stage)
#define UBOX_CPYMAT4F(T, stage, name) R_VK_TransformUniformMatrix4f(SHADER_PARM_OFFSET(stage, T, name), VKGLOBAL_PARM_OFFSET(name), vk_backend.currentProgram->ubo_##stage)
#define UBOX_CPY(T, stage, name) R_VK_TransformUniform(SHADER_PARM_OFFSET(stage, T, name), VKGLOBAL_PARM_OFFSET(name), vk_backend.currentProgram->ubo_##stage, sizeof(globalUBO.name))
#define UBOX_CPYN(T, stage, name, n) R_VK_TransformUniform(SHADER_PARMS_N_OFFSET(stage, T, name, n), VKSHADER_PARM_N_ADDR(name, n), vk_backend.currentProgram->ubo_##stage, sizeof(globalUBO.name[n]))


#define UBO0_CPYS(T, DT, name) UBOX_CPYS(T, vertex, DT, name)
#define UBO0_CPY1F(T, name) UBOX_CPY1F(T, vertex, name)
#define UBO0_CPY4F(T, name) UBOX_CPY4F(T, vertex, name)
#define UBO0_CPYMAT4F(T, name) UBOX_CPYMAT4F(T, vertex, name)
#define UBO0_CPY(T, name) UBOX_CPY(T, vertex, name)
#define UBO0_CPYN(T, name, n) UBOX_CPYN(T, vertex, name, n)

#define UBO1_CPYS(T, DT, name) UBOX_CPYS(T, fragment, DT, name)
#define UBO1_CPY1F(T, name) UBOX_CPY1F(T, fragment, name)
#define UBO1_CPY4F(T, name) UBOX_CPY4F(T, fragment, name)
#define UBO1_CPYMAT4F(T, name) UBOX_CPYMAT4F(T, fragment, name)
#define UBO1_CPY(T, name) UBOX_CPY(T, fragment, name)
#define UBO1_CPYN(T, name, n) UBOX_CPYN(T, fragment, name, n)

#include "ubo_copy_func.inc.h"

void R_VK_InitGlobalUBO();

#endif //_VULKAN_VKUBO_H
