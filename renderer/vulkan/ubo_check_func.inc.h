
#define UBOX_CHECK_FUNC_DEFINE(stage, T) UBOX_CHECK_DEFINE(stage, T);
#define UBO0_CHECK_FUNC_DEFINE(T) UBOX_CHECK_FUNC_DEFINE(vertex, T)
#define UBO1_CHECK_FUNC_DEFINE(T) UBOX_CHECK_FUNC_DEFINE(fragment, T)



#define UBO_VERTEX_PROC_BEGIN UBO0_CHECK_FUNC_DEFINE
#define UBO_FRAGMENT_PROC_BEGIN UBO1_CHECK_FUNC_DEFINE
#define UBO_VERTEX_PROC_END(x)
#define UBO_FRAGMENT_PROC_END(x)
#define UBO_MAT4_PROC(X)
#define UBO_VEC4_PROC(X)
#define UBO_FLOAT_PROC(X)
#define UBO_INT_N_PROC(X, N)
#define UBO_VEC4_N_PROC(X, N)
#define UBO_MAT4_N_PROC(X, N)

#include "ubo_prop_shader.inc.h"


typedef void (*VKCheckUniformState_f)(void);


#if 1
#define UBOX_CHECK_PROC_ALIAS(stage, alias, original) \
    ID_INLINE void R_VK_CheckUniformState_##stage##_##alias(void) { \
        R_VK_CheckUniformState_##stage##_##original(); \
    }
#else
#define UBOX_CHECK_PROC_ALIAS(stage, alias, original) static const VKCheckUniformState_f R_VK_CheckUniformState_##stage##_##alias = &R_VK_CheckUniformState_##stage##_##original;
#endif

#define UBO_VERTEX_PROC_ALIAS(alias, original) UBOX_CHECK_PROC_ALIAS(vertex, alias, original)
#define UBO_FRAGMENT_PROC_ALIAS(alias, original) UBOX_CHECK_PROC_ALIAS(fragment, alias, original)

#include "ubo_prop_alias.inc.h"
