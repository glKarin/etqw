#ifndef _RVM_OCCLUSION_QUERY_H
#define _RVM_OCCLUSION_QUERY_H

class rvmOcclusionQuery {
public:
	enum rvnmOcclusionQueryState
	{
		OCCLUSION_QUERY_STATE_UNINITIALIZED = 0,
		OCCLUSION_QUERY_STATE_READY,
		OCCLUSION_QUERY_STATE_DRAW,
		OCCLUSION_QUERY_STATE_WAITING,
		OCCLUSION_QUERY_STATE_FINISH,
	};

    rvmOcclusionQuery();

    //~rvmOcclusionQuery(){}

    bool IsQueryStale(void) const;
    bool IsVisible(bool def = true);

	static void BeginRender(void);
	static void EndRender(void);
	void Init(void);
	void Destroy(void);
	void Begin(GLenum mode = GL_ANY_SAMPLES_PASSED, int ms = 0);
	void End(void);
	void Sync(bool wait = false);
	int GetResult(void) const;
	void Next(void);

private:
	void Reset(void);

private:
    int queryStartTime;
    int queryTimeOutTime;
    GLuint id;
    rvnmOcclusionQueryState queryState;
	GLenum mode;
	int result;
};

ID_INLINE int rvmOcclusionQuery::GetResult(void) const
{
	return result;
}

#endif
