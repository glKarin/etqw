// Copyright (C) 2007 Id Software, Inc.
//

#include "idlib/precompiled.h"

#include "declDecal.h"

sdDeclDecal::sdDeclDecal(void)
    : lifeTime(0.0f),
      minSize(0.0f),
      sizeDiff(0.0f),
      material(NULL)
{
    startColor.Set(1.0f, 0.0f, 0.0f, 1.0f);
    endColor.Set(1.0f, 0.0f, 0.0f, 1.0f);
    idList< sdBounds2D >	images;
}

const char* sdDeclDecal::DefaultDefinition( void ) const {
    return "{  }";
}

bool sdDeclDecal::Parse( const char *text, const int textLength ) {
    lifeTime = 0.0f;
    minSize = 0.0f;
    sizeDiff = 0.0f;
    material = NULL;
    startColor.Set(1.0f, 0.0f, 0.0f, 1.0f);
    endColor.Set(1.0f, 0.0f, 0.0f, 1.0f);
    images.Clear();
    return true;
}

void sdDeclDecal::FreeData( void ) {
}

void sdDeclDecal::CacheFromDict( const idDict& dict ) {
}
