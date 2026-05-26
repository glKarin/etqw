#include "idlib/precompiled.h"

#include "OcclusionTest.h"

#include "renderer/tr_local.h"

static idCVar harm_r_drawOcclusionBounds("harm_r_drawOcclusionBounds", "0", CVAR_BOOL | CVAR_RENDERER, "render occlusion bounds");
static idCVar harm_r_disableOcclusionTesting("harm_r_disableOcclusionTesting", "0", CVAR_BOOL | CVAR_RENDERER, "disable occlusion testing");

sdOcclusionTestLocal::sdOcclusionTestLocal(void)
	: index(-1),
		world(NULL),
		query(NULL),
		tri(NULL),
		lastResult(0),
		dirty(DIRTY_NONE)
{
	parms.axis = mat3_identity;
	parms.origin.Zero();
	parms.bb.Zero();
	((idMat4 *)modelMatrix)->Identity();
}

sdOcclusionTestLocal::~sdOcclusionTestLocal(void)
{

}

bool sdOcclusionTestLocal::IsVisible(void) {
	if(!CreateQuery())
		query->SetMode(GL_ANY_SAMPLES_PASSED);
	return lastResult > 0;
}

int sdOcclusionTestLocal::CountVisible(void) {
	if(!CreateQuery())
		query->SetMode(GL_SAMPLES_PASSED);
	return lastResult > 0 ? lastResult : INT_MAX;
}

void sdOcclusionTestLocal::UpdateOcclusionTest(const occlusionTest_t *testInfo) {
	bool changed = false;
	if(parms.axis != testInfo->axis || parms.origin != testInfo->origin)
	{
		dirty |= DIRTY_MATRIX;
		changed = true;
	}
	if(parms.bb != testInfo->bb)
	{
		dirty |= DIRTY_BOUNDS;
		changed = true;
	}
	if(changed)
	{
		parms = *testInfo;
	}
}

void sdOcclusionTestLocal::FreeOcclusionTest(void) {
	if(query)
	{
		query->Destroy();
		query = NULL;
	}
	if (tri)
	{
		R_FreeStaticTriSurf(tri);
		tri = NULL;
	}
}

bool sdOcclusionTestLocal::ReadResults(void) {
	if (!query)
		return true;

	if (query->IsWaiting())
	{
		int res = query->Query();
		bool hasResult = res >= 0;
		if(hasResult)
			lastResult = res;
		return hasResult;
	}
	return true;
}

bool sdOcclusionTestLocal::CreateQuery(void) {
	if(query)
	{
		if(ReadResults())
		{
			query->Next();
		}
		else
			return true;
	}
	else
	{
		query = new rvmOcclusionQuery;
		query->Init();
	}
	return false;
}

void sdOcclusionTestLocal::UpdateTri(void)
{
	if (!tri)
	{
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
		tri->indexes[7] = 5;
		tri->indexes[8] = 6;
		tri->indexes[9] = 4;
		tri->indexes[10] = 6;
		tri->indexes[11] = 7;
		// left
		tri->indexes[12] = 0;
		tri->indexes[13] = 3;
		tri->indexes[14] = 4;
		tri->indexes[15] = 3;
		tri->indexes[16] = 7;
		tri->indexes[17] = 4;
		// right
		tri->indexes[18] = 1;
		tri->indexes[19] = 2;
		tri->indexes[20] = 5;
		tri->indexes[21] = 2;
		tri->indexes[22] = 6;
		tri->indexes[23] = 5;
		// forward
		tri->indexes[24] = 0;
		tri->indexes[25] = 1;
		tri->indexes[26] = 4;
		tri->indexes[27] = 1;
		tri->indexes[28] = 5;
		tri->indexes[29] = 4;
		// backward
		tri->indexes[30] = 3;
		tri->indexes[31] = 2;
		tri->indexes[32] = 7;
		tri->indexes[33] = 2;
		tri->indexes[34] = 6;
		tri->indexes[35] = 7;
	}

	if (!tri->verts)
	{
		tri->numVerts = 8;
		R_AllocStaticTriSurfVerts(tri, tri->numVerts);
	}
	idVec3 points[8];
	parms.bb.ToPoints(points);
	for (int i = 0; i < 8; i++)
	{
		idDrawVert &dv = tri->verts[i];
		dv.Clear();
		dv.xyz = points[i];
	}

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
}

