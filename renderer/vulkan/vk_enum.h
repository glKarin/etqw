#ifndef _VULKAN_ENUM_H
#define _VULKAN_ENUM_H

#define VKENUM_TO_NAME_FUNC_PROTO(enumType) const char * VK_##enumType##ToName(enumType value)
#define VKENUMBIT_TO_NAME_FUNC_PROTO(enumType) const char * VK_##enumType##BitToName(enumType value)

VKENUM_TO_NAME_FUNC_PROTO(VkPhysicalDeviceType);
VKENUMBIT_TO_NAME_FUNC_PROTO(VkQueueFlags);
VKENUM_TO_NAME_FUNC_PROTO(VkObjectType);
VKENUM_TO_NAME_FUNC_PROTO(VkFormat);
VKENUM_TO_NAME_FUNC_PROTO(VkColorSpaceKHR);
VKENUM_TO_NAME_FUNC_PROTO(VkSamplerAddressMode);
VKENUM_TO_NAME_FUNC_PROTO(VkFilter);
VKENUM_TO_NAME_FUNC_PROTO(VkPresentModeKHR);

#endif //_VULKAN_ENUM_H
