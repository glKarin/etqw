#include "../../idlib/precompiled.h"

#include "../tr_local.h"

const int rvmOcclusionQuery::RESULT_INVALID = 1;

rvmOcclusionQuery::rvmOcclusionQuery()
{
	Reset();
}

int rvmOcclusionQuery::Query(int def) {
	if (queryState == OCCLUSION_QUERY_STATE_FINISH)
		return result;

	if(IsQueryStale())
	{
		Sync(false);
		if (queryState == OCCLUSION_QUERY_STATE_FINISH)
			return result;
		return def;
	}

	Sync(false);
	if (queryState == OCCLUSION_QUERY_STATE_FINISH)
		return result;
	return def;
}

bool rvmOcclusionQuery::IsQueryStale(void) const {
	if(queryTimeOutTime < 0)
		return false;
	int currentTime = Sys_Milliseconds();
	return queryTimeOutTime < currentTime;
}

void rvmOcclusionQuery::SetMode(GLenum type)
{
	mode = type;
}

void rvmOcclusionQuery::Begin(int ms)
{
	assert(id != 0);
	if (queryState != OCCLUSION_QUERY_STATE_READY)
		return;

	queryState = OCCLUSION_QUERY_STATE_DRAW;

	if(ms > 0)
	{
		queryStartTime = Sys_Milliseconds();
		queryTimeOutTime = queryStartTime + ms;
	}

#ifdef GL_ES_VERSION_2_0 //karin: GL_SAMPLES_PASSED not support on OpenGLES
	qglBeginQuery(mode == GL_SAMPLES_PASSED ? GL_ANY_SAMPLES_PASSED : mode, id);
#else
	qglBeginQuery(mode, id);
#endif
}

void rvmOcclusionQuery::End(void)
{
	if (queryState != OCCLUSION_QUERY_STATE_DRAW)
		return;

	queryState = OCCLUSION_QUERY_STATE_WAITING;
#ifdef GL_ES_VERSION_2_0 //karin: GL_SAMPLES_PASSED not support on OpenGLES
	qglEndQuery(mode == GL_SAMPLES_PASSED ? GL_ANY_SAMPLES_PASSED : mode);
#else
	qglEndQuery(mode);
#endif
}

void rvmOcclusionQuery::BeginRender(void)
{
	qglColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	qglDepthMask(GL_FALSE);
	qglStencilMask(GL_FALSE);
}

void rvmOcclusionQuery::EndRender(void)
{
	qglDepthMask(GL_TRUE);
	qglColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	qglStencilMask(GL_TRUE);
}

void rvmOcclusionQuery::Init(void) 
{
	if(id != 0)
		return;

	Reset();
	qglGenQueries(1, &id);
	queryState = OCCLUSION_QUERY_STATE_READY;
}

void rvmOcclusionQuery::Destroy(void) 
{
	if(id == 0)
		return;
	qglDeleteQueries(1, &id);
	Reset();
}

void rvmOcclusionQuery::Reset(void) 
{
	queryStartTime = -1;
	queryTimeOutTime = -1;
	queryState = OCCLUSION_QUERY_STATE_UNINITIALIZED;
	id = 0;
	mode = GL_ANY_SAMPLES_PASSED;
	result = RESULT_INVALID;
}

void rvmOcclusionQuery::Sync(bool wait)
{
	if (queryState != OCCLUSION_QUERY_STATE_WAITING)
		return;

	GLuint passed = 0;

	do
	{
		qglGetQueryObjectuiv(id, GL_QUERY_RESULT_AVAILABLE, &passed);
		if(!passed && !wait)
			return;
	}
	while(!passed);

	qglGetQueryObjectuiv(id, GL_QUERY_RESULT, &passed);
#ifdef GL_ES_VERSION_2_0 //karin: GL_SAMPLES_PASSED not support on OpenGLES
	result = mode == GL_SAMPLES_PASSED ? passed ? INT_MAX : 0 : (int)passed;
#else
	result = passed;
#endif

	queryState = OCCLUSION_QUERY_STATE_FINISH;
}

void rvmOcclusionQuery::Next(void)
{
	if(id == 0)
		Init();
	else
	{
		queryStartTime = -1;
		queryTimeOutTime = -1;
		queryState = OCCLUSION_QUERY_STATE_READY;
		result = RESULT_INVALID;
	}
}

