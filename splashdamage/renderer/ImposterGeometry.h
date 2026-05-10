// Copyright (C) 2007 Id Software, Inc.
//

#ifndef __IMPOSTERGEOMETRY_H__
#define __IMPOSTERGEOMETRY_H__
#include "renderer/Model.h"

class sdDeclImposter;

class sdImposterGeometry
{
public:
                                sdImposterGeometry(void);
                                //~sdImposterGeometry(void);
    void                        Init(const sdDeclImposter *decl);
    void                        Purged(void);
    const idBounds &			Bounds(void) const {
        return bounds;
    }
    const sdDeclImposter        *GetDeclImposter(void) const {
        return declImposter;
    }
    idRenderModel 		        *InstantiateDynamicModel(const struct renderEntity_s *ent, const struct viewDef_s *view, idRenderModel *cachedModel) const;

private:
    void                        UpdateSurface(const sdInstInfo *instInfo, const struct renderEntity_s *ent, const struct viewDef_s *view, modelSurface_t *surf) const;

private:
    const sdDeclImposter        *declImposter;
    struct srfTriangles_t       *triSurf;
    idBounds                    bounds;
};

class sdImposterGeometryManager {
public:
                                sdImposterGeometryManager(void);
                                //~sdImposterGeometryManager(void);
    void                        Init(void);
    void                        Shutdown(void);
    const sdImposterGeometry    *Find(const char *name);
    const sdImposterGeometry    *Find(const sdDeclImposter *imposter);
    const sdImposterGeometry    *Get(const char *name);
    const sdImposterGeometry    *Get(const sdDeclImposter *imposter);
    idRenderModel 		        *InstantiateDynamicModel(const struct renderEntity_s *ent, const struct viewDef_s *view, idRenderModel *cachedModel);

private:
    idList<sdImposterGeometry>  list;
};

extern sdImposterGeometryManager *imposterGeometryManager;

#endif /* !__IMPOSTERGEOMETRY_H__ */