void sdOcclusionTestLocal::MakeModelMatrix(void)
{
    R_AxisToModelMatrix(parms.axis, parms.origin, modelMatrix);
}

void sdOcclusionTestLocal::Ready(void)
{
	if (dirty & DIRTY_BOUNDS)
	{
		UpdateTri();
		dirty &= ~DIRTY_BOUNDS;
	}

	if (dirty & DIRTY_MATRIX)
	{
		MakeModelMatrix();
		dirty &= ~DIRTY_MATRIX;
	}

	if (!tri)
		UpdateTri();

	vertexCache.Touch(tri->ambientCache);
	if (tri->indexCache)
		vertexCache.Touch(tri->indexCache);

	if (harm_r_drawOcclusionBounds.GetBool())
	{
		world->DebugBounds(lastResult > 0 ? colorGreen : (lastResult < 0 ? colorBlue : colorRed), parms.bb, parms.origin, parms.axis, 0);
		world->DrawText(va("%d = %d", lastResult, query ? query->GetResult() : -2), parms.origin + idVec3(0, 0, parms.bb[1].z + 50), 1.0f, lastResult > 0 ? colorGreen : (lastResult < 0 ? colorBlue : colorRed), parms.axis);
	}
}

void sdOcclusionTestLocal::Render(void)
{
	if (!tri || !query)
		return;

	if (!ReadResults())
		return;

	float mvp[16];
	float modelViewMatrix[16];
    myGlMultMatrix(modelMatrix, backEnd.viewDef->worldSpace.modelViewMatrix, modelViewMatrix);
	myGlMultMatrix(modelViewMatrix, backEnd.viewDef->projectionMatrix, mvp);
	GL_UniformMatrix4fv(offsetof(shaderProgram_t, modelViewProjectionMatrix), mvp);

    idDrawVert *ac = (idDrawVert *)vertexCache.Position(tri->ambientCache);
	GL_VertexAttribPointer(offsetof(shaderProgram_t, attr_Vertex), 3, GL_FLOAT, false, sizeof(idDrawVert), ac->xyz.ToFloatPtr());

	query->Begin();
	{
		RB_DrawElementsWithCounters(tri);
	}
	query->End();
}


void sdOcclusionQueryWrapper::Begin(void)
{
	GL_UseProgram(&occlusionTestShader);

	GL_EnableVertexAttribArray(offsetof(shaderProgram_t, attr_Vertex));

	qglDisable(GL_BLEND);
	qglDisable(GL_CULL_FACE);
	if (!harm_r_drawOcclusionBounds.GetBool())
		qglColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	qglDepthMask(GL_FALSE);
	qglStencilMask(GL_FALSE);
}

void sdOcclusionQueryWrapper::End(void)
{
	qglDepthMask(GL_TRUE);
	if (!harm_r_drawOcclusionBounds.GetBool())
		qglColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	qglStencilMask(GL_TRUE);
	qglEnable(GL_CULL_FACE);
	qglEnable(GL_BLEND);

	GL_DisableVertexAttribArray(offsetof(shaderProgram_t, attr_Vertex));
	GL_UseProgram(NULL);
}

void sdOcclusionQueryWrapper::Render(idList<sdOcclusionTestLocal *> &list)
{
	if (harm_r_disableOcclusionTesting.GetBool())
		return;

	Begin();
	{
		sdOcclusionTestLocal *test;
		for (int i = 0; i < list.Num(); i++)
		{
			test = list[i];
			if (backEnd.viewDef->renderView.viewID != test->GetViewID())
				continue;

			test->Render();
		}
	}
	End();
}

