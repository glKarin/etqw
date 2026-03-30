void __stdcall rvDecalParticle::InitSizeEnv(idJointMat *joints, const int numJoints)
{
  ;
}
int __stdcall rvParticle::Update(rvParticleTemplate *pt, float time)
{
  return 1;
}
bool __stdcall rvParticle::Render(rvBSE *effect, rvParticleTemplate *pt, idMat3 *view, srfTriangles_t *tri, float time, float override)
{
  return 0;
}
void __stdcall rvParticle::RenderBurnTrail(rvBSE *effect, rvParticleTemplate *pt, idMat3 *view, srfTriangles_t *tri, float time)
{
  ;
}
bool __stdcall rvParticle::PresentLight(rvBSE *effect, rvParticleTemplate *pt, float time, bool infinite)
{
  return 0;
}
void __stdcall idCVar::InternalSetString(idVec4 *tint, idVec3 *size, idVec3 *rotate)
{
  ;
}
rvParticle *__thiscall rvParticle::`vector deleting destructor'(rvParticle *this, unsigned int a2)
{
  rvParticle *v2; // esi

  v2 = this;
  this->vfptr = (rvParticleVtbl *)&rvParticle::`vftable';
  if ( a2 & 1 )
    operator delete(this);
  return v2;
}
void __thiscall rvLightParticle::~rvLightParticle(rvLightParticle *this)
{
  rvLightParticle *v1; // esi

  v1 = this;
  this->vfptr = (rvParticleVtbl *)&rvLightParticle::`vftable';
  rvLightParticle::Destroy(this);
  v1->vfptr = (rvParticleVtbl *)&rvParticle::`vftable';
}
void __thiscall rvDebrisParticle::EvaluateRotation(rvDebrisParticle *this, rvEnvParms *rotation, const float time, float oneOverDuration, float *dest)
{
  rvEnvParms::Evaluate(rotation, &this->mRotationEnv, time, oneOverDuration, dest);
}
void __stdcall rvDebrisParticle::InitRotationEnv(rvEnvParms *env, float duration)
{
  rvDecalParticle::InitSizeEnv((idJointMat *)env, SLODWORD(duration));
}
double __thiscall rvSegmentTemplate::GetSoundVolume(rvSegmentTemplate *this)
{
  return rvRandom::flrand(this->mSoundVolume.x, this->mSoundVolume.y);
}
double __thiscall rvSegmentTemplate::GetFreqShift(rvSegmentTemplate *this)
{
  return rvRandom::flrand(this->mFreqShift.x, this->mFreqShift.y);
}
BOOL __thiscall rvBSE::CanInterpolate(rvBSE *this)
{
  return this->mCurrentTime - this->mLastTime > 0.002000000094994903;
}
void __thiscall rvSegment::ValidateSpawnRates(rvSegment *this)
{
  double v1; // st6
  double v2; // st7
  double v3; // st6
  float v4; // ST00_4
  float v5; // ST00_4
  float v6; // [esp+0h] [ebp-4h]
  float v7; // [esp+0h] [ebp-4h]

  v1 = this->mSecondsPerParticle.y;
  if ( v1 >= 0.0020000001 )
  {
    v2 = 300.0;
    if ( v1 > 300.0 )
    {
      v6 = 300.0;
      goto LABEL_4;
    }
  }
  else
  {
    v1 = 0.0020000001;
    v2 = 300.0;
  }
  v6 = v1;
LABEL_4:
  v3 = v6;
  this->mSecondsPerParticle.y = v6;
  v7 = this->mSecondsPerParticle.x;
  if ( v7 < v3 || (v3 = v7, v7 <= v2) )
  {
    v4 = v3;
    this->mSecondsPerParticle.x = v4;
  }
  else
  {
    v5 = v2;
    this->mSecondsPerParticle.x = v5;
  }
}
void __thiscall rvSegment::GetSecondsPerParticle(rvSegment *this, rvBSE *effect, rvSegmentTemplate *st, rvParticleTemplate *pt)
{
  double v4; // st7
  rvSegmentTemplate *v5; // edi
  rvSegment *v6; // esi
  double v7; // st7
  int v8; // edi
  float volume; // [esp+10h] [ebp+8h]
  float volumea; // [esp+10h] [ebp+8h]

  v4 = 0.0;
  v5 = st;
  v6 = this;
  if ( 0.0 == st->mDensity.y )
  {
    this->mCount.x = st->mCount.x;
    this->mCount.y = st->mCount.y;
  }
  else
  {
    volume = rvParticleTemplate::GetSpawnVolume(pt, effect);
    v7 = 0.0020000001;
    if ( volume >= 0.0020000001 )
    {
      v7 = volume;
      if ( volume > 2048.0 )
        v7 = 2048.0;
    }
    volumea = v7;
    v6->mCount.x = v5->mDensity.x * volumea;
    v6->mCount.y = volumea * v5->mDensity.y;
    v4 = 0.0;
  }
  v8 = v5->mSegType;
  if ( v8 == 2 || v8 == 4 )
  {
    if ( v4 != v6->mCount.x )
      v6->mSecondsPerParticle.x = 1.0 / v6->mCount.x;
    if ( v4 != v6->mCount.y )
      v6->mSecondsPerParticle.y = 1.0 / v6->mCount.y;
    rvSegment::ValidateSpawnRates(v6);
  }
}
void __thiscall rvSegment::InitTime(rvSegment *this, rvBSE *effect, rvSegmentTemplate *st, float time)
{
  rvSegmentTemplate *v4; // edi
  rvSegment *v5; // esi
  float sta; // [esp+20h] [ebp+8h]
  float timea; // [esp+24h] [ebp+Ch]

  v4 = st;
  v5 = this;
  v5->mFlags &= 0xFFFFFFFE;
  this->mSegStartTime = rvRandom::flrand(st->mLocalStartTime.x, st->mLocalStartTime.y) + time;
  sta = rvRandom::flrand(v4->mLocalDuration.x, v4->mLocalDuration.y);
  v5->mSegEndTime = v5->mSegStartTime + sta;
  if ( !(((unsigned int)v4->mFlags >> 4) & 1) || !(effect->mFlags & 1) && !rvSegmentTemplate::GetSoundLooping(v4) )
  {
    timea = v5->mSegEndTime - time;
    rvBSE::SetDuration(effect, timea);
  }
}
void __thiscall rvLineParticle::~rvLineParticle(rvDebrisParticle *this)
{
  this->vfptr = (rvParticleVtbl *)&rvParticle::`vftable';
}
void *__thiscall rvLightParticle::`vector deleting destructor'(rvLightParticle *this, unsigned int a2)
{
  rvLightParticle *v2; // esi
  void *result; // eax

  v2 = this;
  if ( a2 & 2 )
  {
    `eh vector destructor iterator'(
      this,
      0x224u,
      (int)this[-1].mLight.atmosLightProjection,
      (void (__thiscall *)(void *))rvLightParticle::~rvLightParticle);
    if ( a2 & 1 )
      operator delete(&v2[-1].mLight.atmosLightProjection);
    result = &v2[-1].mLight.atmosLightProjection;
  }
  else
  {
    rvLightParticle::~rvLightParticle(this);
    if ( a2 & 1 )
      operator delete(v2);
    result = v2;
  }
  return result;
}
double __thiscall rvSegment::AttenuateDuration(rvSegment *this, rvBSE *effect, rvSegmentTemplate *st)
{
  return (float)(rvBSE::GetAttenuation(effect, st) * (this->mSegEndTime - this->mSegStartTime));
}
double __thiscall rvSegment::AttenuateInterval(rvSegment *this, rvBSE *effect, rvSegmentTemplate *st)
{
  rvSegmentTemplate *v3; // esi
  double result; // st7
  float v5; // [esp+4h] [ebp-Ch]
  float v6; // [esp+8h] [ebp-8h]
  float v7; // [esp+Ch] [ebp-4h]
  float atten; // [esp+18h] [ebp+8h]
  float attena; // [esp+18h] [ebp+8h]
  float attenb; // [esp+18h] [ebp+8h]

  v5 = (this->mSecondsPerParticle.y - this->mSecondsPerParticle.x) * bse_detailLevel.internalVar->floatValue
     + this->mSecondsPerParticle.x;
  v6 = this->mSecondsPerParticle.y;
  v7 = this->mSecondsPerParticle.x;
  if ( v6 <= (double)v5 )
  {
    if ( v7 < (double)v5 )
      v5 = v7;
  }
  else
  {
    v5 = v6;
  }
  v3 = st;
  if ( !(((unsigned int)st->mFlags >> 6) & 1) )
    return v5;
  atten = rvBSE::GetAttenuation(effect, st);
  if ( ((unsigned int)v3->mFlags >> 7) & 1 )
    atten = 1.0 - atten;
  if ( atten < 0.0020000001 )
    return 1.0;
  attena = v5 / atten;
  result = attena;
  if ( attena <= 0.00000011920929 )
  {
    attenb = 0.00000011920929 + 0.00000011920929;
    result = attenb;
  }
  return result;
}
double __stdcall rvSegment::AttenuateCount(rvBSE *effect, rvSegmentTemplate *st, float min, float max)
{
  float v4; // ST08_4
  double v5; // st7
  float v7; // [esp+8h] [ebp-4h]
  float atten; // [esp+18h] [ebp+Ch]

  v4 = (max - min) * bse_detailLevel.internalVar->floatValue + min;
  v7 = rvRandom::flrand(min, v4);
  if ( min <= (double)v7 )
  {
    if ( max < (double)v7 )
      v7 = max;
  }
  else
  {
    v7 = min;
  }
  if ( ((unsigned int)st->mFlags >> 6) & 1 )
  {
    atten = rvBSE::GetAttenuation(effect, st);
    v5 = atten;
    if ( ((unsigned int)st->mFlags >> 7) & 1 )
      v5 = 1.0 - v5;
    v7 = v5 * v7;
  }
  return v7;
}
void __thiscall rvSegment::RefreshParticles(rvSegment *this, rvBSE *effect, rvSegmentTemplate *st)
{
  rvSegment *v3; // esi
  rvParticle *v4; // ecx
  rvParticle *v5; // esi

  v3 = this;
  if ( rvParticleTemplate::UsesEndOrigin(&st->mParticleTemplate) )
  {
    v4 = v3->mUsedHead;
    if ( v4 )
    {
      do
      {
        v5 = v4->mNext;
        ((void (__stdcall *)(rvBSE *, rvSegmentTemplate *, rvParticleTemplate *))v4->vfptr->Refresh)(
          effect,
          st,
          &st->mParticleTemplate);
        v4 = v5;
      }
      while ( v5 );
    }
  }
}
void __thiscall rvParticle::DoRenderBurnTrail(rvParticle *this, rvBSE *effect, rvParticleTemplate *pt, idMat3 *view, srfTriangles_t *tri, float time)
{
  rvParticle *v6; // esi
  int v7; // ecx
  int v8; // edi
  srfTriangles_t *v9; // ebx
  double v10; // st7
  float v11; // ST10_4
  signed int delta; // [esp+18h] [ebp-8h]
  float v13; // [esp+1Ch] [ebp-4h]
  float trailTime; // [esp+30h] [ebp+10h]
  float trailTimea; // [esp+30h] [ebp+10h]

  v6 = this;
  v7 = this->mTrailCount;
  if ( v7 )
  {
    if ( 0.0 != v6->mTrailTime )
    {
      v8 = 1;
      delta = 1;
      v13 = v6->mTrailTime / (double)v7;
      if ( v7 + 1 > 1 )
      {
        v9 = tri;
        do
        {
          trailTime = time - (double)delta * v13;
          v10 = trailTime;
          if ( v6->mStartTime <= (double)trailTime && v6->mEndTime > v10 )
          {
            trailTimea = (double)(v6->mTrailCount - v8) / (double)v6->mTrailCount;
            v11 = v10;
            ((void (__thiscall *)(rvParticle *, rvBSE *, rvParticleTemplate *, idMat3 *, srfTriangles_t *, _DWORD, _DWORD))v6->vfptr->Render)(
              v6,
              effect,
              pt,
              view,
              v9,
              LODWORD(v11),
              LODWORD(trailTimea));
          }
          delta = ++v8;
        }
        while ( v8 < v6->mTrailCount + 1 );
      }
    }
  }
}
void __thiscall rvSegment::RenderMotion(rvSegment *this, rvBSE *effect, renderEffect_s *owner, idRenderModel *model, rvParticleTemplate *pt, float time)
{
  rvSegment *v6; // esi
  int v7; // eax
  rvParticle *v8; // esi
  int v9; // ebx
  modelSurface_s *surf; // [esp+28h] [ebp+Ch]

  v6 = this;
  v7 = ((int (__stdcall *)(int))model->vfptr->Surface)(this->mSurfaceIndex + 1);
  v8 = v6->mUsedHead;
  v9 = *(_DWORD *)(v7 + 8);
  surf = (modelSurface_s *)v7;
  if ( v8 )
  {
    do
    {
      ((void (__thiscall *)(rvParticle *, rvBSE *, rvParticleTemplate *, int, renderEffect_s *, _DWORD, float))v8->vfptr->RenderMotion)(
        v8,
        effect,
        pt,
        v9,
        owner,
        LODWORD(time),
        pt->mTrailInfo->mTrailScale);
      v8 = v8->mNext;
    }
    while ( v8 );
    v7 = (int)surf;
  }
  (*(void (__stdcall **)(_DWORD))&renderUtilities->vfptr->gap4[8])(*(_DWORD *)(v7 + 8));
}
CURLcode __cdecl idFile::Tell()
{
  return 0;
}
void __thiscall rvLightParticle::rvLightParticle(rvLightParticle *this)
{
  this->vfptr = (rvParticleVtbl *)&rvLightParticle::`vftable';
  this->mLightDefHandle = -1;
}
void __thiscall rvSegment::~rvSegment(rvSegment *this)
{
  rvSegment *v1; // esi
  rvParticle *v2; // ecx
  int v3; // eax

  v1 = this;
  v2 = this->mParticles;
  if ( v2 )
  {
    v3 = v1->mParticleType;
    if ( LODWORD(v2[-1].mPosition.z) )
      ((void (__stdcall *)(signed int))v2->vfptr->__vecDelDtor)(3);
    else
      operator delete(&v2[-1].mPosition.z);
    v1->mParticles = 0;
  }
}
rvSegmentTemplate *__thiscall rvSegment::GetSegmentTemplate(rvSegment *this)
{
  return &this->mEffectDecl->mSegmentTemplates.list[this->mSegmentTemplateHandle];
}
unsigned int __thiscall rvSegment::GetLocked(rvSegment *this)
{
  return ((unsigned int)this->mEffectDecl->mSegmentTemplates.list[this->mSegmentTemplateHandle].mFlags >> 1) & 1;
}
void __thiscall rvSpriteParticle::rvSpriteParticle(rvSpriteParticle *this)
{
  this->vfptr = (rvParticleVtbl *)&rvSpriteParticle::`vftable';
}
void __thiscall rvSpriteParticle::EvaluateSize(rvSpriteParticle *this, rvEnvParms *size, const float time, float oneOverDuration, float *dest)
{
  rvEnvParms::Evaluate(size, &this->mSizeEnv, time, oneOverDuration, dest);
}
void __thiscall rvSpriteParticle::EvaluateRotation(rvSpriteParticle *this, rvEnvParms *rotation, const float time, float oneOverDuration, float *dest)
{
  rvEnvParms::Evaluate(rotation, &this->mRotationEnv, time, oneOverDuration, dest);
}
void __stdcall rvSpriteParticle::InitRotationEnv(rvEnvParms *env, float duration)
{
  rvDecalParticle::InitSizeEnv((idJointMat *)env, SLODWORD(duration));
}
rvEnvParms1Particle *__thiscall rvSpriteParticle::GetInitRotation(rvSpriteParticle *this)
{
  return &this->mRotationEnv;
}
idVec3 *__thiscall rvLineParticle::GetDestLength(rvLineParticle *this)
{
  return &this->mLengthEnv.mEnd;
}
void __thiscall rvSpriteParticle::ScaleRotation(rvSpriteParticle *this, float constant)
{
  this->mRotationEnv.mStart = this->mRotationEnv.mStart * constant;
  this->mRotationEnv.mEnd = constant * this->mRotationEnv.mEnd;
}
void __thiscall rvSpriteParticle::AttenuateSize(rvSpriteParticle *this, float atten, rvParticleParms *parms)
{
  rvParticle::Attenuate(atten, parms, &this->mSizeEnv);
}
void __thiscall rvLineParticle::RenderBurnTrail(rvLineParticle *this, rvBSE *effect, rvParticleTemplate *pt, idMat3 *view, srfTriangles_t *tri, float time)
{
  rvParticle::DoRenderBurnTrail((rvParticle *)&this->vfptr, effect, pt, view, tri, time);
}
void *__thiscall rvLineParticle::`vector deleting destructor'(rvDecalParticle *this, unsigned int a2)
{
  rvDecalParticle *v2; // esi
  void *result; // eax

  v2 = this;
  if ( a2 & 2 )
  {
    `eh vector destructor iterator'(
      this,
      0x108u,
      LODWORD(this[-1].mRotationEnv.mEnd),
      (void (__thiscall *)(void *))rvLineParticle::~rvLineParticle);
    if ( a2 & 1 )
      operator delete(&v2[-1].mRotationEnv.mEnd);
    result = &v2[-1].mRotationEnv.mEnd;
  }
  else
  {
    this->vfptr = (rvParticleVtbl *)&rvParticle::`vftable';
    if ( a2 & 1 )
      operator delete(this);
    result = v2;
  }
  return result;
}
void __thiscall rvLineParticle::rvLineParticle(rvLineParticle *this)
{
  this->vfptr = (rvParticleVtbl *)&rvLineParticle::`vftable';
}
void __thiscall rvLineParticle::EvaluateSize(rvLinkedParticle *this, rvEnvParms *size, const float time, float oneOverDuration, float *dest)
{
  rvEnvParms::Evaluate(size, &this->mSizeEnv, time, oneOverDuration, dest);
}
void __stdcall rvLineParticle::InitLengthEnv(rvEnvParms *env, float duration)
{
  rvDecalParticle::InitSizeEnv((idJointMat *)env, SLODWORD(duration));
}
void __thiscall rvLinkedParticle::AttenuateSize(rvLinkedParticle *this, float atten, rvParticleParms *parms)
{
  rvParticle::Attenuate(atten, parms, &this->mSizeEnv);
}
void __thiscall rvLineParticle::AttenuateLength(rvLineParticle *this, float atten, rvParticleParms *parms)
{
  rvParticle::Attenuate(atten, parms, &this->mLengthEnv);
}
void __thiscall rvEnvParms3Particle::Transform(rvEnvParms3Particle *this, idVec3 normal)
{
  rvEnvParms3Particle *v2; // esi
  double v3; // st6
  double v4; // st5
  double v5; // st7
  double v6; // st3
  double v7; // st4
  idMat3 m; // [esp+4h] [ebp-28h]
  float v9; // [esp+28h] [ebp-4h]

  v2 = this;
  idVec3::ToMat3(&normal, (idMat3 *)((char *)&m + 4));
  v3 = m.mat[0].y;
  v4 = m.mat[1].y;
  v5 = m.mat[2].y;
  normal.x = v2->mStart.x * m.mat[0].y + m.mat[1].y * v2->mStart.y + m.mat[2].y * v2->mStart.z;
  v6 = m.mat[0].z;
  v7 = m.mat[1].z;
  m.mat[0].x = v2->mStart.x * m.mat[0].z + m.mat[1].z * v2->mStart.y + m.mat[2].z * v2->mStart.z;
  v2->mStart.z = v2->mStart.x * m.mat[1].x + m.mat[2].x * v2->mStart.y + v9 * v2->mStart.z;
  v2->mStart.x = normal.x;
  v2->mStart.y = m.mat[0].x;
  normal.x = v5 * v2->mEnd.z + v3 * v2->mEnd.x + v4 * v2->mEnd.y;
  m.mat[0].x = v6 * v2->mEnd.x + v7 * v2->mEnd.y + v2->mEnd.z * m.mat[2].z;
  v2->mEnd.z = v2->mEnd.y * m.mat[2].x + v2->mEnd.x * m.mat[1].x + v2->mEnd.z * v9;
  v2->mEnd.x = normal.x;
  v2->mEnd.y = m.mat[0].x;
}
void __thiscall rvLineParticle::EvaluateLength(rvLineParticle *this, rvEnvParms *length, const float time, float oneOverDuration, idVec3 *dest)
{
  rvEnvParms::Evaluate(length, &this->mLengthEnv, time, oneOverDuration, &dest->x);
}
rvEnvParms3Particle *__thiscall rvSpriteParticle::GetDestSize(rvLineParticle *this)
{
  return &this->mLengthEnv;
}
void __thiscall rvLineParticle::TransformLength(rvLineParticle *this, idVec3 normal)
{
  idVec3 v2; // ST00_12

  v2.x = normal.x;
  *(_QWORD *)&v2.y = *(_QWORD *)&normal.y;
  rvEnvParms3Particle::Transform(&this->mLengthEnv, v2);
}
void __thiscall rvLineParticle::ScaleLength(rvLineParticle *this, float constant)
{
  this->mLengthEnv.mStart.x = this->mLengthEnv.mStart.x * constant;
  this->mLengthEnv.mStart.y = this->mLengthEnv.mStart.y * constant;
  this->mLengthEnv.mStart.z = this->mLengthEnv.mStart.z * constant;
  this->mLengthEnv.mEnd.x = this->mLengthEnv.mEnd.x * constant;
  this->mLengthEnv.mEnd.y = this->mLengthEnv.mEnd.y * constant;
  this->mLengthEnv.mEnd.z = constant * this->mLengthEnv.mEnd.z;
}
void *__thiscall rvOrientedParticle::`vector deleting destructor'(rvOrientedParticle *this, unsigned int a2)
{
  rvOrientedParticle *v2; // esi
  void *result; // eax

  v2 = this;
  if ( a2 & 2 )
  {
    `eh vector destructor iterator'(
      this,
      0x128u,
      LODWORD(this[-1].mRotationEnv.mEnd.z),
      (void (__thiscall *)(void *))rvLineParticle::~rvLineParticle);
    if ( a2 & 1 )
      operator delete(&v2[-1].mRotationEnv.mEnd.z);
    result = &v2[-1].mRotationEnv.mEnd.z;
  }
  else
  {
    this->vfptr = (rvParticleVtbl *)&rvParticle::`vftable';
    if ( a2 & 1 )
      operator delete(this);
    result = v2;
  }
  return result;
}
void __thiscall rvOrientedParticle::rvOrientedParticle(rvOrientedParticle *this)
{
  this->vfptr = (rvParticleVtbl *)&rvOrientedParticle::`vftable';
}
void __thiscall rvOrientedParticle::EvaluateSize(rvOrientedParticle *this, rvEnvParms *size, const float time, float oneOverDuration, float *dest)
{
  rvEnvParms::Evaluate(size, &this->mSizeEnv, time, oneOverDuration, dest);
}
void __thiscall rvOrientedParticle::EvaluateRotation(rvOrientedParticle *this, rvEnvParms *rotation, const float time, float oneOverDuration, float *dest)
{
  rvEnvParms::Evaluate(rotation, &this->mRotationEnv, time, oneOverDuration, dest);
}
void __stdcall rvOrientedParticle::InitRotationEnv(rvEnvParms *env, float duration)
{
  rvDecalParticle::InitSizeEnv((idJointMat *)env, SLODWORD(duration));
}
void __thiscall rvOrientedParticle::HandleOrientation(rvOrientedParticle *this, rvAngles *angles)
{
  rvEnvParms3Particle::Rotate(&this->mRotationEnv, angles);
}
rvEnvParms3Particle *__thiscall rvDecalParticle::GetInitRotation(rvModelParticle *this)
{
  return &this->mRotationEnv;
}
idVec2 *__thiscall rvOrientedParticle::GetDestSize(rvOrientedParticle *this)
{
  return &this->mSizeEnv.mEnd;
}
rvEnvParms3Particle *__thiscall rvOrientedParticle::GetInitRotation(rvOrientedParticle *this)
{
  return &this->mRotationEnv;
}
idVec3 *__thiscall rvOrientedParticle::GetDestRotation(rvOrientedParticle *this)
{
  return &this->mRotationEnv.mEnd;
}
void __thiscall rvOrientedParticle::ScaleRotation(rvOrientedParticle *this, float constant)
{
  this->mRotationEnv.mStart.x = this->mRotationEnv.mStart.x * constant;
  this->mRotationEnv.mStart.y = this->mRotationEnv.mStart.y * constant;
  this->mRotationEnv.mStart.z = this->mRotationEnv.mStart.z * constant;
  this->mRotationEnv.mEnd.x = this->mRotationEnv.mEnd.x * constant;
  this->mRotationEnv.mEnd.y = this->mRotationEnv.mEnd.y * constant;
  this->mRotationEnv.mEnd.z = constant * this->mRotationEnv.mEnd.z;
}
void __thiscall rvDecalParticle::rvDecalParticle(rvDecalParticle *this)
{
  this->vfptr = (rvParticleVtbl *)&rvDecalParticle::`vftable';
}
void __thiscall rvDecalParticle::EvaluateRotation(rvDecalParticle *this, rvEnvParms *rotation, const float time, float oneOverDuration, float *dest)
{
  rvEnvParms::Evaluate(rotation, &this->mRotationEnv, time, oneOverDuration, dest);
}
void __thiscall rvDecalParticle::ScaleRotation(rvDecalParticle *this, float constant)
{
  this->mRotationEnv.mStart = this->mRotationEnv.mStart * constant;
  this->mRotationEnv.mEnd = constant * this->mRotationEnv.mEnd;
}
idVec3 *__thiscall rvDebrisParticle::GetDestRotation(rvDebrisParticle *this)
{
  return &this->mRotationEnv.mEnd;
}
void *__thiscall rvModelParticle::`vector deleting destructor'(rvModelParticle *this, unsigned int a2)
{
  rvModelParticle *v2; // esi
  void *result; // eax

  v2 = this;
  if ( a2 & 2 )
  {
    `eh vector destructor iterator'(
      this,
      0x11Cu,
      (int)this[-1].mModel,
      (void (__thiscall *)(void *))rvLineParticle::~rvLineParticle);
    if ( a2 & 1 )
      operator delete(&v2[-1].mModel);
    result = &v2[-1].mModel;
  }
  else
  {
    this->vfptr = (rvParticleVtbl *)&rvParticle::`vftable';
    if ( a2 & 1 )
      operator delete(this);
    result = v2;
  }
  return result;
}
void __thiscall rvModelParticle::rvModelParticle(rvModelParticle *this)
{
  this->vfptr = (rvParticleVtbl *)&rvModelParticle::`vftable';
}
void __thiscall rvModelParticle::EvaluateRotation(rvModelParticle *this, rvEnvParms *rotation, const float time, float oneOverDuration, float *dest)
{
  rvEnvParms::Evaluate(rotation, &this->mRotationEnv, time, oneOverDuration, dest);
}
void __stdcall rvModelParticle::InitRotationEnv(rvEnvParms *env, float duration)
{
  rvDecalParticle::InitSizeEnv((idJointMat *)env, SLODWORD(duration));
}
void __thiscall rvModelParticle::SetModel(rvModelParticle *this, idRenderModel *model)
{
  this->mModel = model;
}
void __thiscall rvLightParticle::AttenuateSize(rvModelParticle *this, float atten, rvParticleParms *parms)
{
  rvParticle::Attenuate(atten, parms, &this->mSizeEnv);
}
idVec3 *__thiscall rvModelParticle::GetDestRotation(rvModelParticle *this)
{
  return &this->mRotationEnv.mEnd;
}
void *__thiscall rvElectricityParticle::`vector deleting destructor'(rvElectricityParticle *this, unsigned int a2)
{
  rvElectricityParticle *v2; // esi
  void *result; // eax

  v2 = this;
  if ( a2 & 2 )
  {
    `eh vector destructor iterator'(
      this,
      0x144u,
      (int)this[-1].mJitterTable,
      (void (__thiscall *)(void *))rvLineParticle::~rvLineParticle);
    if ( a2 & 1 )
      operator delete(&v2[-1].mJitterTable);
    result = &v2[-1].mJitterTable;
  }
  else
  {
    this->vfptr = (rvParticleVtbl *)&rvParticle::`vftable';
    if ( a2 & 1 )
      operator delete(this);
    result = v2;
  }
  return result;
}
void __thiscall rvElectricityParticle::rvElectricityParticle(rvElectricityParticle *this)
{
  this->vfptr = (rvParticleVtbl *)&rvElectricityParticle::`vftable';
}
void *__thiscall rvLinkedParticle::`vector deleting destructor'(sdOrientedLinkedParticle *this, unsigned int a2)
{
  sdOrientedLinkedParticle *v2; // esi
  void *result; // eax

  v2 = this;
  if ( a2 & 2 )
  {
    `eh vector destructor iterator'(
      this,
      0xF0u,
      LODWORD(this[-1].mSizeEnv.mEnd),
      (void (__thiscall *)(void *))rvLineParticle::~rvLineParticle);
    if ( a2 & 1 )
      operator delete(&v2[-1].mSizeEnv.mEnd);
    result = &v2[-1].mSizeEnv.mEnd;
  }
  else
  {
    this->vfptr = (rvParticleVtbl *)&rvParticle::`vftable';
    if ( a2 & 1 )
      operator delete(this);
    result = v2;
  }
  return result;
}
void __thiscall rvLinkedParticle::rvLinkedParticle(rvLinkedParticle *this)
{
  this->vfptr = (rvParticleVtbl *)&rvLinkedParticle::`vftable';
}
void __stdcall rvParticle::EvaluateLength(rvEnvParms *length, const float time, float oneOverDuration, idVec3 *dest)
{
  ;
}
float *__thiscall rvLineParticle::GetDestSize(rvLinkedParticle *this)
{
  return &this->mSizeEnv.mEnd;
}
void __thiscall sdOrientedLinkedParticle::sdOrientedLinkedParticle(sdOrientedLinkedParticle *this)
{
  this->vfptr = (rvParticleVtbl *)&sdOrientedLinkedParticle::`vftable';
}
void *__thiscall rvDebrisParticle::`vector deleting destructor'(rvDebrisParticle *this, unsigned int a2)
{
  rvDebrisParticle *v2; // esi
  void *result; // eax

  v2 = this;
  if ( a2 & 2 )
  {
    `eh vector destructor iterator'(
      this,
      0x100u,
      LODWORD(this[-1].mRotationEnv.mEnd.z),
      (void (__thiscall *)(void *))rvLineParticle::~rvLineParticle);
    if ( a2 & 1 )
      operator delete(&v2[-1].mRotationEnv.mEnd.z);
    result = &v2[-1].mRotationEnv.mEnd.z;
  }
  else
  {
    this->vfptr = (rvParticleVtbl *)&rvParticle::`vftable';
    if ( a2 & 1 )
      operator delete(this);
    result = v2;
  }
  return result;
}
void __thiscall rvDebrisParticle::rvDebrisParticle(rvDebrisParticle *this)
{
  this->vfptr = (rvParticleVtbl *)&rvDebrisParticle::`vftable';
}
rvEnvParms3Particle *__thiscall rvModelParticle::GetInitSize(rvDebrisParticle *this)
{
  return &this->mRotationEnv;
}
void __thiscall rvDebrisParticle::ScaleRotation(rvDebrisParticle *this, float constant)
{
  this->mRotationEnv.mStart.x = this->mRotationEnv.mStart.x * constant;
  this->mRotationEnv.mStart.y = this->mRotationEnv.mStart.y * constant;
  this->mRotationEnv.mStart.z = this->mRotationEnv.mStart.z * constant;
  this->mRotationEnv.mEnd.x = this->mRotationEnv.mEnd.x * constant;
  this->mRotationEnv.mEnd.y = this->mRotationEnv.mEnd.y * constant;
  this->mRotationEnv.mEnd.z = constant * this->mRotationEnv.mEnd.z;
}
rvParticle *__thiscall rvSegment::SpawnParticle(rvSegment *this, rvBSE *effect, rvSegmentTemplate *st, float birthTime, idVec3 *initOffset, idMat3 *initAxis)
{
  rvParticle *v6; // esi
  rvParticle *v7; // edx
  idMat3 *v8; // ST14_4
  float initAxisa; // [esp+28h] [ebp+14h]

  if ( (LOWORD(this->mEffectDecl->mSegmentTemplates.list[this->mSegmentTemplateHandle].mFlags) >> 8) & 1 )
  {
    v6 = this->mParticles;
  }
  else
  {
    v6 = this->mFreeHead;
    if ( !v6 )
      return v6;
    v7 = this->mUsedHead;
    this->mFreeHead = v6->mNext;
    v6->mNext = v7;
    this->mUsedHead = v6;
  }
  if ( v6 )
  {
    v8 = initAxis;
    initAxisa = birthTime - effect->mStartTime;
    ((void (__thiscall *)(rvParticle *, rvBSE *, rvSegment *, _DWORD, _DWORD, idVec3 *, idMat3 *))v6->vfptr->FinishSpawn)(
      v6,
      effect,
      this,
      LODWORD(birthTime),
      LODWORD(initAxisa),
      initOffset,
      v8);
  }
  return v6;
}
void __thiscall rvSegment::SpawnParticles(rvSegment *this, rvBSE *effect, rvSegmentTemplate *st, float birthTime, int count)
{
  int v5; // edi
  int v6; // ebp
  rvSegment *v7; // esi
  rvParticle *v8; // ecx
  rvParticle *v9; // eax
  rvParticleVtbl *v10; // edx
  signed int v11; // [esp+20h] [ebp-4h]
  float counta; // [esp+34h] [ebp+10h]

  v5 = count;
  v6 = 0;
  v7 = this;
  v11 = 0;
  if ( count > 0 )
  {
    while ( !((LOWORD(v7->mEffectDecl->mSegmentTemplates.list[v7->mSegmentTemplateHandle].mFlags) >> 8) & 1) )
    {
      v8 = v7->mFreeHead;
      if ( v8 )
      {
        v9 = v7->mUsedHead;
        v7->mFreeHead = v8->mNext;
        v8->mNext = v9;
        v7->mUsedHead = v8;
LABEL_6:
        if ( v8 )
        {
          v10 = v8->vfptr;
          if ( v5 == 1 )
          {
            ((void (__stdcall *)(rvBSE *, rvSegment *, _DWORD, _DWORD, idVec3 *, idMat3 *))v10->FinishSpawn)(
              effect,
              v7,
              LODWORD(birthTime),
              0.0,
              &vec3_origin,
              &mat3_identity);
          }
          else
          {
            counta = (double)v11 / (double)(v5 - 1);
            ((void (__stdcall *)(rvBSE *, rvSegment *, _DWORD, _DWORD, idVec3 *, idMat3 *))v10->FinishSpawn)(
              effect,
              v7,
              LODWORD(birthTime),
              LODWORD(counta),
              &vec3_origin,
              &mat3_identity);
          }
        }
      }
      v11 = ++v6;
      if ( v6 >= v5 )
        return;
    }
    v8 = v7->mParticles;
    goto LABEL_6;
  }
}
void __stdcall rvSegment::PlayEffect(rvBSE *effect, rvSegmentTemplate *st, float depthOffset)
{
  int v3; // eax
  int v4; // eax
  int v5; // eax

  v3 = st->mNumEffects;
  if ( v3 )
  {
    v4 = rvRandom::irand(0, v3 - 1);
    v5 = ((int (*)(void))st->mEffects[v4]->base->vfptr->Index)();
    ((void (__stdcall *)(int, idVec3 *, idVec3 *, idMat3 *, _DWORD, idVec3 *, _DWORD))game->vfptr->PlayEffect)(
      v5,
      &effect->mMaterialColor,
      &effect->mCurrentOrigin,
      &effect->mCurrentAxis,
      0,
      &vec3_origin,
      LODWORD(depthOffset));
  }
}
void __thiscall rvSegment::Handle(rvSegment *this, rvBSE *effect, float time)
{
  rvSegmentTemplate *v3; // edx

  v3 = &this->mEffectDecl->mSegmentTemplates.list[this->mSegmentTemplateHandle];
  if ( v3 && this->mSegStartTime <= (double)time )
  {
    switch ( v3->mSegType )
    {
      case 2:
      case 3:
        if ( ((unsigned int)effect->mFlags >> 2) & 1 )
          rvSegment::RefreshParticles(this, effect, v3);
        break;
      case 5:
        rvBSE::UpdateSoundEmitter(effect, v3, this);
        break;
      case 7:
        if ( v3->mFlags & 1 )
          rvSegment::HandleLight(this, effect, v3, time);
        break;
      default:
        return;
    }
  }
}
void __thiscall rvSegment::UpdateSimpleParticles(rvSegment *this, float time)
{
  rvSegment *v2; // edi
  rvDeclEffect *v3; // eax
  int v4; // ecx
  rvParticle *v5; // esi
  rvParticle *v6; // ebp
  rvParticle *v7; // ebx
  float pt; // ST18_4
  bool (__thiscall *v9)(rvParticle *); // eax
  int v10; // [esp+18h] [ebp-4h]

  v2 = this;
  v3 = this->mEffectDecl;
  v4 = this->mSegmentTemplateHandle;
  v5 = v2->mUsedHead;
  v6 = 0;
  v2->mActiveCount = 0;
  v10 = (int)&v3->mSegmentTemplates.list[v4].mParticleTemplate;
  if ( v5 )
  {
    do
    {
      v7 = v5->mNext;
      pt = v5->mEndTime - 0.002000000094994903;
      if ( pt > (double)time )
      {
        v6 = v5;
        v2->mActiveCount += ((int (__thiscall *)(rvParticle *, int, _DWORD))v5->vfptr->Update)(v5, v10, LODWORD(time));
      }
      else
      {
        if ( v6 )
          v6->mNext = v7;
        else
          v2->mUsedHead = v7;
        v9 = v5->vfptr->Destroy;
        v5->mNext = v2->mFreeHead;
        v9(v5);
        v2->mFreeHead = v5;
      }
      v5 = v7;
    }
    while ( v7 );
  }
}
void __thiscall rvSegment::UpdateGenericParticles(rvSegment *this, rvBSE *effect, rvSegmentTemplate *st, float time)
{
  rvSegmentTemplate *v4; // edi
  rvSegment *v5; // esi
  unsigned int v6; // eax
  rvParticle *v7; // esi
  char v8; // al
  rvBSE *v9; // ebp
  bool v10; // bl
  float v11; // ST2C_4
  int v12; // eax
  bool (__thiscall *v13)(rvParticle *); // eax
  bool v14; // [esp+17h] [ebp-Dh]
  rvParticle *prev; // [esp+18h] [ebp-Ch]
  rvParticle *v16; // [esp+1Ch] [ebp-8h]
  rvParticle *next; // [esp+28h] [ebp+4h]
  char infinite; // [esp+2Ch] [ebp+8h]

  v4 = st;
  v5 = this;
  prev = (rvParticle *)this;
  v14 = rvSegmentTemplate::GetSmoker(st);
  v6 = (unsigned int)st->mFlags >> 5;
  v5->mActiveCount = 0;
  v7 = v5->mUsedHead;
  v8 = v6 & 1;
  infinite = v8;
  v16 = 0;
  if ( v7 )
  {
    v9 = effect;
    while ( 1 )
    {
      v10 = 0;
      next = v7->mNext;
      if ( v8 )
      {
        rvParticle::RunPhysics(v7, v9, v4, time);
        if ( ((unsigned int)v9->mFlags >> 3) & 1 )
          goto LABEL_9;
      }
      else
      {
        v11 = v7->mEndTime - 0.002000000094994903;
        if ( v11 <= (double)time )
        {
          rvParticle::CheckTimeoutEffect(v7, v9, v4, time);
LABEL_9:
          v10 = 1;
          goto LABEL_10;
        }
        v10 = rvParticle::RunPhysics(v7, v9, v4, time);
      }
LABEL_10:
      if ( ((unsigned int)v9->mFlags >> 3) & 1 && !(v7->mFlags & 0x200000) )
        v10 = 1;
      if ( v14 )
      {
        v12 = v4->mTrailSegmentIndex;
        if ( v12 >= 0 )
          rvParticle::EmitSmokeParticles(v7, v9, &v9->mSegments.list[v12], &v4->mParticleTemplate, time);
      }
      if ( v10 )
      {
        if ( v16 )
          v16->mNext = next;
        else
          LODWORD(prev->mInitAxis.mat[1].x) = next;
        v13 = v7->vfptr->Destroy;
        v7->mNext = (rvParticle *)LODWORD(prev->mInitAxis.mat[0].z);
        v13(v7);
        LODWORD(prev->mInitAxis.mat[0].z) = v7;
      }
      else
      {
        v16 = v7;
        LODWORD(prev->mInitAxis.mat[0].y) += ((int (__thiscall *)(rvParticle *, rvParticleTemplate *, _DWORD))v7->vfptr->Update)(
                                               v7,
                                               &v4->mParticleTemplate,
                                               LODWORD(time));
      }
      v7 = next;
      if ( !next )
        return;
      v8 = infinite;
    }
  }
}
bool __thiscall rvSegment::UpdateParticles(rvSegment *this, rvBSE *effect, float time)
{
  rvSegment *v3; // edi
  rvSegmentTemplate *v4; // esi

  v3 = this;
  v4 = &this->mEffectDecl->mSegmentTemplates.list[this->mSegmentTemplateHandle];
  if ( !v4 )
    return 0;
  rvSegment::Handle(this, effect, time);
  if ( ((unsigned int)v4->mFlags >> 5) & 1
    || rvSegmentTemplate::GetSmoker(v4)
    || ((unsigned int)v4->mParticleTemplate.mFlags >> 9) & 1
    || v4->mParticleTemplate.mNumTimeoutEffects )
  {
    rvSegment::UpdateGenericParticles(v3, effect, v4, time);
  }
  else
  {
    rvSegment::UpdateSimpleParticles(v3, time);
  }
  if ( bse_speeds.internalVar->integerValue )
  {
    dword_11F4D54 += v3->mActiveCount;
    if ( v3->mUsedHead )
      dword_11F4D58 += idFile::Tell();
  }
  return v3->mUsedHead != 0;
}
bool __thiscall rvSegment::Active(rvSegment *this)
{
  rvSegmentTemplate *v1; // eax
  bool result; // al

  v1 = &this->mEffectDecl->mSegmentTemplates.list[this->mSegmentTemplateHandle];
  if ( v1 && ((unsigned int)v1->mFlags >> 2) & 1 && this->mActiveCount )
    result = v1->mFlags & 1;
  else
    result = 0;
  return result;
}
void __thiscall rvSegment::AllocateSurface(rvSegment *this, rvBSE *effect, idRenderModel *model)
{
  rvSegment *v3; // ebx
  rvSegmentTemplate *v4; // eax
  rvParticleTemplate *v5; // edi
  int v6; // ebp
  srfTriangles_t *v7; // eax
  void (__thiscall *v8)(idRenderModel *, _DWORD, idMaterial *, srfTriangles_t *); // edx
  int v9; // eax
  srfTriangles_t *v10; // eax
  idRenderModelVtbl *v11; // ebp

  v3 = this;
  v4 = &this->mEffectDecl->mSegmentTemplates.list[this->mSegmentTemplateHandle];
  if ( v4 && ((unsigned int)v4->mFlags >> 2) & 1 )
  {
    v5 = &v4->mParticleTemplate;
    if ( effect->mFlags & 1 )
      v6 = this->mLoopParticleCount;
    else
      v6 = this->mParticleCount;
    v7 = model->vfptr->AllocSurfaceTriangles(
           model,
           v6 * v4->mParticleTemplate.mVertexCount,
           v6 * v4->mParticleTemplate.mIndexCount);
    v8 = *(void (__thiscall **)(idRenderModel *, _DWORD, idMaterial *, srfTriangles_t *))&model->vfptr->gap4[12];
    v7->texCoordScale = 100.0;
    v8(model, 0, v5->mMaterial, v7);
    v3->mSurfaceIndex = model->vfptr->NumSurfaces(model) - 1;
    if ( v5->mTrailInfo->mTrailType == 2
      && (rvParticleTemplate::GetMaxTrailCount(v5) || v5->mTrailInfo->mTrailTime.y >= 0.0020000001) )
    {
      v9 = rvParticleTemplate::GetMaxTrailCount(v5);
      v10 = model->vfptr->AllocSurfaceTriangles(model, 2 * v6 * v9 + 2, 12 * v6 * v9);
      v11 = model->vfptr;
      v10->texCoordScale = 100.0;
      (*(void (__thiscall **)(idRenderModel *, _DWORD, idMaterial *, srfTriangles_t *))&v11->gap4[12])(
        model,
        0,
        v5->mTrailInfo->mTrailMaterial,
        v10);
      v3->mFlags |= 4u;
    }
  }
}
void __userpurge rvSegment::ClearSurface(rvSegment *this@<ecx>, int a2@<ebp>, int a3@<esi>, rvBSE *effect, idRenderModel *model, int a6)
{
  rvSegment *v6; // edi
  rvSegmentTemplate *v7; // eax
  rvParticleTemplate *v8; // ebp
  int v9; // eax
  int v10; // ebx
  int v11; // eax
  int v12; // ecx
  srfTriangles_t *v13; // eax
  int v14; // edi
  int v15; // eax
  int v16; // ecx
  int v17; // eax
  srfTriangles_t *v18; // eax
  int v19; // eax
  int v20; // eax
  int v21; // [esp-8h] [ebp-10h]
  int v22; // [esp-4h] [ebp-Ch]
  int v23; // [esp-4h] [ebp-Ch]
  int v24; // [esp+18h] [ebp+10h]

  v6 = this;
  v7 = &this->mEffectDecl->mSegmentTemplates.list[this->mSegmentTemplateHandle];
  if ( v7 && ((unsigned int)v7->mFlags >> 2) & 1 )
  {
    v22 = a2;
    v8 = &v7->mParticleTemplate;
    if ( v7->mParticleTemplate.mType == 7 )
    {
      v24 = ((int (__thiscall *)(idRenderModel *, int, int, int))model->vfptr->Surface)(
              model,
              this->mSurfaceIndex,
              a3,
              v22);
      model->vfptr->FreeSurfaceTriangles(model, *(srfTriangles_t **)(v24 + 8));
      if ( *(_BYTE *)(a6 + 132) & 1 )
        v9 = v6->mLoopParticleCount;
      else
        v9 = v6->mParticleCount;
      v10 = v9 * v6->mActiveCount;
      v11 = v10 * v8->mVertexCount;
      if ( v11 > 10000 )
        v11 = 10000;
      v12 = v10 * v8->mIndexCount;
      if ( v12 > 30000 )
        v12 = 30000;
      v13 = model->vfptr->AllocSurfaceTriangles(model, v11, v12);
      model[2].vfptr = (idRenderModelVtbl *)v13;
      v13->texCoordScale = 100.0;
      if ( ((unsigned int)v6->mFlags >> 2) & 1 )
      {
        v14 = ((int (__thiscall *)(idRenderModel *, int, int, int))model->vfptr->Surface)(
                model,
                v6->mSurfaceIndex + 1,
                v21,
                v23);
        model->vfptr->FreeSurfaceTriangles(model, *(srfTriangles_t **)(v14 + 8));
        v15 = v10 * rvParticleTemplate::GetMaxTrailCount(v8);
        v16 = 2 * v15 + 2;
        if ( v16 > 10000 )
          v16 = 10000;
        v17 = 12 * v15;
        if ( v17 > 30000 )
          v17 = 30000;
        v18 = model->vfptr->AllocSurfaceTriangles(model, v16, v17);
        *(_DWORD *)(v14 + 8) = v18;
        v18->texCoordScale = 100.0;
      }
    }
    else
    {
      v19 = *(_DWORD *)((int)model->vfptr->Surface(model, this->mSurfaceIndex) + 8);
      *(_DWORD *)(v19 + 56) = 0;
      *(_DWORD *)(v19 + 48) = 0;
      if ( ((unsigned int)v6->mFlags >> 2) & 1 )
      {
        v20 = *(_DWORD *)((int)model->vfptr->Surface(model, v6->mSurfaceIndex + 1) + 8);
        *(_DWORD *)(v20 + 56) = 0;
        *(_DWORD *)(v20 + 48) = 0;
      }
    }
  }
}
void __thiscall rvSegment::RenderTrail(rvSegment *this, rvBSE *effect, renderEffect_s *owner, idRenderModel *model, float time)
{
  rvSegment *v5; // edi
  rvSegmentTemplate *v6; // eax
  rvParticleTemplate *v7; // esi
  float v8; // ST1C_4
  rvTrailInfo *v9; // ecx

  v5 = this;
  v6 = &this->mEffectDecl->mSegmentTemplates.list[this->mSegmentTemplateHandle];
  if ( v6 )
  {
    v7 = &v6->mParticleTemplate;
    v8 = _ceil(v6->mParticleTemplate.mTrailInfo->mTrailCount.y);
    if ( (signed int)v8 != -1 )
    {
      v9 = v7->mTrailInfo;
      if ( v9->mTrailTime.y >= 0.0020000001 && v9->mTrailType == 2 )
        rvSegment::RenderMotion(v5, effect, owner, model, v7, time);
    }
  }
}
void __cdecl smoothsort<particlesort>(particlesort *_m, unsigned int _n)
{
  particlesort *v2; // esi
  signed int v3; // ebp
  unsigned int v4; // edi
  unsigned int v5; // ebx
  particlesort *v6; // edx
  unsigned int v7; // ecx
  unsigned int v8; // ebx
  int v9; // ebx
  unsigned int v10; // eax
  rvParticle **v11; // eax
  rvParticle *v12; // ST24_4
  float v13; // ST24_4
  unsigned int v14; // eax
  bool v15; // cf
  unsigned int v16; // ecx
  particlesort *v17; // edx
  unsigned int v18; // ecx
  int v19; // ebx
  unsigned int v20; // ST20_4
  rvParticle **v21; // eax
  rvParticle *v22; // ST20_4
  float v23; // ST24_4
  unsigned int v24; // eax
  particlesort *v25; // ecx
  __int64 v26; // rax
  rvParticle *v27; // eax
  float v28; // ST24_4
  int v29; // ebp
  rvParticle *v30; // ST24_4
  float v31; // ST24_4
  rvParticle *v32; // edx
  float v33; // ST24_4
  unsigned int v34; // eax
  int v35; // edx
  unsigned int v36; // eax
  rvParticle *v37; // eax
  float v38; // ST24_4
  unsigned int v39; // eax
  unsigned int v40; // eax
  unsigned int v41; // eax
  bool v42; // zf
  unsigned int v43; // et0
  unsigned int v44; // edx
  unsigned int v45; // ecx
  int v46; // edx
  rvParticle *v47; // eax
  double v48; // st7
  int v49; // edx
  int v50; // eax
  rvParticle **v51; // eax
  rvParticle *v52; // ST24_4
  unsigned int v53; // eax
  rvParticle **v54; // eax
  int v55; // eax
  int v56; // edx
  int v57; // edx
  unsigned __int64 v58; // rax
  rvParticle **v59; // eax
  rvParticle *v60; // ST24_4
  int v61; // eax
  int v62; // eax
  unsigned int v63; // ecx
  unsigned int v64; // edx
  int v65; // ebp
  int v66; // ecx
  unsigned int v67; // eax
  int v68; // edx
  int v69; // ecx
  rvParticle *v70; // ST20_4
  float v71; // ST20_4
  unsigned int v72; // edx
  unsigned int v73; // ebx
  unsigned int v74; // eax
  unsigned int v75; // eax
  rvParticle *v76; // eax
  float v77; // ST28_4
  int v78; // ebp
  unsigned int v79; // eax
  unsigned int v80; // eax
  rvParticle *v81; // ST28_4
  float v82; // ST28_4
  rvParticle *v83; // ST28_4
  float v84; // ST28_4
  unsigned int v85; // eax
  int v86; // ebx
  unsigned int v87; // eax
  rvParticle *v88; // eax
  float v89; // ST28_4
  unsigned int v90; // eax
  unsigned int v91; // edx
  int v92; // ebp
  int v93; // ecx
  rvParticle *v94; // edx
  unsigned int v95; // edx
  float v96; // ST28_4
  unsigned int v97; // edi
  unsigned int v98; // ebx
  unsigned int v99; // eax
  unsigned int v100; // eax
  rvParticle *v101; // eax
  float v102; // ST28_4
  int v103; // ebp
  unsigned int v104; // eax
  unsigned int v105; // eax
  rvParticle *v106; // ST28_4
  float v107; // ST28_4
  rvParticle *v108; // ST28_4
  float v109; // ST28_4
  unsigned int v110; // eax
  int v111; // ebx
  unsigned int v112; // eax
  rvParticle *v113; // eax
  float v114; // ST28_4
  unsigned int v115; // eax
  unsigned __int64 v116; // kr30_8
  int v117; // [esp+4h] [ebp-38h]
  int v118; // [esp+4h] [ebp-38h]
  int v119; // [esp+4h] [ebp-38h]
  int v120; // [esp+4h] [ebp-38h]
  int v121; // [esp+4h] [ebp-38h]
  int v122; // [esp+8h] [ebp-34h]
  unsigned int v123; // [esp+8h] [ebp-34h]
  unsigned __int64 v124; // [esp+Ch] [ebp-30h]
  unsigned __int64 v125; // [esp+Ch] [ebp-30h]
  int v126; // [esp+Ch] [ebp-30h]
  unsigned __int64 v127; // [esp+14h] [ebp-28h]
  unsigned int v128; // [esp+14h] [ebp-28h]
  unsigned int p; // [esp+18h] [ebp-24h]
  unsigned __int64 p_4; // [esp+1Ch] [ebp-20h]
  unsigned __int64 p_4a; // [esp+1Ch] [ebp-20h]
  unsigned __int64 p_4b; // [esp+1Ch] [ebp-20h]
  signed int b_4; // [esp+24h] [ebp-18h]
  signed int v134; // [esp+28h] [ebp-14h]
  unsigned int v135; // [esp+28h] [ebp-14h]
  unsigned int v136; // [esp+2Ch] [ebp-10h]
  unsigned int v137; // [esp+30h] [ebp-Ch]
  unsigned int v138; // [esp+34h] [ebp-8h]
  unsigned int v139; // [esp+38h] [ebp-4h]
  particlesort *_ma; // [esp+40h] [ebp+4h]
  unsigned int _mb; // [esp+40h] [ebp+4h]
  float _me; // [esp+40h] [ebp+4h]
  float _mf; // [esp+40h] [ebp+4h]
  particlesort *_mg; // [esp+40h] [ebp+4h]
  float _mh; // [esp+40h] [ebp+4h]
  int _mc; // [esp+40h] [ebp+4h]
  float _mi; // [esp+40h] [ebp+4h]
  particlesort *_md; // [esp+40h] [ebp+4h]
  unsigned int _na; // [esp+44h] [ebp+8h]

  v2 = _m;
  if ( _m && _n )
  {
    v3 = 1;
    v4 = 1;
    v5 = 1;
    p_4 = 1i64;
    b_4 = 1;
    v134 = 1;
    if ( _n > 1 )
    {
      _ma = 0;
      while ( 1 )
      {
        if ( (v3 & 7) == 3 )
        {
          v6 = _ma;
          v136 = v4;
          v7 = v5;
          if ( v4 >= 3 )
          {
            v8 = v4;
            do
            {
              v122 = (int)v6 + v7 - v8;
              if ( v2[(signed int)v6 - 1].dist > (double)v2[(unsigned int)v6 + v7 - v8].dist )
              {
                v117 = (int)&v6[-1].p + 3;
                v10 = v7;
                v7 = v8 - v7 - 1;
                v9 = (int)&v6[-1].p + 3;
                v136 = v10;
              }
              else
              {
                v9 = (int)v6 + v7 - v8;
                v117 = v122;
              }
              if ( v2[v9].dist <= (double)v2[(_DWORD)v6].dist )
                break;
              v11 = &v2[v9].p;
              v12 = *v11;
              *v11 = v2[(_DWORD)v6].p;
              v2[(_DWORD)v6].p = v12;
              v13 = v2[v117].dist;
              v2[v117].dist = v2[(_DWORD)v6].dist;
              v2[(_DWORD)v6].dist = v13;
              v6 = (particlesort *)v117;
              v14 = v7;
              v15 = v7 < 3;
              v7 = v136 - v7 - 1;
              v8 = v14;
              v136 = v14;
            }
            while ( !v15 );
            v5 = v134;
          }
          v3 = __PAIR__(HIDWORD(p_4), v3) >> 2;
          v5 += v4 + 1;
          v4 += v5 + 1;
          v16 = HIDWORD(p_4) >> 2;
          goto LABEL_47;
        }
        if ( (v3 & 3) == 1 )
          break;
LABEL_48:
        v43 = (__PAIR__(HIDWORD(p_4), v3++) + 1) >> 32;
        _ma = (particlesort *)((char *)_ma + 1);
        p_4 = __PAIR__(v43, v3);
        if ( (unsigned int)&_ma->dist + 1 >= _n )
          goto LABEL_49;
      }
      v17 = _ma;
      if ( (unsigned int)&_ma->dist + v5 + 1 >= _n )
      {
        v25 = _ma;
        HIDWORD(v26) = v3;
        p = HIDWORD(p_4);
        while ( p | HIDWORD(v26) )
        {
          for ( ; !(v26 & 0x100000000i64); p >>= 1 )
          {
            LODWORD(v26) = v4;
            v4 += v5 + 1;
            v5 = v26;
            HIDWORD(v26) = __PAIR__(p, HIDWORD(v26)) >> 1;
          }
          LODWORD(v26) = (__PAIR__(p, HIDWORD(v26)) - 1) >> 32;
          --HIDWORD(v26);
          p = v26;
          if ( !v26 )
            break;
          v118 = (int)v25 - v4;
          if ( v2[(unsigned int)v25 - v4].dist <= (double)v2[(_DWORD)v25].dist )
            break;
          if ( v4 == 1 )
          {
            v27 = (rvParticle *)*((_DWORD *)&v2[(_DWORD)v25] - 1);
            *((_DWORD *)&v2[(_DWORD)v25] - 1) = v2[(_DWORD)v25].p;
            v2[(_DWORD)v25].p = v27;
            v28 = v2[(signed int)v25 - 1].dist;
            v2[(signed int)v25 - 1].dist = v2[(_DWORD)v25].dist;
            v2[(_DWORD)v25].dist = v28;
            v25 = (particlesort *)((char *)v25 - 1);
          }
          else if ( v4 >= 3 )
          {
            v29 = (int)v25 + v5 - v4;
            if ( v2[v29].dist <= (double)v2[(signed int)v25 - 1].dist )
            {
              LODWORD(v26) = __PAIR__((unsigned int)v26, HIDWORD(v26)) >> 31;
              HIDWORD(v26) *= 2;
              p = v26;
              LODWORD(v26) = v5;
              v5 = v4 - v5 - 1;
              v29 = (int)&v25[-1].p + 3;
              v4 = v26;
            }
            if ( v2[v29].dist > (double)v2[v118].dist )
            {
              v32 = v2[v29].p;
              v2[v29].p = v2[(_DWORD)v25].p;
              v2[(_DWORD)v25].p = v32;
              v33 = v2[v29].dist;
              v2[v29].dist = v2[(_DWORD)v25].dist;
              v34 = v5;
              v2[(_DWORD)v25].dist = v33;
              v5 = v4 - v5 - 1;
              v25 = (particlesort *)v29;
              v4 = v34;
              break;
            }
            v30 = v2[v118].p;
            v2[v118].p = v2[(_DWORD)v25].p;
            v2[(_DWORD)v25].p = v30;
            v31 = v2[v118].dist;
            v2[v118].dist = v2[(_DWORD)v25].dist;
            v2[(_DWORD)v25].dist = v31;
            v25 = (particlesort *)v118;
          }
        }
        if ( v4 >= 3 )
        {
          do
          {
            v35 = (int)v25 + v5 - v4;
            if ( v2[(signed int)v25 - 1].dist > (double)v2[v35].dist )
            {
              v36 = v5;
              v5 = v4 - v5 - 1;
              v35 = (int)&v25[-1].p + 3;
              v4 = v36;
            }
            if ( v2[v35].dist <= (double)v2[(_DWORD)v25].dist )
              break;
            v37 = v2[v35].p;
            v2[v35].p = v2[(_DWORD)v25].p;
            v2[(_DWORD)v25].p = v37;
            v38 = v2[v35].dist;
            v39 = v5;
            v2[v35].dist = v2[(_DWORD)v25].dist;
            v15 = v5 < 3;
            v2[(_DWORD)v25].dist = v38;
            v5 = v4 - v5 - 1;
            v25 = (particlesort *)v35;
            v4 = v39;
          }
          while ( !v15 );
        }
        v4 = b_4;
        v3 = p_4;
      }
      else
      {
        v138 = v4;
        v18 = v5;
        if ( v4 < 3 )
          goto LABEL_45;
        do
        {
          v19 = (int)v17 + v18 - v138;
          if ( v2[(signed int)v17 - 1].dist > (double)v2[v19].dist )
          {
            v20 = v18;
            v18 = v138 - v18 - 1;
            v19 = (int)&v17[-1].p + 3;
            v138 = v20;
          }
          if ( v2[v19].dist <= (double)v2[(_DWORD)v17].dist )
            break;
          v21 = &v2[v19].p;
          v22 = *v21;
          *v21 = v2[(_DWORD)v17].p;
          v2[(_DWORD)v17].p = v22;
          v23 = v2[v19].dist;
          v2[v19].dist = v2[(_DWORD)v17].dist;
          v2[(_DWORD)v17].dist = v23;
          v17 = (particlesort *)v19;
          v24 = v18;
          v15 = v18 < 3;
          v18 = v138 - v18 - 1;
          v138 = v24;
        }
        while ( !v15 );
      }
      v5 = v134;
LABEL_45:
      v16 = __PAIR__(HIDWORD(p_4), v3) >> 31;
      v40 = v5;
      v5 = v4 - v5 - 1;
      v3 *= 2;
      v4 = v40;
      HIDWORD(p_4) = v16;
      v134 = v5;
      b_4 = v40;
      if ( v40 > 1 )
      {
        do
        {
          v16 = __PAIR__(v16, v3) >> 31;
          v41 = v5;
          v3 *= 2;
          v15 = v5 < 1;
          v42 = v5 == 1;
          v5 = v4 - v5 - 1;
          v4 = v41;
        }
        while ( !v15 && !v42 );
LABEL_47:
        HIDWORD(p_4) = v16;
        v134 = v5;
        b_4 = v4;
        goto LABEL_48;
      }
      goto LABEL_48;
    }
LABEL_49:
    v44 = HIDWORD(p_4);
    v45 = _n - 1;
    v127 = __PAIR__(v5, v4);
    v124 = __PAIR__(HIDWORD(p_4), v3);
    if ( HIDWORD(p_4) | v3 )
    {
      do
      {
        for ( ; !(v124 & 1); v124 >>= 1 )
        {
          v46 = v127;
          LODWORD(v127) = v127 + HIDWORD(v127) + 1;
          HIDWORD(v127) = v46;
        }
        if ( !--v124 )
          break;
        _mb = v45 - v127;
        if ( v2[v45 - v127].dist <= (double)v2[v45].dist )
          break;
        if ( (_DWORD)v127 == 1 )
        {
          v47 = (rvParticle *)*((_DWORD *)&v2[v45] - 1);
          *((_DWORD *)&v2[v45] - 1) = v2[v45].p;
          v2[v45].p = v47;
          _me = v2[v45 - 1].dist;
          v48 = v2[v45--].dist;
          v2[v45].dist = v48;
          v2[v45 + 1].dist = _me;
        }
        else if ( (unsigned int)v127 >= 3 )
        {
          v49 = v45 + HIDWORD(v127) - v127;
          v119 = v45 + HIDWORD(v127) - v127;
          if ( v2[v49].dist <= (double)v2[v45 - 1].dist )
          {
            v119 = v45 - 1;
            v124 *= 2i64;
            v50 = HIDWORD(v127);
            HIDWORD(v127) = v127 - HIDWORD(v127) - 1;
            v49 = v45 - 1;
            LODWORD(v127) = v50;
          }
          if ( v2[v49].dist > (double)v2[_mb].dist )
          {
            v54 = &v2[v49].p;
            _mg = (particlesort *)*v54;
            *v54 = v2[v45].p;
            v2[v45].p = (rvParticle *)_mg;
            _mh = v2[v119].dist;
            v2[v119].dist = v2[v45].dist;
            v2[v45].dist = _mh;
            v45 = v119;
            v55 = HIDWORD(v127);
            HIDWORD(v127) = v127 - HIDWORD(v127) - 1;
            LODWORD(v127) = v55;
            break;
          }
          v51 = &v2[_mb].p;
          v52 = *v51;
          *v51 = v2[v45].p;
          v2[v45].p = v52;
          v53 = _mb;
          _mf = v2[_mb].dist;
          v2[v53].dist = v2[v45].dist;
          v2[v45].dist = _mf;
          v45 = v53;
        }
      }
      while ( v124 );
      v44 = HIDWORD(p_4);
    }
    v125 = v127;
    if ( (unsigned int)v127 >= 3 )
    {
      v56 = v127;
      do
      {
        _mc = v45 + HIDWORD(v125) - v56;
        if ( v2[v45 - 1].dist > (double)v2[v45 + HIDWORD(v125) - v56].dist )
        {
          _mc = v45 - 1;
          LODWORD(v58) = HIDWORD(v125);
          HIDWORD(v58) = v56 - HIDWORD(v125) - 1;
          v125 = v58;
          v57 = v45 - 1;
        }
        else
        {
          v57 = v45 + HIDWORD(v125) - v56;
        }
        if ( v2[v57].dist <= (double)v2[v45].dist )
          break;
        v59 = &v2[v57].p;
        v60 = *v59;
        *v59 = v2[v45].p;
        v2[v45].p = v60;
        v61 = _mc;
        _mi = v2[_mc].dist;
        v2[v61].dist = v2[v45].dist;
        v2[v45].dist = _mi;
        v45 = v61;
        v62 = HIDWORD(v125);
        v15 = HIDWORD(v125) < 3;
        HIDWORD(v125) = v125 - HIDWORD(v125) - 1;
        v56 = v62;
        LODWORD(v125) = v62;
      }
      while ( !v15 );
      v44 = HIDWORD(p_4);
    }
    v63 = _n;
    v64 = (__PAIR__(v44, v3) - 1) >> 32;
    v65 = v3 - 1;
    p_4a = __PAIR__(v64, v65);
    if ( _n > 1 )
    {
      v123 = _n - 1;
      _md = &v2[_n - 1];
      do
      {
        --_md;
        --v123;
        v66 = v63 - 1;
        _na = v66;
        if ( v4 == 1 )
        {
          if ( !(v65 & 1) )
          {
            do
            {
              v65 = __PAIR__(v64, v65) >> 1;
              v67 = v4;
              v4 += v5 + 1;
              v5 = v67;
              v64 >>= 1;
            }
            while ( !(v65 & 1) );
            v134 = v67;
            b_4 = v4;
          }
        }
        else if ( v4 >= 3 )
        {
          if ( v64 | v65 )
          {
            v68 = v66 + v5 - v4;
            v69 = v66 - v4;
            v126 = v68;
            if ( v2[v68].dist <= (double)v2[v69].dist )
            {
              v70 = v2[v69].p;
              v2[v69].p = v2[v68].p;
              v2[v68].p = v70;
              v71 = v2[v69].dist;
              v72 = v5;
              v2[v69].dist = v2[v126].dist;
              v73 = v65;
              v2[v126].dist = v71;
              v137 = HIDWORD(p_4a);
              do
              {
                for ( ; !(v73 & 1); v137 >>= 1 )
                {
                  v74 = v4;
                  v4 += v72 + 1;
                  v72 = v74;
                  v73 = __PAIR__(v137, v73) >> 1;
                }
                v75 = (__PAIR__(v137, v73--) - 1) >> 32;
                v137 = v75;
                if ( !__PAIR__(v75, v73) )
                  break;
                v120 = v69 - v4;
                if ( v2[v69 - v4].dist <= (double)v2[v69].dist )
                  break;
                if ( v4 == 1 )
                {
                  v76 = (rvParticle *)*((_DWORD *)&v2[v69] - 1);
                  *((_DWORD *)&v2[v69] - 1) = v2[v69].p;
                  v2[v69].p = v76;
                  v77 = v2[v69 - 1].dist;
                  v2[v69 - 1].dist = v2[v69].dist;
                  v2[v69--].dist = v77;
                }
                else if ( v4 >= 3 )
                {
                  v78 = v69 + v72 - v4;
                  if ( v2[v78].dist <= (double)v2[v69 - 1].dist )
                  {
                    v79 = __PAIR__(v75, v73) >> 31;
                    v73 *= 2;
                    v137 = v79;
                    v80 = v72;
                    v72 = v4 - v72 - 1;
                    v78 = v69 - 1;
                    v4 = v80;
                  }
                  if ( v2[v78].dist > (double)v2[v120].dist )
                  {
                    v83 = v2[v78].p;
                    v2[v78].p = v2[v69].p;
                    v2[v69].p = v83;
                    v84 = v2[v78].dist;
                    v2[v78].dist = v2[v69].dist;
                    v85 = v72;
                    v72 = v4 - v72 - 1;
                    v2[v69].dist = v84;
                    v69 = v78;
                    v4 = v85;
                    break;
                  }
                  v81 = v2[v120].p;
                  v2[v120].p = v2[v69].p;
                  v2[v69].p = v81;
                  v82 = v2[v120].dist;
                  v2[v120].dist = v2[v69].dist;
                  v2[v69].dist = v82;
                  v69 = v120;
                }
              }
              while ( v137 | v73 );
              if ( v4 >= 3 )
              {
                do
                {
                  v86 = v72 - v4 + v69;
                  if ( v2[v69 - 1].dist > (double)v2[v86].dist )
                  {
                    v87 = v72;
                    v72 = v4 - v72 - 1;
                    v86 = v69 - 1;
                    v4 = v87;
                  }
                  if ( v2[v86].dist <= (double)v2[v69].dist )
                    break;
                  v88 = v2[v86].p;
                  v2[v86].p = v2[v69].p;
                  v2[v69].p = v88;
                  v89 = v2[v86].dist;
                  v90 = v72;
                  v2[v86].dist = v2[v69].dist;
                  v15 = v72 < 3;
                  v2[v69].dist = v89;
                  v72 = v4 - v72 - 1;
                  v69 = v86;
                  v4 = v90;
                }
                while ( !v15 );
              }
              v65 = p_4a;
              v4 = b_4;
              v5 = v134;
            }
          }
          v4 = v4 - v5 - 1;
          v128 = v5;
          v135 = v4;
          v91 = (2 * __PAIR__(HIDWORD(p_4a), v65) + 1) >> 32;
          v92 = 2 * v65 + 1;
          v93 = v123 - v4;
          p_4b = __PAIR__(v91, v92);
          if ( _md->dist <= (double)v2[v123 - v4].dist )
          {
            v94 = v2[v93].p;
            v2[v93].p = _md->p;
            _md->p = v94;
            v95 = v4;
            v96 = v2[v93].dist;
            v97 = v5;
            v2[v93].dist = _md->dist;
            v98 = v92;
            _md->dist = v96;
            v139 = HIDWORD(p_4b);
            while ( v139 | v98 )
            {
              for ( ; !(v98 & 1); v139 >>= 1 )
              {
                v99 = v97;
                v97 += v95 + 1;
                v95 = v99;
                v98 = __PAIR__(v139, v98) >> 1;
              }
              v100 = (__PAIR__(v139, v98--) - 1) >> 32;
              v139 = v100;
              if ( !__PAIR__(v100, v98) )
                break;
              v121 = v93 - v97;
              if ( v2[v93 - v97].dist <= (double)v2[v93].dist )
                break;
              if ( v97 == 1 )
              {
                v101 = (rvParticle *)*((_DWORD *)&v2[v93] - 1);
                *((_DWORD *)&v2[v93] - 1) = v2[v93].p;
                v2[v93].p = v101;
                v102 = v2[v93 - 1].dist;
                v2[v93 - 1].dist = v2[v93].dist;
                v2[v93--].dist = v102;
              }
              else if ( v97 >= 3 )
              {
                v103 = v93 + v95 - v97;
                if ( v2[v103].dist <= (double)v2[v93 - 1].dist )
                {
                  v104 = __PAIR__(v100, v98) >> 31;
                  v98 *= 2;
                  v139 = v104;
                  v105 = v95;
                  v95 = v97 - v95 - 1;
                  v103 = v93 - 1;
                  v97 = v105;
                }
                if ( v2[v103].dist > (double)v2[v121].dist )
                {
                  v108 = v2[v103].p;
                  v2[v103].p = v2[v93].p;
                  v2[v93].p = v108;
                  v109 = v2[v103].dist;
                  v2[v103].dist = v2[v93].dist;
                  v110 = v95;
                  v95 = v97 - v95 - 1;
                  v2[v93].dist = v109;
                  v93 = v103;
                  v97 = v110;
                  break;
                }
                v106 = v2[v121].p;
                v2[v121].p = v2[v93].p;
                v2[v93].p = v106;
                v107 = v2[v121].dist;
                v2[v121].dist = v2[v93].dist;
                v2[v93].dist = v107;
                v93 = v121;
              }
            }
            if ( v97 >= 3 )
            {
              do
              {
                v111 = v95 - v97 + v93;
                if ( v2[v93 - 1].dist > (double)v2[v111].dist )
                {
                  v112 = v95;
                  v95 = v97 - v95 - 1;
                  v111 = v93 - 1;
                  v97 = v112;
                }
                if ( v2[v111].dist <= (double)v2[v93].dist )
                  break;
                v113 = v2[v111].p;
                v2[v111].p = v2[v93].p;
                v2[v93].p = v113;
                v114 = v2[v111].dist;
                v115 = v95;
                v2[v111].dist = v2[v93].dist;
                v15 = v95 < 3;
                v2[v93].dist = v114;
                v95 = v97 - v95 - 1;
                v93 = v111;
                v97 = v115;
              }
              while ( !v15 );
            }
            v92 = p_4b;
            v5 = v128;
            v91 = HIDWORD(p_4b);
            v4 = v135;
          }
          v5 = v5 - v4 - 1;
          v134 = v5;
          b_4 = v4;
          v116 = 2 * __PAIR__(v91, v92) + 1;
          v64 = HIDWORD(v116);
          v65 = v116;
        }
        v63 = _na;
        v64 = (__PAIR__(v64, v65--) - 1) >> 32;
        p_4a = __PAIR__(v64, v65);
      }
      while ( _na > 1 );
    }
  }
}
void __thiscall rvSegment::Init(rvSegment *this, rvBSE *effect, rvDeclEffect *effectDecl, int segmentTemplateHandle, float time)
{
  rvSegment *v5; // esi
  rvSegmentTemplate *v6; // edi

  v5 = this;
  this->mSegmentTemplateHandle = segmentTemplateHandle;
  this->mFlags = 0;
  this->mEffectDecl = effectDecl;
  this->mParticleType = effectDecl->mSegmentTemplates.list[segmentTemplateHandle].mParticleTemplate.mType;
  this->mSurfaceIndex = -1;
  v6 = &effectDecl->mSegmentTemplates.list[segmentTemplateHandle];
  if ( v6 )
  {
    this->mActiveCount = 0;
    this->mLastTime = time;
    this->mSecondsPerParticle.y = 0.0;
    this->mSecondsPerParticle.x = 0.0;
    this->mCount.x = 1.0;
    this->mCount.y = 1.0;
    this->mFreqShift = 1.0;
    this->mParticleCount = 0;
    this->mLoopParticleCount = 0;
    this->mParticles = 0;
    this->mSoundVolume = 0.0;
    rvSegment::InitTime(this, effect, v6, time);
    rvSegment::GetSecondsPerParticle(v5, effect, v6, &v6->mParticleTemplate);
  }
}
void __thiscall rvSegment::AddToParticleCount(rvSegment *this, rvBSE *effect, int count, int loopCount, float duration)
{
  rvSegment *v5; // esi
  rvSegmentTemplate *v6; // edi
  int v7; // eax
  int v8; // ebx
  char *v9; // ST0C_4
  const char *v10; // eax
  int v11; // ebx
  char *v12; // ST0C_4
  const char *v13; // eax
  int v14; // eax
  int v15; // eax
  float durationa; // [esp+28h] [ebp+10h]
  float durationb; // [esp+28h] [ebp+10h]
  float durationc; // [esp+28h] [ebp+10h]

  v5 = this;
  v6 = &this->mEffectDecl->mSegmentTemplates.list[this->mSegmentTemplateHandle];
  if ( v6 )
  {
    if ( duration < (double)v6->mParticleTemplate.mDuration.y )
      duration = v6->mParticleTemplate.mDuration.y;
    durationa = duration + 0.01600000075995922;
    durationb = durationa / this->mSecondsPerParticle.y;
    durationc = _ceil(durationb);
    v7 = (signed int)durationc + 1;
    v5->mLoopParticleCount += loopCount * v7;
    v5->mParticleCount += count * v7;
    if ( effect->mFlags & 1 )
    {
      if ( v5->mLoopParticleCount > 2048 )
      {
        v11 = *(_DWORD *)common.type;
        v12 = v6->mSegmentName.data;
        v13 = rvBSE::GetDeclName(effect);
        (*(void (**)(netadrtype_t, const char *, ...))(v11 + 68))(
          common.type,
          "^4BSE:^1 '%s'\nMore than MAX_PARTICLES required for segment %s",
          v13,
          v12);
        v5->mLoopParticleCount = 2048;
      }
    }
    else if ( v5->mParticleCount > 2048 )
    {
      v8 = *(_DWORD *)common.type;
      v9 = v6->mSegmentName.data;
      v10 = rvBSE::GetDeclName(effect);
      (*(void (**)(netadrtype_t, const char *, ...))(v8 + 68))(
        common.type,
        "^4BSE:^1 '%s'\nMore than MAX_PARTICLES required for segment %s",
        v10,
        v9);
      v5->mParticleCount = 2048;
    }
    v14 = count;
    if ( count >= 2048 )
      v14 = 2048;
    v5->mParticleCount = v14;
    v15 = loopCount;
    if ( loopCount >= 2048 )
      v15 = 2048;
    v5->mLoopParticleCount = v15;
  }
}
void __thiscall rvSegment::CalcTrailCounts(rvSegment *this, rvBSE *effect, rvSegmentTemplate *st, rvParticleTemplate *pt, float duration)
{
  int v5; // eax

  v5 = st->mTrailSegmentIndex;
  if ( v5 >= 0 )
    rvSegment::AddToParticleCount(
      &effect->mSegments.list[v5],
      effect,
      this->mParticleCount,
      this->mLoopParticleCount,
      duration);
}
void __thiscall rvSegment::CalcCounts(rvSegment *this, rvBSE *effect, float time)
{
  rvDeclEffect *v3; // ebx
  int v4; // ebp
  bool v5; // zf
  rvSegmentTemplate *v6; // ebp
  int v7; // eax
  int v8; // ecx
  int v9; // edi
  int v10; // esi
  float _X; // ST34_4
  signed int v12; // eax
  float v13; // ST34_4
  double v14; // st7
  double v15; // st7
  float v16; // ST34_4
  bool v17; // sf
  unsigned __int8 v18; // of
  char *v19; // edi
  int v20; // eax
  char *v21; // esi
  int v22; // eax
  int v23; // eax
  int v24; // eax
  rvBSE *v25; // ecx
  float v26; // [esp+14h] [ebp-18h]
  float v27; // [esp+14h] [ebp-18h]
  rvSegment *particleMaxDuration; // [esp+18h] [ebp-14h]
  float effectMinDuration; // [esp+1Ch] [ebp-10h]
  float pt; // [esp+20h] [ebp-Ch]
  float effecta; // [esp+30h] [ebp+4h]

  v3 = this->mEffectDecl;
  v4 = this->mSegmentTemplateHandle;
  v5 = &v3->mSegmentTemplates.list[v4] == 0;
  v6 = &v3->mSegmentTemplates.list[v4];
  particleMaxDuration = this;
  if ( !v5 )
  {
    v7 = v6->mSegType;
    if ( v7 != 4 )
    {
      v8 = v6->mParticleTemplate.mType;
      if ( v8 )
      {
        v26 = 0.0;
        v9 = 0;
        v10 = 0;
        effectMinDuration = v6->mParticleTemplate.mDuration.y + 0.01600000075995922;
        pt = v3->mMinDuration;
        switch ( v7 )
        {
          case 2:
            if ( v8 == 10 )
            {
              v10 = 1;
              v9 = 1;
            }
            else
            {
              v27 = v6->mParticleTemplate.mDuration.y + 0.01600000075995922;
              if ( v6->mLocalDuration.y < (double)effectMinDuration )
                v27 = v6->mLocalDuration.y;
              v26 = v27 + 0.01600000075995922;
              _X = v26 / particleMaxDuration->mSecondsPerParticle.y;
              v12 = (signed int)ceilf(_X);
              v9 = v12 + 1;
              v10 = v12 + 1;
              if ( effectMinDuration > (double)pt )
              {
                v13 = effectMinDuration * (double)(v12 + 1) / pt;
                v10 = (signed int)ceilf(v13) + 1;
              }
            }
            break;
          case 3:
            if ( v8 == 10 )
            {
              v10 = 1;
              v9 = 1;
            }
            else
            {
              v14 = ceilf(particleMaxDuration->mCount.y);
              v9 = (signed int)v14;
              v10 = (signed int)v14;
              v15 = pt;
              if ( pt != 0.0 && !(((unsigned int)v6->mFlags >> 5) & 1) && effectMinDuration > v15 )
              {
                v16 = effectMinDuration / v15;
                v10 = v9 * ((signed int)ceilf(v16) + 1) + 1;
              }
            }
            break;
          case 4:
            break;
          case 6:
          case 7:
            v9 = 1;
            v10 = 1;
            break;
          default:
            v9 = 0;
            v10 = 0;
            break;
        }
        if ( v6->mSegType == 4 )
          goto LABEL_37;
        if ( effect->mFlags & 1 )
        {
          if ( v10 > 2048 )
          {
            v21 = v6->mSegmentName.data;
            v22 = (*(int (**)(void))v3->base->vfptr->gap4)();
            (*(void (**)(netadrtype_t, const char *, ...))(*(_DWORD *)common.type + 68))(
              common.type,
              "^4BSE:^1 '%s'\nMore than MAX_PARTICLES required for segment %s",
              v22,
              v21);
            v10 = 2048;
          }
          v18 = __OFSUB__(v9, 2048);
          v17 = v9 - 2048 < 0;
        }
        else
        {
          v18 = __OFSUB__(v9, 2048);
          v17 = v9 - 2048 < 0;
          if ( v9 > 2048 )
          {
            v19 = v6->mSegmentName.data;
            v20 = (*(int (**)(void))v3->base->vfptr->gap4)();
            (*(void (**)(netadrtype_t, const char *, ...))(*(_DWORD *)common.type + 68))(
              common.type,
              "^4BSE:^1 '%s'\nMore than MAX_PARTICLES required for segment %s",
              v20,
              v19);
            goto LABEL_27;
          }
        }
        if ( v17 ^ v18 )
        {
LABEL_28:
          particleMaxDuration->mParticleCount = v9;
          if ( v10 >= 2048 )
            v10 = 2048;
          particleMaxDuration->mLoopParticleCount = v10;
          if ( ((unsigned int)v6->mFlags >> 2) & 1 )
          {
            if ( !v9 || !v10 )
            {
              v23 = (*(int (**)(void))v3->base->vfptr->gap4)();
              (*(void (**)(netadrtype_t, const char *, ...))(*(_DWORD *)common.type + 68))(
                common.type,
                "^4BSE:^1 '%s'\nSegment with no particles for effect",
                v23);
            }
            v24 = v6->mSegType;
            if ( v24 == 2 || v24 == 3 )
              rvSegment::CalcTrailCounts(particleMaxDuration, effect, v6, &v6->mParticleTemplate, v26);
          }
LABEL_37:
          v25 = effect;
          if ( !(effect->mFlags & 1) )
          {
            effecta = particleMaxDuration->mSegEndTime - time + v6->mParticleTemplate.mDuration.y;
            rvBSE::SetDuration(v25, effecta);
          }
          return;
        }
LABEL_27:
        v9 = 2048;
        goto LABEL_28;
      }
    }
  }
}
void __thiscall rvSegment::ResetTime(rvSegment *this, rvBSE *effect, float time)
{
  rvSegmentTemplate *v3; // eax

  v3 = &this->mEffectDecl->mSegmentTemplates.list[this->mSegmentTemplateHandle];
  if ( v3 )
  {
    if ( !(((unsigned int)v3->mFlags >> 5) & 1) )
      rvSegment::InitTime(this, effect, v3, time);
  }
}
rvParticle *__thiscall rvSegment::InitParticleArray(rvSegment *this, rvBSE *effect)
{
  rvSegment *v2; // edx
  rvParticle *v3; // edi
  int v4; // esi
  int v5; // ecx
  int *v6; // eax
  int v7; // ecx
  int *v8; // eax
  int v9; // ecx
  int *v10; // eax
  int v11; // ecx
  int *v12; // eax
  int v13; // ecx
  int *v14; // eax
  int v15; // ecx
  int *v16; // eax
  int v17; // ecx
  int *v18; // eax
  int v19; // ecx
  int *v20; // eax
  int v21; // ecx
  int *v22; // eax
  int v23; // ecx
  int *v24; // eax
  int v25; // esi
  int i; // ebx
  int v27; // ebp
  int v28; // eax
  rvParticle *result; // eax
  rvSegment *v30; // [esp+10h] [ebp-10h]

  v2 = this;
  v3 = 0;
  v30 = this;
  if ( effect->mFlags & 1 )
    v4 = this->mLoopParticleCount;
  else
    v4 = this->mParticleCount;
  if ( v4 )
  {
    switch ( this->mEffectDecl->mSegmentTemplates.list[this->mSegmentTemplateHandle].mParticleTemplate.mType )
    {
      case 2:
        v5 = 264 * v4 | -(264 * (unsigned __int64)(unsigned int)v4 >> 32 != 0);
        v6 = (int *)operator new((v5 + 4) | -__CFADD__(v5, 4));
        if ( !v6 )
          goto LABEL_26;
        v3 = (rvParticle *)(v6 + 1);
        *v6 = v4;
        `eh vector constructor iterator'(
          v6 + 1,
          0x108u,
          v4,
          (void (__thiscall *)(void *))rvLineParticle::rvLineParticle,
          (void (__thiscall *)(void *))rvLineParticle::~rvLineParticle);
        break;
      case 3:
        v7 = 296 * v4 | -(296 * (unsigned __int64)(unsigned int)v4 >> 32 != 0);
        v8 = (int *)operator new((v7 + 4) | -__CFADD__(v7, 4));
        if ( !v8 )
          goto LABEL_26;
        v3 = (rvParticle *)(v8 + 1);
        *v8 = v4;
        `eh vector constructor iterator'(
          v8 + 1,
          0x128u,
          v4,
          (void (__thiscall *)(void *))rvOrientedParticle::rvOrientedParticle,
          (void (__thiscall *)(void *))rvLineParticle::~rvLineParticle);
        break;
      case 4:
        v9 = 264 * v4 | -(264 * (unsigned __int64)(unsigned int)v4 >> 32 != 0);
        v10 = (int *)operator new((v9 + 4) | -__CFADD__(v9, 4));
        if ( !v10 )
          goto LABEL_26;
        v3 = (rvParticle *)(v10 + 1);
        *v10 = v4;
        `eh vector constructor iterator'(
          v10 + 1,
          0x108u,
          v4,
          (void (__thiscall *)(void *))rvDecalParticle::rvDecalParticle,
          (void (__thiscall *)(void *))rvLineParticle::~rvLineParticle);
        break;
      case 5:
        v11 = 284 * v4 | -(284 * (unsigned __int64)(unsigned int)v4 >> 32 != 0);
        v12 = (int *)operator new((v11 + 4) | -__CFADD__(v11, 4));
        if ( !v12 )
          goto LABEL_26;
        v3 = (rvParticle *)(v12 + 1);
        *v12 = v4;
        `eh vector constructor iterator'(
          v12 + 1,
          0x11Cu,
          v4,
          (void (__thiscall *)(void *))rvModelParticle::rvModelParticle,
          (void (__thiscall *)(void *))rvLineParticle::~rvLineParticle);
        break;
      case 6:
        v13 = 548 * v4 | -(548 * (unsigned __int64)(unsigned int)v4 >> 32 != 0);
        v14 = (int *)operator new((v13 + 4) | -__CFADD__(v13, 4));
        if ( !v14 )
          goto LABEL_26;
        v3 = (rvParticle *)(v14 + 1);
        *v14 = v4;
        `eh vector constructor iterator'(
          v14 + 1,
          0x224u,
          v4,
          (void (__thiscall *)(void *))rvLightParticle::rvLightParticle,
          (void (__thiscall *)(void *))rvLightParticle::~rvLightParticle);
        break;
      case 7:
        v15 = 324 * v4 | -(324 * (unsigned __int64)(unsigned int)v4 >> 32 != 0);
        v16 = (int *)operator new((v15 + 4) | -__CFADD__(v15, 4));
        if ( !v16 )
          goto LABEL_26;
        v3 = (rvParticle *)(v16 + 1);
        *v16 = v4;
        `eh vector constructor iterator'(
          v16 + 1,
          0x144u,
          v4,
          (void (__thiscall *)(void *))rvElectricityParticle::rvElectricityParticle,
          (void (__thiscall *)(void *))rvLineParticle::~rvLineParticle);
        break;
      case 8:
        v17 = 240 * v4 | -(240 * (unsigned __int64)(unsigned int)v4 >> 32 != 0);
        v18 = (int *)operator new((v17 + 4) | -__CFADD__(v17, 4));
        if ( !v18 )
          goto LABEL_26;
        v3 = (rvParticle *)(v18 + 1);
        *v18 = v4;
        `eh vector constructor iterator'(
          v18 + 1,
          0xF0u,
          v4,
          (void (__thiscall *)(void *))rvLinkedParticle::rvLinkedParticle,
          (void (__thiscall *)(void *))rvLineParticle::~rvLineParticle);
        break;
      case 9:
        v19 = 240 * v4 | -(240 * (unsigned __int64)(unsigned int)v4 >> 32 != 0);
        v20 = (int *)operator new((v19 + 4) | -__CFADD__(v19, 4));
        if ( !v20 )
          goto LABEL_26;
        v3 = (rvParticle *)(v20 + 1);
        *v20 = v4;
        `eh vector constructor iterator'(
          v20 + 1,
          0xF0u,
          v4,
          (void (__thiscall *)(void *))sdOrientedLinkedParticle::sdOrientedLinkedParticle,
          (void (__thiscall *)(void *))rvLineParticle::~rvLineParticle);
        break;
      case 0xA:
        v21 = (v4 << 8) | -((unsigned __int64)(unsigned int)v4 >> 24 != 0);
        v22 = (int *)operator new((v21 + 4) | -__CFADD__(v21, 4));
        if ( !v22 )
          goto LABEL_26;
        v3 = (rvParticle *)(v22 + 1);
        *v22 = v4;
        `eh vector constructor iterator'(
          v22 + 1,
          0x100u,
          v4,
          (void (__thiscall *)(void *))rvDebrisParticle::rvDebrisParticle,
          (void (__thiscall *)(void *))rvLineParticle::~rvLineParticle);
        break;
      default:
        v23 = (v4 << 8) | -((unsigned __int64)(unsigned int)v4 >> 24 != 0);
        v24 = (int *)operator new((v23 + 4) | -__CFADD__(v23, 4));
        if ( v24 )
        {
          v3 = (rvParticle *)(v24 + 1);
          *v24 = v4;
          `eh vector constructor iterator'(
            v24 + 1,
            0x100u,
            v4,
            (void (__thiscall *)(void *))rvSpriteParticle::rvSpriteParticle,
            (void (__thiscall *)(void *))rvLineParticle::~rvLineParticle);
        }
        else
        {
LABEL_26:
          v3 = 0;
        }
        break;
    }
    v25 = v4 - 1;
    for ( i = 0; i < v25; *(_DWORD *)(v27 + 4) = (*(int (__thiscall **)(rvParticle *, int))v3->vfptr->gap4)(v3, i) )
      v27 = (*(int (__thiscall **)(rvParticle *, int))v3->vfptr->gap4)(v3, i++);
    v28 = (*(int (__thiscall **)(rvParticle *, int))v3->vfptr->gap4)(v3, v25);
    v2 = v30;
    *(_DWORD *)(v28 + 4) = 0;
  }
  v2->mFreeHead = v3;
  result = v3;
  v2->mUsedHead = 0;
  return result;
}
void __thiscall rvSegment::Sort(rvSegment *this, idVec3 *eyePos)
{
  rvSegment *v2; // ebx
  void *v3; // esp
  rvParticle *v4; // ecx
  signed int v5; // esi
  idVec3 *v6; // edx
  double v7; // st7
  bool v8; // al
  int v9; // ecx
  float *v10; // eax
  int v11; // esi
  int v12; // [esp+0h] [ebp-24h]
  rvParticle *v13; // [esp+4h] [ebp-20h]
  float v14; // [esp+Ch] [ebp-18h]
  float v15; // [esp+10h] [ebp-14h]
  float v16; // [esp+14h] [ebp-10h]
  float v17; // [esp+1Ch] [ebp-8h]
  char v18; // [esp+23h] [ebp-1h]
  float v19; // [esp+2Ch] [ebp+8h]

  v2 = this;
  v3 = alloca(8 * this->mActiveCount);
  v4 = this->mUsedHead;
  v17 = -1.0;
  v5 = 0;
  v18 = 1;
  if ( v4 )
  {
    v6 = eyePos;
    do
    {
      v14 = v4->mPosition.x - v6->x;
      v15 = v4->mPosition.y - v6->y;
      v7 = v4->mPosition.z - v6->z;
      *(&v12 + 2 * v5 + 1) = (int)v4;
      v16 = v7;
      v19 = v14 * v14 + v15 * v15 + v16 * v16;
      *((float *)&v12 + 2 * v5) = v19;
      v8 = v17 <= (double)v19;
      v4 = v4->mNext;
      v17 = v19;
      v18 &= v8;
      ++v5;
    }
    while ( v4 );
    if ( !v18 )
    {
      smoothsort<particlesort>((particlesort *)&v12, v5);
      v9 = (int)v13;
      v2->mUsedHead = v13;
      if ( v5 > 1 )
      {
        v10 = &v14;
        v11 = v5 - 1;
        do
        {
          *(float *)(v9 + 4) = *v10;
          v9 = *(_DWORD *)v10;
          v10 += 2;
          --v11;
        }
        while ( v11 );
      }
      *(_DWORD *)(v9 + 4) = 0;
    }
  }
}
void __thiscall rvSegment::InitParticles(rvSegment *this, rvBSE *effect)
{
  rvSegment *v2; // esi

  v2 = this;
  if ( &this->mEffectDecl->mSegmentTemplates.list[this->mSegmentTemplateHandle] )
  {
    this->mParticles = rvSegment::InitParticleArray(this, effect);
    v2->mActiveCount = 0;
  }
}
void __userpurge rvSegment::CreateDecal(rvSegment *this@<ecx>, int a2@<ebp>, int a3@<edi>, int a4@<esi>, rvBSE *effect, float time, int a7, int a8, float a9)
{
  rvSegment *v9; // ebx
  float v10; // ecx
  float v11; // edx
  float v12; // ST64_4
  float v13; // ST68_4
  float st; // ST6C_4
  float v15; // ST68_4
  float v16; // ST64_4
  float v17; // ST64_4
  float v18; // esi
  rvParticle *v19; // eax
  signed int v20; // eax
  int v21; // eax
  double v22; // st7
  float *v23; // eax
  double v24; // st7
  unsigned int v25; // eax
  double v26; // st7
  float *v27; // eax
  double v28; // st7
  double v29; // st6
  double v30; // st5
  float v31; // ST68_4
  float v32; // ST64_4
  float v33; // ST6C_4
  float v34; // ST68_4
  float v35; // ST64_4
  float v36; // ST64_4
  float v37; // ST6C_4
  float v38; // ST64_4
  float v39; // ST68_4
  float v40; // ST64_4
  float v41; // ST6C_4
  float v42; // ST68_4
  float v43; // ST64_4
  float v44; // ST6C_4
  float v45; // ST68_4
  float v46; // eax
  float v47; // ST64_4
  float v48; // ST6C_4
  double v49; // st5
  double v50; // st7
  float v51; // ST64_4
  float v52; // ST68_4
  double v53; // st6
  float v54; // ST64_4
  double v55; // st5
  float v56; // ST64_4
  float v57; // ST6C_4
  float v58; // ST68_4
  float v59; // ST64_4
  float v60; // ST6C_4
  float v61; // ST68_4
  float v62; // ST64_4
  float v63; // ST6C_4
  float v64; // ST68_4
  float v65; // ST64_4
  float v66; // ST6C_4
  float v67; // ST68_4
  float v68; // ST64_4
  float v69; // ST6C_4
  float v70; // ST68_4
  int v71; // eax
  float v72; // edx
  float v73; // ecx
  float v74; // edx
  float v75; // ecx
  float v76; // ST64_4
  float v77; // ST6C_4
  float v78; // ST68_4
  float v79; // ST64_4
  float v80; // ST6C_4
  float v81; // ST68_4
  float v82; // ST64_4
  float v83; // ST6C_4
  float v84; // ST68_4
  int v85; // eax
  float v86; // edx
  float v87; // ecx
  float v88; // edx
  float v89; // ecx
  float v90; // ST64_4
  float v91; // ST6C_4
  float v92; // ST68_4
  float v93; // ST64_4
  float v94; // ST6C_4
  float v95; // ST68_4
  float v96; // ST64_4
  float v97; // ST6C_4
  float v98; // ST68_4
  int v99; // eax
  float v100; // edx
  float v101; // ecx
  float v102; // edx
  float v103; // ecx
  float v104; // ST64_4
  float v105; // ST6C_4
  float v106; // ST68_4
  float v107; // ST64_4
  float v108; // ST6C_4
  float v109; // ST68_4
  float v110; // ST64_4
  float v111; // ST6C_4
  float v112; // ST68_4
  int v113; // eax
  float v114; // edx
  float v115; // ecx
  float v116; // edx
  float v117; // ecx
  float v118; // ST64_4
  int v119; // ST48_4
  int v120; // [esp+4Ch] [ebp-614h]
  idMat3 axis; // [esp+70h] [ebp-5F0h]
  float fovx; // [esp+94h] [ebp-5CCh]
  float asin; // [esp+98h] [ebp-5C8h]
  float acos; // [esp+9Ch] [ebp-5C4h]
  float v125; // [esp+A0h] [ebp-5C0h]
  idVec3 swap; // [esp+A8h] [ebp-5B8h]
  float v127; // [esp+B4h] [ebp-5ACh]
  float v128; // [esp+B8h] [ebp-5A8h]
  float v129; // [esp+BCh] [ebp-5A4h]
  float v130; // [esp+C0h] [ebp-5A0h]
  float v131; // [esp+C4h] [ebp-59Ch]
  float v132; // [esp+C8h] [ebp-598h]
  idVec3 size; // [esp+CCh] [ebp-594h]
  idVec3 projectionOrigin; // [esp+DCh] [ebp-584h]
  idVec3 viewOrg; // [esp+E8h] [ebp-578h]
  idVec3 origin; // [esp+F4h] [ebp-56Ch]
  idVec3 rotation; // [esp+100h] [ebp-560h]
  idVec4 tint; // [esp+10Ch] [ebp-554h]
  idMat3 viewAxis; // [esp+11Ch] [ebp-544h]
  idFixedWinding winding; // [esp+140h] [ebp-520h]
  float v141; // [esp+65Ch] [ebp-4h]

  v9 = this;
  if ( !(_S7 & 1) )
  {
    _S7 |= 1u;
    decalWinding[0].x = 1.0;
    flt_126D0A4 = 1.0;
    flt_126D0A8 = 0.0;
    flt_126D0AC = -1.0;
    flt_126D0B8 = -1.0;
    flt_126D0BC = -1.0;
    flt_126D0C8 = -1.0;
    flt_126D0B0 = 1.0;
    flt_126D0C4 = 1.0;
    flt_126D0B4 = 0.0;
    flt_126D0C0 = 0.0;
    flt_126D0CC = 0.0;
  }
  if ( bse_render.internalVar->integerValue )
  {
    LODWORD(axis.mat[0].x) = &this->mEffectDecl->mSegmentTemplates.list[this->mSegmentTemplateHandle];
    if ( LODWORD(axis.mat[0].x) )
    {
      v10 = effect->mCurrentOrigin.x;
      v11 = effect->mCurrentOrigin.y;
      v120 = a4;
      rotation.x = effect->mCurrentOrigin.z;
      origin.y = v10;
      qmemcpy(&axis.mat[0].y, &effect->mCurrentAxis, sizeof(idMat3));
      origin.z = v11;
      ((void (__stdcall *)(float *, float *, float *))game->vfptr->GetPlayerView)(&viewOrg.y, &viewAxis.mat[0].y, &asin);
      v12 = viewOrg.y - origin.y;
      v13 = viewOrg.z - origin.z;
      st = origin.x - rotation.x;
      v15 = v12 * v12 + v13 * v13 + st * st;
      v16 = 90.0 / asin * bse_maxDecalDistance.internalVar->floatValue;
      v17 = v16 * v16;
      if ( v17 >= (double)v15 )
      {
        winding.numPoints = (int)&idFixedWinding::`vftable';
        winding.p = 0;
        winding.allocedSize = (int)&winding.data[0].y;
        LODWORD(winding.data[0].x) = 64;
        v18 = axis.mat[0].x;
        v141 = 0.0;
        v19 = rvSegment::SpawnParticle(
                v9,
                effect,
                (rvSegmentTemplate *)LODWORD(axis.mat[0].x),
                time,
                &vec3_origin,
                &mat3_identity);
        ((void (__thiscall *)(rvParticle *, float *, float *, float *, int, int, int))v19->vfptr->GetSpawnInfo)(
          v19,
          &tint.y,
          &size.y,
          &rotation.y,
          a3,
          v120,
          a2);
        if ( 0.0 == projectionOrigin.z )
          projectionOrigin.z = 8.0;
        axis.mat[0].z = tint.y;
        LODWORD(axis.mat[0].y) = (char *)&swap + 4;
        swap.y = cos(tint.y);
        swap.x = sin(tint.y);
        v20 = *(_DWORD *)(LODWORD(axis.mat[0].x) + 400);
        if ( v20 >= 6 )
        {
          v43 = -axis.mat[1].y;
          v128 = asin;
          v129 = acos;
          v44 = -axis.mat[1].z;
          v130 = v125;
          v45 = -axis.mat[2].x;
          asin = v43;
          acos = v44;
          v125 = v45;
          axis.mat[1].y = v128;
          axis.mat[1].z = v129;
          axis.mat[2].x = v130;
          v28 = v44;
          v29 = v43;
        }
        else
        {
          if ( v20 < 3 )
          {
            v25 = v20;
            v26 = axis.mat[v25 + 1].y;
            v27 = &axis.mat[v25 + 1].y;
            asin = v26;
            acos = v27[1];
            v24 = v27[2];
          }
          else
          {
            v21 = 3 * v20 - 9;
            v22 = *(&axis.mat[1].y + v21);
            v23 = &axis.mat[1].y + v21;
            axis.mat[0].x = -v22;
            axis.mat[0].y = -v23[1];
            axis.mat[0].z = -v23[2];
            asin = axis.mat[0].x;
            acos = axis.mat[0].y;
            v24 = axis.mat[0].z;
          }
          v125 = v24;
          axis.mat[0].x = v125 * v125 + asin * asin + acos * acos;
          axis.mat[0].x = sqrt(axis.mat[0].x);
          if ( axis.mat[0].x >= 0.00000011920929 )
          {
            axis.mat[0].x = 1.0 / axis.mat[0].x;
            asin = asin * axis.mat[0].x;
            acos = acos * axis.mat[0].x;
            v125 = axis.mat[0].x * v125;
          }
          v28 = acos;
          v29 = asin;
          axis.mat[0].y = acos * acos + asin * asin;
          v30 = 0.0;
          if ( axis.mat[0].y == 0.0 )
          {
            v128 = 1.0;
          }
          else
          {
            axis.mat[0].x = sqrt(axis.mat[0].y);
            axis.mat[0].y = 1.0 / axis.mat[0].x;
            v28 = acos;
            v128 = -acos * axis.mat[0].y;
            v30 = axis.mat[0].y * asin;
            v29 = asin;
          }
          v129 = v30;
          axis.mat[0].y = v125 * v129 - v28 * 0.0;
          axis.mat[0].z = v29 * 0.0 - v125 * v128;
          axis.mat[1].x = v28 * v128 - v29 * v129;
          axis.mat[0].x = -swap.x;
          v127 = axis.mat[0].y * axis.mat[0].x;
          v31 = axis.mat[0].z * axis.mat[0].x;
          v32 = axis.mat[0].x * axis.mat[1].x;
          swap.z = v128 * swap.y;
          viewOrg.x = v129 * swap.y;
          v33 = swap.y * 0.0;
          v127 = swap.z + v127;
          v34 = viewOrg.x + v31;
          v35 = v33 + v32;
          axis.mat[1].y = v127;
          axis.mat[1].z = v34;
          axis.mat[2].x = v35;
          v36 = -swap.y;
          v37 = v36 * axis.mat[0].y;
          viewOrg.x = v36 * axis.mat[0].z;
          swap.z = v36 * axis.mat[1].x;
          v38 = v128 * axis.mat[0].x;
          v39 = v129 * axis.mat[0].x;
          v127 = axis.mat[0].x * 0.0;
          v40 = v38 + v37;
          v41 = v39 + viewOrg.x;
          v42 = v127 + swap.z;
          axis.mat[2].y = v40;
          axis.mat[2].z = v41;
          fovx = v42;
        }
        v46 = 0.0;
        winding.data[0].y = 0.0;
        v47 = v29 * projectionOrigin.z;
        v48 = v47;
        v49 = v28 * projectionOrigin.z;
        v50 = v47;
        v51 = v49;
        v52 = v51;
        v53 = v51;
        v54 = projectionOrigin.z * v125;
        v55 = v54;
        axis.mat[1].x = v48 + rotation.y;
        axis.mat[0].z = rotation.z + v52;
        axis.mat[0].y = tint.x + v54;
        v56 = v50;
        v57 = v53;
        v58 = v55;
        v59 = rotation.y - v56;
        v60 = rotation.z - v57;
        v61 = tint.x - v58;
        viewOrg.y = v59;
        viewOrg.z = v60;
        origin.x = v61;
        v62 = asin * flt_126D0A8 + axis.mat[1].y * decalWinding[0].x + axis.mat[2].y * flt_126D0A4;
        v63 = axis.mat[2].z * flt_126D0A4 + axis.mat[1].z * decalWinding[0].x + acos * flt_126D0A8;
        v64 = decalWinding[0].x * axis.mat[2].x + flt_126D0A4 * fovx + flt_126D0A8 * v125;
        v65 = projectionOrigin.x * v62;
        v66 = projectionOrigin.x * v63;
        v67 = projectionOrigin.x * v64;
        v68 = v65 + axis.mat[1].x;
        v69 = v66 + axis.mat[0].z;
        v70 = v67 + axis.mat[0].y;
        v128 = v68;
        v129 = v69;
        v130 = v70;
        v131 = 1.0;
        v132 = 1.0;
        if ( SLODWORD(winding.data[0].s) < 1 )
        {
          if ( !(*(unsigned __int8 (__thiscall **)(idVec5 *, signed int, signed int))(LODWORD(winding.data[0].x) + 8))(
                  winding.data,
                  1,
                  1) )
            goto LABEL_23;
          v46 = winding.data[0].y;
        }
        v71 = LODWORD(winding.data[0].z) + 20 * LODWORD(v46);
        v72 = v129;
        *(float *)v71 = v128;
        v73 = v130;
        *(float *)(v71 + 4) = v72;
        v74 = v131;
        *(float *)(v71 + 8) = v73;
        v75 = v132;
        *(float *)(v71 + 12) = v74;
        *(float *)(v71 + 16) = v75;
        ++LODWORD(winding.data[0].y);
LABEL_23:
        v76 = asin * flt_126D0B4 + axis.mat[1].y * flt_126D0AC + axis.mat[2].y * flt_126D0B0;
        v77 = axis.mat[2].z * flt_126D0B0 + axis.mat[1].z * flt_126D0AC + acos * flt_126D0B4;
        v78 = flt_126D0AC * axis.mat[2].x + flt_126D0B0 * fovx + flt_126D0B4 * v125;
        v79 = projectionOrigin.x * v76;
        v80 = projectionOrigin.x * v77;
        v81 = projectionOrigin.x * v78;
        v82 = v79 + axis.mat[1].x;
        v83 = v80 + axis.mat[0].z;
        v84 = v81 + axis.mat[0].y;
        v128 = v82;
        v129 = v83;
        v130 = v84;
        v131 = 0.0;
        v132 = 1.0;
        if ( LODWORD(winding.data[0].y) + 1 <= SLODWORD(winding.data[0].s)
          || (*(unsigned __int8 (__thiscall **)(idVec5 *, int, signed int))(LODWORD(winding.data[0].x) + 8))(
               winding.data,
               LODWORD(winding.data[0].y) + 1,
               1) )
        {
          v85 = LODWORD(winding.data[0].z) + 20 * LODWORD(winding.data[0].y);
          v86 = v129;
          *(float *)v85 = v128;
          v87 = v130;
          *(float *)(v85 + 4) = v86;
          v88 = v131;
          *(float *)(v85 + 8) = v87;
          v89 = v132;
          *(float *)(v85 + 12) = v88;
          *(float *)(v85 + 16) = v89;
          ++LODWORD(winding.data[0].y);
        }
        v90 = asin * flt_126D0C0 + axis.mat[1].y * flt_126D0B8 + axis.mat[2].y * flt_126D0BC;
        v91 = axis.mat[2].z * flt_126D0BC + axis.mat[1].z * flt_126D0B8 + acos * flt_126D0C0;
        v92 = flt_126D0B8 * axis.mat[2].x + flt_126D0BC * fovx + flt_126D0C0 * v125;
        v93 = projectionOrigin.x * v90;
        v94 = projectionOrigin.x * v91;
        v95 = projectionOrigin.x * v92;
        v96 = v93 + axis.mat[1].x;
        v97 = v94 + axis.mat[0].z;
        v98 = v95 + axis.mat[0].y;
        v128 = v96;
        v129 = v97;
        v130 = v98;
        v131 = 0.0;
        v132 = 0.0;
        if ( LODWORD(winding.data[0].y) + 1 <= SLODWORD(winding.data[0].s)
          || (*(unsigned __int8 (__thiscall **)(idVec5 *, int, signed int))(LODWORD(winding.data[0].x) + 8))(
               winding.data,
               LODWORD(winding.data[0].y) + 1,
               1) )
        {
          v99 = LODWORD(winding.data[0].z) + 20 * LODWORD(winding.data[0].y);
          v100 = v129;
          *(float *)v99 = v128;
          v101 = v130;
          *(float *)(v99 + 4) = v100;
          v102 = v131;
          *(float *)(v99 + 8) = v101;
          v103 = v132;
          *(float *)(v99 + 12) = v102;
          *(float *)(v99 + 16) = v103;
          ++LODWORD(winding.data[0].y);
        }
        v104 = asin * flt_126D0CC + axis.mat[1].y * flt_126D0C4 + axis.mat[2].y * flt_126D0C8;
        v105 = axis.mat[2].z * flt_126D0C8 + axis.mat[1].z * flt_126D0C4 + acos * flt_126D0CC;
        v106 = flt_126D0C4 * axis.mat[2].x + flt_126D0C8 * fovx + flt_126D0CC * v125;
        v107 = projectionOrigin.x * v104;
        v108 = projectionOrigin.x * v105;
        v109 = projectionOrigin.x * v106;
        v110 = v107 + axis.mat[1].x;
        v111 = v108 + axis.mat[0].z;
        v112 = v109 + axis.mat[0].y;
        v128 = v110;
        v129 = v111;
        v130 = v112;
        v131 = 1.0;
        v132 = 0.0;
        if ( LODWORD(winding.data[0].y) + 1 <= SLODWORD(winding.data[0].s)
          || (*(unsigned __int8 (__thiscall **)(idVec5 *, int, signed int))(LODWORD(winding.data[0].x) + 8))(
               winding.data,
               LODWORD(winding.data[0].y) + 1,
               1) )
        {
          v113 = LODWORD(winding.data[0].z) + 20 * LODWORD(winding.data[0].y);
          v114 = v129;
          *(float *)v113 = v128;
          v115 = v130;
          *(float *)(v113 + 4) = v114;
          v116 = v131;
          *(float *)(v113 + 8) = v115;
          v117 = v132;
          *(float *)(v113 + 12) = v116;
          *(float *)(v113 + 16) = v117;
          ++LODWORD(winding.data[0].y);
        }
        v118 = 1000.0 * a9;
        LODWORD(axis.mat[0].y) = (signed int)v118;
        v119 = *(_DWORD *)(LODWORD(v18) + 92);
        ((void (__stdcall *)(idVec5 *, float *, signed int, _DWORD))session->rw->vfptr->ProjectDecalOntoWorld)(
          winding.data,
          &viewOrg.y,
          1,
          LODWORD(projectionOrigin.z));
        v141 = -6.8056469e38/*NaN*/;
        winding.allocedSize = 0;
        winding.numPoints = (int)&idWinding::`vftable';
        operator delete(0);
        return;
      }
    }
  }
}
void __thiscall rvSegment::Render(rvSegment *this, rvBSE *effect, renderEffect_s *owner, idRenderModel *model, float time)
{
  rvDeclEffect *v5; // edx
  int v6; // eax
  bool v7; // zf
  rvSegmentTemplate *v8; // eax
  int v9; // ecx
  modelSurface_s *v10; // eax
  srfTriangles_t *v11; // edi
  int v12; // ecx
  void (__stdcall *v13)(idMat3 *, idVec3 *, float *); // edx
  double v14; // st7
  rvSegment *v15; // esi
  double v16; // st6
  unsigned __int8 v17; // al
  void *v18; // esp
  int v19; // eax
  int v20; // eax
  rvParticle *v21; // ebx
  double v22; // st7
  rvParticleTemplate *v23; // esi
  bool (__thiscall *v24)(rvParticle *, rvBSE *, rvParticleTemplate *, idMat3 *, srfTriangles_t *, float, float); // edx
  float v25; // ST3C_4
  bool v26; // sf
  int v27; // eax
  rvParticle *v28; // ebx
  int v29; // ecx
  int v30; // ecx
  int v31; // [esp+44h] [ebp-4Ch]
  float modelMatrix[16]; // [esp+50h] [ebp-40h]
  idMat3 view; // [esp+90h] [ebp+0h]
  float v34; // [esp+B4h] [ebp+24h]
  float v35; // [esp+B8h] [ebp+28h]
  float v36; // [esp+BCh] [ebp+2Ch]
  modelSurface_s *surf; // [esp+C0h] [ebp+30h]
  float v38; // [esp+C4h] [ebp+34h]
  int startIdx; // [esp+C8h] [ebp+38h]
  rvParticleTemplate *pt; // [esp+CCh] [ebp+3Ch]
  rvSegmentTemplate *st; // [esp+D0h] [ebp+40h]
  int startVtx; // [esp+D4h] [ebp+44h]
  float v43; // [esp+D8h] [ebp+48h]
  float v44; // [esp+DCh] [ebp+4Ch]
  float v45; // [esp+E0h] [ebp+50h]
  rvSegment *v46; // [esp+E4h] [ebp+54h]
  int numAllocedIndices; // [esp+E8h] [ebp+58h]
  int numAllocedVerts; // [esp+ECh] [ebp+5Ch]
  rvParticle **inverseList; // [esp+F0h] [ebp+60h]
  int numRender; // [esp+F4h] [ebp+64h]
  int i; // [esp+104h] [ebp+74h]
  int ia; // [esp+104h] [ebp+74h]
  float modela; // [esp+108h] [ebp+78h]
  unsigned __int8 model_3; // [esp+10Bh] [ebp+7Bh]

  v5 = this->mEffectDecl;
  v6 = this->mSegmentTemplateHandle;
  v7 = &v5->mSegmentTemplates.list[v6] == 0;
  v8 = &v5->mSegmentTemplates.list[v6];
  v46 = this;
  st = v8;
  if ( !v7 )
  {
    v9 = this->mSurfaceIndex;
    pt = &v8->mParticleTemplate;
    v10 = model->vfptr->Surface(model, v9);
    v11 = v10->geometry;
    v12 = v11->numIndexes;
    surf = v10;
    startVtx = v11->numVerts;
    startIdx = v12;
    v13 = *(void (__stdcall **)(idMat3 *, idVec3 *, float *))&renderUtilities->vfptr->gap4[4];
    numRender = 0;
    v13(&owner->axis, &owner->origin, modelMatrix);
    (*(void (__stdcall **)(float *, idVec3 *, idVec3 *))&renderUtilities->vfptr->gap4[12])(
      modelMatrix,
      &effect->mViewAxis.mat[1],
      &view.mat[1]);
    (*(void (__stdcall **)(float *, idVec3 *, idVec3 *))&renderUtilities->vfptr->gap4[12])(
      modelMatrix,
      &effect->mViewAxis.mat[2],
      &view.mat[2]);
    v43 = effect->mViewOrg.x - owner->origin.x;
    v44 = effect->mViewOrg.y - owner->origin.y;
    v45 = effect->mViewOrg.z - owner->origin.z;
    v35 = owner->axis.mat[2].z;
    v34 = owner->axis.mat[1].z;
    numAllocedIndices = SLODWORD(owner->axis.mat[0].z);
    v36 = owner->axis.mat[2].y;
    numAllocedVerts = SLODWORD(owner->axis.mat[1].y);
    modela = owner->axis.mat[0].y;
    v38 = owner->axis.mat[2].x;
    inverseList = (rvParticle **)LODWORD(owner->axis.mat[1].x);
    v14 = v44;
    v15 = v46;
    v16 = v43;
    v43 = owner->axis.mat[0].x * v43 + v44 * modela + v45 * *(float *)&numAllocedIndices;
    v44 = v44 * *(float *)&numAllocedVerts + *(float *)&inverseList * v16 + v45 * v34;
    v45 = v45 * v35 + v16 * v38 + v14 * v36;
    view.mat[0].x = v43;
    view.mat[0].y = v44;
    view.mat[0].z = v45;
    if ( r_sortParticles.internalVar->integerValue
      && ((unsigned int)v46->mEffectDecl->mSegmentTemplates.list[v46->mSegmentTemplateHandle].mFlags >> 10) & 1 )
    {
      rvSegment::Sort(v46, view.mat);
    }
    v17 = ((unsigned int)st->mFlags >> 11) & 1;
    *(float *)&inverseList = 0.0;
    i = 0;
    model_3 = v17;
    if ( v17 )
    {
      v18 = alloca(4 * v15->mActiveCount);
      *(float *)&inverseList = COERCE_FLOAT(&v31);
    }
    v19 = v11->numAllocedVerts;
    numAllocedVerts = 9500;
    if ( v19 <= 9500 )
      numAllocedVerts = v19;
    v20 = v11->numAllocedIndices;
    numAllocedIndices = 29500;
    if ( v20 <= 29500 )
      numAllocedIndices = v20;
    v21 = v15->mUsedHead;
    v22 = time;
    if ( v21 )
    {
      while ( 1 )
      {
        if ( ((unsigned int)st->mFlags >> 5) & 1 )
          v21->mEndTime = v22 + 1.0;
        v23 = pt;
        if ( v11->numVerts + pt->mVertexCount > numAllocedVerts || v11->numIndexes + pt->mIndexCount > numAllocedIndices )
          break;
        if ( model_3 )
        {
          inverseList[i++] = v21;
        }
        else
        {
          v24 = v21->vfptr->Render;
          ++numRender;
          v25 = v22;
          if ( ((unsigned __int8 (__thiscall *)(rvParticle *, rvBSE *, rvParticleTemplate *, idMat3 *, srfTriangles_t *, _DWORD, _DWORD))v24)(
                 v21,
                 effect,
                 pt,
                 &view,
                 v11,
                 LODWORD(v25),
                 1.0)
            && v23->mTrailInfo->mTrailType == 1 )
          {
            ((void (__thiscall *)(rvParticle *, rvBSE *, rvParticleTemplate *, idMat3 *, srfTriangles_t *, _DWORD))v21->vfptr->RenderBurnTrail)(
              v21,
              effect,
              v23,
              &view,
              v11,
              LODWORD(time));
          }
        }
        v21 = v21->mNext;
        if ( !v21 )
        {
          v23 = pt;
          break;
        }
        v22 = time;
      }
    }
    else
    {
      v23 = pt;
    }
    if ( model_3 )
    {
      v26 = i - 1 < 0;
      v27 = i - 1;
      ia = i - 1;
      if ( !v26 )
      {
        numRender += v27 + 1;
        while ( 1 )
        {
          v28 = inverseList[v27];
          if ( ((unsigned __int8 (__thiscall *)(rvParticle *, rvBSE *, rvParticleTemplate *, idMat3 *, srfTriangles_t *, _DWORD, _DWORD))v28->vfptr->Render)(
                 v28,
                 effect,
                 v23,
                 &view,
                 v11,
                 LODWORD(time),
                 1.0)
            && v23->mTrailInfo->mTrailType == 1 )
          {
            ((void (__thiscall *)(rvParticle *, rvBSE *, rvParticleTemplate *, idMat3 *, srfTriangles_t *, _DWORD))v28->vfptr->RenderBurnTrail)(
              v28,
              effect,
              v23,
              &view,
              v11,
              LODWORD(time));
          }
          if ( --ia < 0 )
            break;
          v27 = ia;
        }
      }
    }
    v29 = v46->mActiveCount;
    if ( v11->numVerts > v29 * v23->mVertexCount )
      (*(void (**)(netadrtype_t, const char *, ...))(*(_DWORD *)common.type + 68))(
        common.type,
        "rvSegment::Render - tri->numVerts > pt->GetVertexCount() * mActiveCount ( [%d %d] [%d %d] [%d %d] [%d %d %d] )",
        startVtx,
        startIdx,
        v11->numVerts,
        v11->numIndexes,
        numRender,
        v29,
        v23->mIndexCount,
        v23->mVertexCount,
        model_3);
    v30 = v46->mActiveCount;
    if ( v11->numIndexes > v30 * v23->mIndexCount )
      (*(void (**)(netadrtype_t, const char *, ...))(*(_DWORD *)common.type + 68))(
        common.type,
        "rvSegment::Render - tri->numIndexes > pt->GetIndexCount() * mActiveCount ( [%d %d] [%d %d] [%d %d] [%d %d %d] )",
        startVtx,
        startIdx,
        v11->numVerts,
        v11->numIndexes,
        numRender,
        v30,
        v23->mIndexCount,
        v23->mVertexCount,
        model_3);
    (*(void (__stdcall **)(srfTriangles_t *))&renderUtilities->vfptr->gap4[8])(surf->geometry);
  }
}
bool __userpurge rvSegment::Check@<al>(rvSegment *this@<ecx>, int a2@<ebx>, int a3@<edi>, rvBSE *effect, float time, float offset)
{
  rvSegment *v6; // esi
  bool v7; // zf
  rvBSE *v8; // ebp
  int v9; // eax
  rvSegmentTemplate *v10; // edi
  double v11; // st7
  double i; // st6
  float v13; // ST24_4
  double v14; // st7
  int v15; // eax
  idSoundEmitter *v16; // ebx
  void (__thiscall **v17)(idSoundEmitter *, idSoundShader *, int, int, _DWORD, signed int); // ebp
  float min; // ST0C_4
  void (__thiscall **v19)(_DWORD, _DWORD, _DWORD, _DWORD, _DWORD, _DWORD); // ebp
  float v20; // ST0C_4
  void (__thiscall **v21)(rvBSEManager *, float, float); // ebx
  double v22; // st7
  float v23; // ST0C_4
  float depthOffset; // [esp+8h] [ebp-18h]
  int v26; // [esp+10h] [ebp-10h]
  float v27; // [esp+14h] [ebp-Ch]
  float v28; // [esp+1Ch] [ebp-4h]
  float delta; // [esp+24h] [ebp+4h]
  float timea; // [esp+28h] [ebp+8h]
  int timeb; // [esp+28h] [ebp+8h]
  int timec; // [esp+28h] [ebp+8h]
  float timed; // [esp+28h] [ebp+8h]
  float timee; // [esp+28h] [ebp+8h]
  float offseta; // [esp+2Ch] [ebp+Ch]

  v6 = this;
  v7 = (this->mFlags & 1) == 0;
  v28 = this->mLastTime;
  this->mLastTime = time;
  if ( !v7 || (v8 = effect, ((unsigned int)effect->mFlags >> 3) & 1) )
  {
    LOBYTE(v9) = 1;
  }
  else if ( this->mSegStartTime <= (double)time )
  {
    v26 = a3;
    v10 = &this->mEffectDecl->mSegmentTemplates.list[this->mSegmentTemplateHandle];
    if ( v10 && !rvSegmentTemplate::DetailCull(v10) )
    {
      switch ( v10->mSegType )
      {
        case 1:
          if ( !(v10->mFlags & 1) )
            goto $LN1_8;
          rvSegment::PlayEffect(effect, v10, offset);
          v6->mFlags |= 1u;
          return v6->mFlags & 1;
        case 2:
          if ( !(unsigned __int8)rvBSE::CanInterpolate(effect) )
            goto LABEL_40;
          timea = time + 0.01600000075995922;
          v11 = timea;
          if ( v6->mSegEndTime - 0.002000000094994903 <= timea )
          {
            timea = v6->mSegEndTime;
            v11 = timea;
          }
          delta = v28 - v11;
          for ( i = delta; delta < 0.0; v11 = timea )
          {
            if ( v6->mSegStartTime - v11 <= i )
            {
              offseta = v11 + i;
              rvSegment::SpawnParticle(v6, v8, v10, offseta, &vec3_origin, &mat3_identity);
            }
            delta = rvSegment::AttenuateInterval(v6, v8, v10) + delta;
            i = delta;
          }
          if ( v6->mSegEndTime - 0.002000000094994903 <= v11 )
            v6->mFlags |= 1u;
          v9 = v6->mFlags;
          v13 = i + v11;
          v6->mLastTime = v13;
          LOBYTE(v9) = v9 & 1;
          return v9;
        case 3:
          v14 = rvSegment::AttenuateCount(effect, v10, v6->mCount.x, v6->mCount.y);
          v15 = (signed int)v14;
          if ( (signed int)v14 > 2048 )
          {
            (*(void (**)(netadrtype_t, const char *, ...))(*(_DWORD *)common.type + 68))(
              common.type,
              "^4BSE: Exceed MAX_PARTICLES ( tried to spawn %i )",
              (signed int)v14);
            v15 = 1024;
          }
          rvSegment::SpawnParticles(v6, effect, v10, v6->mSegStartTime, v15);
          v6->mFlags |= 1u;
          return v6->mFlags & 1;
        case 4:
        case 8:
          goto $LN1_8;
        case 5:
          if ( !(v10->mFlags & 1) )
            goto $LN1_8;
          v16 = effect->mReferenceSound;
          if ( !v16 )
            goto $LN1_8;
          v6->mSoundVolume = rvSegmentTemplate::GetSoundVolume(v10);
          v6->mFreqShift = rvSegmentTemplate::GetFreqShift(v10);
          rvBSE::UpdateSoundEmitter(effect, v10, v6);
          if ( rvSegmentTemplate::GetSoundLooping(v10) )
          {
            if ( ((unsigned int)v6->mFlags >> 1) & 1 )
            {
$LN1_8:
              v6->mFlags |= 1u;
LABEL_40:
              v9 = v6->mFlags & 1;
            }
            else
            {
              v6->mFlags |= 2u;
              timeb = v6->mSegmentTemplateHandle + 1;
              v17 = (void (__thiscall **)(idSoundEmitter *, idSoundShader *, int, int, _DWORD, signed int))&v16->vfptr->StartSound;
              min = rvRandom::flrand(0.0, 1.0);
              (*v17)(v16, v10->mSoundShader, timeb, timeb, LODWORD(min), 32);
              v6->mFlags |= 1u;
              v9 = v6->mFlags & 1;
            }
          }
          else
          {
            timec = v6->mSegmentTemplateHandle + 1;
            v19 = (void (__thiscall **)(_DWORD, _DWORD, _DWORD, _DWORD, _DWORD, _DWORD))&v16->vfptr->StartSound;
            v20 = rvRandom::flrand(0.0, 1.0);
            (*v19)(v16, v10->mSoundShader, timec, timec, LODWORD(v20), 0);
            v6->mFlags |= 1u;
            v9 = v6->mFlags & 1;
          }
          break;
        case 6:
          if ( !(v10->mFlags & 1) )
            goto $LN1_8;
          rvSegment::CreateDecal(v6, (int)effect, (int)v10, (int)v6, effect, v6->mSegStartTime, a2, v26, v27);
          v6->mFlags |= 1u;
          return v6->mFlags & 1;
        case 7:
          if ( !(v10->mFlags & 1) )
            goto $LN1_8;
          rvSegment::InitLight(v6, effect, v10, v6->mSegStartTime);
          v6->mFlags |= 1u;
          return v6->mFlags & 1;
        case 9:
          if ( !(v10->mFlags & 1) )
            goto $LN1_8;
          v21 = &bse->vfptr->SetShakeParms;
          depthOffset = rvBSE::GetAttenuation(effect, v10);
          timed = rvSegment::AttenuateDuration(v6, effect, v10) + v6->mSegStartTime;
          v22 = timed;
          goto LABEL_38;
        case 0xA:
          if ( v10->mFlags & 1 )
          {
            v21 = &bse->vfptr->SetTunnelParms;
            depthOffset = rvBSE::GetAttenuation(effect, v10);
            timee = rvSegment::AttenuateDuration(v6, effect, v10) + v6->mSegStartTime;
            v22 = timee;
LABEL_38:
            v23 = v22;
            ((void (__thiscall *)(rvBSEManager *, _DWORD, float))*v21)(
              bse,
              LODWORD(v23),
              COERCE_FLOAT(LODWORD(depthOffset)));
          }
          goto $LN1_8;
        default:
          goto LABEL_40;
      }
    }
    else
    {
      LOBYTE(v9) = 1;
    }
  }
  else
  {
    LOBYTE(v9) = 0;
  }
  return v9;
}
int dynamic_initializer_for__singletonLock___109()
{
  sdLock::sdLock(&singletonLock_109);
  return atexit(dynamic_atexit_destructor_for__singletonLock___109);
}
int dynamic_initializer_for__bse_maxDecalDistance__()
{
  if ( idCVar::staticVars == (idCVar *)15 )
  {
    ((void (__stdcall *)(idCVar *))cvarSystem->vfptr->Register)(&bse_maxDecalDistance);
  }
  else
  {
    bse_maxDecalDistance.next = idCVar::staticVars;
    idCVar::staticVars = &bse_maxDecalDistance;
  }
  return atexit(dynamic_atexit_destructor_for__bse_maxDecalDistance__);
}
int dynamic_initializer_for__r_sortParticles__()
{
  if ( idCVar::staticVars == (idCVar *)15 )
  {
    ((void (__stdcall *)(idCVar *))cvarSystem->vfptr->Register)(&r_sortParticles);
  }
  else
  {
    r_sortParticles.next = idCVar::staticVars;
    idCVar::staticVars = &r_sortParticles;
  }
  return atexit(dynamic_atexit_destructor_for__r_sortParticles__);
}
void __cdecl dynamic_atexit_destructor_for__bse_maxDecalDistance__()
{
  bse_maxDecalDistance.vfptr = (idCVarVtbl *)&idCVar::`vftable';
}
void __cdecl dynamic_atexit_destructor_for__r_sortParticles__()
{
  r_sortParticles.vfptr = (idCVarVtbl *)&idCVar::`vftable';
}
void __cdecl dynamic_atexit_destructor_for__singletonLock___109()
{
  sdLock::~sdLock(&singletonLock_109);
}
