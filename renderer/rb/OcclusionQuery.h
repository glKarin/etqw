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

	static const int RESULT_INVALID;

    rvmOcclusionQuery();

    //~rvmOcclusionQuery(){}

    bool IsQueryStale(void) const;
    int Query(int def = RESULT_INVALID);

	static void BeginRender(void);
	static void EndRender(void);
	void Init(void);
	void Destroy(void);
	void SetMode(GLenum m);
	void Begin(int ms = 0);
	void End(void);
	void Sync(bool wait = false);
	int GetResult(void) const;
	void Next(void);
	bool IsWaiting(void) const;
	bool IsFinished(void) const;
	bool HasResult(void) const;

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

ID_INLINE bool rvmOcclusionQuery::IsWaiting(void) const
{
	return queryState == OCCLUSION_QUERY_STATE_WAITING;
}

ID_INLINE bool rvmOcclusionQuery::IsFinished(void) const
{
	return queryState == OCCLUSION_QUERY_STATE_FINISH;
}

ID_INLINE bool rvmOcclusionQuery::HasResult(void) const
{
	return result > RESULT_INVALID;
}

#endif
