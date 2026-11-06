#include "../idlib/precompiled.h"
#pragma hdrstop

#include "../tr_local.h"

int R_VK_MakeVertexInputAttributes(globalState_t mask, uint32_t binding, VkVertexInputAttributeDescription vertexInputAttributeDescriptions[VIA_TOTAL])
{
    int res = 0;

    if (mask & VIA_VERTEX_BIT)
    {
        vertexInputAttributeDescriptions[res].binding = binding;
        vertexInputAttributeDescriptions[res].location = VIA_VERTEX;
    	if (mask & VIA_VERTEX4_BIT)
		{
			vertexInputAttributeDescriptions[res].format = VK_FORMAT_R32G32B32A32_SFLOAT;
			vertexInputAttributeDescriptions[res].offset = offsetof(shadowCache_t, xyz);
		}
    	else
		{
			vertexInputAttributeDescriptions[res].format = VK_FORMAT_R32G32B32_SFLOAT;
			vertexInputAttributeDescriptions[res].offset = offsetof(idDrawVert, xyz);
		}
        res++;
    }
    if (mask & VIA_TEXCOORD_BIT)
    {
        vertexInputAttributeDescriptions[res].binding = binding;
        vertexInputAttributeDescriptions[res].location = VIA_TEXCOORD;
        vertexInputAttributeDescriptions[res].format = VK_FORMAT_R32G32_SFLOAT;
        vertexInputAttributeDescriptions[res].offset = offsetof(idDrawVert, st);
        res++;
    }
    if (mask & VIA_NORMAL_BIT)
    {
        vertexInputAttributeDescriptions[res].binding = binding;
        vertexInputAttributeDescriptions[res].location = VIA_NORMAL;
        vertexInputAttributeDescriptions[res].format = VK_FORMAT_R32G32B32_SFLOAT;
        vertexInputAttributeDescriptions[res].offset = offsetof(idDrawVert, normal);
        res++;
    }
    if (mask & VIA_COLOR_BIT)
    {
        vertexInputAttributeDescriptions[res].binding = binding;
        vertexInputAttributeDescriptions[res].location = VIA_COLOR;
        vertexInputAttributeDescriptions[res].format = VK_FORMAT_R8G8B8A8_UNORM;
        vertexInputAttributeDescriptions[res].offset = offsetof(idDrawVert, color);
        res++;
    }
    if (mask & VIA_TANGENT_BIT)
    {
        vertexInputAttributeDescriptions[res].binding = binding;
        vertexInputAttributeDescriptions[res].location = VIA_TANGENT;
        vertexInputAttributeDescriptions[res].format = VK_FORMAT_R32G32B32_SFLOAT;
        vertexInputAttributeDescriptions[res].offset = offsetof(idDrawVert, tangents);
        res++;
    }
    if (mask & VIA_BITANGENT_BIT)
    {
        vertexInputAttributeDescriptions[res].binding = binding;
        vertexInputAttributeDescriptions[res].location = VIA_BITANGENT;
        vertexInputAttributeDescriptions[res].format = VK_FORMAT_R32G32B32_SFLOAT;
        vertexInputAttributeDescriptions[res].offset = offsetof(idDrawVert, tangents) + sizeof(((idDrawVert *)0)->tangents[0]);
        res++;
    }

    return res;
}

VkBlendFactor R_VK_GetSrcBlendFactor(globalState_t stateBits)
{
	VkBlendFactor srcFactor;

	switch (stateBits & ST_SRCBLEND_BITS) {
		case ST_SRCBLEND_ZERO_BIT:
			srcFactor = VK_BLEND_FACTOR_ZERO;
			break;
		case ST_SRCBLEND_ONE_BIT:
			srcFactor = VK_BLEND_FACTOR_ONE;
			break;
		case ST_SRCBLEND_DST_COLOR_BIT:
			srcFactor = VK_BLEND_FACTOR_DST_COLOR;
			break;
		case ST_SRCBLEND_ONE_MINUS_DST_COLOR_BIT:
			srcFactor = VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
			break;
		case ST_SRCBLEND_SRC_ALPHA_BIT:
			srcFactor = VK_BLEND_FACTOR_SRC_ALPHA;
			break;
		case ST_SRCBLEND_ONE_MINUS_SRC_ALPHA_BIT:
			srcFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
			break;
		case ST_SRCBLEND_DST_ALPHA_BIT:
			srcFactor = VK_BLEND_FACTOR_DST_ALPHA;
			break;
		case ST_SRCBLEND_ONE_MINUS_DST_ALPHA_BIT:
			srcFactor = VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
			break;
		case ST_SRCBLEND_ALPHA_SATURATE_BIT:
			srcFactor = VK_BLEND_FACTOR_SRC_ALPHA_SATURATE;
			break;
		case ST_SRCBLEND_SRC_COLOR_BIT:
			srcFactor = VK_BLEND_FACTOR_SRC_COLOR;
			break;
		case ST_SRCBLEND_ONE_MINUS_SRC_COLOR_BIT:
			srcFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
			break;
		default:
			srcFactor = VK_BLEND_FACTOR_ONE;		// to get warning to shut up
			break;
	}

	return srcFactor;
}

