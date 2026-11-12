#include "vk_gl.h"

#include "../idlib/precompiled.h"
#pragma hdrstop

#include "../tr_local.h"

#if 1
#define QGLPROC_PLACEHOLDER(name, ret, args) ret vk##name args { \
    Sys_Printf("VKGL: call " #name " not implement\n"); \
    return (ret)0; \
}
#define QGLPROC(name, rettype, args) q##name = vk##name;
#else
#define QGLPROC_PLACEHOLDER(name, ret, args) ret name args { \
    Sys_Printf("VKGL: call " #name " not implement\n"); \
    return (ret)0; \
}
#define QGLPROC(name, rettype, args) q##name = name;
#endif

#define GLERROR_OUTPUT 0

static GLenum error = GL_NO_ERROR;

#if GLERROR_OUTPUT
static void GL_RaiseError(GLenum err, const char *fmt, ...)
{
    error = err;
    if (fmt)
    {
        va_list ap;
        va_start(ap, fmt);
        printf(fmt, ap);
        printf("\n");
        va_end(ap);
    }
}

#define GLRAISE_ERROR(err, ...) GL_RaiseError(err, __VA_ARGS__)
#else
static void GL_RaiseError(GLenum err)
{
    error = err;
}

#define GLRAISE_ERROR(err, ...) GL_RaiseError(err)
#endif

static int GL_GetCullFace(GLenum mode)
{
    if(mode == GL_FRONT)
        return ST_CULL_FRONT;
    else if(mode == GL_BACK)
        return ST_CULL_BACK;
    else if(mode == GL_FRONT_AND_BACK)
        return ST_CULL_FRONT_AND_BACK;
    else
        return ST_CULL_NONE;
}

static int GL_GetStencilOp(GLenum op)
{
    switch (op) {
        case GL_KEEP:
            return ST_STENCILOP_KEEP;
        case GL_ZERO:
            return ST_STENCILOP_ZERO;
        case GL_REPLACE:
            return ST_STENCILOP_REPLACE;
        case GL_INCR:
            return ST_STENCILOP_INCR;
        case GL_INCR_WRAP:
            return ST_STENCILOP_INCRWRAP;
        case GL_DECR:
            return ST_STENCILOP_DECR;
        case GL_DECR_WRAP:
            return ST_STENCILOP_DECRWRAP;
        case GL_INVERT:
            return ST_STENCILOP_INVERT;
        default:
            return -1;
    }
}

static int GL_GetDepthCompareOp(GLenum op)
{
    switch (op) {
        case GL_NEVER:
            return ST_DEPTHFUNC_NEVER;
        case GL_LESS:
            return ST_DEPTHFUNC_LESS;
        case GL_EQUAL:
            return ST_DEPTHFUNC_EQUALS;
        case GL_LEQUAL:
            return ST_DEPTHFUNC_LEQUALS;
        case GL_GREATER:
            return ST_DEPTHFUNC_GREATER;
        case GL_NOTEQUAL:
            return ST_DEPTHFUNC_NEQUALS;
        case GL_GEQUAL:
            return ST_DEPTHFUNC_GEQUALS;
        case GL_ALWAYS:
            return ST_DEPTHFUNC_ALWAYS;
        default:
            return -1;
    }
}

static int GL_GetStencilCompareOp(GLenum op)
{
    switch (op) {
        case GL_NEVER:
            return ST_STENCILFUNC_NEVER;
        case GL_LESS:
            return ST_STENCILFUNC_LESS;
        case GL_EQUAL:
            return ST_STENCILFUNC_EQUALS;
        case GL_LEQUAL:
            return ST_STENCILFUNC_LEQUALS;
        case GL_GREATER:
            return ST_STENCILFUNC_GREATER;
        case GL_NOTEQUAL:
            return ST_STENCILFUNC_NEQUALS;
        case GL_GEQUAL:
            return ST_STENCILFUNC_GEQUALS;
        case GL_ALWAYS:
            return ST_STENCILFUNC_ALWAYS;
        default:
            return -1;
    }
}

