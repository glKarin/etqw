#include "../idlib/precompiled.h"
#pragma hdrstop

#include "../tr_local.h"

#if IDVK_CONTAINER_SIZE_CONFIG
#define IDVK_FRAME_QUERY_COUNT ( r_vkQueryFrameCount.GetInteger() )
#else
#define IDVK_FRAME_QUERY_COUNT 256
#endif

#define currentQueryPool query.queryPool[vk_context.frameIndex]
#define currentQueryResult query.queryResult[vk_context.frameIndex]

void R_VK_InitQuery(void)
{
    VKZERO(vk_backend.query);

    for(int i = 0; i < NUM_INFLIGHT_FRAME; i++)
    {
        VK_AllocQueryPool(&vk_backend.query.queryPool[i]);
        VK_CreateQueryPool(vk_backend.query.queryPool[i], VK_QUERY_TYPE_OCCLUSION, IDVK_FRAME_QUERY_COUNT);

        VKARRAY_INIT(uint64_t, vk_backend.query.queryResult[i], vk_backend.query.queryPool[i]->numQuery);
        VKARRAY_ALLOC(uint64_t, vk_backend.query.queryResult[i]);
        vk_backend.query.queryResult[i].size = 0;
    }
}

void R_VK_ResetQuery(void)
{
    if(vk_backend.currentQueryResult.size == 0)
    {
        VK_ResetQueryPool(vk_backend.currentQueryPool);
        vk_backend.currentQueryResult.size = vk_backend.currentQueryPool->numQuery;
        return;
    }

    if(vk_backend.currentQueryResult.num == 0)
        return;

    VK_ResetQueryPool(vk_backend.currentQueryPool);
    //VKPTRNZERO(vk_backend.query.result[vk_context.frameIndex], vk_backend.query.frameUsed[vk_context.frameIndex]);
    vk_backend.currentQueryResult.num = 0;
}

void R_VK_BeginQuery(bool count)
{
    assert(vk_backend.currentQueryResult.num < vk_backend.currentQueryPool->numQuery);

    VK_BeginQuery(vk_backend.currentQueryPool, vk_backend.currentQueryResult.num, count);
}

void R_VK_EndQuery(void)
{
    assert(vk_backend.currentQueryResult.num < vk_backend.currentQueryPool->numQuery);

    VK_EndQuery(vk_backend.currentQueryPool, vk_backend.currentQueryResult.num);
    vk_backend.currentQueryResult.num++;
}

void R_VK_GetQueryResult(void)
{
    if(vk_backend.query.queryResult[vk_context.frameIndex].num == 0)
        return;
    VK_GetQueryResult(vk_backend.currentQueryPool, 0, vk_backend.currentQueryResult.num, vk_backend.currentQueryResult.list);
}

uint64_t R_VK_GetQueryObject(int offset)
{
    if(offset < vk_backend.currentQueryResult.num)
        return vk_backend.currentQueryResult.list[offset];
    else
        return ID_VK_INVALID_QUERY_RESULT;
}