VkBlendFactor R_VK_GetDstBlendFactor(globalState_t stateBits)
{
	VkBlendFactor dstFactor;
	switch (stateBits & ST_DSTBLEND_BITS) {
		case ST_DSTBLEND_ZERO_BIT:
			dstFactor = VK_BLEND_FACTOR_ZERO;
			break;
		case ST_DSTBLEND_ONE_BIT:
			dstFactor = VK_BLEND_FACTOR_ONE;
			break;
		case ST_DSTBLEND_SRC_COLOR_BIT:
			dstFactor = VK_BLEND_FACTOR_SRC_COLOR;
			break;
		case ST_DSTBLEND_ONE_MINUS_SRC_COLOR_BIT:
			dstFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
			break;
		case ST_DSTBLEND_SRC_ALPHA_BIT:
			dstFactor = VK_BLEND_FACTOR_SRC_ALPHA;
			break;
		case ST_DSTBLEND_ONE_MINUS_SRC_ALPHA_BIT:
			dstFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
			break;
		case ST_DSTBLEND_DST_ALPHA_BIT:
			dstFactor = VK_BLEND_FACTOR_DST_ALPHA;
			break;
		case ST_DSTBLEND_ONE_MINUS_DST_ALPHA_BIT:
			dstFactor = VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
			break;
        case ST_DSTBLEND_DST_COLOR_BIT:
			dstFactor = VK_BLEND_FACTOR_DST_COLOR;
			break;
		case ST_DSTBLEND_ONE_MINUS_DST_COLOR_BIT:
			dstFactor = VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
			break;
		default:
			dstFactor = VK_BLEND_FACTOR_ZERO;		// to get warning to shut up
			break;
	}
	return dstFactor;
}

VkPrimitiveTopology R_VK_GetPrimitiveTopology(globalState_t stateBits)
{
	VkPrimitiveTopology ret;
	switch (stateBits & ST_PRIMITIVE_BITS)
	{
		case ST_TRIANGLES_BIT:
			ret = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			break;
		case ST_LINES_BIT:
			ret = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
			break;
		case ST_POINTS_BIT:
			ret = VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
			break;
		case ST_LINE_LOOP_BIT:
			ret = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
			break;
		case ST_TRIANGLE_FAN_BIT:
			ret = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;
			break;
		case ST_TRIANGLE_STRIP_BIT:
			ret = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
			break;
		default:
			ret = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			break;
	}
	return ret;
}

VkBool32 R_VK_GetDepthWriteEnable(globalState_t stateBits)
{
	VkBool32 depthWriteEnable;
	if (stateBits & ST_DEPTH_MASK_BIT) {
		depthWriteEnable = VK_FALSE;
	} else {
		depthWriteEnable = VK_TRUE;
	}
	return depthWriteEnable;
}

VkColorComponentFlags R_VK_GetColorWriteMask(globalState_t stateBits)
{
	VkColorComponentFlags		r, g, b, a;
	VkColorComponentFlags colorWriteMask;
	r = (stateBits & ST_COLORMASK_R_BIT) ? 0 : VK_COLOR_COMPONENT_R_BIT;
	g = (stateBits & ST_COLORMASK_G_BIT) ? 0 : VK_COLOR_COMPONENT_G_BIT;
	b = (stateBits & ST_COLORMASK_B_BIT) ? 0 : VK_COLOR_COMPONENT_B_BIT;
	a = (stateBits & ST_COLORMASK_A_BIT) ? 0 : VK_COLOR_COMPONENT_A_BIT;
	colorWriteMask = r | g | b | a;
	return colorWriteMask;
}

VkPolygonMode R_VK_GetPolygonMode(globalState_t stateBits)
{
	VkPolygonMode polygonMode;
	switch (stateBits & ST_POLYGON_BITS)
	{
		case ST_POLYGON_FILL_BIT:
			polygonMode = VK_POLYGON_MODE_FILL;
			break;
		case ST_POLYGON_LINE_BIT:
			polygonMode = VK_POLYGON_MODE_LINE;
			break;
		case ST_POLYGON_POINT_BIT:
			polygonMode = VK_POLYGON_MODE_POINT;
			break;
		default:
			polygonMode = VK_POLYGON_MODE_FILL;
			break;
	}
	return polygonMode;
}

VkCullModeFlags R_VK_GetCullMode(globalState_t stateBits, bool isMirror)
{
	VkCullModeFlags cullMode;
	switch (stateBits & ST_CULL_BITS)
	{
		case ST_CULL_NONE_BIT:
			cullMode = VK_CULL_MODE_NONE;
			break;
		case ST_CULL_BACK_BIT:
			cullMode = isMirror ? VK_CULL_MODE_FRONT_BIT : VK_CULL_MODE_BACK_BIT;
			break;
		case ST_CULL_FRONT_BIT:
			cullMode = isMirror ? VK_CULL_MODE_BACK_BIT : VK_CULL_MODE_FRONT_BIT;
			break;
		case ST_CULL_TWO_SIDES_BIT:
			cullMode = VK_CULL_MODE_FRONT_AND_BACK;
			break;
		default:
			cullMode = VK_CULL_MODE_NONE;
			break;
	}
	return cullMode;
}

VkFrontFace R_VK_GetFrontFace(globalState_t state)
{
	VkFrontFace frontFace;
	if (state & ST_FRONT_CW_BIT) {
		frontFace = VK_FRONT_FACE_CLOCKWISE;
	} else {
		frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	}
	return frontFace;
}

