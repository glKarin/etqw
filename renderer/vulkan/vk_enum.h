#ifndef _VULKAN_ENUM_H
#define _VULKAN_ENUM_H

#define VKENUM_TO_NAME_FUNC_PROTO(enumType) const char * VK_##enumType##_ToName(enumType value)
#define VKENUMBIT_TO_NAME_FUNC_PROTO(enumType, flagsType) const char * VK_##enumType##_BitToName(flagsType value)

VKENUM_TO_NAME_FUNC_PROTO(VkPhysicalDeviceType);
VKENUMBIT_TO_NAME_FUNC_PROTO(VkQueueFlagBits, VkQueueFlags);
VKENUM_TO_NAME_FUNC_PROTO(VkObjectType);
VKENUM_TO_NAME_FUNC_PROTO(VkFormat);
VKENUM_TO_NAME_FUNC_PROTO(VkColorSpaceKHR);
VKENUM_TO_NAME_FUNC_PROTO(VkSamplerAddressMode);
VKENUM_TO_NAME_FUNC_PROTO(VkFilter);
VKENUM_TO_NAME_FUNC_PROTO(VkPresentModeKHR);
VKENUMBIT_TO_NAME_FUNC_PROTO(VkCompositeAlphaFlagBitsKHR, VkCompositeAlphaFlagsKHR);
VKENUMBIT_TO_NAME_FUNC_PROTO(VkSurfaceTransformFlagBitsKHR, VkSurfaceTransformFlagsKHR);

#endif //_VULKAN_ENUM_H