#if 0
#include "QueueList.h"

static idCVar harm_r_useGPUOcclusionCulling("harm_r_useGPUOcclusionCulling", "0", CVAR_RENDERER | CVAR_BOOL | CVAR_ARCHIVE, "Enable GPU Occlusion culling(OpenGLES 3.0+)");

#define USING_GPU_OCCLUSION_CULLING() (harm_r_useGPUOcclusionCulling.GetBool() && USING_GLES3)

static idQueueList<rvmOcclusionQuery *> occlusionCacheList(true);
static idList<rvmOcclusionQuery *> occlusionTestList;

void RB_ClearOcclutionTestList(void)
{
	for(int i = 0; i < occlusionTestList.Num(); i++)
	{
		occlusionCacheList.Append(occlusionTestList[i]);
	}
	occlusionTestList.SetNum(0);
}

rvmOcclusionQuery * RB_AllocOcclusionTest(void)
{
	rvmOcclusionQuery *query;

	if(occlusionCacheList.Get(query))
	{
		occlusionCacheList.Remove();
	}
	else
	{
		query = new rvmOcclusionQuery;
	}

	occlusionTestList.Append(query);
	query->Next();
	return query;
}

void RB_ShutdownOcclusionTest(void)
{
	rvmOcclusionQuery *query;

	while((query = occlusionCacheList.Get()))
	{
		query->Destroy();
		delete query;
		occlusionCacheList.Remove();
	}

	for(int i = 0; i < occlusionTestList.Num(); i++)
	{
		occlusionTestList[i]->Destroy();
	}
	occlusionTestList.DeleteContents(true);
}

bool RB_LightIsVisible(viewLight_t *vLight)
{
	if(!USING_GPU_OCCLUSION_CULLING())
		return true;

	if(vLight->viewInsideLight)
		return true;

	if(vLight->occlusionTestId > 0)
	{
		return occlusionTestList[vLight->occlusionTestId - 1]->IsVisible(true);
	}

	return true;
}

void RB_TestOcclutionLights(void)
{
	if(!USING_GPU_OCCLUSION_CULLING())
		return;

	viewLight_t		*vLight;
	int total = 0, passed = 0, skip = 0;
	idDrawVert *ac;

	RB_ClearOcclutionTestList();

	rvmOcclusionQuery *query;
	GL_UseProgram(&occlusionTestShader);

	GL_EnableVertexAttribArray(offsetof(shaderProgram_t, attr_Vertex));
	qglBindBuffer(GL_ARRAY_BUFFER, 0);

	qglDisable(GL_BLEND);
	qglDisable(GL_CULL_FACE);
	rvmOcclusionQuery::BeginRender();

	float mvp[16];
	myGlMultMatrix(backEnd.viewDef->worldSpace.modelViewMatrix, backEnd.viewDef->projectionMatrix, mvp);
	GL_UniformMatrix4fv(offsetof(shaderProgram_t, modelViewProjectionMatrix), mvp);

	//
	// for each light, perform adding and shadowing
	//
	for (vLight = backEnd.viewDef->viewLights ; vLight ; vLight = vLight->next)
	{
		if (vLight->lightShader->IsFogLight()) {
			continue;
		}

		if (vLight->lightShader->IsBlendLight()) {
			continue;
		}

		if (!vLight->localInteractions && !vLight->globalInteractions
			&& !vLight->translucentInteractions) {
			continue;
		}

		total++;
		// skip if view in light
		if(vLight->viewInsideLight)
		{
			skip++;
			continue;
		}

		query = RB_AllocOcclusionTest();

		query->Begin();
		{
			ac = vLight->frustumTris->verts;
			GL_VertexAttribPointer(offsetof(shaderProgram_t, attr_Vertex), 3, GL_FLOAT, false, sizeof(idDrawVert), ac->xyz.ToFloatPtr());
			RB_DrawElementsWithCounters(vLight->frustumTris);
		}
		query->End();

		vLight->occlusionTestId = occlusionTestList.Num();
			passed++;
	}

	rvmOcclusionQuery::EndRender();
	qglEnable(GL_CULL_FACE);
	qglEnable(GL_BLEND);

	GL_DisableVertexAttribArray(offsetof(shaderProgram_t, attr_Vertex));
	GL_UseProgram(NULL);

	if(total)
		common->Printf("occ %d %d %d\n", total, passed, skip);
}
#endif
