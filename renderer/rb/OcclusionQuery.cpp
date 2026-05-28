#include "../../idlib/precompiled.h"

#include "../tr_local.h"

static idCVar harm_r_drawOcclusionBounds("harm_r_drawOcclusionBounds", "0", CVAR_BOOL | CVAR_RENDERER, "render occlusion bounds");
idCVar harm_r_skipOcclusionTesting("harm_r_skipOcclusionTesting", "0", CVAR_BOOL | CVAR_RENDERER, "disable occlusion testing");

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
	result = mode == GL_SAMPLES_PASSED ? (passed ? INT_MAX : 0) : (int)passed;
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



idOcclusionTestJob::idOcclusionTestJob(void)
	: index(-1),
		viewID(-1),
		query(NULL),
		tri(NULL),
		lastResult(0),
		update(UT_NONE),
		running(false)
{
	parms.axis = mat3_identity;
	parms.origin.Zero();
	parms.bounds.Zero();
	parms.dirty = DIRTY_NONE;
	parms.mode = 0;
	parms.viewID = -1;
	parms.start = false;

	((idMat4 *)modelMatrix)->Identity();
}

idOcclusionTestJob::~idOcclusionTestJob(void)
{

}

void idOcclusionTestJob::Free(void) {
	if (tri)
	{
		R_FreeStaticTriSurf(tri);
		tri = NULL;
	}
}

void idOcclusionTestJob::ActualFree(void) {
	if(query)
	{
		query->Destroy();
		delete query;
		query = NULL;
	}
}

void idOcclusionTestJob::UpdateGeometry(const idBounds &bounds) {
	if(parms.bounds != bounds)
	{
		parms.dirty |= DIRTY_BOUNDS;
		parms.bounds = bounds;
	}
}

void idOcclusionTestJob::UpdatePosition(const idVec3 &origin, const idMat3 &axis) {
	if(parms.origin != origin)
	{
		parms.dirty |= DIRTY_MATRIX;
		parms.origin = origin;
	}
	if(parms.axis != axis)
	{
		parms.dirty |= DIRTY_MATRIX;
		parms.axis = axis;
	}
}

void idOcclusionTestJob::UpdateView(int viewId) {
	parms.viewID = viewId;
}

void idOcclusionTestJob::UpdateQueryMode( GLenum mode )
{
	parms.mode = mode;
}

void idOcclusionTestJob::UpdateTri(void)
{
	// free old, don't modify it, because maybe used in multi-threading
	if (tri)
	{
		R_FreeStaticTriSurf(tri);
		tri = NULL;
	}

	tri = R_AllocStaticTriSurf();

	tri->numIndexes = 36;
	R_AllocStaticTriSurfIndexes(tri, tri->numIndexes);
	// bottom
	tri->indexes[0] = 0;
	tri->indexes[1] = 1;
	tri->indexes[2] = 2;
	tri->indexes[3] = 0;
	tri->indexes[4] = 2;
	tri->indexes[5] = 3;
	// top
	tri->indexes[6] = 4;
	tri->indexes[7] = 6;
	tri->indexes[8] = 5;
	tri->indexes[9] = 4;
	tri->indexes[10] = 7;
	tri->indexes[11] = 6;
	// left
	tri->indexes[12] = 0;
	tri->indexes[13] = 3;
	tri->indexes[14] = 4;
	tri->indexes[15] = 3;
	tri->indexes[16] = 7;
	tri->indexes[17] = 4;
	// right
	tri->indexes[18] = 1;
	tri->indexes[19] = 5;
	tri->indexes[20] = 2;
	tri->indexes[21] = 2;
	tri->indexes[22] = 5;
	tri->indexes[23] = 6;
	// forward
	tri->indexes[24] = 0;
	tri->indexes[25] = 4;
	tri->indexes[26] = 1;
	tri->indexes[27] = 1;
	tri->indexes[28] = 4;
	tri->indexes[29] = 5;
	// backward
	tri->indexes[30] = 3;
	tri->indexes[31] = 2;
	tri->indexes[32] = 7;
	tri->indexes[33] = 2;
	tri->indexes[34] = 6;
	tri->indexes[35] = 7;

	tri->numVerts = 8;
	R_AllocStaticTriSurfVerts(tri, tri->numVerts);
	idVec3 points[8];
	parms.bounds.ToPoints(points);
	for (int i = 0; i < 8; i++)
	{
		idDrawVert &dv = tri->verts[i];
		dv.Clear();
		dv.xyz = points[i];
	}
}

void idOcclusionTestJob::MakeModelMatrix(void)
{
	R_AxisToModelMatrix(parms.axis, parms.origin, modelMatrix);
}

