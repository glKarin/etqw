const char *rvDeclEffect::DefaultDefinition()
{
  return "{\n}\n";
}
void __thiscall rvDeclEffect::SetMinDuration(rvDeclEffect *this, float duration)
{
  if ( this->mMinDuration < (double)duration )
    this->mMinDuration = duration;
}
void __thiscall rvDeclEffect::SetMaxDuration(rvDeclEffect *this, float duration)
{
  if ( this->mMaxDuration < (double)duration )
    this->mMaxDuration = duration;
}
void __thiscall rvSegmentTemplate::rvSegmentTemplate(rvSegmentTemplate *this)
{
  rvSegmentTemplate *v1; // esi

  v1 = this;
  this->mSegmentName.len = 0;
  this->mSegmentName.alloced = -20;
  v1->mSegmentName.data = v1->mSegmentName.baseBuffer;
  this->mSegmentName.baseBuffer[0] = 0;
  this->mParticleTemplate.mFlags = 0;
  this->mParticleTemplate.mEntityDefName.len = 0;
  this->mParticleTemplate.mEntityDefName.alloced = -20;
  v1->mParticleTemplate.mEntityDefName.data = v1->mParticleTemplate.mEntityDefName.baseBuffer;
  this->mParticleTemplate.mEntityDefName.baseBuffer[0] = 0;
  rvSegmentTemplate::Init(this, 0);
  v1->mFlags |= 1u;
}
bool __thiscall rvDeclEffect::SetDefaultText(rvDeclEffect *this)
{
  rvDeclEffect *v1; // esi
  int v2; // edi
  int v3; // eax
  char generated[1024]; // [esp+4h] [ebp-404h]

  v1 = this;
  v2 = (*(int (**)(void))this->base->vfptr->gap4)();
  v3 = (int)v1->vfptr->DefaultDefinition((idDecl *)&v1->vfptr);
  idStr::snPrintf(&generated[4], 1024, "effect %s // IMPLICITLY GENERATED\n%s", v2, v3);
  return 0;
}
int __thiscall rvDeclEffect::GetTrailSegmentIndex(rvDeclEffect *this, idStr *name)
{
  rvDeclEffect *v2; // esi
  int v3; // edi
  int v4; // ebx
  int v5; // eax
  int result; // eax

  v2 = this;
  v3 = 0;
  if ( this->mSegmentTemplates.num <= 0 )
  {
LABEL_6:
    (*(void (**)(netadrtype_t, const char *, ...))(*(_DWORD *)common.type + 68))(
      common.type,
      "^4BSE:^1 Unable to find segment '%s'",
      name->data);
    result = -1;
  }
  else
  {
    v4 = 0;
    while ( 1 )
    {
      v5 = (int)&v2->mSegmentTemplates.list[v4];
      if ( v5 )
      {
        if ( !idStr::Icmp(name->data, *(const char **)(v5 + 8)) )
          break;
      }
      ++v3;
      ++v4;
      if ( v3 >= v2->mSegmentTemplates.num )
        goto LABEL_6;
    }
    result = v3;
  }
  return result;
}
int __thiscall rvDeclEffect::Size(rvDeclEffect *this)
{
  return 404 * this->mSegmentTemplates.size + 52;
}
double __thiscall rvDeclEffect::EvaluateCost(rvDeclEffect *this, int activeParticles, int segment)
{
  rvDeclEffect *v3; // esi
  int v5; // edi
  int v6; // ebx
  double v7; // st7
  float cost; // [esp+Ch] [ebp+8h]

  v3 = this;
  if ( segment != -1 )
    return rvSegmentTemplate::EvaluateCost(&this->mSegmentTemplates.list[segment], activeParticles);
  v5 = 0;
  cost = 0.0;
  if ( this->mSegmentTemplates.num > 0 )
  {
    v6 = 0;
    do
    {
      v7 = rvSegmentTemplate::EvaluateCost(&v3->mSegmentTemplates.list[v6], activeParticles);
      ++v5;
      ++v6;
      cost = v7 + cost;
    }
    while ( v5 < v3->mSegmentTemplates.num );
  }
  return cost;
}
void __thiscall rvDeclEffect::FreeData(rvDeclEffect *this)
{
  rvDeclEffect *v1; // esi
  int v2; // ebx
  int v3; // edi
  rvSegmentTemplate *v4; // eax
  int *v5; // edi

  v1 = this;
  v2 = 0;
  if ( this->mSegmentTemplates.num > 0 )
  {
    v3 = 0;
    do
    {
      rvParticleTemplate::Purge(&v1->mSegmentTemplates.list[v3].mParticleTemplate);
      rvParticleTemplate::PurgeTraceModel(&v1->mSegmentTemplates.list[v3].mParticleTemplate);
      ++v2;
      ++v3;
    }
    while ( v2 < v1->mSegmentTemplates.num );
  }
  v4 = v1->mSegmentTemplates.list;
  if ( v4 )
  {
    v5 = &v4[-1].mDecalAxis;
    `eh vector destructor iterator'(
      v4,
      0x194u,
      v4[-1].mDecalAxis,
      (void (__thiscall *)(void *))rvSegmentTemplate::~rvSegmentTemplate);
    operator delete(v5);
  }
  v1->mSegmentTemplates.list = 0;
  v1->mSegmentTemplates.num = 0;
  v1->mSegmentTemplates.size = 0;
}
void __thiscall rvDeclEffect::Init(rvDeclEffect *this)
{
  rvDeclEffect *v1; // esi
  rvSegmentTemplate *v2; // eax
  int *v3; // ebx

  v1 = this;
  this->mMinDuration = 0.0;
  this->mMaxDuration = 0.0;
  this->mSize = 512.0;
  this->mFlags = 0;
  this->mPlayCount = 0;
  this->mLoopCount = 0;
  this->mCutOffDistance = 0.0;
  v2 = this->mSegmentTemplates.list;
  if ( v2 )
  {
    v3 = &v2[-1].mDecalAxis;
    `eh vector destructor iterator'(
      v2,
      0x194u,
      v2[-1].mDecalAxis,
      (void (__thiscall *)(void *))rvSegmentTemplate::~rvSegmentTemplate);
    operator delete(v3);
  }
  v1->mSegmentTemplates.list = 0;
  v1->mSegmentTemplates.num = 0;
  v1->mSegmentTemplates.size = 0;
}
void __cdecl rvDeclEffect::CacheFromDict(idDict *dict)
{
  idKeyValue *v1; // edi
  char *v2; // esi
  int v3; // ebp
  signed int v4; // eax
  char *v5; // edi
  sdStringBuilder<idStr,sdCharAllocator_Heap<512,char> > *v6; // eax
  char v7; // cl
  int i; // eax
  int v9; // eax
  int v10; // edi
  signed int v11; // eax
  char *v12; // esi
  sdStringBuilder<idStr,sdCharAllocator_Heap<512,char> > *v13; // eax
  int v14; // eax
  char v15; // cl
  int v16; // esi
  sdDeclTypeHolder *v17; // eax
  char *v18; // esi
  sdDeclTypeHolder *v19; // eax
  char kv_3; // [esp+17h] [ebp-21Dh]
  sdStringBuilder<idStr,sdCharAllocator_Heap<512,char> > builder; // [esp+18h] [ebp-21Ch]
  int v22; // [esp+224h] [ebp-10h]
  int v23; // [esp+230h] [ebp-4h]

  v1 = idDict::MatchPrefix(dict, "fx", 0);
  *(_DWORD *)builder.baseBuffer = v1;
  if ( v1 )
  {
    while ( 1 )
    {
      if ( idStr::Cmp(v1->value->data, (const char *)&s2) )
      {
        kv_3 = 0;
        if ( !bse_simple.internalVar->integerValue
          && !com_makingBuild.internalVar->integerValue
          && !com_makingRC.internalVar->integerValue )
        {
          goto LABEL_52;
        }
        builder.len = (int)&builder.baseBuffer[4];
        builder.baseBuffer[4] = 0;
        builder.alloced = 0;
        v22 = 512;
        v2 = v1->value->data;
        v23 = 0;
        builder.alloced = 0;
        builder.baseBuffer[4] = 0;
        if ( v2 )
        {
          v3 = 0;
          if ( *v2 )
          {
            do
              ++v3;
            while ( v2[v3] );
          }
          v4 = v3 + 1;
          if ( v3 + 1 > 512 )
          {
            if ( v4 % 512 )
              v4 = v4 - v4 % 512 + 512;
            v22 = v4;
            v5 = (char *)operator new(v4);
            v6 = (sdStringBuilder<idStr,sdCharAllocator_Heap<512,char> > *)builder.len;
            if ( builder.len )
            {
              if ( builder.alloced )
              {
                _strncpy(v5, (const char *)builder.len, builder.alloced);
                v5[builder.alloced] = 0;
              }
              else
              {
                *v5 = 0;
              }
              v6 = (sdStringBuilder<idStr,sdCharAllocator_Heap<512,char> > *)builder.len;
            }
            if ( v6 != (sdStringBuilder<idStr,sdCharAllocator_Heap<512,char> > *)&builder.baseBuffer[4] )
              operator delete(v6);
            builder.len = (int)v5;
          }
          v7 = *v2;
          for ( i = 0; v7; ++i )
          {
            *(_BYTE *)(builder.len + builder.alloced + i) = v7;
            v7 = v2[i + 1];
          }
          builder.alloced = v3;
          *(_BYTE *)(builder.len + v3) = 0;
        }
        v9 = 0;
        do
          ++v9;
        while ( asc_777BA0[v9] );
        v10 = builder.alloced + v9;
        v11 = builder.alloced + v9 + 1;
        if ( v22 < v11 )
        {
          if ( v11 % 512 )
            v11 = v11 - v11 % 512 + 512;
          v22 = v11;
          v12 = (char *)operator new(v11);
          v13 = (sdStringBuilder<idStr,sdCharAllocator_Heap<512,char> > *)builder.len;
          if ( builder.len )
          {
            if ( builder.alloced )
            {
              _strncpy(v12, (const char *)builder.len, builder.alloced);
              v12[builder.alloced] = 0;
            }
            else
            {
              *v12 = 0;
            }
            v13 = (sdStringBuilder<idStr,sdCharAllocator_Heap<512,char> > *)builder.len;
          }
          if ( v13 != (sdStringBuilder<idStr,sdCharAllocator_Heap<512,char> > *)&builder.baseBuffer[4] )
            operator delete(v13);
          builder.len = (int)v12;
        }
        v14 = 0;
        v15 = 95;
        do
        {
          *(_BYTE *)(builder.len + builder.alloced + v14) = v15;
          v15 = byte_777BA1[v14++];
        }
        while ( v15 );
        builder.alloced = v10;
        *(_BYTE *)(builder.len + v10) = 0;
        v16 = builder.len;
        v17 = sdSingleton<sdDeclTypeHolder>::GetInstance();
        if ( ((int (__stdcall *)(int, int, _DWORD))declManager->vfptr->FindType)(
               v17->declEffectsType.declTypeHandle,
               v16,
               0) )
        {
          if ( com_makingBuild.internalVar->integerValue || (kv_3 = 1, com_makingRC.internalVar->integerValue) )
            kv_3 = 0;
        }
        v23 = -1;
        if ( (char *)builder.len != &builder.baseBuffer[4] )
          operator delete((void *)builder.len);
        v1 = *(idKeyValue **)builder.baseBuffer;
        builder.len = (int)&builder.baseBuffer[4];
        builder.baseBuffer[4] = 0;
        builder.alloced = 0;
        v22 = 512;
        if ( !kv_3 )
        {
LABEL_52:
          v18 = v1->value->data;
          v19 = sdSingleton<sdDeclTypeHolder>::GetInstance();
          ((void (__stdcall *)(int, char *, signed int))declManager->vfptr->FindType)(
            v19->declEffectsType.declTypeHandle,
            v18,
            1);
        }
      }
      else
      {
        (*(void (**)(netadrtype_t, const char *, ...))(*(_DWORD *)common.type + 68))(
          common.type,
          "rvDeclEffect::CacheFromDict: '%s' has an empty value",
          v1->key->data);
      }
      *(_DWORD *)builder.baseBuffer = idDict::MatchPrefix(dict, "fx", v1);
      if ( !*(_DWORD *)builder.baseBuffer )
        break;
      v1 = *(idKeyValue **)builder.baseBuffer;
    }
  }
}
void __thiscall rvDeclEffect::Finish(rvDeclEffect *this)
{
  rvDeclEffect *v1; // esi
  int v2; // ebp
  rvSegmentTemplate *v3; // eax
  int *v4; // ebx
  bool v5; // sf
  unsigned __int8 v6; // of
  rvSegmentTemplate *v7; // ebx
  int v8; // ecx
  int *v9; // eax
  int v10; // esi
  rvSegmentTemplate *v11; // eax
  int v12; // edi
  int v13; // esi
  rvSegmentTemplate *v14; // esi
  rvParticleTemplate *v15; // ebx
  idMaterial *v16; // ecx
  sdRenderBindings *v17; // eax
  materialStage_t *v18; // esi
  int v19; // edi
  int v20; // ebp
  sdDeclRenderBinding **v21; // edx
  sdDeclRenderBinding *v22; // ecx
  sdDeclRenderProgram *v23; // esi
  int v24; // edi
  int v25; // edx
  sdDeclRenderBinding **v26; // esi
  sdDeclRenderBinding *v27; // ecx
  idMaterial *v28; // ecx
  materialStage_t *v29; // esi
  int v30; // edi
  int v31; // ebp
  sdDeclRenderBinding **v32; // edx
  sdDeclRenderBinding *v33; // ecx
  sdDeclRenderProgram *v34; // esi
  int v35; // edi
  int v36; // edx
  sdDeclRenderBinding **v37; // esi
  sdDeclRenderBinding *v38; // ecx
  idMaterial *v39; // ecx
  materialStage_t *v40; // ebp
  int v41; // esi
  int v42; // edi
  sdDeclRenderBinding **v43; // edx
  sdDeclRenderBinding *v44; // ecx
  sdDeclRenderProgram *v45; // ecx
  int v46; // edi
  int v47; // edx
  sdDeclRenderBinding **v48; // esi
  sdDeclRenderBinding *v49; // ecx
  sdDeclRenderProgram *v50; // ecx
  int v51; // edi
  int v52; // esi
  sdDeclRenderBinding **v53; // edx
  sdDeclRenderBinding *v54; // ecx
  rvDeclEffect *material; // [esp+10h] [ebp-24h]
  idMaterial *v56; // [esp+14h] [ebp-20h]
  idMaterial *v57; // [esp+14h] [ebp-20h]
  idMaterial *v58; // [esp+14h] [ebp-20h]
  int i; // [esp+18h] [ebp-1Ch]
  int ia; // [esp+18h] [ebp-1Ch]
  int ib; // [esp+18h] [ebp-1Ch]
  int j; // [esp+1Ch] [ebp-18h]
  rvParticleTemplate *pt; // [esp+20h] [ebp-14h]
  rvParticleTemplate *pta; // [esp+20h] [ebp-14h]
  unsigned int ptb; // [esp+20h] [ebp-14h]
  int v66; // [esp+24h] [ebp-10h]
  int v67; // [esp+24h] [ebp-10h]

  v1 = this;
  v2 = this->mSegmentTemplates.num;
  this->mMinDuration = 0.0;
  v1->mFlags &= 0xFFFFFFFE;
  this->mMaxDuration = 0.0;
  material = this;
  if ( v2 > 0 )
  {
    if ( v2 != this->mSegmentTemplates.size )
    {
      v6 = __OFSUB__(v2, this->mSegmentTemplates.num);
      v5 = v2 - this->mSegmentTemplates.num < 0;
      v7 = this->mSegmentTemplates.list;
      this->mSegmentTemplates.size = v2;
      if ( v5 ^ v6 )
        this->mSegmentTemplates.num = v2;
      v8 = 404 * v2 | -(404 * (unsigned __int64)(unsigned int)v2 >> 32 != 0);
      v9 = (int *)operator new((v8 + 4) | -__CFADD__(v8, 4));
      if ( v9 )
      {
        v10 = (int)(v9 + 1);
        *v9 = v2;
        `eh vector constructor iterator'(
          v9 + 1,
          0x194u,
          v2,
          (void (__thiscall *)(void *))rvSegmentTemplate::rvSegmentTemplate,
          (void (__thiscall *)(void *))rvSegmentTemplate::~rvSegmentTemplate);
        v11 = (rvSegmentTemplate *)v10;
      }
      else
      {
        v11 = 0;
      }
      v1 = material;
      material->mSegmentTemplates.list = v11;
      if ( v7 )
      {
        v12 = 0;
        if ( material->mSegmentTemplates.num > 0 )
        {
          v13 = 0;
          do
          {
            rvSegmentTemplate::operator=(&material->mSegmentTemplates.list[v13], &v7[v13]);
            ++v12;
            ++v13;
          }
          while ( v12 < material->mSegmentTemplates.num );
        }
        `eh vector destructor iterator'(
          v7,
          0x194u,
          v7[-1].mDecalAxis,
          (void (__thiscall *)(void *))rvSegmentTemplate::~rvSegmentTemplate);
        operator delete(&v7[-1].mDecalAxis);
        v1 = material;
      }
    }
  }
  else
  {
    v3 = this->mSegmentTemplates.list;
    if ( v3 )
    {
      v4 = &v3[-1].mDecalAxis;
      `eh vector destructor iterator'(
        v3,
        0x194u,
        v3[-1].mDecalAxis,
        (void (__thiscall *)(void *))rvSegmentTemplate::~rvSegmentTemplate);
      operator delete(v4);
    }
    v1->mSegmentTemplates.list = 0;
    v1->mSegmentTemplates.num = 0;
    v1->mSegmentTemplates.size = 0;
  }
  v1->mSegmentTemplates.num = v2;
  j = 0;
  if ( v2 > 0 )
  {
    do
    {
      v14 = &material->mSegmentTemplates.list[j];
      if ( v14 )
      {
        rvSegmentTemplate::Finish(v14, material);
        if ( v14->mSegType == 5 )
          material->mFlags |= 1u;
        v15 = &v14->mParticleTemplate;
        v66 = (int)&v14->mParticleTemplate;
        if ( rvParticleTemplate::UsesEndOrigin(&v14->mParticleTemplate) )
          material->mFlags |= 2u;
        if ( ((unsigned int)v15->mFlags >> 9) & 1 )
          material->mFlags |= 0x80u;
        if ( ((unsigned int)v14->mFlags >> 6) & 1 )
          material->mFlags |= 4u;
        rvSegmentTemplate::EvaluateTrailSegment(v14, material);
        rvSegmentTemplate::SetMinDuration(v14, material);
        rvSegmentTemplate::SetMaxDuration(v14, material);
        if ( !(((unsigned int)material->mFlags >> 6) & 1) )
        {
          v16 = v14->mParticleTemplate.mMaterial;
          v56 = v14->mParticleTemplate.mMaterial;
          if ( v16 && (pt = 0, v16->numStages > 0) )
          {
            i = 0;
            v17 = rbinds;
            do
            {
              v18 = &v16->stages[i];
              v19 = 0;
              v20 = v18->numTextures;
              if ( v20 > 0 )
              {
                v21 = &v18->textures->renderBinding;
                while ( 1 )
                {
                  v22 = *v21;
                  if ( *v21 == v17->ambientCubeMap
                    || v22 == v17->ambientCubeMapSun
                    || v22 == v17->specularCubeMap
                    || v22 == v17->environmentCubeMap
                    || v22 == v17->gradientMap )
                  {
                    break;
                  }
                  ++v19;
                  v21 += 2;
                  if ( v19 >= v20 )
                    goto LABEL_40;
                }
                material->mFlags |= 0x40u;
                v17 = rbinds;
              }
LABEL_40:
              if ( !(((unsigned int)material->mFlags >> 6) & 1) )
              {
                v23 = v18->renderProgram;
                v24 = v23->numTextureBindings;
                v25 = 0;
                if ( v24 > 0 )
                {
                  v26 = v23->textureBindings;
                  while ( 1 )
                  {
                    v27 = *v26;
                    if ( *v26 == v17->ambientCubeMap
                      || v27 == v17->ambientCubeMapSun
                      || v27 == v17->specularCubeMap
                      || v27 == v17->environmentCubeMap
                      || v27 == v17->gradientMap )
                    {
                      break;
                    }
                    ++v25;
                    ++v26;
                    if ( v25 >= v24 )
                      goto LABEL_51;
                  }
                  material->mFlags |= 0x40u;
                  v17 = rbinds;
                }
              }
LABEL_51:
              v16 = v56;
              ++i;
              pt = (rvParticleTemplate *)((char *)pt + 1);
            }
            while ( (signed int)pt < v56->numStages );
            v15 = (rvParticleTemplate *)v66;
          }
          else
          {
            v17 = rbinds;
          }
          if ( !(((unsigned int)material->mFlags >> 6) & 1) )
          {
            v28 = v15->mTrailInfo->mTrailMaterial;
            v57 = v15->mTrailInfo->mTrailMaterial;
            if ( v28 )
            {
              pta = 0;
              if ( v28->numStages > 0 )
              {
                ia = 0;
                do
                {
                  v29 = &v28->stages[ia];
                  v30 = 0;
                  v31 = v29->numTextures;
                  if ( v31 > 0 )
                  {
                    v32 = &v29->textures->renderBinding;
                    while ( 1 )
                    {
                      v33 = *v32;
                      if ( *v32 == v17->ambientCubeMap
                        || v33 == v17->ambientCubeMapSun
                        || v33 == v17->specularCubeMap
                        || v33 == v17->environmentCubeMap
                        || v33 == v17->gradientMap )
                      {
                        break;
                      }
                      ++v30;
                      v32 += 2;
                      if ( v30 >= v31 )
                        goto LABEL_68;
                    }
                    material->mFlags |= 0x40u;
                    v17 = rbinds;
                  }
LABEL_68:
                  if ( !(((unsigned int)material->mFlags >> 6) & 1) )
                  {
                    v34 = v29->renderProgram;
                    v35 = v34->numTextureBindings;
                    v36 = 0;
                    if ( v35 > 0 )
                    {
                      v37 = v34->textureBindings;
                      while ( 1 )
                      {
                        v38 = *v37;
                        if ( *v37 == v17->ambientCubeMap
                          || v38 == v17->ambientCubeMapSun
                          || v38 == v17->specularCubeMap
                          || v38 == v17->environmentCubeMap
                          || v38 == v17->gradientMap )
                        {
                          break;
                        }
                        ++v36;
                        ++v37;
                        if ( v36 >= v35 )
                          goto LABEL_79;
                      }
                      material->mFlags |= 0x40u;
                      v17 = rbinds;
                    }
                  }
LABEL_79:
                  v28 = v57;
                  ++ia;
                  pta = (rvParticleTemplate *)((char *)pta + 1);
                }
                while ( (signed int)pta < v57->numStages );
                v15 = (rvParticleTemplate *)v66;
              }
            }
          }
          if ( !(((unsigned int)material->mFlags >> 6) & 1) )
          {
            v39 = v15->mMaterial;
            v58 = v15->mMaterial;
            if ( v39 )
            {
              v67 = 0;
              if ( v39->numStages > 0 )
              {
                ib = 0;
                while ( 1 )
                {
                  v40 = &v39->stages[ib];
                  v41 = 0;
                  v42 = v40->numTextures;
                  if ( v42 > 0 )
                  {
                    v43 = &v40->textures->renderBinding;
                    while ( 1 )
                    {
                      v44 = *v43;
                      if ( *v43 == v17->ambientCubeMap
                        || v44 == v17->ambientCubeMapSun
                        || v44 == v17->specularCubeMap
                        || v44 == v17->environmentCubeMap
                        || v44 == v17->gradientMap )
                      {
                        break;
                      }
                      ++v41;
                      v43 += 2;
                      if ( v41 >= v42 )
                        goto LABEL_96;
                    }
                    material->mFlags |= 0x40u;
                    v17 = rbinds;
                  }
LABEL_96:
                  ptb = material->mFlags;
                  if ( !((ptb >> 6) & 1) )
                  {
                    v45 = v40->renderProgram;
                    v46 = v45->numTextureBindings;
                    v47 = 0;
                    if ( v46 > 0 )
                    {
                      v48 = v45->textureBindings;
                      while ( 1 )
                      {
                        v49 = *v48;
                        if ( *v48 == v17->ambientCubeMap
                          || v49 == v17->ambientCubeMapSun
                          || v49 == v17->specularCubeMap
                          || v49 == v17->environmentCubeMap
                          || v49 == v17->gradientMap )
                        {
                          break;
                        }
                        ++v47;
                        ++v48;
                        if ( v47 >= v46 )
                          goto LABEL_107;
                      }
                      material->mFlags = ptb | 0x40;
                      v17 = rbinds;
                    }
LABEL_107:
                    v50 = v40->renderProgram->versionForAmbientLighting;
                    if ( v50 )
                    {
                      v51 = v50->numTextureBindings;
                      v52 = 0;
                      if ( v51 > 0 )
                      {
                        v53 = v50->textureBindings;
                        while ( 1 )
                        {
                          v54 = *v53;
                          if ( *v53 == v17->ambientCubeMap
                            || v54 == v17->ambientCubeMapSun
                            || v54 == v17->specularCubeMap
                            || v54 == v17->environmentCubeMap
                            || v54 == v17->gradientMap )
                          {
                            break;
                          }
                          ++v52;
                          ++v53;
                          if ( v52 >= v51 )
                            goto LABEL_118;
                        }
                        material->mFlags |= 0x40u;
                        v17 = rbinds;
                      }
                    }
                  }
LABEL_118:
                  ++ib;
                  if ( ++v67 >= v58->numStages )
                    break;
                  v39 = v58;
                }
              }
            }
          }
        }
      }
      ++j;
    }
    while ( j < material->mSegmentTemplates.num );
  }
  material->mSize = rvDeclEffect::CalculateBounds(material);
}
char __thiscall rvDeclEffect::Parse(rvDeclEffect *this, const char *text, const int textLength)
{
  rvDeclEffect *v3; // ebp
  int v4; // eax
  unsigned __int8 (__thiscall *v5)(char *); // eax
  idTokenCache *v6; // edi
  int v7; // esi
  int v8; // eax
  const char *v9; // eax
  int v10; // esi
  int v11; // edi
  int v12; // eax
  const char *v13; // eax
  idStrList *v14; // eax
  int v15; // eax
  size_t v16; // edi
  signed int v17; // eax
  char *v18; // esi
  char *v19; // eax
  int v20; // eax
  char v21; // cl
  char *v22; // edi
  int v23; // eax
  size_t v24; // ebp
  signed int v25; // eax
  char *v26; // esi
  char *v27; // eax
  char v28; // cl
  int i; // eax
  int v30; // eax
  size_t v31; // edi
  signed int v32; // eax
  char *v33; // esi
  char *v34; // eax
  int v35; // eax
  char v36; // cl
  int v37; // eax
  size_t v38; // edi
  signed int v39; // eax
  char *v40; // esi
  char *v41; // eax
  int v42; // eax
  char v43; // cl
  int v44; // esi
  int v45; // edi
  int v46; // eax
  const char *v47; // eax
  idStrList *v48; // eax
  int v49; // eax
  int v50; // edx
  char *v51; // ecx
  int v52; // esi
  bool v53; // sf
  unsigned __int8 v54; // of
  int v55; // ecx
  int *v56; // eax
  int v57; // edi
  int v58; // edi
  int v59; // eax
  int v60; // esi
  rvSegmentTemplate *v61; // eax
  int *v62; // esi
  rvDeclEffect *v63; // eax
  rvSegmentTemplate *v64; // ebp
  int v65; // ecx
  int *v66; // eax
  int v67; // edi
  rvDeclEffect *v68; // ecx
  int v69; // edi
  int v70; // esi
  idDeclTypeInterface *v71; // eax
  int v72; // esi
  int v73; // eax
  char result; // al
  idDeclTypeInterface *v75; // eax
  int v76; // esi
  int v77; // eax
  sdDeclParseHelper declHelper; // [esp+24h] [ebp-524h]
  int length; // [esp+3Ch] [ebp-50Ch]
  rvDeclEffect *v80; // [esp+40h] [ebp-508h]
  idToken token; // [esp+44h] [ebp-504h]
  int v82; // [esp+94h] [ebp-4B4h]
  idToken v83; // [esp+98h] [ebp-4B0h]
  idStr v84; // [esp+E8h] [ebp-460h]
  idParser v85; // [esp+108h] [ebp-440h]
  char v86; // [esp+19Ch] [ebp-3ACh]
  char *Source; // [esp+39Ch] [ebp-1ACh]
  size_t Count; // [esp+3A0h] [ebp-1A8h]
  rvSegmentTemplate segment; // [esp+3A4h] [ebp-1A4h]
  int v90; // [esp+544h] [ebp-4h]

  v3 = this;
  v80 = this;
  v84.len = 0;
  v84.alloced = -20;
  v84.data = v84.baseBuffer;
  v84.baseBuffer[0] = 0;
  v90 = 0;
  idParser::idParser(&v85);
  *(long double *)((char *)&token.floatvalue + 4) = 0.0;
  token.data = 0;
  *(_DWORD *)token.baseBuffer = -20;
  token.alloced = (int)&token.baseBuffer[4];
  token.baseBuffer[4] = 0;
  token.subtype = 0;
  token.line = 0;
  token.linesCrossed = 0;
  token.flags = 0;
  token.intvalue = 0;
  LODWORD(token.floatvalue) = 0;
  token.whiteSpaceEnd_p = 0;
  token.next = 0;
  *(_DWORD *)&token.binaryIndex = 0;
  LOWORD(v82) = 0;
  LOBYTE(v90) = 2;
  rvSegmentTemplate::rvSegmentTemplate((rvSegmentTemplate *)((char *)&segment + 4));
  LOBYTE(v90) = 3;
  idParser::SetFlags(&v85, 78128);
  declHelper.decl = 0;
  declHelper.type = (idDeclTypeInterface *)v3;
  declHelper.declBufferLength = 0;
  length = 0;
  v4 = (*(int (**)(void))&v3->base->vfptr->gap4[4])();
  declHelper.declBuffer = (char *)((int (__stdcall *)(int))declManager->vfptr->GetDeclType)(v4);
  v5 = *(unsigned __int8 (__thiscall **)(char *))(*(_DWORD *)declHelper.declBuffer + 24);
  v6 = 0;
  token.len = 0;
  if ( !v5(declHelper.declBuffer) )
  {
    v6 = (idTokenCache *)((int (*)(void))declManager->vfptr->GetGlobalTokenCache)();
    token.len = (int)v6;
  }
  if ( ((unsigned __int8 (*)(void))v3->base->vfptr->HasBinaryBuffer)()
    && (*(int (**)(void))&v3->base->vfptr->gap4[8])() != 1 )
  {
    ((void (__stdcall *)(int *, int *))v3->base->vfptr->GetBinarySource)(&declHelper.declBufferLength, &length);
    v7 = (*(int (**)(void))v3->base->vfptr->gap4)();
    v8 = ((int (*)(void))v3->base->vfptr->GetFileName)();
    v9 = va("%s: %s", v8, v7);
    idParser::LoadMemoryBinary(&v85, (const char *)declHelper.declBufferLength, length, v9, v6);
  }
  else
  {
    if ( ((unsigned __int8 (__stdcall *)(const char *))cvarSystem->vfptr->GetCVarBool)("com_writeBinaryDecls")
      && !(*(unsigned __int8 (**)(void))(*(_DWORD *)declHelper.declBuffer + 28))()
      && !(*(unsigned __int8 (**)(void))(*(_DWORD *)declHelper.declBuffer + 20))()
      && (*(int (**)(void))&v3->base->vfptr->gap4[8])() != 1 )
    {
      v10 = ((int (*)(void))v3->base->vfptr->GetLineNum)();
      v11 = (*(int (**)(void))v3->base->vfptr->gap4)();
      v12 = ((int (*)(void))v3->base->vfptr->GetFileName)();
      v13 = va("%s: %s", v12, v11);
      idParser::LoadMemory(&v85, text, textLength, v13, v10);
      if ( ((int (*)(void))v3->vfptr->GetFileLevelIncludeDependencies)() )
      {
        v14 = (idStrList *)((int (*)(void))v3->vfptr->GetFileLevelIncludeDependencies)();
        idParser::AddIncludes(&v85, v14);
      }
      Source = &v86;
      v86 = 0;
      Count = 0;
      segment.mDeclEffect = (rvDeclEffect *)512;
      v83.floatvalue = 0.0;
      v83.len = 0;
      v83.alloced = -20;
      v83.data = v83.baseBuffer;
      v83.baseBuffer[0] = 0;
      v83.type = 0;
      v83.subtype = 0;
      v83.line = 0;
      v83.linesCrossed = 0;
      v83.flags = 0;
      v83.intvalue = 0;
      v83.whiteSpaceStart_p = 0;
      v83.whiteSpaceEnd_p = 0;
      v83.next = 0;
      v83.binaryIndex = 0;
      for ( LOBYTE(v90) = 5; idParser::ReadToken(&v85, &v83); Source[v38] = 0 )
      {
        if ( v83.type == 1 )
        {
          v15 = 0;
          do
            ++v15;
          while ( SubStr[v15] );
          v16 = Count + v15;
          v17 = Count + v15 + 1;
          if ( (_DWORD)segment.mDeclEffect < v17 )
          {
            if ( v17 % 512 )
              v17 = v17 - v17 % 512 + 512;
            segment.mDeclEffect = (rvDeclEffect *)v17;
            v18 = (char *)operator new(v17);
            v19 = Source;
            if ( Source )
            {
              if ( Count )
              {
                _strncpy(v18, Source, Count);
                v18[Count] = 0;
              }
              else
              {
                *v18 = 0;
              }
              v19 = Source;
            }
            if ( v19 != &v86 )
              operator delete(v19);
            Source = v18;
          }
          v20 = 0;
          v21 = 34;
          do
          {
            Source[Count + v20] = v21;
            v21 = *(_BYTE *)(v20++ + 7569837);
          }
          while ( v21 );
          Count = v16;
          Source[v16] = 0;
        }
        v22 = v83.data;
        if ( v83.data )
        {
          v23 = 0;
          if ( *v83.data )
          {
            do
              ++v23;
            while ( v83.data[v23] );
          }
          v24 = Count + v23;
          v25 = Count + v23 + 1;
          if ( (_DWORD)segment.mDeclEffect < v25 )
          {
            if ( v25 % 512 )
              v25 = v25 - v25 % 512 + 512;
            segment.mDeclEffect = (rvDeclEffect *)v25;
            v26 = (char *)operator new(v25);
            v27 = Source;
            if ( Source )
            {
              if ( Count )
              {
                _strncpy(v26, Source, Count);
                v26[Count] = 0;
              }
              else
              {
                *v26 = 0;
              }
              v27 = Source;
            }
            if ( v27 != &v86 )
              operator delete(v27);
            Source = v26;
          }
          v28 = *v22;
          for ( i = 0; v28; ++i )
          {
            Source[Count + i] = v28;
            v28 = v22[i + 1];
          }
          Count = v24;
          Source[v24] = 0;
        }
        if ( v83.type == 1 )
        {
          v30 = 0;
          do
            ++v30;
          while ( SubStr[v30] );
          v31 = Count + v30;
          v32 = Count + v30 + 1;
          if ( (_DWORD)segment.mDeclEffect < v32 )
          {
            if ( v32 % 512 )
              v32 = v32 - v32 % 512 + 512;
            segment.mDeclEffect = (rvDeclEffect *)v32;
            v33 = (char *)operator new(v32);
            v34 = Source;
            if ( Source )
            {
              if ( Count )
              {
                _strncpy(v33, Source, Count);
                v33[Count] = 0;
              }
              else
              {
                *v33 = 0;
              }
              v34 = Source;
            }
            if ( v34 != &v86 )
              operator delete(v34);
            Source = v33;
          }
          v35 = 0;
          v36 = 34;
          do
          {
            Source[Count + v35] = v36;
            v36 = *(_BYTE *)(v35++ + 7569837);
          }
          while ( v36 );
          Count = v31;
          Source[v31] = 0;
        }
        v37 = 0;
        do
          ++v37;
        while ( asc_7381A0[v37] );
        v38 = Count + v37;
        v39 = Count + v37 + 1;
        if ( (_DWORD)segment.mDeclEffect < v39 )
        {
          if ( v39 % 512 )
            v39 = v39 - v39 % 512 + 512;
          segment.mDeclEffect = (rvDeclEffect *)v39;
          v40 = (char *)operator new(v39);
          v41 = Source;
          if ( Source )
          {
            if ( Count )
            {
              _strncpy(v40, Source, Count);
              v40[Count] = 0;
            }
            else
            {
              *v40 = 0;
            }
            v41 = Source;
          }
          if ( v41 != &v86 )
            operator delete(v41);
          Source = v40;
        }
        v42 = 0;
        v43 = 32;
        do
        {
          Source[Count + v42] = v43;
          v43 = asc_7381A0[v42++ + 1];
        }
        while ( v43 );
        Count = v38;
      }
      (*((void (__stdcall **)(char *))declHelper.type[1].vfptr->__vecDelDtor + 20))(Source);
      idParser::FreeSource(&v85, 1);
      LOBYTE(v90) = 4;
      idStr::FreeData((idStr *)&v83.len);
      v83.len = 0;
      v83.alloced = -20;
      v83.data = v83.baseBuffer;
      v83.baseBuffer[0] = 0;
      LOBYTE(v90) = 3;
      if ( Source != &v86 )
        operator delete(Source);
      v3 = v80;
      Source = &v86;
      v86 = 0;
      Count = 0;
      segment.mDeclEffect = (rvDeclEffect *)512;
    }
    v44 = (*((int (**)(void))declHelper.type[1].vfptr->__vecDelDtor + 10))();
    v45 = (*((int (**)(void))declHelper.type[1].vfptr->__vecDelDtor + 1))();
    v46 = (*((int (**)(void))declHelper.type[1].vfptr->__vecDelDtor + 13))();
    v47 = va("%s: %s", v46, v45);
    idParser::LoadMemory(&v85, text, textLength, v47, v44);
    v6 = (idTokenCache *)token.len;
  }
  if ( ((int (*)(void))declHelper.type->vfptr->OnReload)()
    && !(*((unsigned __int8 (**)(void))declHelper.type[1].vfptr->__vecDelDtor + 19))() )
  {
    v48 = (idStrList *)((int (*)(void))declHelper.type->vfptr->OnReload)();
    idParser::AddIncludes(&v85, v48);
  }
  if ( (*(unsigned __int8 (**)(void))(*(_DWORD *)declHelper.declBuffer + 28))()
    && (((unsigned __int8 (__stdcall *)(const char *))cvarSystem->vfptr->GetCVarBool)("com_writeBinaryDecls")
     || (*(unsigned __int8 (**)(void))(*(_DWORD *)declHelper.declBuffer + 20))())
    && (!(*((unsigned __int8 (**)(void))declHelper.type[1].vfptr->__vecDelDtor + 19))()
     || (*(unsigned __int8 (**)(void))(*(_DWORD *)declHelper.declBuffer + 20))()
     && (*((int (**)(void))declHelper.type[1].vfptr->__vecDelDtor + 3))() == 1) )
  {
    v49 = (*((int (**)(void))declHelper.type[1].vfptr->__vecDelDtor + 1))();
    declHelper.decl = (idDecl *)((int (__stdcall *)(int))fileSystem->vfptr->OpenMemoryFile)(v49);
    declHelper.decl[6].base = (idDeclBase *)256;
    idParser::WriteBinary(&v85, (idFile *)declHelper.decl, v6);
    idParser::ResetBinaryParsing(&v85);
  }
  LOBYTE(v90) = 6;
  idParser::SkipUntilString(&v85, "{", (idToken *)((char *)&token + 4));
  if ( idParser::ReadToken(&v85, (idToken *)((char *)&token + 4)) )
  {
    while ( idStr::Cmp((const char *)token.alloced, "}") )
    {
      rvSegmentTemplate::Init((rvSegmentTemplate *)((char *)&segment + 4), v3);
      if ( idStr::Icmp((const char *)token.alloced, "size") )
      {
        if ( idStr::Icmp((const char *)token.alloced, "cutOffDistance") )
        {
          if ( idStr::Icmp((const char *)token.alloced, "effect") )
          {
            if ( idStr::Icmp((const char *)token.alloced, "emitter") )
            {
              if ( idStr::Icmp((const char *)token.alloced, "spawner") )
              {
                if ( idStr::Icmp((const char *)token.alloced, "trail") )
                {
                  if ( idStr::Icmp((const char *)token.alloced, "sound") )
                  {
                    if ( idStr::Icmp((const char *)token.alloced, "decal") )
                    {
                      if ( idStr::Icmp((const char *)token.alloced, "light") )
                      {
                        if ( idStr::Icmp((const char *)token.alloced, "delay") )
                        {
                          if ( idStr::Icmp((const char *)token.alloced, "shake") )
                          {
                            if ( idStr::Icmp((const char *)token.alloced, "tunnel") )
                            {
                              if ( v85.scriptstack )
                              {
                                v50 = v85.scriptstack->line;
                                v51 = v85.scriptstack->filename.data;
                              }
                              else
                              {
                                v50 = 0;
                                v51 = (char *)&s2;
                              }
                              (*(void (**)(netadrtype_t, const char *, ...))(*(_DWORD *)common.type + 68))(
                                common.type,
                                "^4BSE:^1 Invalid segment type '%s' (file: %s, line: %d)",
                                token.alloced,
                                v51,
                                v50);
                              idParser::SkipBracedSection(&v85, 1);
                            }
                            else
                            {
                              rvSegmentTemplate::Parse((rvSegmentTemplate *)((char *)&segment + 4), v3, 10, &v85);
                            }
                          }
                          else
                          {
                            rvSegmentTemplate::Parse((rvSegmentTemplate *)((char *)&segment + 4), v3, 9, &v85);
                          }
                        }
                        else
                        {
                          rvSegmentTemplate::Parse((rvSegmentTemplate *)((char *)&segment + 4), v3, 8, &v85);
                        }
                      }
                      else
                      {
                        rvSegmentTemplate::Parse((rvSegmentTemplate *)((char *)&segment + 4), v3, 7, &v85);
                      }
                    }
                    else
                    {
                      rvSegmentTemplate::Parse((rvSegmentTemplate *)((char *)&segment + 4), v3, 6, &v85);
                    }
                  }
                  else
                  {
                    rvSegmentTemplate::Parse((rvSegmentTemplate *)((char *)&segment + 4), v3, 5, &v85);
                  }
                }
                else
                {
                  rvSegmentTemplate::Parse((rvSegmentTemplate *)((char *)&segment + 4), v3, 4, &v85);
                }
              }
              else
              {
                rvSegmentTemplate::Parse((rvSegmentTemplate *)((char *)&segment + 4), v3, 3, &v85);
              }
            }
            else
            {
              rvSegmentTemplate::Parse((rvSegmentTemplate *)((char *)&segment + 4), v3, 2, &v85);
            }
          }
          else
          {
            rvSegmentTemplate::Parse((rvSegmentTemplate *)((char *)&segment + 4), v3, 1, &v85);
          }
          if ( rvSegmentTemplate::Finish((rvSegmentTemplate *)((char *)&segment + 4), v3) )
          {
            if ( !v3->mSegmentTemplates.list )
            {
              v52 = v3->mSegmentTemplates.granularity;
              if ( v52 > 0 )
              {
                if ( v52 != v3->mSegmentTemplates.size )
                {
                  v54 = __OFSUB__(v52, v3->mSegmentTemplates.num);
                  v53 = v52 - v3->mSegmentTemplates.num < 0;
                  v3->mSegmentTemplates.size = v52;
                  if ( v53 ^ v54 )
                    v3->mSegmentTemplates.num = v52;
                  v55 = 404 * v52 | -(404 * (unsigned __int64)(unsigned int)v52 >> 32 != 0);
                  v56 = (int *)operator new((v55 + 4) | -__CFADD__(v55, 4));
                  token.len = (int)v56;
                  LOBYTE(v90) = 7;
                  if ( v56 )
                  {
                    v57 = (int)(v56 + 1);
                    *v56 = v52;
                    `eh vector constructor iterator'(
                      v56 + 1,
                      0x194u,
                      v52,
                      (void (__thiscall *)(void *))rvSegmentTemplate::rvSegmentTemplate,
                      (void (__thiscall *)(void *))rvSegmentTemplate::~rvSegmentTemplate);
                  }
                  else
                  {
                    v57 = 0;
                  }
                  v3 = v80;
                  LOBYTE(v90) = 6;
                  v80->mSegmentTemplates.list = (rvSegmentTemplate *)v57;
                }
              }
              else
              {
                v3->mSegmentTemplates.list = 0;
                v3->mSegmentTemplates.num = 0;
                v3->mSegmentTemplates.size = 0;
              }
            }
            v58 = v3->mSegmentTemplates.num;
            v59 = v3->mSegmentTemplates.size;
            if ( v58 == v59 )
            {
              if ( !v3->mSegmentTemplates.granularity )
                v3->mSegmentTemplates.granularity = 16;
              v60 = v59
                  + v3->mSegmentTemplates.granularity
                  - (v59 + v3->mSegmentTemplates.granularity) % v3->mSegmentTemplates.granularity;
              if ( v60 > 0 )
              {
                if ( v60 != v3->mSegmentTemplates.size )
                {
                  v63 = v80;
                  v64 = v80->mSegmentTemplates.list;
                  v80->mSegmentTemplates.size = v60;
                  if ( v60 < v58 )
                    v63->mSegmentTemplates.num = v60;
                  v65 = 404 * v60 | -(404 * (unsigned __int64)(unsigned int)v60 >> 32 != 0);
                  v66 = (int *)operator new((v65 + 4) | -__CFADD__(v65, 4));
                  token.len = (int)v66;
                  LOBYTE(v90) = 8;
                  if ( v66 )
                  {
                    v67 = (int)(v66 + 1);
                    *v66 = v60;
                    `eh vector constructor iterator'(
                      v66 + 1,
                      0x194u,
                      v60,
                      (void (__thiscall *)(void *))rvSegmentTemplate::rvSegmentTemplate,
                      (void (__thiscall *)(void *))rvSegmentTemplate::~rvSegmentTemplate);
                  }
                  else
                  {
                    v67 = 0;
                  }
                  v68 = v80;
                  LOBYTE(v90) = 6;
                  v80->mSegmentTemplates.list = (rvSegmentTemplate *)v67;
                  if ( v64 )
                  {
                    v69 = 0;
                    if ( v68->mSegmentTemplates.num > 0 )
                    {
                      v70 = 0;
                      do
                      {
                        rvSegmentTemplate::operator=(&v80->mSegmentTemplates.list[v70], &v64[v70]);
                        ++v69;
                        ++v70;
                      }
                      while ( v69 < v80->mSegmentTemplates.num );
                    }
                    `eh vector destructor iterator'(
                      v64,
                      0x194u,
                      v64[-1].mDecalAxis,
                      (void (__thiscall *)(void *))rvSegmentTemplate::~rvSegmentTemplate);
                    operator delete(&v64[-1].mDecalAxis);
                  }
                }
              }
              else
              {
                v61 = v3->mSegmentTemplates.list;
                if ( v61 )
                {
                  v62 = &v61[-1].mDecalAxis;
                  `eh vector destructor iterator'(
                    v61,
                    0x194u,
                    v61[-1].mDecalAxis,
                    (void (__thiscall *)(void *))rvSegmentTemplate::~rvSegmentTemplate);
                  operator delete(v62);
                }
                v3->mSegmentTemplates.list = 0;
                v3->mSegmentTemplates.num = 0;
                v3->mSegmentTemplates.size = 0;
              }
            }
            v3 = v80;
            rvSegmentTemplate::operator=(
              &v80->mSegmentTemplates.list[v80->mSegmentTemplates.num],
              (rvSegmentTemplate *)((char *)&segment + 4));
            ++v3->mSegmentTemplates.num;
          }
        }
        else
        {
          v3->mCutOffDistance = idParser::ParseFloat(&v85, 0);
        }
      }
      else
      {
        v3->mSize = idParser::ParseFloat(&v85, 0);
      }
      if ( !idParser::ReadToken(&v85, (idToken *)((char *)&token + 4)) )
        goto LABEL_155;
    }
    rvDeclEffect::Finish(v3);
    LOBYTE(v90) = 3;
    if ( declHelper.type )
    {
      if ( !declHelper.decl || (*(unsigned __int8 (**)(void))(*(_DWORD *)declHelper.declBuffer + 20))() )
        ((void (__stdcall *)(idDeclTypeInterface *, idParser *))declManager->vfptr->AddDependencies)(
          declHelper.type,
          &v85);
      v75 = declHelper.type;
      if ( declHelper.type )
      {
        if ( declHelper.decl )
        {
          if ( (*((int (__thiscall **)(idDeclTypeInterfaceVtbl *))declHelper.type[1].vfptr->__vecDelDtor + 3))(declHelper.type[1].vfptr) != 1 )
          {
            v76 = ((int (*)(void))declHelper.decl->vfptr->Size)();
            v77 = ((int (*)(void))declHelper.decl->vfptr[4].FreeData)();
            (*((void (__stdcall **)(int, int))declHelper.type[1].vfptr->__vecDelDtor + 16))(v77, v76);
          }
          v75 = declHelper.type;
        }
        if ( v75 && declHelper.declBufferLength )
        {
          (*((void (__stdcall **)(int))v75[1].vfptr->__vecDelDtor + 18))(declHelper.declBufferLength);
          declHelper.declBufferLength = 0;
        }
      }
    }
    if ( declHelper.decl )
      ((void (__stdcall *)(idDecl *))fileSystem->vfptr->CloseFile)(declHelper.decl);
    LOBYTE(v90) = 2;
    rvSegmentTemplate::~rvSegmentTemplate((rvSegmentTemplate *)((char *)&segment + 4));
    LOBYTE(v90) = 1;
    idStr::FreeData((idStr *)&token.data);
    token.alloced = (int)&token.baseBuffer[4];
    token.data = 0;
    *(_DWORD *)token.baseBuffer = -20;
    token.baseBuffer[4] = 0;
    LOBYTE(v90) = 0;
    idParser::~idParser(&v85);
    v90 = -1;
    idStr::FreeData(&v84);
    result = 1;
  }
  else
  {
LABEL_155:
    LOBYTE(v90) = 3;
    if ( declHelper.type )
    {
      if ( !declHelper.decl || (*(unsigned __int8 (**)(void))(*(_DWORD *)declHelper.declBuffer + 20))() )
        ((void (__stdcall *)(idDeclTypeInterface *, idParser *))declManager->vfptr->AddDependencies)(
          declHelper.type,
          &v85);
      v71 = declHelper.type;
      if ( declHelper.type )
      {
        if ( declHelper.decl )
        {
          if ( (*((int (__thiscall **)(idDeclTypeInterfaceVtbl *))declHelper.type[1].vfptr->__vecDelDtor + 3))(declHelper.type[1].vfptr) != 1 )
          {
            v72 = ((int (*)(void))declHelper.decl->vfptr->Size)();
            v73 = ((int (*)(void))declHelper.decl->vfptr[4].FreeData)();
            (*((void (__stdcall **)(int, int))declHelper.type[1].vfptr->__vecDelDtor + 16))(v73, v72);
          }
          v71 = declHelper.type;
        }
        if ( v71 && declHelper.declBufferLength )
        {
          (*((void (__stdcall **)(int))v71[1].vfptr->__vecDelDtor + 18))(declHelper.declBufferLength);
          declHelper.declBufferLength = 0;
        }
      }
    }
    if ( declHelper.decl )
      ((void (__stdcall *)(idDecl *))fileSystem->vfptr->CloseFile)(declHelper.decl);
    LOBYTE(v90) = 2;
    rvSegmentTemplate::~rvSegmentTemplate((rvSegmentTemplate *)((char *)&segment + 4));
    LOBYTE(v90) = 1;
    idStr::FreeData((idStr *)&token.data);
    token.alloced = (int)&token.baseBuffer[4];
    token.data = 0;
    *(_DWORD *)token.baseBuffer = -20;
    token.baseBuffer[4] = 0;
    LOBYTE(v90) = 0;
    idParser::~idParser(&v85);
    v90 = -1;
    idStr::FreeData(&v84);
    result = 0;
  }
  return result;
}
int dynamic_initializer_for__singletonLock___106()
{
  sdLock::sdLock(&singletonLock_106);
  return atexit(dynamic_atexit_destructor_for__singletonLock___106);
}
void __cdecl dynamic_atexit_destructor_for__singletonLock___106()
{
  sdLock::~sdLock(&singletonLock_106);
}
