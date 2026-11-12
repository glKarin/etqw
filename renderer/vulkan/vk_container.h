#ifndef _VULKAN_VKCONTAINER_H
#define _VULKAN_VKCONTAINER_H

#define VKMEM_ALIGN( x, a ) ( ( ( x ) + ((a)-1) ) & ~((a)-1) )

#define VKZERO(x) memset(&(x), 0, sizeof(x))
#define VKPTRZERO(x) memset(x, 0, sizeof(*(x)))
#define VKPTRNZERO(x, n) memset((x), 0, sizeof(*(x)) * (n))
#define VKSWAP(T, a, b) \
    { \
        T _vkc_tmp = a; \
        a = b; \
        b = _vkc_tmp; \
    }
#define VKPTRSWAP(T, a, b) \
    { \
        T *_vkc_tmp = a; \
        a = b; \
        b = _vkc_tmp; \
    }

#define VKBLOCK_NUM(initSizeVal, sizeVal, granularityVal) (sizeVal > 0 ? ( sizeVal > initSizeVal ? (1 + ((sizeVal - initSizeVal) / granularityVal) + ((sizeVal - initSizeVal) % granularityVal) > 0 ? 1 : 0) : 1 ) : 0)

// array
#define VKARRAY(T) struct { \
	T *list; \
	int size; \
	int num; \
}

#define VKARRAY_ALLOC2(T, data, num) \
	data = (T *)malloc(sizeof(*data) * num);

#define VKARRAY_ALLOC(T, arr) VKARRAY_ALLOC2(T, (arr).list, (arr).size)

#define VKARRAY_INIT(T, arr, sizeVal) \
	(arr).list = NULL; \
	(arr).num = 0; \
	(arr).size = sizeVal; \

#define VKARRAY_FREE2(T, data, num) \
	if(data) { \
		free(data); \
		data = NULL; \
	} \
	num = 0;

#define VKARRAY_FREE(T, arr) VKARRAY_FREE2(T, (arr).list, (arr).num)


// array(element is heap pointer)
#define VKPTRARRAY_FREE2(T, data, num) \
	if(data) { \
		for(int i = 0; i < num; i++) { \
			free(data[i]); \
		} \
		free(data); \
		data = (T **)0; \
	} \
	num = 0;


// list(element is built-in type)
#define VKLIST(T) struct { \
	T *list; \
	int size; \
	int num; \
	int granularity; \
}

#define VKLIST_INIT(T, lst, sizeVal, granularityVal) \
	(lst).list = NULL; \
	(lst).num = 0; \
	(lst).size = sizeVal; \
	(lst).granularity = granularityVal;

#define VKLIST_RESIZE2(T, data, size, num, granularity) \
	if(data) { \
		if(num == size) { \
			T *_vkc_oldData = data; \
			size += granularity; \
			data = (T *)malloc(sizeof(*data) * size); \
			memcpy(data, _vkc_oldData, sizeof(*data) * num); \
			free(_vkc_oldData); \
		} \
	} else { \
		data = (T *)malloc(sizeof(*data) * size); \
		num = 0; \
	}

#define VKLIST_RESIZE(T, lst) VKLIST_RESIZE2(T, (lst).list, (lst).size, (lst).num, (lst).granularity)

#define VKLIST_FREE2(T, data, size, num) \
	if(data) { \
		free(data); \
		data = NULL; \
	} \
	num = size = 0;

#define VKLIST_FREE(T, lst) VKLIST_FREE2(T, (lst).list, (lst).size, (lst).num)

#define VKLIST_ALLOC2(T, data, size, num, granularity) \
	VKLIST_RESIZE2(T, data, size, num, granularity) \
	int _vkc_index = num++; \
	T *_vkc_value = &data[_vkc_index]; \
	memset(_vkc_value, 0, sizeof(*_vkc_value));

#define VKLIST_ALLOC(T, lst) VKLIST_ALLOC2(T, (lst).list, (lst).size, (lst).num, (lst).granularity)

#define VKLIST_FOREACH(T, lst) for(int i = 0; i < (lst).num; i++)