void idOcclusionTestJob::Ready(void)
{
	if(!query || !parms.mode || !parms.start || update == UT_NONE)
		return;

	if (parms.dirty & DIRTY_BOUNDS)
	{
		UpdateTri();
		parms.dirty &= ~DIRTY_BOUNDS;
	}

	if (parms.dirty & DIRTY_MATRIX)
	{
		MakeModelMatrix();
		parms.dirty &= ~DIRTY_MATRIX;
	}

	if (!tri)
		UpdateTri();

	if (!tri)
		return;

	if (!tri->ambientCache) {
		if (!R_CreateAmbientCache(tri, false)) {
			R_FreeStaticTriSurf(tri);
			tri = NULL;
			return;
		}
	}

	if (!tri->indexCache) {
		vertexCache.Alloc(tri->indexes, tri->numIndexes * sizeof(tri->indexes[0]), &tri->indexCache, true);
	}

	vertexCache.Touch(tri->ambientCache);

	if (tri->indexCache)
		vertexCache.Touch(tri->indexCache);

	if (harm_r_drawOcclusionBounds.GetBool())
	{
		session->rw->DebugBounds(lastResult > 0 ? colorGreen : (lastResult < 0 ? colorBlue : colorRed), parms.bounds, parms.origin, parms.axis, 0);
		session->rw->DrawText(va("%d: %d = %d", index, lastResult, query ? query->GetResult() : -2), parms.origin + idVec3(0, 0, parms.bounds[1].z + 50), 1.0f, lastResult > 0 ? colorGreen : (lastResult < 0 ? colorBlue : colorRed), parms.axis);
	}

	viewID = parms.viewID;

	if (this->update == UT_MANUAL)
		parms.start = false;
	running = true;
}

void idOcclusionTestJob::Start(updateType_e mode)
{
	if (!parms.mode)
	{
		common->Error("Occlusion test mode not set");
		return;
	}
	if (mode == UT_NONE)
	{
		common->Error("Occlusion test update type is invalid");
		return;
	}

	if(!query)
		query = new rvmOcclusionQuery;

	this->update = mode;
	this->parms.start = true;
}

void idOcclusionTestJob::Restart(void)
{
	if (!parms.mode)
	{
		common->Error("Occlusion test mode not set");
		return;
	}
	if (update == UT_NONE)
	{
		common->Error("Occlusion test update type is invalid");
		return;
	}

	if(!query)
		query = new rvmOcclusionQuery;

	this->parms.start = true;
}

bool idOcclusionTestJob::CanQuery(void) const
{
	return tri && query && running && parms.mode && update != UT_NONE && !query->IsWaiting();
}

void idOcclusionTestJob::Query(void)
{
	if(!query)
		return;

	if (query->IsUninitialized())
		return;

	if (query->IsWaiting())
	{
		int res = query->Query();
		bool hasResult = res >= 0;
		if(hasResult)
		{
			lastResult = res;
			query->Next();
		}
	}
	else if(query->IsFinished())
	{
		lastResult = query->GetResult();
		query->Next();
	}
}

void idOcclusionTestJob::Render(void)
{
	float mvp[16];
	float modelViewMatrix[16];
	myGlMultMatrix(modelMatrix, backEnd.viewDef->worldSpace.modelViewMatrix, modelViewMatrix);
	myGlMultMatrix(modelViewMatrix, backEnd.viewDef->projectionMatrix, mvp);
	GL_UniformMatrix4fv(offsetof(shaderProgram_t, modelViewProjectionMatrix), mvp);

	idDrawVert *ac = (idDrawVert *)vertexCache.Position(tri->ambientCache);
	GL_VertexAttribPointer(offsetof(shaderProgram_t, attr_Vertex), 3, GL_FLOAT, false, sizeof(idDrawVert), ac->xyz.ToFloatPtr());

	query->Next();
	query->SetMode(parms.mode);
	query->Begin();
	{
		RB_DrawElementsWithCounters(tri);
	}
	query->End();

	if (this->update == UT_MANUAL)
		this->running = false;
}



void idOcclusionTestManager::Init(void) {

}

void idOcclusionTestManager::Shutdown(void) {
	for (int i = 0; i < list.Num(); i++)
	{
		idOcclusionTestJob *item = list[i];
		if (!item)
			continue;
		item->Free();
		item->ActualFree();
	}
	list.DeleteContents(true);
}

void idOcclusionTestManager::Update(void) {
	HandleFree();
	HandleUpdate();
}

void idOcclusionTestManager::Ready(void) {
	HandleDelete();
	HandleRender();
}

void idOcclusionTestManager::Query(void) {
	HandleQuery();
}