VkCompareOp R_VK_GetDepthCompareOp(globalState_t stateBits)
{
	VkCompareOp depthCompareOp;
	switch (stateBits & ST_DEPTHFUNC_BITS)
	{
		case ST_DEPTHFUNC_ALWAYS_BIT:
			depthCompareOp = VK_COMPARE_OP_ALWAYS;
			break;
		case ST_DEPTHFUNC_NEVER_BIT:
			depthCompareOp = VK_COMPARE_OP_NEVER;
			break;
		case ST_DEPTHFUNC_LESS_BIT:
			depthCompareOp = VK_COMPARE_OP_LESS;
			break;
		case ST_DEPTHFUNC_EQUALS_BIT:
			depthCompareOp = VK_COMPARE_OP_EQUAL;
			break;
		case ST_DEPTHFUNC_LEQUALS_BIT:
			depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
			break;
		case ST_DEPTHFUNC_GREATER_BIT:
			depthCompareOp = VK_COMPARE_OP_GREATER;
			break;
		case ST_DEPTHFUNC_NEQUALS_BIT:
			depthCompareOp = VK_COMPARE_OP_NOT_EQUAL;
			break;
		case ST_DEPTHFUNC_GEQUALS_BIT:
			depthCompareOp = VK_COMPARE_OP_GREATER_OR_EQUAL;
			break;
		default:
			depthCompareOp = VK_COMPARE_OP_ALWAYS;
			break;
	}
	return depthCompareOp;
}

VkCompareOp R_VK_GetStencilFrontCompareOp(globalState2_t stateBits)
{
	VkCompareOp stencilCompareOp;
	switch (stateBits & ST_STENCILFUNC_FRONT_BITS)
	{
		case ST_STENCILFUNC_FRONT_ALWAYS_BIT:
			stencilCompareOp = VK_COMPARE_OP_ALWAYS;
			break;
		case ST_STENCILFUNC_FRONT_NEVER_BIT:
			stencilCompareOp = VK_COMPARE_OP_NEVER;
			break;
		case ST_STENCILFUNC_FRONT_LESS_BIT:
			stencilCompareOp = VK_COMPARE_OP_LESS;
			break;
		case ST_STENCILFUNC_FRONT_EQUALS_BIT:
			stencilCompareOp = VK_COMPARE_OP_EQUAL;
			break;
		case ST_STENCILFUNC_FRONT_LEQUALS_BIT:
			stencilCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
			break;
		case ST_STENCILFUNC_FRONT_GREATER_BIT:
			stencilCompareOp = VK_COMPARE_OP_GREATER;
			break;
		case ST_STENCILFUNC_FRONT_NEQUALS_BIT:
			stencilCompareOp = VK_COMPARE_OP_NOT_EQUAL;
			break;
		case ST_STENCILFUNC_FRONT_GEQUALS_BIT:
			stencilCompareOp = VK_COMPARE_OP_GREATER_OR_EQUAL;
			break;
		default:
			stencilCompareOp = VK_COMPARE_OP_ALWAYS;
			break;
	}
	return stencilCompareOp;
}

VkCompareOp R_VK_GetStencilBackCompareOp(globalState2_t stateBits)
{
	VkCompareOp stencilCompareOp;
	switch (stateBits & ST_STENCILFUNC_BACK_BITS)
	{
		case ST_STENCILFUNC_BACK_ALWAYS_BIT:
			stencilCompareOp = VK_COMPARE_OP_ALWAYS;
			break;
		case ST_STENCILFUNC_BACK_NEVER_BIT:
			stencilCompareOp = VK_COMPARE_OP_NEVER;
			break;
		case ST_STENCILFUNC_BACK_LESS_BIT:
			stencilCompareOp = VK_COMPARE_OP_LESS;
			break;
		case ST_STENCILFUNC_BACK_EQUALS_BIT:
			stencilCompareOp = VK_COMPARE_OP_EQUAL;
			break;
		case ST_STENCILFUNC_BACK_LEQUALS_BIT:
			stencilCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
			break;
		case ST_STENCILFUNC_BACK_GREATER_BIT:
			stencilCompareOp = VK_COMPARE_OP_GREATER;
			break;
		case ST_STENCILFUNC_BACK_NEQUALS_BIT:
			stencilCompareOp = VK_COMPARE_OP_NOT_EQUAL;
			break;
		case ST_STENCILFUNC_BACK_GEQUALS_BIT:
			stencilCompareOp = VK_COMPARE_OP_GREATER_OR_EQUAL;
			break;
		default:
			stencilCompareOp = VK_COMPARE_OP_ALWAYS;
			break;
	}
	return stencilCompareOp;
}