//#define VKLIST_BLOCK_NUM(T, lst, initSizeVal) VKBLOCK_NUM(initSizeVal, (lst).size, (lst).granularity)



// pool(element is heap pointer, allow free slots)
#define VKPOOL(T) struct { \
	T **list; \
	int size; \
	int num; \
	int granularity; \
	int base; \
}

#define VKPOOL_INIT(T, pool, sizeVal, granularityVal, baseVal) \
	assert((sizeVal) > (baseVal)); \
	(pool).list = NULL; \
	(pool).num = 0; \
	(pool).size = sizeVal; \
	(pool).granularity = granularityVal; \
	(pool).base = baseVal;

#define VKPOOL_FOREACH(T, pool) for(int i = (pool).base; i < (pool).size; i++)

#define VKPOOL_RESIZE2(T, data, size, num, granularity) \
	if(data) { \
		T **_vkc_oldData = data; \
		int _vkc_oldSize = size; \
		size += granularity; \
		data = (T **)malloc(sizeof(*data) * size); \
		memset(data, 0, sizeof(*data) * size); \
		memcpy(data, _vkc_oldData, sizeof(*data) * _vkc_oldSize); \
		free(_vkc_oldData); \
	} else { \
		data = (T **)malloc(sizeof(*data) * size); \
		memset(data, 0, sizeof(*data) * size); \
		num = 0; \
	}

#define VKPOOL_RESIZE(T, pool) VKPOOL_RESIZE2(T, (pool).list, (pool).size, (pool).num, (pool).granularity)

#define VKPOOL_FREE2(T, data, size, num, base) \
	if(data) { \
		for(int _vkc_index = base; _vkc_index < size; _vkc_index++) { \
			if(data[_vkc_index]) { \
				free(data[_vkc_index]); \
			} \
		} \
		free(data); \
		data = (T **)0; \
	} \
	num = size = 0;

#define VKPOOL_FREE(T, pool) VKPOOL_FREE2(T, (pool).list, (pool).size, (pool).num, (pool).base)

#define VKPOOL_ALLOC2(T, data, size, num, granularity, base) \
	int _vkc_index; \
	if(data) { \
		for(_vkc_index = base; _vkc_index < size; _vkc_index++) { \
			if(!data[_vkc_index]) { \
				break; \
			} \
		} \
	} else { \
		VKPOOL_RESIZE2(T, data, size, num, granularity) \
		_vkc_index = base; \
	} \
	if(_vkc_index == size) { \
		VKPOOL_RESIZE2(T, data, size, num, granularity) \
	} \
	num++; \
	data[_vkc_index] = (T *)malloc(sizeof(*data[_vkc_index])); \
	memset(data[_vkc_index], 0, sizeof(*data[_vkc_index])); \
	T *_vkc_value = data[_vkc_index];

#define VKPOOL_ALLOC(T, pool) VKPOOL_ALLOC2(T, (pool).list, (pool).size, (pool).num, (pool).granularity, (pool).base)

#define VKPOOL_FREESLOT2(T, data, num, size, base, index) \
	if(data) { \
		if(index >= base && index < size && data[index]) { \
			free(data[index]); \
			data[index] = NULL; \
			num--; \
		} \
	}

#define VKPOOL_FREESLOT(T, pool, index) VKPOOL_FREESLOT2(T, (pool).list, (pool).num, (pool).size, (pool).base, index)



// list(element is heap pointer)
#define VKPTRLIST(T) struct { \
	T **list; \
	int size; \
	int num; \
	int granularity; \
}

#define VKPTRLIST_INIT(T, plst, sizeVal, granularityVal) \
	(plst).list = NULL; \
	(plst).num = 0; \
	(plst).size = sizeVal; \
	(plst).granularity = granularityVal;

#define VKPTRLIST_FOREACH(T, plst) for(int i = 0; i < (plst).num; i++)

#define VKPTRLIST_RESIZE2(T, data, size, num, granularity) \
	if(data) { \
		if(num == size) { \
			T **old = data; \
			int oldSize = size; \
			size += granularity; \
			data = (T **)malloc(sizeof(*data) * size); \
			memset(data, 0, sizeof(*data) * size); \
			memcpy(data, old, sizeof(*data) * oldSize); \
			free(old); \
		} \
	} else { \
		data = (T **)malloc(sizeof(*data) * size); \
		memset(data, 0, sizeof(*data) * size); \
		num = 0; \
	}

