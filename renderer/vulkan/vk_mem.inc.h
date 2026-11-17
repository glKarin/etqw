#ifdef _VULKAN_MEM_H

#define VKMALLOC(size, tag) VK_Malloc(size, tag)
#define VKMALLOCT(size, tag) VK_Malloc(size, #tag)
#define VKFREE(ptr) VK_Free(ptr)

#ifdef _VK_MEM_DEF

_VK_MEM_DEF void * VK_Malloc(int size, const char *tag);
_VK_MEM_DEF void VK_Free(void *ptr);
_VK_MEM_DEF void VK_ListMemory(bool full);

#undef _VK_MEM_DEF

#endif

#ifdef _VK_MEM_IMPL
#include <stdint.h>

#define VKCMA_GRANULARITY 1024
#define VKCMA_TAG_SIZE 64

typedef struct VKMemoryBlock_s
{
    void *ptr;
    uint32_t size;
    char tag[VKCMA_TAG_SIZE];
} VKMemoryBlock_t;

typedef struct VKMemoryAllocator_s
{
    VKMemoryBlock_t *list;
    int num;
    int size;
    uint32_t allocSize;
    uint32_t allocCount;
    uint32_t freeSize;
    uint32_t freeCount;
    uint32_t totalSize;
    uint32_t totalCount;
} VKMemoryAllocator_t;

static VKMemoryAllocator_t cma;

static VKMemoryBlock_t * VK_AllocList(VKMemoryAllocator_t *ma)
{
    if (!ma->list)
    {
        int len = sizeof(*ma->list) * VKCMA_GRANULARITY;
        ma->list = (VKMemoryBlock_t *)malloc(len);
        memset(ma->list, 0, len);
        ma->size = VKCMA_GRANULARITY;
        ma->num = 0;
    }
    else if (ma->num == ma->size)
    {
        int len = sizeof(*ma->list) * (ma->size + VKCMA_GRANULARITY);
        VKMemoryBlock_t *old = ma->list;
        ma->list = (VKMemoryBlock_t *)malloc(len);
        memset(ma->list, 0, len);
        memcpy(ma->list, old, sizeof(*ma->list) * ma->size);
        free(old);
        ma->size += VKCMA_GRANULARITY;
    }
    return &ma->list[ma->num++];
}

static uint32_t VK_FreeList(VKMemoryAllocator_t *ma, void *ptr, VKMemoryBlock_t *info)
{
    if (!ma->list)
        return 0;
    for (int i = 0; i < ma->num; i++)
    {
        if (ptr == ma->list[i].ptr)
        {
            if (info)
                *info = ma->list[i];
            int size = ma->list[i].size;
            for (int m = i; m < ma->num - 1; m++)
            {
                ma->list[m] = ma->list[m + 1];
            }
            ma->num--;

            if (ma->num == 0)
            {
                free(ma->list);
                ma->list = NULL;
                ma->size = 0;
            }

            return size;
        }
    }

    //assert(0);
    return 0;
}

_VK_MEM_IMPL void * VK_Malloc(int size, const char *tag)
{
    void *ret = malloc(size);
    VKMemoryBlock_t *ptr = VK_AllocList(&cma);
    ptr->ptr = ret;
    ptr->size = size;
    strncpy(ptr->tag, tag, sizeof(ptr->tag));
#ifdef _VK_MEM_VERBOSE
    printf("CMA Alloc %d: %p %u %s\n", cma.num, ret, size, tag);
#endif
    cma.allocCount++;
    cma.allocSize += size;
    cma.totalCount++;
    cma.totalSize += size;
    return ret;
}

_VK_MEM_IMPL void VK_Free(void *ptr)
{
    if (!ptr)
        return;

    VKMemoryBlock_t info;
    uint32_t size = VK_FreeList(&cma, ptr, &info);
    free(ptr);
    if (size > 0)
    {
#ifdef _VK_MEM_VERBOSE
        printf("CMA Free %d: %p %u %s\n", cma.num, ptr, size, info.tag);
#endif
        cma.freeCount++;
        cma.freeSize += size;
        cma.allocCount--;
        cma.allocSize -= size;
    }
}

_VK_MEM_IMPL void VK_ListMemory(bool full)
{
    printf("CMA: num=%d, size=%d, count=%u | %u | %u, bytes=%u | %u | %u\n",
        cma.num, cma.size,
        cma.allocCount, cma.freeCount, cma.totalCount,
        cma.allocSize, cma.freeSize, cma.totalSize
        );
    if (!full)
        return;

    if (!cma.list)
    {
        printf("CMA: empty\n");
        return;
    }

    for (int i = 0; i < cma.num; i++)
    {
        printf("%3d: %p %u %s\n", i, cma.list[i].ptr, cma.list[i].size, cma.list[i].tag);
    }
}

#undef _VK_MEM_IMPL
#endif //_VK_MEM_IMPL

#endif //_VULKAN_MEM_H