VkStencilOp R_VK_GetStencilFrontFailOp(globalState2_t stateBits)
{
	VkStencilOp stencilOp;
	switch (stateBits & ST_STENCILOP_FRONT_FAIL_BITS)
	{
		case ST_STENCILOP_FRONT_FAIL_KEEP_BIT:
			stencilOp = VK_STENCIL_OP_KEEP;
			break;
		case ST_STENCILOP_FRONT_FAIL_ZERO_BIT:
			stencilOp = VK_STENCIL_OP_ZERO;
			break;
		case ST_STENCILOP_FRONT_FAIL_REPLACE_BIT:
			stencilOp = VK_STENCIL_OP_REPLACE;
			break;
		case ST_STENCILOP_FRONT_FAIL_INCR_BIT:
			stencilOp = VK_STENCIL_OP_INCREMENT_AND_CLAMP;
			break;
		case ST_STENCILOP_FRONT_FAIL_DECR_BIT:
			stencilOp = VK_STENCIL_OP_DECREMENT_AND_CLAMP;
			break;
		case ST_STENCILOP_FRONT_FAIL_INVERT_BIT:
			stencilOp = VK_STENCIL_OP_INVERT;
			break;
		case ST_STENCILOP_FRONT_FAIL_INCRWRAP_BIT:
			stencilOp = VK_STENCIL_OP_INCREMENT_AND_WRAP;
			break;
		case ST_STENCILOP_FRONT_FAIL_DECRWRAP_BIT:
			stencilOp = VK_STENCIL_OP_DECREMENT_AND_WRAP;
			break;
		default:
			stencilOp = VK_STENCIL_OP_KEEP;
			break;
	}
	return stencilOp;
}

VkStencilOp R_VK_GetStencilFrontPassOp(globalState2_t stateBits)
{
	VkStencilOp stencilOp;
	switch (stateBits & ST_STENCILOP_FRONT_PASS_BITS)
	{
		case ST_STENCILOP_FRONT_PASS_KEEP_BIT:
			stencilOp = VK_STENCIL_OP_KEEP;
			break;
		case ST_STENCILOP_FRONT_PASS_ZERO_BIT:
			stencilOp = VK_STENCIL_OP_ZERO;
			break;
		case ST_STENCILOP_FRONT_PASS_REPLACE_BIT:
			stencilOp = VK_STENCIL_OP_REPLACE;
			break;
		case ST_STENCILOP_FRONT_PASS_INCR_BIT:
			stencilOp = VK_STENCIL_OP_INCREMENT_AND_CLAMP;
			break;
		case ST_STENCILOP_FRONT_PASS_DECR_BIT:
			stencilOp = VK_STENCIL_OP_DECREMENT_AND_CLAMP;
			break;
		case ST_STENCILOP_FRONT_PASS_INVERT_BIT:
			stencilOp = VK_STENCIL_OP_INVERT;
			break;
		case ST_STENCILOP_FRONT_PASS_INCRWRAP_BIT:
			stencilOp = VK_STENCIL_OP_INCREMENT_AND_WRAP;
			break;
		case ST_STENCILOP_FRONT_PASS_DECRWRAP_BIT:
			stencilOp = VK_STENCIL_OP_DECREMENT_AND_WRAP;
			break;
		default:
			stencilOp = VK_STENCIL_OP_KEEP;
			break;
	}
	return stencilOp;
}

VkStencilOp R_VK_GetStencilFrontDepthFailOp(globalState2_t stateBits)
{
	VkStencilOp stencilOp;
	switch (stateBits & ST_STENCILOP_FRONT_DEPTHFAIL_BITS)
	{
		case ST_STENCILOP_FRONT_DEPTHFAIL_KEEP_BIT:
			stencilOp = VK_STENCIL_OP_KEEP;
			break;
		case ST_STENCILOP_FRONT_DEPTHFAIL_ZERO_BIT:
			stencilOp = VK_STENCIL_OP_ZERO;
			break;
		case ST_STENCILOP_FRONT_DEPTHFAIL_REPLACE_BIT:
			stencilOp = VK_STENCIL_OP_REPLACE;
			break;
		case ST_STENCILOP_FRONT_DEPTHFAIL_INCR_BIT:
			stencilOp = VK_STENCIL_OP_INCREMENT_AND_CLAMP;
			break;
		case ST_STENCILOP_FRONT_DEPTHFAIL_DECR_BIT:
			stencilOp = VK_STENCIL_OP_DECREMENT_AND_CLAMP;
			break;
		case ST_STENCILOP_FRONT_DEPTHFAIL_INVERT_BIT:
			stencilOp = VK_STENCIL_OP_INVERT;
			break;
		case ST_STENCILOP_FRONT_DEPTHFAIL_INCRWRAP_BIT:
			stencilOp = VK_STENCIL_OP_INCREMENT_AND_WRAP;
			break;
		case ST_STENCILOP_FRONT_DEPTHFAIL_DECRWRAP_BIT:
			stencilOp = VK_STENCIL_OP_DECREMENT_AND_WRAP;
			break;
		default:
			stencilOp = VK_STENCIL_OP_KEEP;
			break;
	}
	return stencilOp;
}

