#include "../idlib/precompiled.h"
#pragma hdrstop

#include "../tr_local.h"

static VkSampler R_VK_FindSampler(VKSamplerKey_t key)
{
	VKMAP_FIND(VkSampler, VKSamplerKey_t, vk_backend.samplerPool, key, samplerKeyEquals);
	return _vkc_value;
}

static void R_VK_SetSampler(VKSamplerKey_t key, VkSampler sampler)
{
	VKMAP_ALLOC(VkSampler, VKSamplerKey_t, vk_backend.samplerPool);
	*_vkc_key = key;
	*_vkc_value = sampler;
}

VKSamplerKey_t R_VK_GenSamplerKey(bool minLinear, bool magLinear, bool mipmapLinear, int repeatS, int repeatT, int repeatU, bool depthCmp, float lod)
{
#if 1
    VKSamplerKey_t key;
    key.repeatS = repeatS;
    key.repeatT = repeatT;
    key.repeatU = repeatU;
    key.depthCmp = depthCmp;
    key.minLinear = minLinear;
    key.magLinear = magLinear;
    key.mipmapLinear = mipmapLinear;
    key.lod = lod;
    return key;
#else
    char text[1024];
    snprintf(text, sizeof(text), "tf=%d_%d_%d&tr=%d_%d_%d&cmp=%d&lod=%f", minLinear, magLinear, mipmapLinear, repeatS, repeatT, repeatU, depthCmp, lod);
    return VK_StrIHash(text);
#endif
}

static VkSampler R_VK_CreateSampler(bool minLinear, bool magLinear, bool mipmapLinear, int repeatS, int repeatT, int repeatU, bool depthCmp, float lod)
{
    VkSampler *sampler;
    VK_AllocSampler(&sampler);
    VK_CreateSampler(sampler, minLinear, magLinear, mipmapLinear, repeatS, repeatT, repeatU, depthCmp, 1.0f, lod);
    VKDEBUG_SET_NAME(VK_OBJECT_TYPE_SAMPLER, (uint64_t)*sampler, "filter=%d/%d/%d,repeat=%d/%d/%d,depth-cmp=%d,lod=%f", minLinear, magLinear, mipmapLinear, repeatS, repeatT, repeatU, depthCmp, lod);
    return *sampler;
}

VkSampler R_VK_AllocSampler(bool minLinear, bool magLinear, bool mipmapLinear, int repeatS, int repeatT, int repeatU, bool depthCmp, float lod)
{
    VKSamplerKey_t key = R_VK_GenSamplerKey(minLinear, magLinear, mipmapLinear, repeatS, repeatT, repeatU, depthCmp, lod);
    VkSampler cachedSampler = R_VK_FindSampler(key);
    if (VK_NULL_HANDLE != cachedSampler)
        return cachedSampler;

    VkSampler sampler = R_VK_CreateSampler(minLinear, magLinear, mipmapLinear, repeatS, repeatT, repeatU, depthCmp, lod);
    R_VK_SetSampler(key, sampler);
    return sampler;
}

bool R_VK_FindSamplerKey(VkSampler sampler, VKSamplerKey_t *key)
{
    VKMAP_FOREACH(VkSampler, VKSamplerKey_t, vk_backend.samplerPool)
    {
        if (vk_backend.samplerPool.list[i] == sampler)
        {
            *key = vk_backend.samplerPool.keys[i];
            return true;
        }
    }
    return false;
}

void R_VK_ListSampler(const idCmdArgs &)
{
    common->Printf("Sampler pool usage: %d/%d\n", vk_backend.samplerPool.num, vk_backend.samplerPool.size);

    VKSamplerKey_t *key;
    VKMAP_FOREACH(VkSampler, VKSamplerKey_t, vk_backend.samplerPool)
    {
        key = &vk_backend.samplerPool.keys[i];
        common->Printf("%3d: %p -> filter: min=%s, mag=%s, mipmap=%s; repeat: x=%s, y=%s, z=%s; compare depth=%s; max lod=%f\n", i + 1, vk_backend.samplerPool.list[i],
            VK_VkFilterToName(key->minLinear ? VK_FILTER_LINEAR : VK_FILTER_NEAREST),
            VK_VkFilterToName(key->magLinear ? VK_FILTER_LINEAR : VK_FILTER_NEAREST),
            VK_VkFilterToName(key->mipmapLinear ? VK_FILTER_LINEAR : VK_FILTER_NEAREST),
            VK_VkSamplerAddressModeToName((VkSamplerAddressMode)key->repeatS),
            VK_VkSamplerAddressModeToName((VkSamplerAddressMode)key->repeatT),
            VK_VkSamplerAddressModeToName((VkSamplerAddressMode)key->repeatU),
            key->depthCmp ? "true" : "false", key->lod);
    }
}
