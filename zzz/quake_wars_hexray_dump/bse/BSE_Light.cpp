char __thiscall rvLightParticle::Destroy(rvLightParticle *this)
{
  rvLightParticle *v1; // esi
  int v2; // eax

  v1 = this;
  v2 = this->mLightDefHandle;
  if ( v2 != -1 )
  {
    (*(void (__stdcall **)(int))&session->rw->vfptr->gap4[36])(v2);
    v1->mLightDefHandle = -1;
  }
  return 1;
}
void __thiscall rvSegment::InitLight(rvSegment *this, rvBSE *effect, rvSegmentTemplate *st, float time)
{
  rvSegment *v4; // esi

  v4 = this;
  if ( !this->mUsedHead )
  {
    rvSegment::SpawnParticle(this, effect, st, time, &vec3_origin, &mat3_identity);
    ((void (__stdcall *)(rvBSE *, rvSegmentTemplate *, _DWORD))v4->mUsedHead->vfptr->InitLight)(
      effect,
      st,
      LODWORD(time));
  }
}
char __userpurge rvLightParticle::InitLight@<al>(rvLightParticle *this@<ecx>, int a2@<edi>, rvBSE *effect, rvSegmentTemplate *st, float time, float a6)
{
  rvLightParticle *v6; // ebx
  char result; // al
  rvSegmentTemplate *v8; // ebp
  rvEnvParms *v9; // esi
  rvEnvParms *v10; // ecx
  rvParticleTemplate *v11; // ebp
  float *v12; // esi
  double v13; // st7
  idMaterial *v14; // ecx
  renderLight_t::renderLightFlags_t v15; // al
  float v16; // [esp+24h] [ebp-48h]
  float v17; // [esp+28h] [ebp-44h]
  float v18; // [esp+30h] [ebp-3Ch]
  float v19; // [esp+34h] [ebp-38h]
  float v20; // [esp+38h] [ebp-34h]
  float v21; // [esp+3Ch] [ebp-30h]
  idVec3 position; // [esp+40h] [ebp-2Ch]
  idVec3 size; // [esp+4Ch] [ebp-20h]
  idVec4 tint; // [esp+58h] [ebp-14h]
  float dest; // [esp+68h] [ebp-4h]
  float oodurationa; // [esp+74h] [ebp+8h]
  float ooduration; // [esp+74h] [ebp+8h]
  float v28; // [esp+7Ch] [ebp+10h]

  v6 = this;
  result = rvParticle::GetEvaluationTime((rvParticle *)&this->vfptr, time, &v16, 0);
  if ( result )
  {
    memset(&v6->mLight, 0, 0x120u);
    v8 = st;
    v9 = st->mParticleTemplate.mpFadeEnvelope;
    oodurationa = v6->mEndTime - v6->mStartTime;
    v10 = v8->mParticleTemplate.mpTintEnvelope;
    v11 = &v8->mParticleTemplate;
    ooduration = 1.0 / oodurationa;
    v17 = v16;
    rvEnvParms::Evaluate(v10, &v6->mTintEnv, v16, ooduration, &tint.y);
    rvEnvParms::Evaluate(v9, &v6->mFadeEnv, v17, ooduration, &dest);
    (*(void (__thiscall **)(rvLightParticle *, rvEnvParms *, _DWORD, float, float *, int))&v6->vfptr->gap4[8])(
      v6,
      v11->mpSizeEnvelope,
      LODWORD(v16),
      COERCE_FLOAT(LODWORD(ooduration)),
      &size.y,
      a2);
    v12 = (float *)LODWORD(ooduration);
    v28 = a6 - v6->mMotionStartTime;
    rvParticle::EvaluatePosition(
      (rvParticle *)&v6->vfptr,
      (rvBSE *)LODWORD(ooduration),
      v11,
      (idVec3 *)((char *)&position + 8),
      v28);
    v18 = v12[15] * size.y + v12[9] * position.z + v12[12] * size.x;
    v19 = v12[13] * size.x + v12[10] * position.z + v12[16] * size.y;
    v20 = position.z * v12[11] + size.x * v12[14] + size.y * v12[17];
    v21 = v18 + *(float *)(LODWORD(ooduration) + 12);
    position.x = *(float *)(LODWORD(ooduration) + 16) + v19;
    position.y = *(float *)(LODWORD(ooduration) + 20) + v20;
    v6->mLight.origin.x = v21;
    v6->mLight.origin.y = position.x;
    v6->mLight.origin.z = position.y;
    v6->mLight.lightRadius.x = size.z;
    v6->mLight.lightRadius.y = tint.x;
    v6->mLight.lightRadius.z = tint.y;
    if ( v6->mLight.lightRadius.x < 1.0 )
      v6->mLight.lightRadius.x = 1.0;
    if ( v6->mLight.lightRadius.y < 1.0 )
      v6->mLight.lightRadius.y = 1.0;
    if ( v6->mLight.lightRadius.z < 1.0 )
      v6->mLight.lightRadius.z = 1.0;
    v13 = tint.z;
    qmemcpy(&v6->mLight, (const void *)(LODWORD(ooduration) + 300), 0x24u);
    v6->mLight.shaderParms[0] = v13;
    v6->mLight.shaderParms[1] = tint.w;
    v6->mLight.shaderParms[2] = dest;
    v6->mLight.maxVisDist = 4096;
    v14 = v11->mMaterial;
    *(_BYTE *)&v6->mLight.flags |= 4u;
    v6->mLight.material = v14;
    v15 = (renderLight_t::renderLightFlags_t)(*(_BYTE *)&v6->mLight.flags ^ (*(_BYTE *)&v6->mLight.flags ^ ~(unsigned __int8)((unsigned int)v11->mFlags >> 17)) & 1);
    v6->mLight.flags = v15;
    v6->mLight.flags = (renderLight_t::renderLightFlags_t)(*(_BYTE *)&v15 ^ (*(_BYTE *)&v15 ^ ~(2
                                                                                              * ((unsigned int)v11->mFlags >> 18))) & 2);
    v6->mLight.manualPriority = 1;
    v6->mLight.lightId = LODWORD(ooduration) + 300;
    v6->mLightDefHandle = (*(int (__cdecl **)(renderLight_t *))&session->rw->vfptr->gap4[28])(&v6->mLight);
    result = 1;
  }
  return result;
}
char __userpurge rvLightParticle::PresentLight@<al>(rvLightParticle *this@<ecx>, int a2@<edi>, int a3@<esi>, rvBSE *effect, rvParticleTemplate *pt, float time, bool infinite, float a8)
{
  rvLightParticle *v8; // ebx
  char result; // al
  int v10; // ST18_4
  rvParticleTemplate *v11; // esi
  rvEnvParms *v12; // ecx
  int v13; // ST14_4
  rvEnvParms *v14; // edi
  float v15; // ebp
  double v16; // st7
  float oneOverDuration; // [esp+24h] [ebp-44h]
  float v18; // [esp+30h] [ebp-38h]
  float v19; // [esp+34h] [ebp-34h]
  float v20; // [esp+38h] [ebp-30h]
  idVec3 position; // [esp+3Ch] [ebp-2Ch]
  idVec3 size; // [esp+48h] [ebp-20h]
  idVec4 tint; // [esp+54h] [ebp-14h]
  float dest; // [esp+64h] [ebp-4h]
  float retaddr; // [esp+68h] [ebp+0h]
  float pta; // [esp+70h] [ebp+8h]
  float ooduration; // [esp+78h] [ebp+10h]
  float oodurationa; // [esp+78h] [ebp+10h]
  float v29; // [esp+7Ch] [ebp+14h]

  v8 = this;
  result = rvParticle::GetEvaluationTime((rvParticle *)&this->vfptr, time, &oneOverDuration, infinite);
  if ( result )
  {
    v10 = a3;
    v11 = pt;
    v12 = pt->mpTintEnvelope;
    ooduration = v8->mEndTime - v8->mStartTime;
    v13 = a2;
    v14 = pt->mpFadeEnvelope;
    oodurationa = 1.0 / ooduration;
    pta = oneOverDuration;
    rvEnvParms::Evaluate(v12, &v8->mTintEnv, oodurationa, oneOverDuration, &tint.y);
    rvEnvParms::Evaluate(v14, &v8->mFadeEnv, oodurationa, pta, &dest);
    (*(void (__thiscall **)(rvLightParticle *, rvEnvParms *, _DWORD, _DWORD, float *, int, int))&v8->vfptr->gap4[8])(
      v8,
      v11->mpSizeEnvelope,
      LODWORD(oneOverDuration),
      LODWORD(oodurationa),
      &size.y,
      v13,
      v10);
    v15 = time;
    v29 = a8 - v8->mMotionStartTime;
    rvParticle::EvaluatePosition((rvParticle *)&v8->vfptr, (rvBSE *)LODWORD(time), v11, &size, v29);
    v18 = *(float *)(LODWORD(v15) + 324) * size.z
        + *(float *)(LODWORD(v15) + 300) * size.x
        + *(float *)(LODWORD(v15) + 312) * size.y;
    v19 = *(float *)(LODWORD(v15) + 316) * size.y
        + *(float *)(LODWORD(v15) + 304) * size.x
        + *(float *)(LODWORD(v15) + 328) * size.z;
    v20 = size.x * *(float *)(LODWORD(v15) + 308)
        + size.y * *(float *)(LODWORD(v15) + 320)
        + size.z * *(float *)(LODWORD(v15) + 332);
    position.x = v18 + *(float *)(LODWORD(time) + 264);
    position.y = *(float *)(LODWORD(time) + 268) + v19;
    position.z = *(float *)(LODWORD(time) + 272) + v20;
    v8->mLight.origin = position;
    v8->mLight.lightRadius.x = tint.x;
    v8->mLight.lightRadius.y = tint.y;
    v8->mLight.lightRadius.z = tint.z;
    if ( v8->mLight.lightRadius.x < 1.0 )
      v8->mLight.lightRadius.x = 1.0;
    if ( v8->mLight.lightRadius.y < 1.0 )
      v8->mLight.lightRadius.y = 1.0;
    if ( v8->mLight.lightRadius.z < 1.0 )
      v8->mLight.lightRadius.z = 1.0;
    v16 = tint.w;
    qmemcpy(&v8->mLight, (const void *)(LODWORD(time) + 300), 0x24u);
    v8->mLight.shaderParms[0] = v16;
    v8->mLight.shaderParms[1] = dest;
    v8->mLight.shaderParms[2] = retaddr;
    v8->mLight.suppressLightInViewID = *(_WORD *)(LODWORD(time) + 136);
    (*(void (__cdecl **)(int, renderLight_t *))&session->rw->vfptr->gap4[32])(v8->mLightDefHandle, &v8->mLight);
    result = 1;
  }
  return result;
}
char __thiscall rvSegment::HandleLight(rvSegment *this, rvBSE *effect, rvSegmentTemplate *st, float time)
{
  rvSegment *v4; // esi
  rvParticle *v5; // ecx
  double v6; // st7
  float *v7; // ecx
  float timea; // [esp+1Ch] [ebp+Ch]

  v4 = this;
  v5 = this->mUsedHead;
  if ( !v5 )
    return 0;
  ((void (__stdcall *)(rvBSE *, rvParticleTemplate *, _DWORD, unsigned int))v5->vfptr->PresentLight)(
    effect,
    &st->mParticleTemplate,
    LODWORD(time),
    ((unsigned int)st->mFlags >> 5) & 0xFFFFFF01);
  if ( ((unsigned int)st->mFlags >> 5) & 1 )
    return 0;
  v6 = time;
  v7 = (float *)v4->mUsedHead;
  timea = v7[6] - 0.002000000094994903;
  if ( timea > v6 )
    return 0;
  (*(void (**)(void))(*(_DWORD *)v7 + 120))();
  v4->mFreeHead = v4->mUsedHead;
  v4->mUsedHead = 0;
  return 1;
}
int dynamic_initializer_for__singletonLock___117()
{
  sdLock::sdLock(&singletonLock_117);
  return atexit(dynamic_atexit_destructor_for__singletonLock___117);
}
void __cdecl dynamic_atexit_destructor_for__singletonLock___117()
{
  sdLock::~sdLock(&singletonLock_117);
}