static int GL_GetBlendSrcFactor(GLenum op)
{
    switch (op) {
        case GL_ZERO:
            return ST_SRCBLEND_ZERO;
        case GL_ONE:
            return ST_SRCBLEND_ONE;
        case GL_SRC_COLOR:
            return ST_SRCBLEND_SRC_COLOR;
        case GL_ONE_MINUS_SRC_COLOR:
            return ST_SRCBLEND_ONE_MINUS_SRC_COLOR;
        case GL_DST_COLOR:
            return ST_SRCBLEND_DST_COLOR;
        case GL_ONE_MINUS_DST_COLOR:
            return ST_SRCBLEND_ONE_MINUS_DST_COLOR;
        case GL_SRC_ALPHA:
            return ST_SRCBLEND_SRC_ALPHA;
        case GL_ONE_MINUS_SRC_ALPHA:
            return ST_SRCBLEND_ONE_MINUS_SRC_ALPHA;
        case GL_DST_ALPHA:
            return ST_SRCBLEND_DST_ALPHA;
        case GL_ONE_MINUS_DST_ALPHA:
            return ST_SRCBLEND_ONE_MINUS_DST_ALPHA;
        case GL_SRC_ALPHA_SATURATE:
            return ST_SRCBLEND_ALPHA_SATURATE;
        case GL_ONE_MINUS_CONSTANT_COLOR:
        case GL_CONSTANT_ALPHA:
        case GL_ONE_MINUS_CONSTANT_ALPHA:
        default:
            return -1;
    }
}

static int GL_GetBlendDstFactor(GLenum op)
{
    switch (op) {
        case GL_ZERO:
            return ST_DSTBLEND_ZERO;
        case GL_ONE:
            return ST_DSTBLEND_ONE;
        case GL_SRC_COLOR:
            return ST_DSTBLEND_SRC_COLOR;
        case GL_ONE_MINUS_SRC_COLOR:
            return ST_DSTBLEND_ONE_MINUS_SRC_COLOR;
        case GL_DST_COLOR:
            return ST_DSTBLEND_DST_COLOR;
        case GL_ONE_MINUS_DST_COLOR:
            return ST_DSTBLEND_ONE_MINUS_DST_COLOR;
        case GL_SRC_ALPHA:
            return ST_DSTBLEND_SRC_ALPHA;
        case GL_ONE_MINUS_SRC_ALPHA:
            return ST_DSTBLEND_ONE_MINUS_SRC_ALPHA;
        case GL_DST_ALPHA:
            return ST_DSTBLEND_DST_ALPHA;
        case GL_ONE_MINUS_DST_ALPHA:
            return ST_DSTBLEND_ONE_MINUS_DST_ALPHA;
        case GL_SRC_ALPHA_SATURATE:
            return ST_SRCBLEND_ALPHA_SATURATE;
        case GL_ONE_MINUS_CONSTANT_COLOR:
        case GL_CONSTANT_ALPHA:
        case GL_ONE_MINUS_CONSTANT_ALPHA:
        default:
            return -1;
    }
}

int GL_GetDrawMode(GLenum mode)
{
    switch (mode) {
        case GL_POINTS:
            return ST_POINTS;
        case GL_LINE_STRIP:
            return ST_LINE_LOOP;
        case GL_LINE_LOOP:
            return ST_LINE_LOOP;
        case GL_LINES:
            return ST_LINES;
        case GL_TRIANGLE_STRIP:
            return ST_TRIANGLE_STRIP;
        case GL_TRIANGLE_FAN:
            return ST_TRIANGLE_FAN;
        case GL_TRIANGLES:
            return ST_TRIANGLES;
        default:
            return -1;
    }
}

void VKGL_FUNC(glActiveTexture)(GLenum texture)
{
    R_VK_SelectTexture(texture - GL_TEXTURE0);
}

void VKGL_FUNC(glBlendFunc)(GLenum sfactor, GLenum dfactor)
{
    int src = GL_GetBlendSrcFactor(sfactor);
    if (src < 0)
    {
        GLRAISE_ERROR(GL_INVALID_ENUM, "glBlendFunc(<0x%X>, 0x%X)", sfactor, dfactor);
        return;
    }
    int dst = GL_GetBlendDstFactor(dfactor);
    if (dst < 0)
    {
        GLRAISE_ERROR(GL_INVALID_ENUM, "glBlendFunc(0x%X, <0x%X>)", sfactor, dfactor);
        return;
    }
    R_VK_BlendFunc(src, dst);
}