#define VKPTRLIST_RESIZE(T, plst) VKPTRLIST_RESIZE2(T, (plst).list, (plst).size, (plst).num, (plst).granularity)

#define VKPTRLIST_FREE2(T, data, size, num) \
	if(data) { \
		for(int i = 0; i < num; i++) { \
			free(data[i]); \
		} \
		free(data); \
		data = (T **)0; \
	} \
	num = size = 0;

#define VKPTRLIST_FREE(T, plst) VKPTRLIST_FREE2(T, (plst).list, (plst).size, (plst).num)

#define VKPTRLIST_ALLOC2(T, data, size, num, granularity) \
	VKPTRLIST_RESIZE2(T, data, size, num, granularity) \
	int _vkc_index = num++; \
	data[_vkc_index] = (T *)malloc(sizeof(*data[_vkc_index])); \
	memset(data[_vkc_index], 0, sizeof(*data[_vkc_index])); \
	T *_vkc_value = data[_vkc_index];

#define VKPTRLIST_ALLOC(T, plst) VKPTRLIST_ALLOC2(T, (plst).list, (plst).size, (plst).num, (plst).granularity)



// map(key-value double list)
#define VKMAP(T, I) struct { \
	T *list; \
	I *keys; \
	int size; \
	int num; \
	int granularity; \
}

#define VKMAP_INIT(T, I, map, sizeVal, granularityVal) \
	(map).list = NULL; \
	(map).keys = NULL; \
	(map).num = 0; \
	(map).size = sizeVal; \
	(map).granularity = granularityVal;

#define VKMAP_FINDINDEX2(T, I, keys, num, key, cmp) \
	int _vkc_index; \
	if(keys) { \
		for(_vkc_index = 0; _vkc_index < num; _vkc_index++) { \
			if(cmp(keys[_vkc_index], key)) { \
				break; \
			} \
		} \
	} else { \
		_vkc_index = -1; \
	}

#define VKMAP_FIND2(T, I, data, keys, num, key, cmp) \
	VKMAP_FINDINDEX2(T, I, keys, num, key, cmp) \
	T _vkc_value; \
	if(_vkc_index >= 0 && _vkc_index < num) { \
		_vkc_value = data[_vkc_index]; \
	} else { \
		_vkc_value = (T)0; \
	}

#define VKMAP_FINDINDEX(T, I, map, key, cmp) VKMAP_FINDINDEX2(T, I, (map).keys, (map).num, key, cmp)

#define VKMAP_FIND(T, I, map, key, cmp) VKMAP_FIND2(T, I, (map).list, (map).keys, (map).num, key, cmp)

#define VKMAP_RESIZE2(T, I, data, keys, size, num, granularity) \
	if(data) { \
		if(num == size) { \
			T *_vkc_oldData = data; \
			I *_vkc_oldKeys = keys; \
			int _vkc_oldSize = size; \
			size += granularity; \
			data = (T *)malloc(sizeof(*data) * size); \
			memset(data, 0, sizeof(*data) * size); \
			keys = (I *)malloc(sizeof(*keys) * size); \
			memset(keys, 0, sizeof(*keys) * size); \
			memcpy(data, _vkc_oldData, sizeof(*data) * _vkc_oldSize); \
			memcpy(keys, _vkc_oldKeys, sizeof(*keys) * _vkc_oldSize); \
			free(_vkc_oldData); \
			free(_vkc_oldKeys); \
		} \
	} else { \
		data = (T *)malloc(sizeof(*data) * size); \
		memset(data, 0, sizeof(*data) * size); \
		keys = (I *)malloc(sizeof(*keys) * size); \
		memset(keys, 0, sizeof(*keys) * size); \
		num = 0; \
	}

#define VKMAP_RESIZE(T, I, map) VKMAP_RESIZE2(T, I, (map).list, (map).keys, (map).size, (map).num, (map).granularity)

