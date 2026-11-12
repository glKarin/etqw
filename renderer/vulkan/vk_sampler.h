#ifndef _VULKAN_VKSAMPLER_H
#define _VULKAN_VKSAMPLER_H

#if 1
typedef struct VKSamplerKey_s
{
	int repeatS;
	int repeatT;
	int repeatU;
	float lod;
	bool minLinear;
	bool magLinear;
	bool mipmapLinear;
	bool depthCmp;
} VKSamplerKey_t;

#define samplerKeyEquals(a, b) \
	((a).repeatS == (b).repeatS \
		&& (a).repeatT == (b).repeatT \
		&& (a).repeatU == (b).repeatU \
		&& (a).minLinear == (b).minLinear \
		&& (a).magLinear == (b).magLinear \
		&& (a).mipmapLinear == (b).mipmapLinear \
		&& (a).depthCmp == (b).depthCmp \
		&& (a).lod == (b).lod)
#else
typedef int VKSamplerKey_t;
#define samplerKeyEquals(a, b) ((a) == (b))
#endif

VKSamplerKey_t R_VK_GenSamplerKey(bool minLinear, bool magLinear, bool mipmapLinear, int repeatS, int repeatT, int repeatU, bool depthCmp, float lod);
VkSampler R_VK_AllocSampler(bool minLinear, bool magLinear, bool mipmapLinear, int repeatS, int repeatT, int repeatU, bool depthCmp, float lod);
bool R_VK_FindSamplerKey(VkSampler sampler, VKSamplerKey_t *key);
void R_VK_ListSampler(const idCmdArgs &);

#endif //_VULKAN_VKSAMPLER_H