void VKGL_FUNC(glClear)(GLbitfield mask)
{
    R_VK_Clear(mask & GL_COLOR_BUFFER_BIT, mask & GL_DEPTH_BUFFER_BIT, mask & GL_STENCIL_BUFFER_BIT);
}

void VKGL_FUNC(glClearColor)(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
    R_VK_ClearColor(red, green, blue, alpha);
}

void VKGL_FUNC(glClearDepthf)(GLclampf depth)
{
    R_VK_ClearDepth(depth);
}

void VKGL_FUNC(glClearStencil)(GLint s)
{
    R_VK_ClearStencil(s);
}

void VKGL_FUNC(glColorMask)(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)
{
    R_VK_ColorMask(red, green, blue, alpha);
}

void VKGL_FUNC(glCullFace)(GLenum mode)
{
    R_VK_CullFace(GL_GetCullFace(mode));
}

void VKGL_FUNC(glDepthFunc)(GLenum func)
{
    int depthFunc = GL_GetDepthCompareOp(func);
    if (depthFunc < 0)
    {
        GLRAISE_ERROR(GL_INVALID_ENUM, "glDepthFunc(<0x%X>)", func);
        return;
    }
    R_VK_DepthFunc(depthFunc);
}

void VKGL_FUNC(glDepthMask)(GLboolean flag)
{
    R_VK_DepthMask(flag);
}

void VKGL_FUNC(glDepthRangef)(GLfloat n, GLfloat f)
{
    R_VK_DepthRange(n, f);
}

void VKGL_FUNC(glDisable)(GLenum cap)
{
    switch(cap)
    {
        case GL_BLEND:
            R_VK_EnableBlend(false);
            break;
        case GL_CULL_FACE:
            R_VK_EnableCullFace(false);
            break;
        case GL_DEPTH_TEST:
            R_VK_EnableDepthTest(false);
            break;
        case GL_POLYGON_OFFSET_FILL:
            R_VK_EnablePolygonOffset(false);
            break;
        case GL_SCISSOR_TEST:
            R_VK_EnableScissorTest(false);
            break;
        case GL_STENCIL_TEST:
            R_VK_EnableStencilTest(false);
            break;
        default:
            GLRAISE_ERROR(GL_INVALID_ENUM, "glDisable(<0x%X>)", cap);
            break;
    }
}

void VKGL_FUNC(glDrawArrays)(GLenum mode, GLint first, GLsizei count)
{
    int poly = GL_GetDrawMode(mode);
    if (poly < 0)
    {
        GLRAISE_ERROR(GL_INVALID_ENUM, "glDrawArrays(<0x%X>, %d, %u)", mode, first, count);
        return;
    }
    R_VK_DrawArrays(poly, first, count);
}

void VKGL_FUNC(glEnable)(GLenum cap)
{
    switch(cap)
    {
        case GL_BLEND:
            R_VK_EnableBlend(true);
            break;
        case GL_CULL_FACE:
            R_VK_EnableCullFace(true);
            break;
        case GL_DEPTH_TEST:
            R_VK_EnableDepthTest(true);
            break;
        case GL_POLYGON_OFFSET_FILL:
            R_VK_EnablePolygonOffset(true);
            break;
        case GL_SCISSOR_TEST:
            R_VK_EnableScissorTest(true);
            break;
        case GL_STENCIL_TEST:
            R_VK_EnableStencilTest(true);
            break;
        default:
            GLRAISE_ERROR(GL_INVALID_ENUM, "glEnable(<0x%X>)", cap);
            break;
    }
}

void VKGL_FUNC(glFinish)(void)
{
    //VK_WaitFrameFence();
}

void VKGL_FUNC(glFrontFace)(GLenum mode)
{
    R_VK_FrontFace(mode == GL_CW ? ST_FRONT_CW : ST_FRONT_CCW);
}

GLenum VKGL_FUNC(glGetError)(void)
{
    return error;
}

