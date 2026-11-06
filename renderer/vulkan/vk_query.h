#ifndef _VULKAN_VKQUERY_H
#define _VULKAN_VKQUERY_H

#define ID_VK_INVALID_QUERY_RESULT (-1)

typedef struct VKQuery_s
{
    VKQueryPool_t *queryPool[NUM_INFLIGHT_FRAME];
    VKARRAY(uint64_t) queryResult[NUM_INFLIGHT_FRAME];
} VKQuery_t;

void R_VK_InitQuery(void);
void R_VK_ResetQuery(void);
void R_VK_GetQueryResult(void);

void R_VK_BeginQuery(bool count);
void R_VK_EndQuery(void);
uint64_t R_VK_GetQueryObject(int offset);

#endif //_VULKAN_VKQUERY_H
