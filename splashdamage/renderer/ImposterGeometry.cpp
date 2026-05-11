// Copyright (C) 2007 Id Software, Inc.
//

#include "idlib/precompiled.h"

#include "ImposterGeometry.h"

#include "renderer/tr_local.h"

#include "splashdamage/decllib/declImposter.h"

static const char *Imposter_SnapshotName = "_Imposter_Snapshot_";

sdImposterGeometry::sdImposterGeometry(void)
    : declImposter(NULL),
        triSurf(NULL)
{

}

//sdImposterGeometry::~sdImposterGeometry(void) {}

void sdImposterGeometry::Purged(void)
{
    if (triSurf) {
        R_FreeStaticTriSurf(triSurf);
        triSurf = NULL;
    }
}

void sdImposterGeometry::Init(const sdDeclImposter *decl) {
    int num;
    idDrawVert *dv;
    glIndex_t *idx;

    Purged();

    if (!decl->IsValid() || decl->IsImplicit())
        return;

    num = decl->GetNumAngles() / 2;
    /*for (int i = 0; i < num; i++) {
        const sdImposterSubImage &subImage = declImposter->GetSubImage(i);
        if (subImage)
    }*/


    declImposter = decl;
    triSurf = R_AllocStaticTriSurf();

    num = 1;
    triSurf->numVerts = 4 * num;
    triSurf->numIndexes = 6 * num;
    R_AllocStaticTriSurfVerts(triSurf, triSurf->numVerts);
    R_AllocStaticTriSurfIndexes(triSurf, triSurf->numIndexes);

    byte red = 255;
    byte green = 255;
    byte blue = 255;
    byte alpha = 255;

    dv = &triSurf->verts[0];
    idx = &triSurf->indexes[0];
    for (int i = 0; i < num; i++) {
        const sdImposterSubImage &subImage = declImposter->GetSubImage(i);

        float width = declImposter->GetScaleX();
        float height = declImposter->GetScaleY();
        float x1 = -subImage.GetMins()[0] * width * 0.5f;
        float y1 = -subImage.GetMins()[1] * height * 0.5f;
        float x2 = subImage.GetMaxs()[0] * width * 0.5f;
        float y2 = subImage.GetMaxs()[1] * height * 0.5f;
        idVec3 right	= idVec3(0.0f, width * 0.5f, 0.0f);
        idVec3 up		= idVec3(0.0f, 0.0f, height * 0.5f);

        dv[ 0 ].Clear();
        dv[ 0 ].normal.Set(1.0f, 0.0f, 0.0f);
        dv[ 0 ].tangents[0].Set(0.0f, 1.0f, 0.0f);
        dv[ 0 ].tangents[1].Set(0.0f, 0.0f, 1.0f);
        dv[ 0 ].st = subImage.GetTexCoord(0);
        dv[ 0 ].color[ 0 ] = red;
        dv[ 0 ].color[ 1 ] = green;
        dv[ 0 ].color[ 2 ] = blue;
        dv[ 0 ].color[ 3 ] = alpha;
	    dv[ 0 ].xyz.Set(0.0f, x2, y2);
	    dv[ 0 ].xyz += declImposter->GetOrigin();

        dv[ 1 ].Clear();
        dv[ 1 ].normal.Set(1.0f, 0.0f, 0.0f);
        dv[ 1 ].tangents[0].Set(0.0f, 1.0f, 0.0f);
        dv[ 1 ].tangents[1].Set(0.0f, 0.0f, 1.0f);
        dv[ 0 ].st = subImage.GetTexCoord(1);
        dv[ 1 ].color[ 0 ] = red;
        dv[ 1 ].color[ 1 ] = green;
        dv[ 1 ].color[ 2 ] = blue;
        dv[ 1 ].color[ 3 ] = alpha;
	    dv[ 1 ].xyz.Set(0.0f, x1, y2);
	    dv[ 1 ].xyz += declImposter->GetOrigin();

        dv[ 2 ].Clear();
        dv[ 2 ].normal.Set(1.0f, 0.0f, 0.0f);
        dv[ 2 ].tangents[0].Set(0.0f, 1.0f, 0.0f);
        dv[ 2 ].tangents[1].Set(0.0f, 0.0f, 1.0f);
        dv[ 0 ].st = subImage.GetTexCoord(2);
        dv[ 2 ].color[ 0 ] = red;
        dv[ 2 ].color[ 1 ] = green;
        dv[ 2 ].color[ 2 ] = blue;
        dv[ 2 ].color[ 3 ] = alpha;
	    dv[ 2 ].xyz.Set(0.0f, x1, y1);
	    dv[ 2 ].xyz += declImposter->GetOrigin();

        dv[ 3 ].Clear();
        dv[ 3 ].normal.Set(1.0f, 0.0f, 0.0f);
        dv[ 3 ].tangents[0].Set(0.0f, 1.0f, 0.0f);
        dv[ 3 ].tangents[1].Set(0.0f, 0.0f, 1.0f);
        dv[ 0 ].st = subImage.GetTexCoord(3);
        dv[ 3 ].color[ 0 ] = red;
        dv[ 3 ].color[ 1 ] = green;
        dv[ 3 ].color[ 2 ] = blue;
        dv[ 3 ].color[ 3 ] = alpha;
	    dv[ 3 ].xyz.Set(0.0f, x2, y1);
	    dv[ 3 ].xyz += declImposter->GetOrigin();

        idx[ 0 ] = 0;
        idx[ 1 ] = 1;
        idx[ 2 ] = 3;
        idx[ 3 ] = 1;
        idx[ 4 ] = 2;
        idx[ 5 ] = 3;

        dv += 4;
        idx += 6;
    }

    R_BoundTriSurf(triSurf);
}