#define GLGET_WRITE1(params, a) params[0] = (GLGET_WRITE_TYPE)a;
#define GLGET_WRITE2(params, a, b) params[0] = (GLGET_WRITE_TYPE)a; params[1] = (GLGET_WRITE_TYPE)b;
#define GLGET_WRITE2V(params, a) GLGET_WRITE2(params, a[0], a[1]);
#define GLGET_WRITE4(params, a, b, c, d) params[0] = (GLGET_WRITE_TYPE)a; params[1] = (GLGET_WRITE_TYPE)b; params[2] = (GLGET_WRITE_TYPE)c; params[3] = (GLGET_WRITE_TYPE)d;
#define GLGET_WRITE4V(params, a) GLGET_WRITE4(params, a[0], a[1], a[2], a[3]);

#define GLGET_FUNC(Type) void VKGL_FUNC(glGet##Type##v)(GLenum pname, GLGET_WRITE_TYPE *params) \
{ \
    switch (pname) \
    { \
        case GL_ACTIVE_TEXTURE: \
            GLGET_WRITE1(params, vk_backend.currenttmu); \
            break; \
        case GL_BLEND: \
            GLGET_WRITE1(params, R_VK_IsBlendEnabled()); \
            break; \
        case GL_BLEND_DST_ALPHA: \
        case GL_BLEND_DST_RGB: \
            break; \
        case GL_BLEND_SRC_ALPHA: \
        case GL_BLEND_SRC_RGB: \
            break; \
        case GL_COLOR_CLEAR_VALUE: \
            GLGET_WRITE4V(params, vk_backend.dynamicState.clearColor); \
            break; \
        case GL_COLOR_WRITEMASK: \
            GLGET_WRITE4(params, \
                VKSTATE1_IS_TRUE(ST_COLORMASK_R_BIT) ? GL_FALSE : GL_TRUE, \
                VKSTATE1_IS_TRUE(ST_COLORMASK_G_BIT) ? GL_FALSE : GL_TRUE, \
                VKSTATE1_IS_TRUE(ST_COLORMASK_B_BIT) ? GL_FALSE : GL_TRUE, \
                VKSTATE1_IS_TRUE(ST_COLORMASK_A_BIT) ? GL_FALSE : GL_TRUE \
                ); \
            break; \
        case GL_CULL_FACE: \
            GLGET_WRITE1(params, R_VK_IsCullFaceEnabled()); \
            break; \
        case GL_CULL_FACE_MODE: \
            break; \
        case GL_DEPTH_CLEAR_VALUE: \
            GLGET_WRITE1(params, vk_backend.dynamicState.clearDepth); \
            break; \
        case GL_DEPTH_RANGE: \
            GLGET_WRITE2V(params, vk_backend.dynamicState.depthRanges); \
            break; \
        case GL_DEPTH_TEST: \
            GLGET_WRITE1(params, R_VK_IsDepthTestEnabled()); \
            break; \
        case GL_DEPTH_WRITEMASK: \
            GLGET_WRITE1(params, VKSTATE1_IS_TRUE(ST_DEPTH_MASK_BIT) ? GL_TRUE : GL_FALSE); \
            break; \
        case GL_FRONT_FACE: \
            GLGET_WRITE1(params, VKSTATE1_IS_TRUE(ST_FRONT_CW_BIT) ? GL_CW : GL_CCW); \
            break; \
        case GL_LINE_WIDTH: \
            GLGET_WRITE1(params, vk_backend.dynamicState.lineWidth); \
            break; \
        case GL_POLYGON_OFFSET_FACTOR: \
            GLGET_WRITE1(params, vk_backend.dynamicState.polygonFactor); \
            break; \
        case GL_POLYGON_OFFSET_FILL: \
            GLGET_WRITE1(params, R_VK_IsPolygonOffsetEnabled()); \
            break; \
        case GL_POLYGON_OFFSET_UNITS: \
            GLGET_WRITE1(params, vk_backend.dynamicState.polygonUnits); \
            break; \
        case GL_SCISSOR_BOX: \
            GLGET_WRITE4V(params, vk_backend.dynamicState.scissor); \
            break; \
        case GL_SCISSOR_TEST: \
            GLGET_WRITE1(params, R_VK_IsScissorTestEnabled()); \
            break; \
        case GL_STENCIL_BACK_FAIL: \
            break; \
        case GL_STENCIL_BACK_FUNC: \
            break; \
        case GL_STENCIL_BACK_PASS_DEPTH_FAIL: \
            break; \
        case GL_STENCIL_BACK_PASS_DEPTH_PASS: \
            break; \
        case GL_STENCIL_BACK_REF: \
            GLGET_WRITE1(params, vk_backend.dynamicState.stencilReference[1]); \
            break; \
        case GL_STENCIL_BACK_VALUE_MASK: \
            GLGET_WRITE1(params, vk_backend.dynamicState.stencilMask[1]); \
            break; \
        case GL_STENCIL_BACK_WRITEMASK: \
            GLGET_WRITE1(params, vk_backend.dynamicState.stencilWriteMask[1]); \
            break; \
        case GL_STENCIL_CLEAR_VALUE: \
            GLGET_WRITE1(params, vk_backend.dynamicState.clearStencil); \
            break; \
        case GL_STENCIL_FAIL: \
            break; \
        case GL_STENCIL_FUNC: \
            break; \
        case GL_STENCIL_PASS_DEPTH_FAIL: \
            break; \
        case GL_STENCIL_PASS_DEPTH_PASS: \
            break; \
        case GL_STENCIL_REF: \
            GLGET_WRITE1(params, vk_backend.dynamicState.stencilReference[0]); \
            break; \
        case GL_STENCIL_TEST: \
            GLGET_WRITE1(params, R_VK_IsStencilTestEnabled()); \
            break; \
        case GL_STENCIL_VALUE_MASK: \
            GLGET_WRITE1(params, vk_backend.dynamicState.stencilMask[0]); \
            break; \
        case GL_STENCIL_WRITEMASK: \
            GLGET_WRITE1(params, vk_backend.dynamicState.stencilWriteMask[0]); \
            break; \
        case GL_VIEWPORT: \
            GLGET_WRITE4V(params, vk_backend.dynamicState.viewport); \
            break; \
        default: \
            break; \
    } \
}

