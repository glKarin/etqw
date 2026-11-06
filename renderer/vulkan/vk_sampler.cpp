#include "../idlib/precompiled.h"
#pragma hdrstop

#include "../tr_local.h"

static VkSampler R_VK_FindSampler(samplerKey_t key)
{
	VKMAP_FIND(VkSampler, samplerKey_t, vk_backend.samplerPool, key, samplerKeyEquals);
	return _vkc_value;
}

static void R_VK_SetSampler(samplerKey_t key, VkSampler sampler)
{
	VKMAP_ALLOC(VkSampler, samplerKey_t, vk_backend.samplerPool);
	*_vkc_key = key;
	*_vkc_value = sampler;
}

static samplerKey_t R_VK_GenSamplerKey(bool linear, int repeatS, int repeatT, int repeatU, bool depthCmp, float lod)
{
#if 1
    samplerKey_t key;
    key.repeatS = repeatS;
    key.repeatT = repeatT;
    key.repeatU = repeatU;
    key.depthCmp = depthCmp;
    key.linear = linear;
    key.lod = lod;
    return key;
#else
    char text[1024];
    snprintf(text, sizeof(text), "tf=%d&tr=%d&cmp=%d&lod=%f", linear, repeat, depthCmp, lod);
    return VK_StrIHash(text);
#endif
}

static VkSampler R_VK_CreateSampler(bool linear, int repeatS, int repeatT, int repeatU, bool depthCmp, float lod)
{
    VkSampler *sampler;
    VK_AllocSampler(&sampler);
    VK_CreateSampler(sampler, linear, repeatS, repeatT, repeatU, depthCmp, 1.0f, lod);
    VKDEBUG_SET_NAME(VK_OBJECT_TYPE_SAMPLER, (uint64_t)*sampler, "linear=%d,repeat=%d/%d/%d,depth-cmp=%d,lod=%f", linear, repeatS, repeatT, repeatU, depthCmp, lod);
    return *sampler;
}

VkSampler R_VK_AllocSampler(bool linear, int repeatS, int repeatT, int repeatU, bool depthCmp, float lod)
{
    samplerKey_t key = R_VK_GenSamplerKey(linear, repeatS, repeatT, repeatU, depthCmp, lod);
    VkSampler cachedSampler = R_VK_FindSampler(key);
    if (VK_NULL_HANDLE != cachedSampler)
        return cachedSampler;

    VkSampler sampler = R_VK_CreateSampler(linear, repeatS, repeatT, repeatU, depthCmp, lod);
    R_VK_SetSampler(key, sampler);
    return sampler;
}