#define VKMAP_FREE2(T, I, data, keys, size, num) \
	if(data) { \
		free(data); \
		data = NULL; \
	} \
	if(keys) { \
		free(keys); \
		keys = NULL; \
	} \
	num = size = 0;

#define VKMAP_FREE(T, I, map) VKMAP_FREE2(T, I, (map).list, (map).keys, (map).size, (map).num)

#define VKMAP_ALLOC2(T, I, data, keys, size, num, granularity) \
	VKMAP_RESIZE2(T, I, data, keys, size, num, granularity) \
	T *_vkc_value = &data[num]; \
	memset(_vkc_value, 0, sizeof(*_vkc_value)); \
	I *_vkc_key = &keys[num]; \
	memset(_vkc_key, 0, sizeof(*_vkc_key)); \
	num++;

#define VKMAP_ALLOC(T, I, map) VKMAP_ALLOC2(T, I, (map).list, (map).keys, (map).size, (map).num, (map).granularity)

#define VKMAP_FOREACH(T, I, arr) for(int i = 0; i < (pool).num; i++)


// map(key-value double list)
#define VKPTRMAP(T, I) struct { \
	T **list; \
	I *keys; \
	int size; \
	int num; \
	int granularity; \
}

#define VKPTRMAP_INIT(T, I, pmap, sizeVal, granularityVal) \
	(pmap).list = NULL; \
	(pmap).keys = NULL; \
	(pmap).num = 0; \
	(pmap).size = sizeVal; \
	(pmap).granularity = granularityVal;

#define VKPTRMAP_FINDINDEX2(T, I, keys, num, key, cmp) \
	int _vkc_index; \
	if(keys) { \
		for(_vkc_index = 0; _vkc_index < num; _vkc_index++) { \
			if(cmp(keys[_vkc_index], key)) { \
				break; \
			} \
		} \
	} else { \
		_vkc_index = -1; \
	}

#define VKPTRMAP_FIND2(T, I, data, keys, num, key, cmp) \
	VKPTRMAP_FINDINDEX2(T, I, keys, num, key, cmp) \
	T _vkc_value; \
	if(_vkc_index >= 0 && _vkc_index < num) { \
		_vkc_value = data[_vkc_index]; \
	} else { \
		_vkc_value = (T)0; \
	}

#define VKPTRMAP_FINDINDEX(T, I, pmap, key, cmp) VKPTRMAP_FINDINDEX2(T, I, (pmap).keys, (pmap).num, key, cmp)

#define VKPTRMAP_FIND(T, I, pmap, key, cmp) VKPTRMAP_FIND2(T, I, (pmap).list, (pmap).keys, (pmap).num, key, cmp)

#define VKPTRMAP_RESIZE2(T, I, data, keys, size, num, granularity) \
	if(data) { \
		if(num == size) { \
			T **_vkc_oldData = data; \
			I *_vkc_oldKeys = keys; \
			int _vkc_oldSize = size; \
			size += granularity; \
			data = (T **)malloc(sizeof(*data) * size); \
			memset(data, 0, sizeof(*data) * size); \
			keys = (I *)malloc(sizeof(*keys) * size); \
			memset(keys, 0, sizeof(*keys) * size); \
			memcpy(data, _vkc_oldData, sizeof(*data) * _vkc_oldSize); \
			memcpy(keys, _vkc_oldKeys, sizeof(*keys) * _vkc_oldSize); \
			free(_vkc_oldData); \
			free(_vkc_oldKeys); \
		} \
	} else { \
		data = (T **)malloc(sizeof(*data) * size); \
		memset(data, 0, sizeof(*data) * size); \
		keys = (I *)malloc(sizeof(*keys) * size); \
		memset(keys, 0, sizeof(*keys) * size); \
		num = 0; \
	}

#define VKPTRMAP_RESIZE(T, I, pmap) VKPTRMAP_RESIZE2(T, I, (pmap).list, (pmap).keys, (pmap).size, (pmap).num, (pmap).granularity)

#define VKPTRMAP_FREE2(T, I, data, keys, size, num) \
	if(data) { \
		for(int i = 0; i < num; i++) { \
			free(data[i]); \
		} \
		free(data); \
		data = (T **)0; \
	} \
	if(keys) { \
		free(keys); \
		keys = (I *)0; \
	} \
	num = size = 0;