#define GLGET_WRITE_TYPE GLint
GLGET_FUNC(Integer)
#undef GLGET_WRITE_TYPE

#define GLGET_WRITE_TYPE GLboolean
GLGET_FUNC(Boolean)
#undef GLGET_WRITE_TYPE

#define GLGET_WRITE_TYPE GLfloat
GLGET_FUNC(Float)
#undef GLGET_WRITE_TYPE

GLboolean VKGL_FUNC(glIsEnabled)(GLenum cap)
{
    switch(cap)
    {
        case GL_BLEND:
            return R_VK_IsBlendEnabled();
        case GL_CULL_FACE:
            return R_VK_IsCullFaceEnabled();
        case GL_DEPTH_TEST:
            return R_VK_IsDepthTestEnabled();
        case GL_POLYGON_OFFSET_FILL:
            return R_VK_IsPolygonOffsetEnabled();
        case GL_SCISSOR_TEST:
            return R_VK_IsScissorTestEnabled();
        case GL_STENCIL_TEST:
            return R_VK_IsStencilTestEnabled();
        default:
            GLRAISE_ERROR(GL_INVALID_ENUM, "glIsEnabled(<0x%X>)", cap);
            return GL_FALSE;
    }
}

void VKGL_FUNC(glLineWidth)(GLfloat width)
{
    R_VK_LineWidth(width);
}

void VKGL_FUNC(glPolygonOffset)(GLfloat factor, GLfloat units)
{
    R_VK_PolygonOffset(factor, units);
}

void VKGL_FUNC(glScissor)(GLint x, GLint y, GLsizei width, GLsizei height)
{
    R_VK_Scissor(x, vk_backend.dynamicState.renderHeight - y - height, width, height);
}

void VKGL_FUNC(glStencilFunc)(GLenum func, GLint ref, GLuint mask)
{
    int stencilFunc = GL_GetStencilCompareOp(func);
    if (stencilFunc < 0)
    {
        GLRAISE_ERROR(GL_INVALID_ENUM, "glStencilFunc(<0x%X>, %d, %u)", func, ref, mask);
        return;
    }
    R_VK_StencilFunc(stencilFunc, ref, mask);
}

void VKGL_FUNC(glStencilFuncSeparate)(GLenum face, GLenum func, GLint ref, GLuint mask)
{
    int stencilFunc = GL_GetStencilCompareOp(func);
    if (stencilFunc < 0)
    {
        GLRAISE_ERROR(GL_INVALID_ENUM, "glStencilFuncSeparate(0x%X, <0x%X>, %d, %u)", face, func, ref, mask);
        return;
    }
    R_VK_StencilFuncSeparate(GL_GetCullFace(face), stencilFunc, ref, mask);
}