void idOcclusionTestManager::HandleQuery(void) {
	if (list.Num() == 0)
		return;

	for (int i = 0; i < list.Num(); i++)
	{
		idOcclusionTestJob *item = list[i];
		if (!item)
			continue;
		item->Query();
	}
}

void idOcclusionTestManager::Render(void) {
	if (renderList.Num() == 0)
		return;
	if (harm_r_skipOcclusionTesting.GetBool())
		return;

	bool draw = false;
	for (int i = 0; i < renderList.Num(); i++)
	{
		idOcclusionTestJob *item = renderList[i];
		if (item->viewID == backEnd.viewDef->renderView.viewID)
		{
			if (!draw)
			{
				draw = true;
				BeginRender();
			}
			item->Render();
		}
	}
	if (draw)
	{
		EndRender();
	}
}

void idOcclusionTestManager::BeginRender(void)
{
	GL_UseProgram(&occlusionTestShader);

	GL_EnableVertexAttribArray(offsetof(shaderProgram_t, attr_Vertex));

	qglDisable(GL_BLEND);
	//qglDisable(GL_CULL_FACE);
	if (!harm_r_drawOcclusionBounds.GetBool())
		qglColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	qglDepthMask(GL_FALSE);
	qglStencilMask(GL_FALSE);
}

void idOcclusionTestManager::EndRender(void)
{
	qglDepthMask(GL_TRUE);
	if (!harm_r_drawOcclusionBounds.GetBool())
		qglColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	qglStencilMask(GL_TRUE);
	//qglEnable(GL_CULL_FACE);
	qglEnable(GL_BLEND);

	GL_DisableVertexAttribArray(offsetof(shaderProgram_t, attr_Vertex));
	GL_UseProgram(NULL);
}

void idOcclusionTestManager::HandleUpdate(void) {
	for (int i = 0; i < list.Num(); i++)
	{
		idOcclusionTestJob *item = list[i];
		if (!item)
			continue;
		item->Ready();
	}
}

void idOcclusionTestManager::HandleFree(void) {
	if (freeList.Num() == 0)
		return;

	for (int i = 0; i < freeList.Num(); i++)
	{
		qhandle_t handle = freeList[i];
		if (handle < 0 || handle >= list.Num())
		{
			common->Warning("idOcclusionTestManager::HandleFree: invalid handle: %d", handle);
			continue;
		}
		idOcclusionTestJob *item = list[handle];
		if (!item)
		{
			common->Warning("idOcclusionTestManager::HandleFree: handle is NULL: %d", handle);
			continue;
		}
		item->Free(); // free tri
		deleteList.Append(item);
		list[handle] = NULL;
	}
	freeList.Clear();
}

void idOcclusionTestManager::HandleDelete(void) {
	if (deleteList.Num() == 0)
		return;

	for (int i = 0; i < deleteList.Num(); i++)
	{
		idOcclusionTestJob *item = deleteList[i];
		item->ActualFree(); // free query
	}

	deleteList.DeleteContents(true);
}

void idOcclusionTestManager::HandleRender(void) {
	renderList.Clear();

	if (list.Num() == 0)
		return;

	for (int i = 0; i < list.Num(); i++)
	{
		idOcclusionTestJob *item = list[i];
		if (!item)
			continue;
		if (item->CanQuery())
			renderList.Append(item);
	}
}

qhandle_t idOcclusionTestManager::Alloc(void) {
	int index = list.FindNull();
	idOcclusionTestJob *item = new idOcclusionTestJob;
	if (index == -1)
	{
		index = list.Append(item);
	}
	else
		list[index] = item;
	item->index = index;
	return index;
}

void idOcclusionTestManager::Free(qhandle_t handle) {
	if (handle < 0 || handle >= list.Num())
	{
		common->Warning("idOcclusionTestManager::Free: invalid handle: %d", handle);
		return;
	}
	freeList.AddUnique(handle);
}

idOcclusionTestJob * idOcclusionTestManager::Get(qhandle_t handle) {
	if (handle < 0 || handle >= list.Num())
	{
		common->Warning("idOcclusionTestManager::Get: invalid handle: %d", handle);
		return NULL;
	}
	return list[handle];
}

int idOcclusionTestManager::GetResult(qhandle_t handle) const {
	if (handle < 0 || handle >= list.Num())
	{
		common->Warning("idOcclusionTestManager::Find: invalid handle: %d", handle);
		return -1;
	}
	idOcclusionTestJob *item = list[handle];
	if (!item)
	{
		common->Warning("idOcclusionTestManager::Find: handle is NULL: %d", handle);
		return -1;
	}
	return item->lastResult;
}

static idOcclusionTestManager occlusionTestManagerLocal;
idOcclusionTestManager *occlusionTestManager = &occlusionTestManagerLocal;

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