VkStencilOp R_VK_GetStencilBackFailOp(globalState2_t stateBits)
{
	VkStencilOp stencilOp;
	switch (stateBits & ST_STENCILOP_BACK_FAIL_BITS)
	{
		case ST_STENCILOP_BACK_FAIL_KEEP_BIT:
			stencilOp = VK_STENCIL_OP_KEEP;
			break;
		case ST_STENCILOP_BACK_FAIL_ZERO_BIT:
			stencilOp = VK_STENCIL_OP_ZERO;
			break;
		case ST_STENCILOP_BACK_FAIL_REPLACE_BIT:
			stencilOp = VK_STENCIL_OP_REPLACE;
			break;
		case ST_STENCILOP_BACK_FAIL_INCR_BIT:
			stencilOp = VK_STENCIL_OP_INCREMENT_AND_CLAMP;
			break;
		case ST_STENCILOP_BACK_FAIL_DECR_BIT:
			stencilOp = VK_STENCIL_OP_DECREMENT_AND_CLAMP;
			break;
		case ST_STENCILOP_BACK_FAIL_INVERT_BIT:
			stencilOp = VK_STENCIL_OP_INVERT;
			break;
		case ST_STENCILOP_BACK_FAIL_INCRWRAP_BIT:
			stencilOp = VK_STENCIL_OP_INCREMENT_AND_WRAP;
			break;
		case ST_STENCILOP_BACK_FAIL_DECRWRAP_BIT:
			stencilOp = VK_STENCIL_OP_DECREMENT_AND_WRAP;
			break;
		default:
			stencilOp = VK_STENCIL_OP_KEEP;
			break;
	}
	return stencilOp;
}

VkStencilOp R_VK_GetStencilBackPassOp(globalState2_t stateBits)
{
	VkStencilOp stencilOp;
	switch (stateBits & ST_STENCILOP_BACK_PASS_BITS)
	{
		case ST_STENCILOP_BACK_PASS_KEEP_BIT:
			stencilOp = VK_STENCIL_OP_KEEP;
			break;
		case ST_STENCILOP_BACK_PASS_ZERO_BIT:
			stencilOp = VK_STENCIL_OP_ZERO;
			break;
		case ST_STENCILOP_BACK_PASS_REPLACE_BIT:
			stencilOp = VK_STENCIL_OP_REPLACE;
			break;
		case ST_STENCILOP_BACK_PASS_INCR_BIT:
			stencilOp = VK_STENCIL_OP_INCREMENT_AND_CLAMP;
			break;
		case ST_STENCILOP_BACK_PASS_DECR_BIT:
			stencilOp = VK_STENCIL_OP_DECREMENT_AND_CLAMP;
			break;
		case ST_STENCILOP_BACK_PASS_INVERT_BIT:
			stencilOp = VK_STENCIL_OP_INVERT;
			break;
		case ST_STENCILOP_BACK_PASS_INCRWRAP_BIT:
			stencilOp = VK_STENCIL_OP_INCREMENT_AND_WRAP;
			break;
		case ST_STENCILOP_BACK_PASS_DECRWRAP_BIT:
			stencilOp = VK_STENCIL_OP_DECREMENT_AND_WRAP;
			break;
		default:
			stencilOp = VK_STENCIL_OP_KEEP;
			break;
	}
	return stencilOp;
}

VkStencilOp R_VK_GetStencilBackDepthFailOp(globalState2_t stateBits)
{
	VkStencilOp stencilOp;
	switch (stateBits & ST_STENCILOP_BACK_DEPTHFAIL_BITS)
	{
		case ST_STENCILOP_BACK_DEPTHFAIL_KEEP_BIT:
			stencilOp = VK_STENCIL_OP_KEEP;
			break;
		case ST_STENCILOP_BACK_DEPTHFAIL_ZERO_BIT:
			stencilOp = VK_STENCIL_OP_ZERO;
			break;
		case ST_STENCILOP_BACK_DEPTHFAIL_REPLACE_BIT:
			stencilOp = VK_STENCIL_OP_REPLACE;
			break;
		case ST_STENCILOP_BACK_DEPTHFAIL_INCR_BIT:
			stencilOp = VK_STENCIL_OP_INCREMENT_AND_CLAMP;
			break;
		case ST_STENCILOP_BACK_DEPTHFAIL_DECR_BIT:
			stencilOp = VK_STENCIL_OP_DECREMENT_AND_CLAMP;
			break;
		case ST_STENCILOP_BACK_DEPTHFAIL_INVERT_BIT:
			stencilOp = VK_STENCIL_OP_INVERT;
			break;
		case ST_STENCILOP_BACK_DEPTHFAIL_INCRWRAP_BIT:
			stencilOp = VK_STENCIL_OP_INCREMENT_AND_WRAP;
			break;
		case ST_STENCILOP_BACK_DEPTHFAIL_DECRWRAP_BIT:
			stencilOp = VK_STENCIL_OP_DECREMENT_AND_WRAP;
			break;
		default:
			stencilOp = VK_STENCIL_OP_KEEP;
			break;
	}
	return stencilOp;
}