void VKGL_FUNC(glStencilMask)(GLuint mask)
{
    R_VK_StencilMask(mask);
}

void VKGL_FUNC(glStencilMaskSeparate)(GLenum face, GLuint mask)
{
    R_VK_StencilMaskSeparate(GL_GetCullFace(face), mask);
}

void VKGL_FUNC(glStencilOp)(GLenum sfail, GLenum dpfail, GLenum dppass)
{
    int sfailOp = GL_GetStencilOp(sfail);
    if (sfailOp < 0)
    {
        GLRAISE_ERROR(GL_INVALID_ENUM, "glStencilOp(<0x%X>, 0x%X, 0x%X)", sfail, dpfail, dppass);
        return;
    }
    int dpfailOp = GL_GetStencilOp(dpfail);
    if (dpfailOp < 0)
    {
        GLRAISE_ERROR(GL_INVALID_ENUM, "glStencilOp(0x%X, <0x%X>, 0x%X)", sfail, dpfail, dppass);
        return;
    }
    int dppassOp = GL_GetStencilOp(dppass);
    if (dppassOp < 0)
    {
        GLRAISE_ERROR(GL_INVALID_ENUM, "glStencilOp(0x%X, 0x%X, <0x%X>)", sfail, dpfail, dppass);
        return;
    }
    R_VK_StencilOp(sfailOp, dpfailOp, dppassOp);
}

void VKGL_FUNC(glStencilOpSeparate)(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass)
{
    int sfailOp = GL_GetStencilOp(sfail);
    if (sfailOp < 0)
    {
        GLRAISE_ERROR(GL_INVALID_ENUM, "glStencilOpSeparate(0x%X, <0x%X>, 0x%X, 0x%X)", face, sfail, dpfail, dppass);
        return;
    }
    int dpfailOp = GL_GetStencilOp(dpfail);
    if (dpfailOp < 0)
    {
        GLRAISE_ERROR(GL_INVALID_ENUM, "glStencilOpSeparate(0x%X, 0x%X, <0x%X>, 0x%X)", face, sfail, dpfail, dppass);
        return;
    }
    int dppassOp = GL_GetStencilOp(dppass);
    if (dppassOp < 0)
    {
        GLRAISE_ERROR(GL_INVALID_ENUM, "glStencilOpSeparate(0x%X, 0x%X, 0x%X, <0x%X>)", face, sfail, dpfail, dppass);
        return;
    }
    R_VK_StencilOpSeparate(GL_GetCullFace(face), sfailOp, dpfailOp, dppassOp);
}

void VKGL_FUNC(glUniform1f)(GLint location, GLfloat x)
{
    R_VK_Uniform1f(location, x);
}

void VKGL_FUNC(glUniform1fv)(GLint location, GLsizei, const GLfloat *value)
{
    R_VK_Uniform1f(location, *value);
}

void VKGL_FUNC(glUniform2fv)(GLint location, GLsizei, const GLfloat *value)
{
    R_VK_Uniform2fv(location, value);
}

void VKGL_FUNC(glUniform3fv)(GLint location, GLsizei, const GLfloat *value)
{
    R_VK_Uniform3fv(location, value);
}

void VKGL_FUNC(glUniform4f)(GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
    R_VK_Uniform4f(location, x, y, z, w);
}

void VKGL_FUNC(glUniform4fv)(GLint location, GLsizei, const GLfloat *value)
{
    R_VK_Uniform4fv(location, value);
}

void VKGL_FUNC(glUniform1i)(GLint location, GLint value)
{
    R_VK_Uniform1i(location, value);
}

void VKGL_FUNC(glUniformMatrix4fv)(GLint location, GLsizei count, GLboolean, const GLfloat *value)
{
    R_VK_UniformMatrix4fv(location, value, count);
}

void VKGL_FUNC(glViewport)(GLint x, GLint y, GLsizei width, GLsizei height)
{
    R_VK_Viewport(x, vk_backend.dynamicState.renderHeight - y - height, width, height);
}

#include "gles2proc_placeholder.inc.h"

void R_VK_LoadOpenGLCompatFunc(void)
{
#include "../../renderer/qgl_proc.h"
}
