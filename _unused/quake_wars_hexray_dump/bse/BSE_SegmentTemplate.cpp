void __thiscall rvSegmentTemplate::CreateParticleTemplate(rvSegmentTemplate *this, rvDeclEffect *effect, idParser *src, int particleType)
{
  rvSegmentTemplate *v4; // edi
  rvParticleTemplate *v5; // esi

  v4 = this;
  v5 = &this->mParticleTemplate;
  rvParticleTemplate::Init(&this->mParticleTemplate);
  v4->mParticleTemplate.mType = particleType;
  rvParticleTemplate::SetParameterCounts(v5);
  rvParticleTemplate::Parse(v5, effect, src);
}
bool __thiscall rvSegmentTemplate::GetSoundLooping(rvSegmentTemplate *this)
{
  idSoundShader *v1; // eax
  int v2; // eax

  v1 = this->mSoundShader;
  if ( v1 )
    v2 = ((unsigned int)v1->parms.soundShaderFlags >> 5) & 1;
  else
    LOBYTE(v2) = 0;
  return v2;
}
void __thiscall rvSegmentTemplate::EvaluateTrailSegment(rvSegmentTemplate *this, rvDeclEffect *et)
{
  rvSegmentTemplate *v2; // esi
  rvTrailInfo *v3; // eax

  v2 = this;
  v3 = this->mParticleTemplate.mTrailInfo;
  if ( v3->mTrailType )
  {
    if ( idStr::Cmp(v3->mTrailTypeName.data, (const char *)&s2) )
      v2->mTrailSegmentIndex = rvDeclEffect::GetTrailSegmentIndex(et, &v2->mParticleTemplate.mTrailInfo->mTrailTypeName);
  }
}
bool __thiscall rvSegmentTemplate::GetSmoker(rvSegmentTemplate *this)
{
  return this->mParticleTemplate.mTrailInfo->mTrailType == 3;
}
bool __thiscall rvSegmentTemplate::DetailCull(rvSegmentTemplate *this)
{
  return 0.0 != this->mDetail && this->mDetail > (double)bse_detailLevel.internalVar->floatValue;
}
double __thiscall rvSegmentTemplate::EvaluateCost(rvSegmentTemplate *this, int activeParticles)
{
  rvParticleTemplate *v3; // esi
  float v4; // [esp+8h] [ebp-4h]
  int activeParticlesa; // [esp+10h] [ebp+4h]

  if ( !(this->mFlags & 1) )
    return 0.0;
  v3 = &this->mParticleTemplate;
  v4 = rvSegmentTemplate::mSegmentBaseCosts[this->mSegType];
  if ( this->mParticleTemplate.mType )
  {
    *(float *)&activeParticlesa = (double)activeParticles;
    v4 = rvParticleTemplate::CostTrail(&this->mParticleTemplate, *(float *)&activeParticlesa) + v4;
    if ( ((unsigned int)v3->mFlags >> 9) & 1 )
      v4 = *(float *)&activeParticlesa * 80.0 + v4;
  }
  return v4;
}
rvParticleTemplate *__thiscall rvParticleTemplate::operator=(rvParticleTemplate *this, rvParticleTemplate *__that)
{
  rvParticleTemplate *v2; // esi
  int v3; // ebp
  idStr *v4; // ebx

  v2 = this;
  this->mFlags = __that->mFlags;
  this->mTraceModelIndex = __that->mTraceModelIndex;
  this->mType = __that->mType;
  this->mMaterial = __that->mMaterial;
  this->mModel = __that->mModel;
  v3 = __that->mEntityDefName.len;
  v4 = &this->mEntityDefName;
  if ( v3 + 1 > abs(this->mEntityDefName.alloced) )
    idStr::ReAllocate(v4, v3 + 1, 0);
  memcpy(v4->data, __that->mEntityDefName.data, v3);
  v4->data[v3] = 0;
  v4->len = v3;
  v2->mGravity.x = __that->mGravity.x;
  v2->mGravity.y = __that->mGravity.y;
  v2->mDuration.x = __that->mDuration.x;
  v2->mDuration.y = __that->mDuration.y;
  v2->mCentre.x = __that->mCentre.x;
  v2->mCentre.y = __that->mCentre.y;
  v2->mCentre.z = __that->mCentre.z;
  v2->mTiling = __that->mTiling;
  v2->mBounce = __that->mBounce;
  v2->mPhysicsDistance = __that->mPhysicsDistance;
  v2->mWindDeviationAngle = __that->mWindDeviationAngle;
  v2->mVertexCount = __that->mVertexCount;
  v2->mIndexCount = __that->mIndexCount;
  v2->mTrailRepeat = __that->mTrailRepeat;
  v2->mNumSizeParms = __that->mNumSizeParms;
  v2->mNumRotateParms = __that->mNumRotateParms;
  v2->mNumFrames = __that->mNumFrames;
  v2->mTrailInfo = __that->mTrailInfo;
  v2->mElecInfo = __that->mElecInfo;
  v2->mpSpawnPosition = __that->mpSpawnPosition;
  v2->mpSpawnDirection = __that->mpSpawnDirection;
  v2->mpSpawnVelocity = __that->mpSpawnVelocity;
  v2->mpSpawnAcceleration = __that->mpSpawnAcceleration;
  v2->mpSpawnFriction = __that->mpSpawnFriction;
  v2->mpSpawnTint = __that->mpSpawnTint;
  v2->mpSpawnFade = __that->mpSpawnFade;
  v2->mpSpawnSize = __that->mpSpawnSize;
  v2->mpSpawnRotate = __that->mpSpawnRotate;
  v2->mpSpawnAngle = __that->mpSpawnAngle;
  v2->mpSpawnOffset = __that->mpSpawnOffset;
  v2->mpSpawnLength = __that->mpSpawnLength;
  v2->mpSpawnWindStrength = __that->mpSpawnWindStrength;
  v2->mpTintEnvelope = __that->mpTintEnvelope;
  v2->mpFadeEnvelope = __that->mpFadeEnvelope;
  v2->mpSizeEnvelope = __that->mpSizeEnvelope;
  v2->mpRotateEnvelope = __that->mpRotateEnvelope;
  v2->mpAngleEnvelope = __that->mpAngleEnvelope;
  v2->mpOffsetEnvelope = __that->mpOffsetEnvelope;
  v2->mpLengthEnvelope = __that->mpLengthEnvelope;
  v2->mpDeathTint = __that->mpDeathTint;
  v2->mpDeathFade = __that->mpDeathFade;
  v2->mpDeathSize = __that->mpDeathSize;
  v2->mpDeathRotate = __that->mpDeathRotate;
  v2->mpDeathAngle = __that->mpDeathAngle;
  v2->mpDeathOffset = __that->mpDeathOffset;
  v2->mpDeathLength = __that->mpDeathLength;
  v2->mNumImpactEffects = __that->mNumImpactEffects;
  v2->mImpactEffects[0] = __that->mImpactEffects[0];
  v2->mImpactEffects[1] = __that->mImpactEffects[1];
  v2->mImpactEffects[2] = __that->mImpactEffects[2];
  v2->mImpactEffects[3] = __that->mImpactEffects[3];
  v2->mNumTimeoutEffects = __that->mNumTimeoutEffects;
  v2->mTimeoutEffects[0] = __that->mTimeoutEffects[0];
  v2->mTimeoutEffects[1] = __that->mTimeoutEffects[1];
  v2->mTimeoutEffects[2] = __that->mTimeoutEffects[2];
  v2->mTimeoutEffects[3] = __that->mTimeoutEffects[3];
  return v2;
}
void __thiscall rvSegmentTemplate::Duplicate(rvSegmentTemplate *this, rvSegmentTemplate *copy)
{
  rvSegmentTemplate *v2; // esi
  int v3; // ebp
  void **v4; // ebx

  v2 = this;
  this->mDeclEffect = copy->mDeclEffect;
  v3 = copy->mSegmentName.len;
  v4 = (void **)&this->mSegmentName;
  if ( v3 + 1 > abs(this->mSegmentName.alloced) )
    idStr::ReAllocate(&this->mSegmentName, v3 + 1, 0);
  memcpy(v4[1], copy->mSegmentName.data, v3);
  *((_BYTE *)v4[1] + v3) = 0;
  *v4 = (void *)v3;
  v2->mFlags = copy->mFlags;
  v2->mSegType = copy->mSegType;
  v2->mLocalStartTime.x = copy->mLocalStartTime.x;
  v2->mLocalStartTime.y = copy->mLocalStartTime.y;
  v2->mLocalDuration.x = copy->mLocalDuration.x;
  v2->mLocalDuration.y = copy->mLocalDuration.y;
  v2->mScale = copy->mScale;
  v2->mAttenuation.x = copy->mAttenuation.x;
  v2->mAttenuation.y = copy->mAttenuation.y;
  v2->mParticleCap = copy->mParticleCap;
  v2->mScale = copy->mScale;
  v2->mDetail = copy->mDetail;
  v2->mCount.x = copy->mCount.x;
  v2->mCount.y = copy->mCount.y;
  v2->mDensity.x = copy->mDensity.x;
  v2->mDensity.y = copy->mDensity.y;
  v2->mTrailSegmentIndex = copy->mTrailSegmentIndex;
  v2->mNumEffects = copy->mNumEffects;
  v2->mEffects[0] = copy->mEffects[0];
  v2->mEffects[1] = copy->mEffects[1];
  v2->mEffects[2] = copy->mEffects[2];
  v2->mEffects[3] = copy->mEffects[3];
  v2->mSoundShader = copy->mSoundShader;
  v2->mSoundVolume.x = copy->mSoundVolume.x;
  v2->mSoundVolume.y = copy->mSoundVolume.y;
  v2->mFreqShift.x = copy->mFreqShift.x;
  v2->mFreqShift.y = copy->mFreqShift.y;
  rvParticleTemplate::Duplicate(&v2->mParticleTemplate, &copy->mParticleTemplate);
  v2->mDecalAxis = copy->mDecalAxis;
}
void __thiscall rvSegmentTemplate::Init(rvSegmentTemplate *this, rvDeclEffect *decl)
{
  float *v2; // ecx

  this->mDeclEffect = decl;
  this->mFlags = 1;
  this->mSegType = 0;
  this->mLocalStartTime.y = 0.0;
  this->mLocalStartTime.x = 0.0;
  v2 = (float *)&this->mParticleTemplate.mFlags;
  *(v2 - 6) = 0.0;
  *(v2 - 7) = 0.0;
  *(v2 - 4) = 0.0;
  *(v2 - 5) = 0.0;
  *(v2 - 3) = 0.0;
  *(v2 - 2) = 1.0;
  *(v2 - 1) = 0.0;
  v2[65] = 1.0;
  v2[66] = 1.0;
  v2[68] = 0.0;
  v2[67] = 0.0;
  v2[69] = -6.8056469e38/*NaN*/;
  v2[70] = 0.0;
  v2[71] = 0.0;
  v2[72] = 0.0;
  v2[73] = 0.0;
  v2[74] = 0.0;
  v2[75] = 0.0;
  v2[77] = 0.0;
  v2[76] = 0.0;
  v2[78] = 1.0;
  v2[79] = 1.0;
  *((_DWORD *)v2 + 80) = 3;
  rvParticleTemplate::Init((rvParticleTemplate *)v2);
}
void __thiscall rvSegmentTemplate::SetMinDuration(rvSegmentTemplate *this, rvDeclEffect *effect)
{
  idSoundShader *v2; // eax
  float duration; // ST04_4

  if ( !(((unsigned int)this->mFlags >> 4) & 1) )
  {
    v2 = this->mSoundShader;
    if ( !v2 || !(((unsigned int)v2->parms.soundShaderFlags >> 5) & 1) )
    {
      duration = this->mLocalDuration.x + this->mLocalStartTime.x;
      rvDeclEffect::SetMinDuration(effect, duration);
    }
  }
}
void __thiscall rvSegmentTemplate::SetMaxDuration(rvSegmentTemplate *this, rvDeclEffect *effect)
{
  rvSegmentTemplate *v2; // esi
  rvDeclEffect *v3; // edi
  float duration; // ST0C_4
  float effecta; // [esp+14h] [ebp+4h]

  v2 = this;
  if ( !(((unsigned int)this->mFlags >> 4) & 1) )
  {
    v3 = effect;
    duration = this->mLocalDuration.x + this->mLocalStartTime.x;
    rvDeclEffect::SetMaxDuration(effect, duration);
    if ( v2->mParticleTemplate.mType )
    {
      effecta = v2->mLocalDuration.x + v2->mLocalStartTime.x + v2->mParticleTemplate.mDuration.y;
      rvDeclEffect::SetMaxDuration(v3, effecta);
    }
  }
}
char __thiscall rvSegmentTemplate::Finish(rvSegmentTemplate *this, rvDeclEffect *effect)
{
  rvSegmentTemplate *v2; // esi
  float v3; // ST08_4
  float v4; // ST08_4
  float v5; // ST08_4
  float v6; // ST08_4
  float v7; // ST08_4
  rvParticleTemplate *v8; // edi
  int v10; // ecx

  v2 = this;
  if ( this->mLocalStartTime.y <= (double)this->mLocalStartTime.x )
  {
    v3 = this->mLocalStartTime.x;
    this->mLocalStartTime.x = this->mLocalStartTime.y;
    this->mLocalStartTime.y = v3;
  }
  if ( this->mLocalDuration.y <= (double)this->mLocalDuration.x )
  {
    v4 = this->mLocalDuration.x;
    this->mLocalDuration.x = this->mLocalDuration.y;
    this->mLocalDuration.y = v4;
  }
  if ( this->mCount.y <= (double)this->mCount.x )
  {
    v5 = this->mCount.x;
    this->mCount.x = this->mCount.y;
    this->mCount.y = v5;
  }
  if ( this->mDensity.y <= (double)this->mDensity.x )
  {
    v6 = this->mDensity.x;
    this->mDensity.x = this->mDensity.y;
    this->mDensity.y = v6;
  }
  if ( this->mAttenuation.y <= (double)this->mAttenuation.x )
  {
    v7 = this->mAttenuation.x;
    this->mAttenuation.x = this->mAttenuation.y;
    this->mAttenuation.y = v7;
  }
  if ( this->mParticleTemplate.mType )
  {
    v8 = &this->mParticleTemplate;
    rvParticleTemplate::Finish(&this->mParticleTemplate, (int)&this->mParticleTemplate);
    v8->mFlags &= 0xFFF7FFFF;
  }
  switch ( v2->mSegType )
  {
    case 2:
      v2->mFlags |= 4u;
      if ( v2->mParticleTemplate.mType && !(((unsigned int)v2->mFlags >> 5) & 1) )
        goto LABEL_25;
      return 0;
    case 3:
      v2->mFlags |= 4u;
      if ( v2->mParticleTemplate.mType )
        goto LABEL_25;
      return 0;
    case 4:
      v2->mFlags |= 4u;
      v2->mLocalStartTime.y = 0.0;
      v2->mLocalStartTime.x = 0.0;
      v2->mLocalDuration.y = 0.0;
      v2->mLocalDuration.x = 0.0;
      if ( !v2->mParticleTemplate.mType )
        return 0;
      v2->mParticleTemplate.mFlags |= 0x80000u;
LABEL_25:
      v10 = v2->mParticleTemplate.mType;
      if ( v10 == 10 )
        v2->mFlags = v2->mFlags & 0xFFFFFFFB | 0x100;
      if ( ((unsigned int)v2->mFlags >> 5) & 1
        || v2->mParticleTemplate.mTrailInfo->mTrailType == 3
        || ((unsigned int)v2->mParticleTemplate.mFlags >> 9) & 1
        || v2->mParticleTemplate.mNumTimeoutEffects )
      {
        v2->mFlags |= 0x2000u;
      }
      if ( v10 == 7 || v10 == 6 )
        v2->mFlags |= 0x2000u;
      return 1;
    case 5:
      v2->mFlags |= 0x10u;
      goto LABEL_25;
    case 6:
      v2->mFlags = v2->mFlags & 0xFFFFFFFB | 0x100;
      goto LABEL_25;
    case 9:
    case 0xA:
      if ( v2->mAttenuation.y > 0.0 )
        v2->mFlags |= 0x40u;
      goto LABEL_24;
    default:
LABEL_24:
      v2->mFlags &= 0xFFFFFFFB;
      goto LABEL_25;
  }
}
void __thiscall rvSegmentTemplate::operator=(rvSegmentTemplate *this, rvSegmentTemplate *copy)
{
  rvSegmentTemplate *v2; // esi
  int v3; // ebp
  void **v4; // ebx

  v2 = this;
  this->mDeclEffect = copy->mDeclEffect;
  v3 = copy->mSegmentName.len;
  v4 = (void **)&this->mSegmentName;
  if ( v3 + 1 > abs(this->mSegmentName.alloced) )
    idStr::ReAllocate(&this->mSegmentName, v3 + 1, 0);
  memcpy(v4[1], copy->mSegmentName.data, v3);
  *((_BYTE *)v4[1] + v3) = 0;
  *v4 = (void *)v3;
  v2->mFlags = copy->mFlags;
  v2->mSegType = copy->mSegType;
  v2->mLocalStartTime.x = copy->mLocalStartTime.x;
  v2->mLocalStartTime.y = copy->mLocalStartTime.y;
  v2->mLocalDuration.x = copy->mLocalDuration.x;
  v2->mLocalDuration.y = copy->mLocalDuration.y;
  v2->mScale = copy->mScale;
  v2->mAttenuation.x = copy->mAttenuation.x;
  v2->mAttenuation.y = copy->mAttenuation.y;
  v2->mParticleCap = copy->mParticleCap;
  v2->mScale = copy->mScale;
  v2->mDetail = copy->mDetail;
  v2->mCount.x = copy->mCount.x;
  v2->mCount.y = copy->mCount.y;
  v2->mDensity.x = copy->mDensity.x;
  v2->mDensity.y = copy->mDensity.y;
  v2->mTrailSegmentIndex = copy->mTrailSegmentIndex;
  v2->mNumEffects = copy->mNumEffects;
  v2->mEffects[0] = copy->mEffects[0];
  v2->mEffects[1] = copy->mEffects[1];
  v2->mEffects[2] = copy->mEffects[2];
  v2->mEffects[3] = copy->mEffects[3];
  v2->mSoundShader = copy->mSoundShader;
  v2->mSoundVolume.x = copy->mSoundVolume.x;
  v2->mSoundVolume.y = copy->mSoundVolume.y;
  v2->mFreqShift.x = copy->mFreqShift.x;
  v2->mFreqShift.y = copy->mFreqShift.y;
  rvParticleTemplate::operator=(&v2->mParticleTemplate, &copy->mParticleTemplate);
  v2->mDecalAxis = copy->mDecalAxis;
}
char __thiscall rvSegmentTemplate::Parse(rvSegmentTemplate *this, rvDeclEffect *effect, int segmentType, idParser *src)
{
  rvSegmentTemplate *v4; // esi
  double v5; // st7
  char result; // al
  rvDeclEffect *v7; // ebp
  const char *v8; // eax
  char *v9; // ebx
  const char *v10; // ST0C_4
  sdDeclWrapperTemplate<idMaterial> *v11; // eax
  idMaterial *v12; // eax
  const char *v13; // ST0C_4
  sdDeclWrapperTemplate<idMaterial> *v14; // eax
  idLexer *v15; // eax
  int v16; // edx
  idBitMsg **v17; // eax
  idToken token; // [esp+0h] [ebp-60h]
  __int16 v19; // [esp+50h] [ebp-10h]
  int v20; // [esp+5Ch] [ebp-4h]
  float effecta; // [esp+64h] [ebp+4h]

  *(long double *)((char *)&token.floatvalue + 4) = 0.0;
  v4 = this;
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
  v19 = 0;
  v20 = 0;
  this->mSegType = segmentType;
  if ( segmentType == 6 )
  {
    v5 = 0.89999998;
  }
  else
  {
    if ( segmentType != 4 )
      goto LABEL_6;
    v5 = 0.69999999;
  }
  this->mDetail = v5;
LABEL_6:
  if ( idParser::ReadToken(src, (idToken *)((char *)&token + 4)) )
  {
    if ( idStr::Icmp((const char *)token.alloced, "{") )
    {
      v9 = token.data;
      if ( (_DWORD)(token.data + 1) > abs(v4->mSegmentName.alloced) )
        idStr::ReAllocate(&v4->mSegmentName, (int)(token.data + 1), 0);
      memcpy(v4->mSegmentName.data, (const void *)token.alloced, (size_t)v9);
      v9[(unsigned int)v4->mSegmentName.data] = 0;
      v4->mSegmentName.len = (int)v9;
      v7 = effect;
    }
    else
    {
      v7 = effect;
      v8 = va("unnamed%d", effect->mSegmentTemplates.num);
      idStr::operator=(&v4->mSegmentName, v8);
      idParser::UnreadToken(src, (idToken *)((char *)&token + 4));
    }
    if ( idParser::ExpectTokenString(src, "{", 0) )
    {
      if ( idParser::ReadToken(src, (idToken *)((char *)&token + 4)) )
      {
        while ( idStr::Cmp((const char *)token.alloced, "}") )
        {
          if ( idStr::Icmp((const char *)token.alloced, "count") && idStr::Icmp((const char *)token.alloced, "rate") )
          {
            if ( idStr::Icmp((const char *)token.alloced, "density") )
            {
              if ( idStr::Icmp((const char *)token.alloced, "particleCap") )
              {
                if ( idStr::Icmp((const char *)token.alloced, "start") )
                {
                  if ( idStr::Icmp((const char *)token.alloced, "duration") )
                  {
                    if ( idStr::Icmp((const char *)token.alloced, "detail") )
                    {
                      if ( idStr::Icmp((const char *)token.alloced, "soundShader") )
                      {
                        if ( idStr::Icmp((const char *)token.alloced, "volume") )
                        {
                          if ( idStr::Icmp((const char *)token.alloced, "freqShift") )
                          {
                            if ( idStr::Icmp((const char *)token.alloced, "effect") )
                            {
                              if ( idStr::Icmp((const char *)token.alloced, "channel") )
                              {
                                if ( idStr::Icmp((const char *)token.alloced, "scale") )
                                {
                                  if ( idStr::Icmp((const char *)token.alloced, "attenuateEmitter") )
                                  {
                                    if ( idStr::Icmp((const char *)token.alloced, "inverseAttenuateEmitter") )
                                    {
                                      if ( idStr::Icmp((const char *)token.alloced, "attenuation") )
                                      {
                                        if ( idStr::Icmp((const char *)token.alloced, "locked") )
                                        {
                                          if ( idStr::Icmp((const char *)token.alloced, "looping") )
                                          {
                                            if ( idStr::Icmp((const char *)token.alloced, "constant") )
                                            {
                                              if ( idStr::Icmp((const char *)token.alloced, "calcDuration") )
                                              {
                                                if ( idStr::Icmp((const char *)token.alloced, "sprite") )
                                                {
                                                  if ( idStr::Icmp((const char *)token.alloced, "line") )
                                                  {
                                                    if ( idStr::Icmp((const char *)token.alloced, "oriented") )
                                                    {
                                                      if ( idStr::Icmp((const char *)token.alloced, "decal") )
                                                      {
                                                        if ( idStr::Icmp((const char *)token.alloced, "model") )
                                                        {
                                                          if ( idStr::Icmp((const char *)token.alloced, "light") )
                                                          {
                                                            if ( idStr::Icmp((const char *)token.alloced, "electricity") )
                                                            {
                                                              if ( idStr::Icmp((const char *)token.alloced, "linked") )
                                                              {
                                                                if ( idStr::Icmp(
                                                                       (const char *)token.alloced,
                                                                       "orientedlinked") )
                                                                {
                                                                  if ( idStr::Icmp(
                                                                         (const char *)token.alloced,
                                                                         "debris") )
                                                                  {
                                                                    if ( idStr::Icmp(
                                                                           (const char *)token.alloced,
                                                                           "depthsort") )
                                                                    {
                                                                      if ( idStr::Icmp(
                                                                             (const char *)token.alloced,
                                                                             "inverseDrawOrder") )
                                                                      {
                                                                        if ( idStr::Icmp(
                                                                               (const char *)token.alloced,
                                                                               "useMaterialColor") )
                                                                        {
                                                                          if ( idStr::Icmp(
                                                                                 (const char *)token.alloced,
                                                                                 "orientateIdentity") )
                                                                          {
                                                                            if ( idStr::Icmp(
                                                                                   (const char *)token.alloced,
                                                                                   "decalAxis") )
                                                                            {
                                                                              v15 = src->scriptstack;
                                                                              if ( v15 )
                                                                                v16 = v15->line;
                                                                              else
                                                                                v16 = 0;
                                                                              if ( v15 )
                                                                                v17 = (idBitMsg **)v15->filename.data;
                                                                              else
                                                                                v17 = &s2;
                                                                              (*(void (**)(netadrtype_t, const char *, ...))(*(_DWORD *)common.type + 68))(
                                                                                common.type,
                                                                                "^4BSE:^1 Invalid segment parameter '%s' "
                                                                                "(file: %s, line: %d)",
                                                                                token.alloced,
                                                                                v17,
                                                                                v16);
                                                                            }
                                                                            else
                                                                            {
                                                                              v4->mDecalAxis = idParser::ParseInt(src);
                                                                            }
                                                                          }
                                                                          else
                                                                          {
                                                                            v4->mFlags |= 0x1000u;
                                                                          }
                                                                        }
                                                                        else
                                                                        {
                                                                          v4->mFlags |= 0x200u;
                                                                        }
                                                                      }
                                                                      else
                                                                      {
                                                                        v4->mFlags |= 0x800u;
                                                                      }
                                                                    }
                                                                    else
                                                                    {
                                                                      v4->mFlags |= 0x400u;
                                                                    }
                                                                  }
                                                                  else
                                                                  {
                                                                    rvSegmentTemplate::CreateParticleTemplate(
                                                                      v4,
                                                                      v7,
                                                                      src,
                                                                      10);
                                                                  }
                                                                }
                                                                else
                                                                {
                                                                  rvSegmentTemplate::CreateParticleTemplate(
                                                                    v4,
                                                                    v7,
                                                                    src,
                                                                    9);
                                                                }
                                                              }
                                                              else
                                                              {
                                                                rvSegmentTemplate::CreateParticleTemplate(
                                                                  v4,
                                                                  v7,
                                                                  src,
                                                                  8);
                                                              }
                                                            }
                                                            else
                                                            {
                                                              rvSegmentTemplate::CreateParticleTemplate(v4, v7, src, 7);
                                                            }
                                                          }
                                                          else
                                                          {
                                                            rvSegmentTemplate::CreateParticleTemplate(v4, v7, src, 6);
                                                          }
                                                        }
                                                        else
                                                        {
                                                          rvSegmentTemplate::CreateParticleTemplate(v4, v7, src, 5);
                                                        }
                                                      }
                                                      else
                                                      {
                                                        rvSegmentTemplate::CreateParticleTemplate(v4, v7, src, 4);
                                                      }
                                                    }
                                                    else
                                                    {
                                                      rvSegmentTemplate::CreateParticleTemplate(v4, v7, src, 3);
                                                    }
                                                  }
                                                  else
                                                  {
                                                    rvSegmentTemplate::CreateParticleTemplate(v4, v7, src, 2);
                                                  }
                                                }
                                                else
                                                {
                                                  rvSegmentTemplate::CreateParticleTemplate(v4, v7, src, 1);
                                                }
                                              }
                                              else
                                              {
                                                v4->mFlags |= 0x4000u;
                                              }
                                            }
                                            else
                                            {
                                              v4->mFlags |= 0x20u;
                                            }
                                          }
                                        }
                                        else
                                        {
                                          v4->mFlags |= 2u;
                                        }
                                      }
                                      else
                                      {
                                        v4->mAttenuation.x = idParser::ParseFloat(src, 0);
                                        idParser::ExpectTokenString(src, ",", 0);
                                        v4->mAttenuation.y = idParser::ParseFloat(src, 0);
                                      }
                                    }
                                    else
                                    {
                                      v4->mFlags |= 0xC0u;
                                    }
                                  }
                                  else
                                  {
                                    v4->mFlags |= 0x40u;
                                  }
                                }
                                else
                                {
                                  v4->mScale = idParser::ParseFloat(src, 0);
                                }
                              }
                              else
                              {
                                idParser::ReadToken(src, (idToken *)((char *)&token + 4));
                              }
                            }
                            else
                            {
                              idParser::ReadToken(src, (idToken *)((char *)&token + 4));
                              if ( v4->mNumEffects >= 4 )
                              {
                                (*(void (**)(netadrtype_t, const char *, ...))(*(_DWORD *)common.type + 68))(
                                  common.type,
                                  "Unable to add effect '%s' - too many effects",
                                  token.alloced);
                              }
                              else
                              {
                                v13 = (const char *)token.alloced;
                                v14 = (sdDeclWrapperTemplate<idMaterial> *)sdSingleton<sdDeclTypeHolder>::GetInstance();
                                v4->mEffects[v4->mNumEffects++] = (rvDeclEffect *)sdDeclWrapperTemplate<idSoundShader>::LocalFind(
                                                                                    v14 + 6,
                                                                                    v13,
                                                                                    1);
                              }
                            }
                          }
                          else
                          {
                            v4->mFreqShift.x = idParser::ParseFloat(src, 0);
                            idParser::ExpectTokenString(src, ",", 0);
                            v4->mFreqShift.y = idParser::ParseFloat(src, 0);
                          }
                        }
                        else
                        {
                          v4->mSoundVolume.x = idParser::ParseFloat(src, 0);
                          idParser::ExpectTokenString(src, ",", 0);
                          v4->mSoundVolume.y = idParser::ParseFloat(src, 0);
                        }
                      }
                      else
                      {
                        idParser::ReadToken(src, (idToken *)((char *)&token + 4));
                        v10 = (const char *)token.alloced;
                        v11 = (sdDeclWrapperTemplate<idMaterial> *)sdSingleton<sdDeclTypeHolder>::GetInstance();
                        v12 = sdDeclWrapperTemplate<idSoundShader>::LocalFind(v11 + 3, v10, 1);
                        v4->mSoundShader = (idSoundShader *)v12;
                        effecta = (double)((int (__thiscall *)(idMaterial *))v12->vfptr[1].Parse)(v12) * 0.001;
                        v4->mLocalDuration.x = effecta;
                        v4->mLocalDuration.y = effecta;
                      }
                    }
                    else
                    {
                      v4->mDetail = idParser::ParseFloat(src, 0);
                    }
                  }
                  else
                  {
                    v4->mLocalDuration.x = idParser::ParseFloat(src, 0);
                    idParser::ExpectTokenString(src, ",", 0);
                    v4->mLocalDuration.y = idParser::ParseFloat(src, 0);
                  }
                }
                else
                {
                  v4->mLocalStartTime.x = idParser::ParseFloat(src, 0);
                  idParser::ExpectTokenString(src, ",", 0);
                  v4->mLocalStartTime.y = idParser::ParseFloat(src, 0);
                }
              }
              else
              {
                v4->mParticleCap = idParser::ParseFloat(src, 0);
              }
            }
            else
            {
              v4->mDensity.x = idParser::ParseFloat(src, 0);
              idParser::ExpectTokenString(src, ",", 0);
              v4->mDensity.y = idParser::ParseFloat(src, 0);
            }
          }
          else
          {
            v4->mCount.x = idParser::ParseFloat(src, 0);
            idParser::ExpectTokenString(src, ",", 0);
            v4->mCount.y = idParser::ParseFloat(src, 0);
          }
          if ( !idParser::ReadToken(src, (idToken *)((char *)&token + 4)) )
            goto LABEL_95;
        }
        v20 = -1;
        idStr::FreeData((idStr *)&token.data);
        result = 1;
      }
      else
      {
LABEL_95:
        v20 = -1;
        idStr::FreeData((idStr *)&token.data);
        result = 0;
      }
    }
    else
    {
      v20 = -1;
      idStr::FreeData((idStr *)&token.data);
      result = 0;
    }
  }
  else
  {
    v20 = -1;
    idStr::FreeData((idStr *)&token.data);
    result = 0;
  }
  return result;
}
int dynamic_initializer_for__singletonLock___108()
{
  sdLock::sdLock(&singletonLock_108);
  return atexit(dynamic_atexit_destructor_for__singletonLock___108);
}
void __cdecl dynamic_atexit_destructor_for__singletonLock___108()
{
  sdLock::~sdLock(&singletonLock_108);
}