sdImposterGeometryManager::sdImposterGeometryManager(void) {

}

//sdImposterGeometryManager::~sdImposterGeometryManager(void) {}

void sdImposterGeometryManager::Init(void) {
    list.Clear();
}

void sdImposterGeometryManager::Shutdown(void) {
    for (int i = 0; i < list.Num(); i++) {
        list[i].Purged();
    }
    list.Clear();
}

const sdImposterGeometry * sdImposterGeometryManager::Get(const char *name) {
    for (int i = 0; i < list.Num(); i++) {
        if (!idStr::Icmp(list[i].GetDeclImposter()->GetName(), name))
            return &list[i];
    }
    return NULL;
}

const sdImposterGeometry * sdImposterGeometryManager::Get(const sdDeclImposter *imposter) {
    for (int i = 0; i < list.Num(); i++) {
        if (list[i].GetDeclImposter() == imposter)
            return &list[i];
    }
    return NULL;
}

const sdImposterGeometry * sdImposterGeometryManager::Find(const char *name) {
    const sdImposterGeometry *item = Get( name );
    if (item)
        return item;

    const idDecl *decl = declManager->FindType(DECL_IMPOSTER, name, false);
    if (!decl || !decl->IsValid() || decl->IsImplicit())
        return NULL;

    const sdDeclImposter *declImposter = static_cast<const sdDeclImposter *>(decl);

    sdImposterGeometry *newItem = &list.Alloc();
    newItem->Init(declImposter);
    return newItem;
}

const sdImposterGeometry * sdImposterGeometryManager::Find(const sdDeclImposter *imposter) {
    if (!imposter || !imposter->IsValid() || imposter->IsImplicit())
        return NULL;

    const sdImposterGeometry *item = Get( imposter );
    if (item)
        return item;

    sdImposterGeometry *newItem = &list.Alloc();
    newItem->Init(imposter);
    return newItem;
}

idRenderModelStatic * sdImposterGeometryManager::GetModel(const char *name) {
    for (int i = 0; i < list.Num(); i++) {
        if (!idStr::Icmp(modelList[i]->Name(), name))
            return modelList[i];
    }
    return NULL;
}

idRenderModelStatic * sdImposterGeometryManager::GetModel(const sdDeclImposter *imposter) {
    for (int i = 0; i < list.Num(); i++) {
        if (!idStr::Icmp(modelList[i]->Name(), imposter->GetName()))
            return modelList[i];
    }
    return NULL;
}

idRenderModelStatic * sdImposterGeometryManager::FindModel(const char *name) {
    idRenderModelStatic *model = GetModel(name);
    if (model)
        return model;

    const sdImposterGeometry *gemo = Find( name );
    if (!gemo)
        return NULL;

    sdRenderModelImposter *imposterModel = new sdRenderModelImposter;
    imposterModel->InitFromImposterGeometry(gemo);
    modelList.Append(imposterModel);

    return model;
}

idRenderModelStatic * sdImposterGeometryManager::FindModel(const sdDeclImposter *imposter) {
    idRenderModelStatic *model = GetModel(imposter);
    if (model)
        return model;

    const sdImposterGeometry *gemo = Find( imposter );
    if (!gemo)
        return NULL;

    sdRenderModelImposter *imposterModel = new sdRenderModelImposter;
    imposterModel->InitFromImposterGeometry(gemo);
    modelList.Append(imposterModel);

    return model;
}