#define VKPTRMAP_FREE(T, I, pmap) VKPTRMAP_FREE2(T, I, (pmap).list, (pmap).keys, (pmap).size, (pmap).num)

#define VKPTRMAP_ALLOC2(T, I, data, keys, size, num, granularity) \
	VKPTRMAP_RESIZE2(T, I, data, keys, size, num, granularity) \
	data[num] = (T *)malloc(sizeof(*data[num])); \
	memset(data[num], 0, sizeof(*data[num])); \
	T *_vkc_value = data[num]; \
	I *_vkc_key = &keys[num]; \
	memset(_vkc_key, 0, sizeof(*_vkc_key)); \
	num++;

#define VKPTRMAP_ALLOC(T, I, pmap) VKPTRMAP_ALLOC2(T, I, (pmap).list, (pmap).keys, (pmap).size, (pmap).num, (pmap).granularity)

#define VKPTRMAP_FOREACH(T, I, pmap) for(int i = 0; i < (pmap).num; i++)


#define VKQUEUE(T) struct { \
	T *list; \
	int size; \
	int num; \
	int granularity; \
	int used; \
}

#define VKQUEUE_INIT(T, que, sizeVal, granularityVal) \
	(que).list = NULL; \
	(que).num = 0; \
	(que).size = sizeVal; \
	(que).granularity = granularityVal; \
	(que).used = 0;

#define VKQUEUE_RESIZE2(T, data, size, num, granularity, used) \
	if(data) { \
		if(num == size) { \
			T *_vkc_oldData = data; \
			size += granularity; \
			data = (T *)malloc(sizeof(*data) * size); \
			memcpy(data, _vkc_oldData, sizeof(*data) * num); \
			free(_vkc_oldData); \
		} \
	} else { \
		data = (T *)malloc(sizeof(*data) * size); \
		num = 0; \
		used = 0; \
	}

#define VKQUEUE_RESIZE(T, que) VKQUEUE_RESIZE2(T, (que).list, (que).size, (que).num, (que).granularity, (que).used)

#define VKQUEUE_FREE2(T, data, size, num, used) \
	if(data) { \
		free(data); \
		data = NULL; \
	} \
	num = size = used = 0;

#define VKQUEUE_FREE(T, que) VKQUEUE_FREE2(T, (que).list, (que).size, (que).num, (que).used)

#define VKQUEUE_ALLOC2(T, data, size, num, granularity, used) \
	VKQUEUE_RESIZE2(T, data, size, num, granularity, used) \
	int _vkc_index = num++; \
	T *_vkc_value = &data[_vkc_index]; \
	memset(_vkc_value, 0, sizeof(*_vkc_value));

#define VKQUEUE_ALLOC(T, que) VKQUEUE_ALLOC2(T, (que).list, (que).size, (que).num, (que).granularity, (que).used)

#define VKQUEUE_NEXT2(T, data, size, num, granularity, used) \
	T *_vkc_value; \
	int _vkc_index; \
	bool _vkc_isNew; \
	if(used < num - 1) { \
		used++; \
		_vkc_index = used; \
		_vkc_value = &data[_vkc_index]; \
		_vkc_isNew = false; \
	} else { \
		VKQUEUE_RESIZE2(T, data, size, num, granularity, used) \
		used++; \
		num++; \
		_vkc_index = used; \
		_vkc_value = &data[_vkc_index]; \
		memset(_vkc_value, 0, sizeof(*_vkc_value)); \
		_vkc_isNew = true; \
	}

#define VKQUEUE_NEXT(T, que) VKQUEUE_NEXT2(T, (que).list, (que).size, (que).num, (que).granularity, (que).used)

#define VKQUEUE_ISFULL(T, que) ((que).used == ((que).num - 1))

#define VKQUEUE_CURRENT(T, que) ((que).list[(que).used])

#define VKQUEUE_RESET(T, que) (que).used = 0

#define VKQUEUE_FOREACH(T, que) for(int i = 0; i < (que).num; i++)


#endif //_VULKAN_VKCONTAINER_H
