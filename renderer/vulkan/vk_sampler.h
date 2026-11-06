#ifndef _VULKAN_VKSAMPLER_H
#define _VULKAN_VKSAMPLER_H

#if 1
typedef struct samplerKey_s
{
	int repeatS;
	int repeatT;
	int repeatU;
	float lod;
	bool linear;
	bool depthCmp;
} samplerKey_t;

#define samplerKeyEquals(a, b) \
	((a).repeatS == (b).repeatS \
		&& (a).repeatT == (b).repeatT \
		&& (a).repeatU == (b).repeatU \
		&& (a).linear == (b).linear \
		&& (a).depthCmp == (b).depthCmp \
		&& (a).lod == (b).lod)
#else
typedef int samplerKey_t;
#define samplerKeyEquals(a, b) ((a) == (b))
#endif

VkSampler R_VK_AllocSampler(bool linear, int repeatS, int repeatT, int repeatU, bool depthCmp, float lod);

#endif //_VULKAN_VKSAMPLER_H