sdRenderModelImposter::sdRenderModelImposter(void)
    : imposterGeometry(NULL)
{

}

void sdRenderModelImposter::InitFromFile(const char* fileName) {
    name = fileName;
    LoadModel();
}

dynamicModel_t sdRenderModelImposter::IsDynamicModel() const {
    return DM_STATIC;
}

idRenderModel * sdRenderModelImposter::InstantiateDynamicModel(const struct renderEntity_s *ent, const struct viewDef_s *view, idRenderModel *cachedModel) {
    int					i, surfaceNum;
    idRenderModelStatic	*staticModel;
    modelSurface_t *surf;

    if (cachedModel && !r_useCachedDynamicModels.GetBool()) {
        delete cachedModel;
        cachedModel = NULL;
    }

    // this may be triggered by a model trace or other non-view related source, to which we should look like an empty model
    if (ent == NULL || view == NULL) {
        delete cachedModel;
        return NULL;
    }

    if (!imposterGeometry) {
        delete cachedModel;
        return NULL;
    }

    if (cachedModel) {
        assert(dynamic_cast<idRenderModelStatic *>(cachedModel) != NULL);
        assert(idStr::Icmp(cachedModel->Name(), Imposter_SnapshotName) == 0);
        staticModel = static_cast<idRenderModelStatic *>(cachedModel);
    } else {
        staticModel = new idRenderModelStatic;
        staticModel->InitEmpty(Imposter_SnapshotName);
    }

    staticModel->bounds.Clear();

    //distance = tr.viewDef->renderView.vieworg.Dist(def->parms.origin);
    for (i = 0; i < ent->numInsts; i++) {
        if (staticModel->FindSurfaceWithId(i, surfaceNum)) {
            surf = &staticModel->surfaces[surfaceNum];
        } else {
            surf = &staticModel->surfaces.Alloc();
            surf->geometry = NULL;
            surf->material = NULL;
            surf->id = i;
        }

        UpdateSurface(ent, view, surf);
    }

    staticModel->bounds = bounds;
    return staticModel;
}


void sdRenderModelImposter::UpdateSurface(const struct renderEntity_s *ent, const struct viewDef_s *view, modelSurface_t *surf) const {
    srfTriangles_t *tri;
    idDrawVert *dv;
    glIndex_t *idx;
    idDrawVert *src;
    const srfTriangles_t *triSurf = imposterGeometry->GetTriTriangles();

    surf->material = imposterGeometry->GetDeclImposter()->GetMaterial();

    if (surf->geometry) {
        /*if (stuff->numVerts == surf->geometry->numVerts && stuff->numIndexes == surf->geometry->numIndexes) {
            R_FreeStaticTriSurfVertexCaches(surf->geometry);
        }
        else*/
        {
            R_FreeStaticTriSurf(surf->geometry);
            surf->geometry = R_AllocStaticTriSurf();
        }
    } else {
        surf->geometry = R_AllocStaticTriSurf();
    }

    tri = surf->geometry;
    tri->numVerts = triSurf->numVerts;
    tri->numIndexes = triSurf->numIndexes;

    R_AllocStaticTriSurfVerts(tri, tri->numVerts);
    R_AllocStaticTriSurfIndexes(tri, tri->numIndexes);
    dv = tri->verts;
    idx = tri->indexes;
    src = triSurf->verts;

    for (int i = 0; i < triSurf->numVerts; i++, src++, dv++) {
        *dv = *src;
        // dv->color[0] = instInfo->inst.color[0];
        // dv->color[1] = instInfo->inst.color[1];
        // dv->color[2] = instInfo->inst.color[2];
        // dv->color[3] = instInfo->inst.color[3];
    }

    for (int i = 0; i < surf->geometry->numIndexes; i++) {
        idx[i] = surf->geometry->indexes[i];
    }

    R_BoundTriSurf(tri);
}

void sdRenderModelImposter::LoadModel(void) {
    imposterGeometry = imposterGeometryManager->Find(name);
}

void sdRenderModelImposter::InitFromImposterGeometry(const sdImposterGeometry* imposter) {
    name = imposter->GetDeclImposter()->GetName();
    imposterGeometry = imposter;
}

void sdRenderModelImposter::PurgeModel(void) {
    imposterGeometry = NULL;
    purged = true;
}


static sdImposterGeometryManager imposterGeometryManagerLocal;
sdImposterGeometryManager *imposterGeometryManager = &imposterGeometryManagerLocal;
