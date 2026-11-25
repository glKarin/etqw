#ifndef _VULKAN_VKTYPE_H
#define _VULKAN_VKTYPE_H

typedef float vec2[2];
typedef float vec3[3];
typedef float vec4[4];
typedef float mat4[16];
typedef unsigned int bool32;
typedef unsigned char byte;
typedef int ivec2[2];
typedef int ivec4[4];
typedef int imat4[16];

typedef struct vec2_s
{
    vec2 v;
} vec2_t;

typedef struct vec3_s
{
    vec3 v;
} vec3_t;

typedef struct vec4_s
{
    vec4 v;
} vec4_t;

typedef struct mat4_s
{
    mat4 m;
} mat4_t;


#define MAT4CPY(dst, src) *((mat4_t *)(dst)) = *((mat4_t *)(src))
#define VEC4CPY(dst, src) *((vec4_t *)(dst)) = *((vec4_t *)(src))
#define VEC3CPY(dst, src) *((vec3_t *)(dst)) = *((vec3_t *)(src))
#define VEC2CPY(dst, src) *((vec2_t *)(dst)) = *((vec2_t *)(src))


static const vec2 vec2_00 = { 0.0f, 0.0f };
static const vec2 vec2_11 = { 1.0f, 1.0f };
static const vec4 vec4_0001 = { 0.0f, 0.0f, 0.0f, 1.0f };
static const vec4 vec4_1001 = { 1.0f, 0.0f, 0.0f, 1.0f };
static const vec4 vec4_0101 = { 0.0f, 1.0f, 0.0f, 1.0f };
static const vec4 vec4_0011 = { 0.0f, 0.0f, 1.0f, 1.0f };
static const mat4 mat4_0001_0100_0010_0001 = {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f,
};

ID_INLINE static void vec2_identity(vec2 vec)
{
    VEC2CPY(vec, vec2_00);
}

ID_INLINE static void vec2_one_identity(vec2 vec)
{
    VEC2CPY(vec, vec2_11);
}

ID_INLINE static void vec4_identity(vec4 vec)
{
    VEC4CPY(vec, vec4_0001);
}

ID_INLINE static void vec4_x_identity(vec4 vec)
{
    VEC4CPY(vec, vec4_1001);
}

ID_INLINE static void vec4_y_identity(vec4 vec)
{
    VEC4CPY(vec, vec4_0101);
}

ID_INLINE static void vec4_z_identity(vec4 vec)
{
    VEC4CPY(vec, vec4_0011);
}

ID_INLINE static void vec4s_identity(vec4 vecs[], int num)
{
    for (int i = 0; i < num; i++)
    {
        vec4_identity(vecs[i]);
    }
}
#define VEC4S_IDENTITY(vec4s) vec4s_identity(vec4s, COUNT_OF(vec4s))

ID_INLINE static void mat4_make_identity(mat4 mat)
{
    MAT4CPY(mat, mat4_0001_0100_0010_0001);
}

ID_INLINE static void mat4s_identity(mat4 mats[], int num)
{
    for (int i = 0; i < num; i++)
    {
        mat4_make_identity(mats[i]);
    }
}
#define MAT4S_IDENTITY(mat4s) mat4s_identity(mat4s, COUNT_OF(mat4s))

#endif //_VULKAN_VKTYPE_H