const char * R_VK_StateStr(const globalStateSet_t stateSet)
{
	globalState_t stateBits = stateSet.st1;
	globalState_t stateBits2 = stateSet.st2;
	static char str[1024*1024];
	str[0] = '\0';

	strcat(str, "[Depth]:");
	strcat(str, VKSTATE1_BOOL(stateSet, ST_DEPTH_TEST_BIT) ? "enable" : "disable");
	strcat(str, ",op=");
	switch (stateBits & ST_DEPTHFUNC_BITS)
	{
		case ST_DEPTHFUNC_ALWAYS_BIT:
			strcat(str, "always");
			break;
		case ST_DEPTHFUNC_NEVER_BIT:
			strcat(str, "never");
			break;
		case ST_DEPTHFUNC_LESS_BIT:
			strcat(str, "less");
			break;
		case ST_DEPTHFUNC_EQUALS_BIT:
			strcat(str, "equal");
			break;
		case ST_DEPTHFUNC_LEQUALS_BIT:
			strcat(str, "less-equal");
			break;
		case ST_DEPTHFUNC_GREATER_BIT:
			strcat(str, "greater");
			break;
		case ST_DEPTHFUNC_NEQUALS_BIT:
			strcat(str, "not-equal");
			break;
		case ST_DEPTHFUNC_GEQUALS_BIT:
			strcat(str, "greater-equal");
			break;
		default:
			strcat(str, "???");
			break;
	}
	strcat(str, ",write=");
	strcat(str, VKSTATE1_BOOL(stateSet, ST_DEPTH_MASK_BIT) ? "false" : "true");

	strcat(str, "|[Stencil]:");
	strcat(str, VKSTATE1_BOOL(stateSet, ST_STENCIL_TEST_BIT) ? "enable" : "disable");
	strcat(str, ",Front:");
	strcat(str, "stencil-test-fail=");
	switch (stateBits2 & ST_STENCILOP_FRONT_FAIL_BITS)
	{
		case ST_STENCILOP_FRONT_FAIL_KEEP_BIT:
			strcat(str, "keep");
			break;
		case ST_STENCILOP_FRONT_FAIL_ZERO_BIT:
			strcat(str, "zero");
			break;
		case ST_STENCILOP_FRONT_FAIL_REPLACE_BIT:
			strcat(str, "replace");
			break;
		case ST_STENCILOP_FRONT_FAIL_INCR_BIT:
			strcat(str, "incr");
			break;
		case ST_STENCILOP_FRONT_FAIL_DECR_BIT:
			strcat(str, "decr");
			break;
		case ST_STENCILOP_FRONT_FAIL_INVERT_BIT:
			strcat(str, "invert");
			break;
		case ST_STENCILOP_FRONT_FAIL_INCRWRAP_BIT:
			strcat(str, "incr-wrap");
			break;
		case ST_STENCILOP_FRONT_FAIL_DECRWRAP_BIT:
			strcat(str, "decr-wrap");
			break;
		default:
			strcat(str, "???");
			break;
	}
	strcat(str, ",depth-test-fail=");
	switch (stateBits2 & ST_STENCILOP_FRONT_DEPTHFAIL_BITS)
	{
		case ST_STENCILOP_FRONT_DEPTHFAIL_KEEP_BIT:
			strcat(str, "keep");
			break;
		case ST_STENCILOP_FRONT_DEPTHFAIL_ZERO_BIT:
			strcat(str, "zero");
			break;
		case ST_STENCILOP_FRONT_DEPTHFAIL_REPLACE_BIT:
			strcat(str, "replace");
			break;
		case ST_STENCILOP_FRONT_DEPTHFAIL_INCR_BIT:
			strcat(str, "incr");
			break;
		case ST_STENCILOP_FRONT_DEPTHFAIL_DECR_BIT:
			strcat(str, "decr");
			break;
		case ST_STENCILOP_FRONT_DEPTHFAIL_INVERT_BIT:
			strcat(str, "invert");
			break;
		case ST_STENCILOP_FRONT_DEPTHFAIL_INCRWRAP_BIT:
			strcat(str, "incr-wrap");
			break;
		case ST_STENCILOP_FRONT_DEPTHFAIL_DECRWRAP_BIT:
			strcat(str, "decr-wrap");
			break;
		default:
			strcat(str, "???");
			break;
	}
	strcat(str, ",pass=");
	switch (stateBits2 & ST_STENCILOP_FRONT_PASS_BITS)
	{
		case ST_STENCILOP_FRONT_PASS_KEEP_BIT:
			strcat(str, "keep");
			break;
		case ST_STENCILOP_FRONT_PASS_ZERO_BIT:
			strcat(str, "zero");
			break;
		case ST_STENCILOP_FRONT_PASS_REPLACE_BIT:
			strcat(str, "replace");
			break;
		case ST_STENCILOP_FRONT_PASS_INCR_BIT:
			strcat(str, "incr");
			break;
		case ST_STENCILOP_FRONT_PASS_DECR_BIT:
			strcat(str, "decr");
			break;
		case ST_STENCILOP_FRONT_PASS_INVERT_BIT:
			strcat(str, "invert");
			break;
		case ST_STENCILOP_FRONT_PASS_INCRWRAP_BIT:
			strcat(str, "incr-wrap");
			break;
		case ST_STENCILOP_FRONT_PASS_DECRWRAP_BIT:
			strcat(str, "decr-wrap");
			break;
		default:
			strcat(str, "???");
			break;
	}
	strcat(str, ",op=");
	switch (stateBits2 & ST_STENCILFUNC_FRONT_BITS)
	{
		case ST_STENCILFUNC_FRONT_ALWAYS_BIT:
			strcat(str, "always");
			break;
		case ST_STENCILFUNC_FRONT_NEVER_BIT:
			strcat(str, "never");
			break;
		case ST_STENCILFUNC_FRONT_LESS_BIT:
			strcat(str, "less");
			break;
		case ST_STENCILFUNC_FRONT_EQUALS_BIT:
			strcat(str, "equal");
			break;
		case ST_STENCILFUNC_FRONT_LEQUALS_BIT:
			strcat(str, "less-equal");
			break;
		case ST_STENCILFUNC_FRONT_GREATER_BIT:
			strcat(str, "greater");
			break;
		case ST_STENCILFUNC_FRONT_NEQUALS_BIT:
			strcat(str, "not-equal");
			break;
		case ST_STENCILFUNC_FRONT_GEQUALS_BIT:
			strcat(str, "greater-equal");
			break;
		default:
			strcat(str, "???");
			break;
	}
	strcat(str, ",Back:");
	strcat(str, "stencil-test-fail=");
	switch (stateBits2 & ST_STENCILOP_BACK_FAIL_BITS)
	{
		case ST_STENCILOP_BACK_FAIL_KEEP_BIT:
			strcat(str, "keep");
			break;
		case ST_STENCILOP_BACK_FAIL_ZERO_BIT:
			strcat(str, "zero");
			break;
		case ST_STENCILOP_BACK_FAIL_REPLACE_BIT:
			strcat(str, "replace");
			break;
		case ST_STENCILOP_BACK_FAIL_INCR_BIT:
			strcat(str, "incr");
			break;
		case ST_STENCILOP_BACK_FAIL_DECR_BIT:
			strcat(str, "decr");
			break;
		case ST_STENCILOP_BACK_FAIL_INVERT_BIT:
			strcat(str, "invert");
			break;
		case ST_STENCILOP_BACK_FAIL_INCRWRAP_BIT:
			strcat(str, "incr-wrap");
			break;
		case ST_STENCILOP_BACK_FAIL_DECRWRAP_BIT:
			strcat(str, "decr-wrap");
			break;
		default:
			strcat(str, "???");
			break;
	}
	strcat(str, ",depth-test-fail=");
	switch (stateBits2 & ST_STENCILOP_BACK_DEPTHFAIL_BITS)
	{
		case ST_STENCILOP_BACK_DEPTHFAIL_KEEP_BIT:
			strcat(str, "keep");
			break;
		case ST_STENCILOP_BACK_DEPTHFAIL_ZERO_BIT:
			strcat(str, "zero");
			break;
		case ST_STENCILOP_BACK_DEPTHFAIL_REPLACE_BIT:
			strcat(str, "replace");
			break;
		case ST_STENCILOP_BACK_DEPTHFAIL_INCR_BIT:
			strcat(str, "incr");
			break;
		case ST_STENCILOP_BACK_DEPTHFAIL_DECR_BIT:
			strcat(str, "decr");
			break;
		case ST_STENCILOP_BACK_DEPTHFAIL_INVERT_BIT:
			strcat(str, "invert");
			break;
		case ST_STENCILOP_BACK_DEPTHFAIL_INCRWRAP_BIT:
			strcat(str, "incr-wrap");
			break;
		case ST_STENCILOP_BACK_DEPTHFAIL_DECRWRAP_BIT:
			strcat(str, "decr-wrap");
			break;
		default:
			strcat(str, "???");
			break;
	}
	strcat(str, ",pass=");
	switch (stateBits2 & ST_STENCILOP_BACK_PASS_BITS)
	{
		case ST_STENCILOP_BACK_PASS_KEEP_BIT:
			strcat(str, "keep");
			break;
		case ST_STENCILOP_BACK_PASS_ZERO_BIT:
			strcat(str, "zero");
			break;
		case ST_STENCILOP_BACK_PASS_REPLACE_BIT:
			strcat(str, "replace");
			break;
		case ST_STENCILOP_BACK_PASS_INCR_BIT:
			strcat(str, "incr");
			break;
		case ST_STENCILOP_BACK_PASS_DECR_BIT:
			strcat(str, "decr");
			break;
		case ST_STENCILOP_BACK_PASS_INVERT_BIT:
			strcat(str, "invert");
			break;
		case ST_STENCILOP_BACK_PASS_INCRWRAP_BIT:
			strcat(str, "incr-wrap");
			break;
		case ST_STENCILOP_BACK_PASS_DECRWRAP_BIT:
			strcat(str, "decr-wrap");
			break;
		default:
			strcat(str, "???");
			break;
	}
	strcat(str, ",op=");
	switch (stateBits2 & ST_STENCILFUNC_BACK_BITS)
	{
		case ST_STENCILFUNC_BACK_ALWAYS_BIT:
			strcat(str, "always");
			break;
		case ST_STENCILFUNC_BACK_NEVER_BIT:
			strcat(str, "never");
			break;
		case ST_STENCILFUNC_BACK_LESS_BIT:
			strcat(str, "less");
			break;
		case ST_STENCILFUNC_BACK_EQUALS_BIT:
			strcat(str, "equal");
			break;
		case ST_STENCILFUNC_BACK_LEQUALS_BIT:
			strcat(str, "less-equal");
			break;
		case ST_STENCILFUNC_BACK_GREATER_BIT:
			strcat(str, "greater");
			break;
		case ST_STENCILFUNC_BACK_NEQUALS_BIT:
			strcat(str, "not-equal");
			break;
		case ST_STENCILFUNC_BACK_GEQUALS_BIT:
			strcat(str, "greater-equal");
			break;
		default:
			strcat(str, "???");
			break;
	}

	strcat(str, "|[Color]:");
	strcat(str, "red=");
	strcat(str, VKSTATE1_BOOL(stateSet, ST_COLORMASK_R_BIT) ? "false" : "true");
	strcat(str, ",green=");
	strcat(str, VKSTATE1_BOOL(stateSet, ST_COLORMASK_G_BIT) ? "false" : "true");
	strcat(str, ",blue=");
	strcat(str, VKSTATE1_BOOL(stateSet, ST_COLORMASK_B_BIT) ? "false" : "true");
	strcat(str, ",alpha=");
	strcat(str, VKSTATE1_BOOL(stateSet, ST_COLORMASK_A_BIT) ? "false" : "true");

	strcat(str, "|[Polygon]:");
	switch (stateBits & ST_PRIMITIVE_BITS)
	{
		case ST_TRIANGLES_BIT:
			strcat(str, "triangles");
			break;
		case ST_LINES_BIT:
			strcat(str, "lines");
			break;
		case ST_POINTS_BIT:
			strcat(str, "points");
			break;
		case ST_LINE_LOOP_BIT:
			strcat(str, "line-strip");
			break;
		case ST_TRIANGLE_FAN_BIT:
			strcat(str, "triangle-fan");
			break;
		case ST_TRIANGLE_STRIP_BIT:
			strcat(str, "triangle-strip");
			break;
		default:
			strcat(str, "???");
			break;
	}
	strcat(str, ",mode=");
	switch (stateBits & ST_POLYGON_BITS)
	{
		case ST_POLYGON_FILL_BIT:
			strcat(str, "fill");
			break;
		case ST_POLYGON_LINE_BIT:
			strcat(str, "line");
			break;
		case ST_POLYGON_POINT_BIT:
			strcat(str, "point");
			break;
		default:
			strcat(str, "???");
			break;
	}

	strcat(str, "|[Cull]:");
	strcat(str, "mode=");
	switch (stateBits & ST_CULL_BITS)
	{
		case ST_CULL_NONE_BIT:
			strcat(str, "none");
			break;
		case ST_CULL_BACK_BIT:
			strcat(str, vk_backend.isMirror ? "front" : "back");
			break;
		case ST_CULL_FRONT_BIT:
			strcat(str, vk_backend.isMirror ? "back" : "front");
			break;
		case ST_CULL_TWO_SIDES_BIT:
			strcat(str, "front-back");
			break;
		default:
			strcat(str, "???");
			break;
	}
	strcat(str, ",face=");
	strcat(str, VKSTATE1_BOOL(stateSet, ST_FRONT_CW_BIT) ? "cw" : "ccw");

	strcat(str, "|[Blend]:");
	strcat(str, VKSTATE1_BOOL(stateSet, ST_BLEND_BIT) ? "enable" : "disable");
	strcat(str, ",src=");
	switch (stateBits & ST_SRCBLEND_BITS) {
		case ST_SRCBLEND_ZERO_BIT:
			strcat(str, "zero");
			break;
		case ST_SRCBLEND_ONE_BIT:
			strcat(str, "one");
			break;
		case ST_SRCBLEND_DST_COLOR_BIT:
			strcat(str, "dst_color");
			break;
		case ST_SRCBLEND_ONE_MINUS_DST_COLOR_BIT:
			strcat(str, "one-dst_color");
			break;
		case ST_SRCBLEND_SRC_ALPHA_BIT:
			strcat(str, "src_alpha");
			break;
		case ST_SRCBLEND_ONE_MINUS_SRC_ALPHA_BIT:
			strcat(str, "one-src_alpha");
			break;
		case ST_SRCBLEND_DST_ALPHA_BIT:
			strcat(str, "dst_alpha");
			break;
		case ST_SRCBLEND_ONE_MINUS_DST_ALPHA_BIT:
			strcat(str, "one-dst_aplha");
			break;
		case ST_SRCBLEND_ALPHA_SATURATE_BIT:
			strcat(str, "alpha_saturate");
			break;
		case ST_SRCBLEND_SRC_COLOR_BIT:
			strcat(str, "src_color");
			break;
		case ST_SRCBLEND_ONE_MINUS_SRC_COLOR_BIT:
			strcat(str, "one-src_color");
			break;
		default:
			strcat(str, "???");
			break;
	}

	strcat(str, ",dst=");
	switch (stateBits & ST_DSTBLEND_BITS) {
		case ST_DSTBLEND_ZERO_BIT:
			strcat(str, "zero");
			break;
		case ST_DSTBLEND_ONE_BIT:
			strcat(str, "one");
			break;
		case ST_DSTBLEND_SRC_COLOR_BIT:
			strcat(str, "src_color");
			break;
		case ST_DSTBLEND_ONE_MINUS_SRC_COLOR_BIT:
			strcat(str, "one-src_color");
			break;
		case ST_DSTBLEND_SRC_ALPHA_BIT:
			strcat(str, "src_alpha");
			break;
		case ST_DSTBLEND_ONE_MINUS_SRC_ALPHA_BIT:
			strcat(str, "one-src_alpha");
			break;
		case ST_DSTBLEND_DST_ALPHA_BIT:
			strcat(str, "dst_alpha");
			break;
		case ST_DSTBLEND_ONE_MINUS_DST_ALPHA_BIT:
			strcat(str, "one-dst_alpha");
			break;
        case ST_DSTBLEND_DST_COLOR_BIT:
			strcat(str, "dst_color");
			break;
		case ST_DSTBLEND_ONE_MINUS_DST_COLOR_BIT:
			strcat(str, "one-dst_color");
			break;
		default:
			strcat(str, "???");
			break;
	}

	return str;
}
