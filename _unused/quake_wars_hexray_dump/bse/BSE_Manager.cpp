rvBSEManagerLocal *__thiscall rvBSEManager::`scalar deleting destructor'(rvBSEManagerLocal *this, unsigned int a2)
{
  rvBSEManagerLocal *v2; // esi

  v2 = this;
  this->vfptr = (rvBSEManagerVtbl *)&rvBSEManager::`vftable';
  if ( a2 & 1 )
    operator delete(this);
  return v2;
}
void __cdecl BSE_Pause_f()
{
  if ( 0.0 == bseLocal.pauseTime )
  {
    bseLocal.pauseTime = -1.0;
    (*(void (__cdecl **)(netadrtype_t, const char *))(*(_DWORD *)common.type + 52))(common.type, "BSE Paused\n");
  }
  else
  {
    bseLocal.pauseTime = 0.0;
    (*(void (__cdecl **)(netadrtype_t, const char *))(*(_DWORD *)common.type + 52))(common.type, "BSE Activated\n");
  }
}
idVec3 *__stdcall rvBSEManagerLocal::GetCubeNormals(int index)
{
  return (idVec3 *)(12 * index + 8282336);
}
void rvBSEManagerLocal::EndLevelLoad()
{
  rvBSEManagerLocal::mEffectRates[0] = 0.0;
  dword_11F4D44 = 0;
  dword_11F4D48 = 0;
}
void rvBSEManagerLocal::StartFrame()
{
  if ( bse_speeds.internalVar->integerValue )
  {
    rvBSEManagerLocal::mPerfCounters[0] = 0;
    dword_11F4D50 = 0;
    dword_11F4D54 = 0;
    dword_11F4D58 = 0;
    dword_11F4D5C = 0;
  }
}
void rvBSEManagerLocal::EndFrame()
{
  if ( bse_speeds.internalVar->integerValue )
    (*(void (**)(netadrtype_t, const char *, ...))(*(_DWORD *)common.type + 52))(
      common.type,
      "bse_active: %i particles: %i traces: %i texels: %i\n",
      rvBSEManagerLocal::mPerfCounters[0],
      dword_11F4D54,
      dword_11F4D50,
      (double)dword_11F4D58 * 0.00000095367431640625);
}
bool __thiscall rvBSEManagerLocal::Filtered(rvBSEManagerLocal *this, const char *name, effectCategory_t category)
{
  rvBSEManagerLocal *v3; // esi
  const char *v4; // ecx
  int v5; // eax
  bool result; // al

  v3 = this;
  v4 = bse_singleEffect.internalVar->value;
  v5 = 0;
  if ( !*v4 )
    goto LABEL_9;
  do
    ++v5;
  while ( v4[v5] );
  if ( v5 && !_strstr(name, bse_singleEffect.internalVar->value) )
    result = 1;
  else
LABEL_9:
    result = v3->vfptr->CanPlayRateLimited((rvBSEManager *)&v3->vfptr, category) == 0;
  return result;
}
void rvBSEManagerLocal::UpdateRateTimes()
{
  rvBSEManagerLocal::mEffectRates[0] = rvBSEManagerLocal::mEffectRates[0] - 0.1000000014901161;
  if ( rvBSEManagerLocal::mEffectRates[0] < 0.0 )
    rvBSEManagerLocal::mEffectRates[0] = 0.0;
  *(float *)&dword_11F4D44 = *(float *)&dword_11F4D44 - 0.1000000014901161;
  if ( *(float *)&dword_11F4D44 < 0.0 )
    *(float *)&dword_11F4D44 = 0.0;
  *(float *)&dword_11F4D48 = *(float *)&dword_11F4D48 - 0.1000000014901161;
  if ( *(float *)&dword_11F4D48 < 0.0 )
    *(float *)&dword_11F4D48 = 0.0;
}
bool __stdcall rvBSEManagerLocal::CanPlayRateLimited(effectCategory_t category)
{
  effectCategory_t v1; // ecx
  float *v2; // esi
  bool result; // al
  float cost; // [esp+14h] [ebp+4h]

  v1 = category;
  if ( category == EC_IGNORE || bse_rateLimit.internalVar->floatValue <= 0.1000000014901161 )
    return 1;
  v2 = (float *)(4 * category + 18828608);
  cost = effectCosts[category] * bse_rateCost.internalVar->floatValue;
  if ( bse_rateLimit.internalVar->floatValue * 0.5 < rvBSEManagerLocal::mEffectRates[v1]
    && cost + *v2 > rvRandom::flrand(0.0, bse_rateLimit.internalVar->floatValue) )
  {
    return 0;
  }
  result = 1;
  *v2 = cost + *v2;
  return result;
}
double __stdcall rvBSEManagerLocal::EffectDuration(rvRenderEffectLocal *def)
{
  double result; // st7

  if ( !def || def->index < 0 || !def->effect )
    return 0.0;
  def->effect->vfptr->GetDuration(def->effect);
  return result;
}
void __stdcall rvBSEManagerLocal::SetShakeParms(float time, float scale)
{
  ((void (__stdcall *)(_DWORD, _DWORD, _DWORD))game->vfptr->StartViewEffect)(0, LODWORD(time), LODWORD(scale));
}
void __stdcall rvBSEManagerLocal::SetTunnelParms(float time, float scale)
{
  ((void (__stdcall *)(signed int, _DWORD, _DWORD))game->vfptr->StartViewEffect)(1, LODWORD(time), LODWORD(scale));
}
void __stdcall rvBSEManagerLocal::MakeEditable(rvParticleTemplate *particle)
{
  rvParticleTemplate::MakeEditable(particle);
}
void __stdcall rvBSEManagerLocal::CopySegment(rvSegmentTemplate *dest, rvSegmentTemplate *src)
{
  rvSegmentTemplate::Duplicate(dest, src);
}
idMat3 *rvBSEManagerLocal::GetModelToBSE()
{
  return &rvBSEManagerLocal::mModelToBSE;
}
BOOL __thiscall rvBSEManagerLocal::IsTimeLocked(rvBSEManagerLocal *this)
{
  return this->pauseTime > 0.0;
}
double __thiscall rvBSEManagerLocal::GetLockedTime(rvBSEManagerLocal *this)
{
  return this->pauseTime;
}
idTraceModel *__stdcall rvBSEManagerLocal::GetTraceModel(int index)
{
  idTraceModel *result; // eax

  if ( index < 0 || index >= rvBSEManagerLocal::mTraceModels.num )
    result = 0;
  else
    result = rvBSEManagerLocal::mTraceModels.list[index];
  return result;
}
void __stdcall rvBSEManagerLocal::FreeTraceModel(int index)
{
  if ( index >= 0 && index < rvBSEManagerLocal::mTraceModels.num )
  {
    operator delete(rvBSEManagerLocal::mTraceModels.list[index]);
    rvBSEManagerLocal::mTraceModels.list[index] = 0;
  }
}
void __stdcall rvBSEManagerLocal::StopEffect(rvRenderEffectLocal *def)
{
  int v1; // eax

  if ( def && def->index >= 0 && def->effect )
  {
    if ( bse_debug.internalVar->integerValue )
    {
      v1 = (*(int (**)(void))def->parms.declEffect->base->vfptr->gap4)();
      (*(void (**)(netadrtype_t, const char *, ...))(*(_DWORD *)common.type + 52))(
        common.type,
        "Stopping effect: %s\n",
        v1);
    }
    ((void (__stdcall *)(signed int))def->effect->vfptr->SetStopped)(1);
  }
  else
  {
    def->newEffect = 0;
    def->expired = 1;
  }
}
void __stdcall rvBSEManagerLocal::RestartEffect(rvRenderEffectLocal *def)
{
  int v1; // eax

  if ( def && def->index >= 0 && def->effect )
  {
    if ( bse_debug.internalVar->integerValue )
    {
      v1 = (*(int (**)(void))def->parms.declEffect->base->vfptr->gap4)();
      (*(void (**)(netadrtype_t, const char *, ...))(*(_DWORD *)common.type + 52))(
        common.type,
        "Restarting effect: %s\n",
        v1);
    }
    ((void (__stdcall *)(_DWORD))def->effect->vfptr->SetStopped)(0);
  }
}
char __thiscall rvBSEManagerLocal::ServiceEffect(rvBSEManagerLocal *this, rvRenderEffectLocal *def, float time, bool *forcePush)
{
  rvBSEManagerLocal *v4; // edi
  rvBSE *v5; // ebp
  int v6; // eax
  bool *v7; // eax
  float *v9; // eax

  v4 = this;
  if ( -1.0 == this->pauseTime )
    this->pauseTime = time;
  if ( this->pauseTime > 0.0 )
    time = this->pauseTime;
  v5 = def->effect;
  if ( !v5 )
    return 1;
  v6 = (*(int (**)(void))def->parms.declEffect->base->vfptr->gap4)();
  if ( v4->vfptr->Filtered((rvBSEManager *)&v4->vfptr, (const char *)v6, 0) )
    return 1;
  v7 = forcePush;
  LOBYTE(v7) = def->gameTime > def->serviceTime;
  if ( ((unsigned __int8 (__thiscall *)(rvBSE *, renderEffect_s *, _DWORD, bool *, bool *))v5->vfptr->Service)(
         v5,
         &def->parms,
         LODWORD(time),
         v7,
         forcePush) )
  {
    return 1;
  }
  def->serviceTime = def->gameTime;
  v9 = (float *)v5->vfptr->GetCurrentLocalBounds(v5);
  def->referenceBounds.b[0].x = *v9;
  def->referenceBounds.b[0].y = v9[1];
  def->referenceBounds.b[0].z = v9[2];
  def->referenceBounds.b[1].x = v9[3];
  def->referenceBounds.b[1].y = v9[4];
  def->referenceBounds.b[1].z = v9[5];
  if ( bse_speeds.internalVar->integerValue )
    ++rvBSEManagerLocal::mPerfCounters[0];
  if ( bse_debug.internalVar->integerValue )
    rvBSE::EvaluateCost(v5, -1);
  return 0;
}
void __stdcall rvBSEManagerLocal::FreeEffect(rvRenderEffectLocal *def)
{
  int v1; // eax
  rvBSE *v2; // eax

  if ( def && def->index >= 0 && def->effect )
  {
    if ( bse_debug.internalVar->integerValue )
    {
      v1 = (*(int (**)(void))def->parms.declEffect->base->vfptr->gap4)();
      (*(void (**)(netadrtype_t, const char *, ...))(*(_DWORD *)common.type + 52))(
        common.type,
        "Freeing effect: %s\n",
        v1);
    }
    rvBSE::Destroy(def->effect);
    v2 = def->effect;
    if ( v2 )
    {
      v2[1].vfptr = (rvBSEVtbl *)unk_11F4D64;
      --unk_11F4D6C;
      unk_11F4D64 = v2;
    }
    def->effect = 0;
  }
}
void __thiscall rvBSE::SetStopped(rvBSE *this, bool stopped)
{
  if ( stopped )
    this->mFlags |= 8u;
  else
    this->mFlags &= 0xFFFFFFF7;
}
void __thiscall rvBSE::SetOrientateIdentity(rvBSE *this, bool orientateIdentity)
{
  if ( orientateIdentity )
    this->mFlags |= 0x10u;
  else
    this->mFlags &= 0xFFFFFFEF;
}
double __thiscall rvBSE::GetDuration(rvBSE *this)
{
  return this->mDuration;
}
idBounds *__thiscall rvBSE::GetCurrentLocalBounds(rvBSE *this)
{
  return &this->mCurrentLocalBounds;
}
int __thiscall rvBSE::GetValidFrames(rvBSE *this)
{
  return this->mValidFrames;
}
void __thiscall rvBSE::~rvBSE(rvBSE *this)
{
  rvBSE *v1; // esi
  rvSegment *v2; // eax
  rvParticle **v3; // edi

  v1 = this;
  this->vfptr = (rvBSEVtbl *)&rvBSE::`vftable';
  v2 = this->mSegments.list;
  if ( v2 )
  {
    v3 = &v2[-1].mParticles;
    `eh vector destructor iterator'(
      v2,
      0x50u,
      (int)v2[-1].mParticles,
      (void (__thiscall *)(void *))rvSegment::~rvSegment);
    operator delete(v3);
  }
  v1->mSegments.list = 0;
  v1->mSegments.num = 0;
  v1->mSegments.size = 0;
}
void __userpurge rvBSEManagerLocal::Stats(rvBSEManagerLocal *this@<ecx>, int a2@<esi>, idCmdArgs *args, int a4)
{
  const char *v4; // eax
  sdDeclTypeHolder *v5; // eax
  int v6; // eax
  int v7; // edi
  sdDeclTypeHolder *v8; // eax
  sdDeclTypeHolder *v9; // eax
  int v10; // esi
  int v11; // ebp
  float *v12; // esi
  int v13; // ebx
  float v14; // ST30_4
  int v15; // [esp+18h] [ebp-24h]
  int haveParts; // [esp+2Ch] [ebp-10h]
  int partTotal; // [esp+30h] [ebp-Ch]
  signed int loadall; // [esp+34h] [ebp-8h]
  int v19; // [esp+38h] [ebp-4h]
  float segTotal; // [esp+40h] [ebp+4h]

  if ( args->argc <= 1 )
    v4 = (const char *)&s2;
  else
    v4 = args->argv[1];
  LOBYTE(v19) = idStr::Icmp(v4, "all") == 0;
  v5 = sdSingleton<sdDeclTypeHolder>::GetInstance();
  v6 = ((int (__stdcall *)(int))declManager->vfptr->GetNumDecls)(v5->declEffectsType.declTypeHandle);
  (*(void (**)(netadrtype_t, const char *, ...))(*(_DWORD *)common.type + 52))(
    common.type,
    "... processing %d registered effects\n",
    v6);
  haveParts = 0;
  loadall = 1;
  partTotal = 1;
  v7 = 1;
  v8 = sdSingleton<sdDeclTypeHolder>::GetInstance();
  if ( ((int (__stdcall *)(int))declManager->vfptr->GetNumDecls)(v8->declEffectsType.declTypeHandle) > 1 )
  {
    v15 = a2;
    do
    {
      v9 = sdSingleton<sdDeclTypeHolder>::GetInstance();
      v10 = ((int (__stdcall *)(int, int, int, int))declManager->vfptr->DeclByIndex)(
              v9->declEffectsType.declTypeHandle,
              v7,
              v19,
              v15);
      if ( (*(int (**)(void))(**(_DWORD **)(v10 + 4) + 12))() == 2 )
      {
        v11 = *(_DWORD *)(v10 + 28);
        ++haveParts;
        if ( v11 > 0 )
        {
          v12 = (float *)(*(_DWORD *)(v10 + 40) + 344);
          v13 = v11;
          do
          {
            if ( (*((_DWORD *)v12 - 77) >> 2) & 1 )
            {
              v19 += (signed int)*v12;
              ++loadall;
            }
            v12 += 101;
            --v13;
          }
          while ( v13 );
        }
        a4 += v11;
      }
      else if ( !(*(unsigned __int8 (__thiscall **)(_DWORD))(**(_DWORD **)(v10 + 4) + 96))(*(_DWORD *)(v10 + 4)) )
      {
        ++partTotal;
      }
      ++v7;
      v15 = sdSingleton<sdDeclTypeHolder>::GetInstance()->declEffectsType.declTypeHandle;
    }
    while ( v7 < ((int (*)(void))declManager->vfptr->GetNumDecls)() );
  }
  (*(void (**)(netadrtype_t, const char *, ...))(*(_DWORD *)common.type + 52))(
    common.type,
    "%d segments in %d loaded effects (%d never referenced)\n",
    1,
    1,
    haveParts);
  segTotal = (double)1;
  (*(void (**)(netadrtype_t, const char *, ...))(*(_DWORD *)common.type + 52))(
    common.type,
    "%.2f segments per effect\n",
    segTotal / (double)1);
  v14 = (double)partTotal;
  (*(void (**)(netadrtype_t, const char *, ...))(*(_DWORD *)common.type + 52))(
    common.type,
    "%.2f of segments have particles\n",
    v14 / segTotal);
  (*(void (**)(netadrtype_t, const char *, ...))(*(_DWORD *)common.type + 52))(
    common.type,
    "%.2f particles per segment with particles\n",
    (double)loadall / v14);
}
void __userpurge rvBSEManagerLocal::Stats(rvBSEManagerLocal *this@<ecx>, int a2@<esi>, idCmdArgs *args, int a4)
{
  const char *v4; // eax
  sdDeclTypeHolder *v5; // eax
  int v6; // eax
  int v7; // edi
  sdDeclTypeHolder *v8; // eax
  sdDeclTypeHolder *v9; // eax
  int v10; // esi
  int v11; // ebp
  float *v12; // esi
  int v13; // ebx
  float v14; // ST30_4
  int v15; // [esp+18h] [ebp-24h]
  int haveParts; // [esp+2Ch] [ebp-10h]
  int partTotal; // [esp+30h] [ebp-Ch]
  signed int loadall; // [esp+34h] [ebp-8h]
  int v19; // [esp+38h] [ebp-4h]
  float segTotal; // [esp+40h] [ebp+4h]

  if ( args->argc <= 1 )
    v4 = (const char *)&s2;
  else
    v4 = args->argv[1];
  LOBYTE(v19) = idStr::Icmp(v4, "all") == 0;
  v5 = sdSingleton<sdDeclTypeHolder>::GetInstance();
  v6 = ((int (__stdcall *)(int))declManager->vfptr->GetNumDecls)(v5->declEffectsType.declTypeHandle);
  (*(void (**)(netadrtype_t, const char *, ...))(*(_DWORD *)common.type + 52))(
    common.type,
    "... processing %d registered effects\n",
    v6);
  haveParts = 0;
  loadall = 1;
  partTotal = 1;
  v7 = 1;
  v8 = sdSingleton<sdDeclTypeHolder>::GetInstance();
  if ( ((int (__stdcall *)(int))declManager->vfptr->GetNumDecls)(v8->declEffectsType.declTypeHandle) > 1 )
  {
    v15 = a2;
    do
    {
      v9 = sdSingleton<sdDeclTypeHolder>::GetInstance();
      v10 = ((int (__stdcall *)(int, int, int, int))declManager->vfptr->DeclByIndex)(
              v9->declEffectsType.declTypeHandle,
              v7,
              v19,
              v15);
      if ( (*(int (**)(void))(**(_DWORD **)(v10 + 4) + 12))() == 2 )
      {
        v11 = *(_DWORD *)(v10 + 28);
        ++haveParts;
        if ( v11 > 0 )
        {
          v12 = (float *)(*(_DWORD *)(v10 + 40) + 344);
          v13 = v11;
          do
          {
            if ( (*((_DWORD *)v12 - 77) >> 2) & 1 )
            {
              v19 += (signed int)*v12;
              ++loadall;
            }
            v12 += 101;
            --v13;
          }
          while ( v13 );
        }
        a4 += v11;
      }
      else if ( !(*(unsigned __int8 (__thiscall **)(_DWORD))(**(_DWORD **)(v10 + 4) + 96))(*(_DWORD *)(v10 + 4)) )
      {
        ++partTotal;
      }
      ++v7;
      v15 = sdSingleton<sdDeclTypeHolder>::GetInstance()->declEffectsType.declTypeHandle;
    }
    while ( v7 < ((int (*)(void))declManager->vfptr->GetNumDecls)() );
  }
  (*(void (**)(netadrtype_t, const char *, ...))(*(_DWORD *)common.type + 52))(
    common.type,
    "%d segments in %d loaded effects (%d never referenced)\n",
    1,
    1,
    haveParts);
  segTotal = (double)1;
  (*(void (**)(netadrtype_t, const char *, ...))(*(_DWORD *)common.type + 52))(
    common.type,
    "%.2f segments per effect\n",
    segTotal / (double)1);
  v14 = (double)partTotal;
  (*(void (**)(netadrtype_t, const char *, ...))(*(_DWORD *)common.type + 52))(
    common.type,
    "%.2f of segments have particles\n",
    v14 / segTotal);
  (*(void (**)(netadrtype_t, const char *, ...))(*(_DWORD *)common.type + 52))(
    common.type,
    "%.2f particles per segment with particles\n",
    (double)loadall / v14);
}
void __usercall BSE_Log_f(int a1@<esi>)
{
  sdDeclTypeHolder *v1; // eax
  int v2; // eax
  int v3; // ebx
  int v4; // edi
  sdDeclTypeHolder *v5; // eax
  sdDeclTypeHolder *v6; // eax
  int v7; // eax
  int v8; // esi
  int v9; // eax
  sdDeclTypeHolder *v10; // eax
  int v11; // eax
  int v12; // [esp-4h] [ebp-Ch]

  v1 = sdSingleton<sdDeclTypeHolder>::GetInstance();
  v2 = ((int (__stdcall *)(int))declManager->vfptr->GetNumDecls)(v1->declEffectsType.declTypeHandle);
  (*(void (**)(netadrtype_t, const char *, ...))(*(_DWORD *)common.type + 52))(
    common.type,
    "Processing %d effect decls....\n",
    v2);
  v3 = 0;
  v4 = 1;
  v5 = sdSingleton<sdDeclTypeHolder>::GetInstance();
  if ( ((int (__stdcall *)(int))declManager->vfptr->GetNumDecls)(v5->declEffectsType.declTypeHandle) > 1 )
  {
    v12 = a1;
    do
    {
      v6 = sdSingleton<sdDeclTypeHolder>::GetInstance();
      v7 = ((int (__stdcall *)(int, int, _DWORD, int))declManager->vfptr->DeclByIndex)(
             v6->declEffectsType.declTypeHandle,
             v4,
             0,
             v12);
      v8 = v7;
      if ( *(_DWORD *)(v7 + 44) || *(_DWORD *)(v7 + 48) )
      {
        v9 = (*(int (**)(void))(**(_DWORD **)(v7 + 4) + 4))();
        (*(void (**)(netadrtype_t, const char *, ...))(*(_DWORD *)common.type + 52))(
          common.type,
          "%d plays (%d loops): '%s'\n",
          *(_DWORD *)(v8 + 44),
          *(_DWORD *)(v8 + 48),
          v9);
        ++v3;
      }
      ++v4;
      v12 = sdSingleton<sdDeclTypeHolder>::GetInstance()->declEffectsType.declTypeHandle;
    }
    while ( v4 < ((int (*)(void))declManager->vfptr->GetNumDecls)() );
  }
  v10 = sdSingleton<sdDeclTypeHolder>::GetInstance();
  v11 = ((int (__stdcall *)(int))declManager->vfptr->GetNumDecls)(v10->declEffectsType.declTypeHandle);
  (*(void (**)(netadrtype_t, const char *, ...))(*(_DWORD *)common.type + 52))(
    common.type,
    "%d effects played or looped out of %d\n",
    v3,
    v11);
}
int __stdcall rvBSEManagerLocal::AddTraceModel(idTraceModel *model)
{
  idTraceModel **v1; // ecx
  int v2; // ebx
  int result; // eax
  int v4; // esi
  int v5; // ebp
  idTraceModel **v6; // eax
  int v7; // eax
  idTraceModel **v8; // esi
  int v9; // eax

  v1 = rvBSEManagerLocal::mTraceModels.list;
  v2 = rvBSEManagerLocal::mTraceModels.num;
  result = 0;
  if ( rvBSEManagerLocal::mTraceModels.num <= 0 )
    goto LABEL_6;
  while ( rvBSEManagerLocal::mTraceModels.list[result] )
  {
    if ( ++result >= rvBSEManagerLocal::mTraceModels.num )
      goto LABEL_6;
  }
  if ( result == -1 )
  {
LABEL_6:
    v4 = rvBSEManagerLocal::mTraceModels.granularity;
    if ( !rvBSEManagerLocal::mTraceModels.list )
    {
      if ( rvBSEManagerLocal::mTraceModels.granularity <= 0 )
      {
        operator delete(0);
        v4 = rvBSEManagerLocal::mTraceModels.granularity;
        v1 = 0;
        v2 = 0;
        v5 = 0;
        rvBSEManagerLocal::mTraceModels.list = 0;
        rvBSEManagerLocal::mTraceModels.num = 0;
        rvBSEManagerLocal::mTraceModels.size = 0;
        goto LABEL_15;
      }
      v5 = rvBSEManagerLocal::mTraceModels.size;
      if ( rvBSEManagerLocal::mTraceModels.granularity == rvBSEManagerLocal::mTraceModels.size )
      {
LABEL_14:
        if ( v2 != v5 )
        {
LABEL_29:
          v1[v2] = model;
          return ++rvBSEManagerLocal::mTraceModels.num - 1;
        }
LABEL_15:
        if ( !v4 )
        {
          v4 = 16;
          rvBSEManagerLocal::mTraceModels.granularity = 16;
        }
        v7 = v4 + v5 - (v4 + v5) % v4;
        if ( v7 > 0 )
        {
          if ( v7 != v5 )
          {
            v8 = v1;
            rvBSEManagerLocal::mTraceModels.size = v7;
            if ( v7 < v2 )
              rvBSEManagerLocal::mTraceModels.num = v7;
            v1 = (idTraceModel **)operator new(4 * v7);
            rvBSEManagerLocal::mTraceModels.list = v1;
            if ( v8 )
            {
              v9 = 0;
              if ( rvBSEManagerLocal::mTraceModels.num > 0 )
              {
                while ( 1 )
                {
                  v1[v9] = v8[v9];
                  if ( ++v9 >= rvBSEManagerLocal::mTraceModels.num )
                    break;
                  v1 = rvBSEManagerLocal::mTraceModels.list;
                }
              }
              operator delete(v8);
              v1 = rvBSEManagerLocal::mTraceModels.list;
            }
            v2 = rvBSEManagerLocal::mTraceModels.num;
          }
        }
        else
        {
          operator delete(v1);
          v1 = 0;
          v2 = 0;
          rvBSEManagerLocal::mTraceModels.list = 0;
          rvBSEManagerLocal::mTraceModels.num = 0;
          rvBSEManagerLocal::mTraceModels.size = 0;
        }
        goto LABEL_29;
      }
      rvBSEManagerLocal::mTraceModels.size = rvBSEManagerLocal::mTraceModels.granularity;
      if ( rvBSEManagerLocal::mTraceModels.granularity < rvBSEManagerLocal::mTraceModels.num )
        rvBSEManagerLocal::mTraceModels.num = rvBSEManagerLocal::mTraceModels.granularity;
      v6 = (idTraceModel **)operator new(4 * rvBSEManagerLocal::mTraceModels.granularity);
      v4 = rvBSEManagerLocal::mTraceModels.granularity;
      v2 = rvBSEManagerLocal::mTraceModels.num;
      v1 = v6;
      rvBSEManagerLocal::mTraceModels.list = v6;
    }
    v5 = rvBSEManagerLocal::mTraceModels.size;
    goto LABEL_14;
  }
  rvBSEManagerLocal::mTraceModels.list[result] = model;
  return result;
}
void __thiscall idBlockAlloc<rvBSE,256,0>::element_t::element_t(idBlockAlloc<rvBSE,256,0>::element_t *this)
{
  this->t.vfptr = (rvBSEVtbl *)&rvBSE::`vftable';
  this->t.mSegments.granularity = 16;
  this->t.mSegments.list = 0;
  this->t.mSegments.num = 0;
  this->t.mSegments.size = 0;
  this->t.mFlags = 0;
}
void __thiscall idBlockAlloc<rvBSE,256,0>::element_t::~element_t(idBlockAlloc<rvBSE,256,0>::element_t *this)
{
  rvBSE::~rvBSE(&this->t);
}
idBlockAlloc<rvBSE,256,0>::element_t *__thiscall idBlockAlloc<rvBSE,256,0>::Alloc(idBlockAlloc<rvBSE,256,0> *this)
{
  idBlockAlloc<rvBSE,256,0> *v1; // esi
  idBlockAlloc<rvBSE,256,0>::block_t *v2; // eax
  idBlockAlloc<rvBSE,256,0>::block_t *v3; // edi
  idBlockAlloc<rvBSE,256,0>::block_t *v4; // eax
  idBlockAlloc<rvBSE,256,0>::element_t **v5; // eax
  signed int v6; // ecx
  int v7; // edx
  idBlockAlloc<rvBSE,256,0>::element_t *result; // eax

  v1 = this;
  if ( !this->free )
  {
    v2 = (idBlockAlloc<rvBSE,256,0>::block_t *)operator new(0x1FC04u);
    v3 = v2;
    if ( v2 )
    {
      `eh vector constructor iterator'(
        v2,
        0x1FCu,
        256,
        (void (__thiscall *)(void *))idBlockAlloc<rvBSE,256,0>::element_t::element_t,
        (void (__thiscall *)(void *))idBlockAlloc<rvBSE,256,0>::element_t::~element_t);
      v4 = v3;
    }
    else
    {
      v4 = 0;
    }
    v4->next = v1->blocks;
    v1->blocks = v4;
    v5 = &v4->elements[1].next;
    v6 = 64;
    do
    {
      *(v5 - 127) = v1->free;
      *v5 = (idBlockAlloc<rvBSE,256,0>::element_t *)(v5 - 253);
      v5[127] = (idBlockAlloc<rvBSE,256,0>::element_t *)(v5 - 126);
      v5[254] = (idBlockAlloc<rvBSE,256,0>::element_t *)(v5 + 1);
      v7 = (int)(v5 + 128);
      v5 += 508;
      --v6;
      v1->free = (idBlockAlloc<rvBSE,256,0>::element_t *)v7;
    }
    while ( v6 );
    v1->total += 256;
  }
  result = v1->free;
  ++v1->active;
  v1->free = result->next;
  result->next = 0;
  return result;
}
char rvBSEManagerLocal::Init()
{
  sdDeclTypeHolder *v0; // eax
  sdDeclTypeHolder *v1; // eax
  sdDeclTypeHolder *v2; // eax

  (*(void (__cdecl **)(netadrtype_t, const char *))(*(_DWORD *)common.type + 52))(
    common.type,
    "----------------- BSE Init ------------------\n");
  v0 = sdSingleton<sdDeclTypeHolder>::GetInstance();
  ((void (__stdcall *)(int, const char *, signed int))declManager->vfptr->FindType)(
    v0->declEffectsType.declTypeHandle,
    "_default",
    1);
  v1 = sdSingleton<sdDeclTypeHolder>::GetInstance();
  ((void (__stdcall *)(int, const char *, signed int))declManager->vfptr->FindType)(
    v1->declMaterialType.declTypeHandle,
    "_default",
    1);
  v2 = sdSingleton<sdDeclTypeHolder>::GetInstance();
  ((void (__stdcall *)(int, const char *, signed int))declManager->vfptr->FindType)(
    v2->declTableType.declTypeHandle,
    "halfsintable",
    1);
  ((void (__stdcall *)(const char *))renderModelManager->vfptr->FindModel)("_default");
  ((void (__stdcall *)(const char *, void (__cdecl *)(idCmdArgs *), _DWORD, const char *, _DWORD))cmdSystem->vfptr->AddCommand)(
    "bseStats",
    BSE_Stats_f,
    0,
    "Dumps the stats of every registered effect - use all to force parse every effect",
    0);
  ((void (__stdcall *)(const char *, void (__usercall *)(int@<esi>), _DWORD, const char *, _DWORD))cmdSystem->vfptr->AddCommand)(
    "bseLog",
    BSE_Log_f,
    0,
    "Dumps the number of times an effect has been played since game start",
    0);
  ((void (__stdcall *)(const char *, void (__cdecl *)(), _DWORD, const char *, _DWORD))cmdSystem->vfptr->AddCommand)(
    "bsePause",
    BSE_Pause_f,
    0,
    "Use to pause all effects at the current time",
    0);
  (*(void (__cdecl **)(netadrtype_t, const char *))(*(_DWORD *)common.type + 52))(
    common.type,
    "--------- BSE Created Successfully ----------\n");
  return 1;
}
char __thiscall rvBSEManagerLocal::PlayEffect(rvBSEManagerLocal *this, rvRenderEffectLocal *def, float time)
{
  rvDeclEffect *v3; // esi
  rvBSEManagerLocal *v4; // edi
  int v5; // eax
  int (*v7)(void); // eax
  int v8; // eax
  idBlockAlloc<rvBSE,256,0>::element_t *v9; // eax

  v3 = def->parms.declEffect;
  v4 = this;
  v5 = (*(int (**)(void))v3->base->vfptr->gap4)();
  if ( v4->vfptr->Filtered((rvBSEManager *)&v4->vfptr, (const char *)v5, 0) )
    return 0;
  if ( bse_debug.internalVar->integerValue )
  {
    v7 = *(int (**)(void))v3->base->vfptr->gap4;
    ++count;
    v8 = v7();
    (*(void (**)(netadrtype_t, const char *, ...))(*(_DWORD *)common.type + 52))(
      common.type,
      "Playing effect %d: %s at %g\n",
      count,
      v8,
      time);
  }
  ++v3->mPlayCount;
  v9 = idBlockAlloc<rvBSE,256,0>::Alloc(&rvBSEManagerLocal::effects);
  def->effect = &v9->t;
  rvBSE::Init(&v9->t, v3, &def->parms, time);
  return 1;
}
void __thiscall idBlockAlloc<rvBSE,256,0>::Shutdown(idBlockAlloc<rvBSE,256,0> *this)
{
  idBlockAlloc<rvBSE,256,0> *v1; // edi
  idBlockAlloc<rvBSE,256,0>::block_t *v2; // esi
  bool v3; // zf

  v1 = this;
  while ( v1->blocks )
  {
    v2 = v1->blocks;
    v3 = v1->blocks == 0;
    v1->blocks = v1->blocks->next;
    if ( !v3 )
    {
      `eh vector destructor iterator'(
        v2,
        0x1FCu,
        256,
        (void (__thiscall *)(void *))idBlockAlloc<rvBSE,256,0>::element_t::~element_t);
      operator delete(v2);
    }
  }
  v1->blocks = 0;
  v1->free = 0;
  v1->active = 0;
  v1->total = 0;
}
char rvBSEManagerLocal::Shutdown()
{
  int i; // esi

  (*(void (__cdecl **)(netadrtype_t, const char *))(*(_DWORD *)common.type + 52))(
    common.type,
    "--------------- BSE Shutdown ----------------\n");
  rvParticleTemplate::ShutdownStatic();
  for ( i = 0; i < rvBSEManagerLocal::mTraceModels.num; ++i )
  {
    operator delete(rvBSEManagerLocal::mTraceModels.list[i]);
    rvBSEManagerLocal::mTraceModels.list[i] = 0;
  }
  operator delete(rvBSEManagerLocal::mTraceModels.list);
  rvBSEManagerLocal::mTraceModels.list = 0;
  rvBSEManagerLocal::mTraceModels.num = 0;
  rvBSEManagerLocal::mTraceModels.size = 0;
  idBlockAlloc<rvBSE,256,0>::Shutdown(&rvBSEManagerLocal::effects);
  rvBSEManagerLocal::mEffectRates[0] = 0.0;
  dword_11F4D44 = 0;
  dword_11F4D48 = 0;
  (*(void (__cdecl **)(netadrtype_t, const char *))(*(_DWORD *)common.type + 52))(
    common.type,
    "---------------------------------------------\n");
  return 1;
}
int dynamic_initializer_for__singletonLock___94()
{
  sdLock::sdLock(&singletonLock_94);
  return atexit(dynamic_atexit_destructor_for__singletonLock___94);
}
int dynamic_initializer_for__bseLocal__()
{
  return atexit(dynamic_atexit_destructor_for__bseLocal__);
}
int dynamic_initializer_for__bse_enabled__()
{
  if ( idCVar::staticVars == (idCVar *)15 )
  {
    ((void (__stdcall *)(idCVar *))cvarSystem->vfptr->Register)(&bse_enabled);
  }
  else
  {
    bse_enabled.next = idCVar::staticVars;
    idCVar::staticVars = &bse_enabled;
  }
  return atexit(dynamic_atexit_destructor_for__bse_enabled__);
}
int dynamic_initializer_for__bse_render__()
{
  if ( idCVar::staticVars == (idCVar *)15 )
  {
    ((void (__stdcall *)(idCVar *))cvarSystem->vfptr->Register)(&bse_render);
  }
  else
  {
    bse_render.next = idCVar::staticVars;
    idCVar::staticVars = &bse_render;
  }
  return atexit(dynamic_atexit_destructor_for__bse_render__);
}
int dynamic_initializer_for__bse_debug__()
{
  if ( idCVar::staticVars == (idCVar *)15 )
  {
    ((void (__stdcall *)(idCVar *))cvarSystem->vfptr->Register)(&bse_debug);
  }
  else
  {
    bse_debug.next = idCVar::staticVars;
    idCVar::staticVars = &bse_debug;
  }
  return atexit(dynamic_atexit_destructor_for__bse_debug__);
}
int dynamic_initializer_for__bse_showBounds__()
{
  if ( idCVar::staticVars == (idCVar *)15 )
  {
    ((void (__stdcall *)(idCVar *))cvarSystem->vfptr->Register)(&bse_showBounds);
  }
  else
  {
    bse_showBounds.next = idCVar::staticVars;
    idCVar::staticVars = &bse_showBounds;
  }
  return atexit(dynamic_atexit_destructor_for__bse_showBounds__);
}
int dynamic_initializer_for__bse_physics__()
{
  if ( idCVar::staticVars == (idCVar *)15 )
  {
    ((void (__stdcall *)(idCVar *))cvarSystem->vfptr->Register)(&bse_physics);
  }
  else
  {
    bse_physics.next = idCVar::staticVars;
    idCVar::staticVars = &bse_physics;
  }
  return atexit(dynamic_atexit_destructor_for__bse_physics__);
}
int dynamic_initializer_for__bse_debris__()
{
  if ( idCVar::staticVars == (idCVar *)15 )
  {
    ((void (__stdcall *)(idCVar *))cvarSystem->vfptr->Register)(&bse_debris);
  }
  else
  {
    bse_debris.next = idCVar::staticVars;
    idCVar::staticVars = &bse_debris;
  }
  return atexit(dynamic_atexit_destructor_for__bse_debris__);
}
int dynamic_initializer_for__bse_singleEffect__()
{
  if ( idCVar::staticVars == (idCVar *)15 )
  {
    ((void (__stdcall *)(idCVar *))cvarSystem->vfptr->Register)(&bse_singleEffect);
  }
  else
  {
    bse_singleEffect.next = idCVar::staticVars;
    idCVar::staticVars = &bse_singleEffect;
  }
  return atexit(dynamic_atexit_destructor_for__bse_singleEffect__);
}
int dynamic_initializer_for__bse_speeds__()
{
  if ( idCVar::staticVars == (idCVar *)15 )
  {
    ((void (__stdcall *)(idCVar *))cvarSystem->vfptr->Register)(&bse_speeds);
  }
  else
  {
    bse_speeds.next = idCVar::staticVars;
    idCVar::staticVars = &bse_speeds;
  }
  return atexit(dynamic_atexit_destructor_for__bse_speeds__);
}
int dynamic_initializer_for__bse_detailLevel__()
{
  if ( idCVar::staticVars == (idCVar *)15 )
  {
    ((void (__stdcall *)(idCVar *))cvarSystem->vfptr->Register)(&bse_detailLevel);
  }
  else
  {
    bse_detailLevel.next = idCVar::staticVars;
    idCVar::staticVars = &bse_detailLevel;
  }
  return atexit(dynamic_atexit_destructor_for__bse_detailLevel__);
}
int dynamic_initializer_for__bse_rateLimit__()
{
  if ( idCVar::staticVars == (idCVar *)15 )
  {
    ((void (__stdcall *)(idCVar *))cvarSystem->vfptr->Register)(&bse_rateLimit);
  }
  else
  {
    bse_rateLimit.next = idCVar::staticVars;
    idCVar::staticVars = &bse_rateLimit;
  }
  return atexit(dynamic_atexit_destructor_for__bse_rateLimit__);
}
int dynamic_initializer_for__bse_rateCost__()
{
  if ( idCVar::staticVars == (idCVar *)15 )
  {
    ((void (__stdcall *)(idCVar *))cvarSystem->vfptr->Register)(&bse_rateCost);
  }
  else
  {
    bse_rateCost.next = idCVar::staticVars;
    idCVar::staticVars = &bse_rateCost;
  }
  return atexit(dynamic_atexit_destructor_for__bse_rateCost__);
}
int dynamic_initializer_for__bse_simple__()
{
  if ( idCVar::staticVars == (idCVar *)15 )
  {
    ((void (__stdcall *)(idCVar *))cvarSystem->vfptr->Register)(&bse_simple);
  }
  else
  {
    bse_simple.next = idCVar::staticVars;
    idCVar::staticVars = &bse_simple;
  }
  return atexit(dynamic_atexit_destructor_for__bse_simple__);
}
int dynamic_initializer_for__rvBSEManagerLocal::effects__()
{
  sdLock::sdLock(&stru_11F4D70);
  rvBSEManagerLocal::effects.blocks = 0;
  unk_11F4D64 = 0;
  unk_11F4D6C = 0;
  unk_11F4D68 = 0;
  return atexit(dynamic_atexit_destructor_for__rvBSEManagerLocal::effects__);
}
int dynamic_initializer_for__rvBSEManagerLocal::mTraceModels__()
{
  operator delete(0);
  rvBSEManagerLocal::mTraceModels.list = 0;
  rvBSEManagerLocal::mTraceModels.num = 0;
  rvBSEManagerLocal::mTraceModels.size = 0;
  return atexit(dynamic_atexit_destructor_for__rvBSEManagerLocal::mTraceModels__);
}
void __cdecl dynamic_atexit_destructor_for__bseLocal__()
{
  bseLocal.vfptr = (rvBSEManagerVtbl *)&rvBSEManager::`vftable';
}
void __cdecl dynamic_atexit_destructor_for__bse_enabled__()
{
  bse_enabled.vfptr = (idCVarVtbl *)&idCVar::`vftable';
}
void __cdecl dynamic_atexit_destructor_for__bse_render__()
{
  bse_render.vfptr = (idCVarVtbl *)&idCVar::`vftable';
}
void __cdecl dynamic_atexit_destructor_for__bse_debug__()
{
  bse_debug.vfptr = (idCVarVtbl *)&idCVar::`vftable';
}
void __cdecl dynamic_atexit_destructor_for__bse_showBounds__()
{
  bse_showBounds.vfptr = (idCVarVtbl *)&idCVar::`vftable';
}
void __cdecl dynamic_atexit_destructor_for__bse_physics__()
{
  bse_physics.vfptr = (idCVarVtbl *)&idCVar::`vftable';
}
void __cdecl dynamic_atexit_destructor_for__bse_debris__()
{
  bse_debris.vfptr = (idCVarVtbl *)&idCVar::`vftable';
}
void __cdecl dynamic_atexit_destructor_for__bse_singleEffect__()
{
  bse_singleEffect.vfptr = (idCVarVtbl *)&idCVar::`vftable';
}
void __cdecl dynamic_atexit_destructor_for__bse_speeds__()
{
  bse_speeds.vfptr = (idCVarVtbl *)&idCVar::`vftable';
}
void __cdecl dynamic_atexit_destructor_for__bse_detailLevel__()
{
  bse_detailLevel.vfptr = (idCVarVtbl *)&idCVar::`vftable';
}
void __cdecl dynamic_atexit_destructor_for__bse_rateLimit__()
{
  bse_rateLimit.vfptr = (idCVarVtbl *)&idCVar::`vftable';
}
void __cdecl dynamic_atexit_destructor_for__bse_rateCost__()
{
  bse_rateCost.vfptr = (idCVarVtbl *)&idCVar::`vftable';
}
void __cdecl dynamic_atexit_destructor_for__bse_simple__()
{
  bse_simple.vfptr = (idCVarVtbl *)&idCVar::`vftable';
}
void __cdecl dynamic_atexit_destructor_for__singletonLock___94()
{
  sdLock::~sdLock(&singletonLock_94);
}
void __cdecl dynamic_atexit_destructor_for__rvBSEManagerLocal::mTraceModels__()
{
  operator delete(rvBSEManagerLocal::mTraceModels.list);
  rvBSEManagerLocal::mTraceModels.list = 0;
  rvBSEManagerLocal::mTraceModels.num = 0;
  rvBSEManagerLocal::mTraceModels.size = 0;
}
void __cdecl dynamic_atexit_destructor_for__rvBSEManagerLocal::effects__()
{
  idBlockAlloc<rvBSE,256,0>::Shutdown(&rvBSEManagerLocal::effects);
  sdLock::~sdLock(&stru_11F4D70);
}
