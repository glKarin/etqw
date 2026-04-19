char __thiscall rvEnvParms::GetMinMax(rvEnvParms *this, float *min, float *max)
{
  char result; // al

  if ( this->mTable )
  {
    *min = this->mTable->minValue;
    result = 1;
    *max = this->mTable->maxValue;
  }
  else
  {
    *min = 0.0;
    *max = 0.0;
    result = 0;
  }
  return result;
}
void __thiscall rvEnvParms::Init(rvEnvParms *this)
{
  this->mTable = 0;
  this->mIsCount = 1;
  this->mFastLookUp = 0;
  this->mEnvOffset.z = 0.0;
  this->mEnvOffset.y = 0.0;
  this->mEnvOffset.x = 0.0;
  this->mRate.x = 1.0;
  this->mRate.y = 1.0;
  this->mRate.z = 1.0;
}
bool __thiscall rvEnvParms::Compare(rvEnvParms *this, rvEnvParms *comp)
{
  return this->mTable == comp->mTable
      && this->mIsCount == comp->mIsCount
      && comp->mRate.x == comp->mRate.x
      && comp->mRate.y == comp->mRate.y
      && comp->mRate.z == comp->mRate.z
      && comp->mEnvOffset.x == comp->mEnvOffset.x
      && comp->mEnvOffset.y == comp->mEnvOffset.y
      && comp->mEnvOffset.z == comp->mEnvOffset.z;
}
void __thiscall rvEnvParms::operator=(rvEnvParms *this, rvEnvParms *copy)
{
  *this = *copy;
}
void __thiscall rvEnvParms::Evaluate(rvEnvParms *this, rvEnvParms1Particle *env, float time, float oneOverDuration, float *dest)
{
  rvEnvParms *v5; // eax
  idDeclTable *v6; // ecx
  float v7; // [esp+4h] [ebp-4h]
  float result; // [esp+14h] [ebp+Ch]

  v5 = this;
  v6 = this->mTable;
  if ( v6 )
  {
    v7 = v5->mRate.x;
    if ( v5->mIsCount )
      v7 = v7 * oneOverDuration;
    result = v7 * time + v5->mEnvOffset.x;
    ((void (__stdcall *)(_DWORD))v6->vfptr[1].__vecDelDtor)(LODWORD(result));
    *dest = (env->mEnd - env->mStart) * result + env->mStart;
  }
  else
  {
    *dest = env->mStart;
  }
}
void __thiscall rvEnvParms::Evaluate(rvEnvParms *this, rvEnvParms2Particle *env, float time, float oneOverDuration, float *dest)
{
  rvEnvParms *v5; // esi
  idDeclTable *v6; // ecx
  float rate_4; // [esp+14h] [ebp-8h]
  float rate_4a; // [esp+14h] [ebp-8h]
  float v9; // [esp+18h] [ebp-4h]
  float timea; // [esp+24h] [ebp+8h]
  float timeb; // [esp+24h] [ebp+8h]
  float result; // [esp+28h] [ebp+Ch]

  v5 = this;
  if ( this->mFastLookUp )
  {
    rate_4 = this->mRate.x;
    if ( this->mIsCount )
      rate_4 = rate_4 * oneOverDuration;
    timea = rate_4 * time + this->mEnvOffset.x;
    ((void (__stdcall *)(_DWORD))this->mTable->vfptr[1].__vecDelDtor)(LODWORD(timea));
    *dest = env->mStart.x + (env->mEnd.x - env->mStart.x) * timea;
    dest[1] = timea * (env->mEnd.y - env->mStart.y) + env->mStart.y;
  }
  else
  {
    v6 = this->mTable;
    if ( v5->mTable )
    {
      rate_4a = v5->mRate.x;
      v9 = v5->mRate.y;
      if ( v5->mIsCount )
      {
        rate_4a = rate_4a * oneOverDuration;
        v9 = oneOverDuration * v9;
      }
      result = rate_4a * time + v5->mEnvOffset.x;
      ((void (__stdcall *)(_DWORD))v6->vfptr[1].__vecDelDtor)(LODWORD(result));
      *dest = (env->mEnd.x - env->mStart.x) * result + env->mStart.x;
      timeb = v9 * time + v5->mEnvOffset.y;
      ((void (__stdcall *)(_DWORD))v5->mTable->vfptr[1].__vecDelDtor)(LODWORD(timeb));
      dest[1] = (env->mEnd.y - env->mStart.y) * timeb + env->mStart.y;
    }
    else
    {
      *dest = env->mStart.x;
      dest[1] = env->mStart.y;
    }
  }
}
void __thiscall rvEnvParms::Evaluate(rvEnvParms *this, rvEnvParms3Particle *env, float time, float oneOverDuration, float *dest)
{
  rvEnvParms *v5; // esi
  idDeclTable *v6; // ecx
  rvEnvParms3Particle *v7; // edi
  float v8; // [esp+18h] [ebp-10h]
  float v9; // [esp+1Ch] [ebp-Ch]
  float v10; // [esp+20h] [ebp-8h]
  float v11; // [esp+24h] [ebp-4h]
  float enva; // [esp+2Ch] [ebp+4h]
  float timea; // [esp+30h] [ebp+8h]
  float timeb; // [esp+30h] [ebp+8h]
  float result; // [esp+34h] [ebp+Ch]

  v5 = this;
  if ( this->mFastLookUp )
  {
    v8 = this->mRate.x;
    if ( this->mIsCount )
      v8 = v8 * oneOverDuration;
    timea = v8 * time + this->mEnvOffset.x;
    ((void (__stdcall *)(_DWORD))this->mTable->vfptr[1].__vecDelDtor)(LODWORD(timea));
    *dest = env->mStart.x + (env->mEnd.x - env->mStart.x) * timea;
    dest[1] = (env->mEnd.y - env->mStart.y) * timea + env->mStart.y;
    dest[2] = timea * (env->mEnd.z - env->mStart.z) + env->mStart.z;
  }
  else
  {
    v6 = this->mTable;
    if ( v5->mTable )
    {
      v9 = v5->mRate.x;
      v10 = v5->mRate.y;
      v11 = v5->mRate.z;
      if ( v5->mIsCount )
      {
        v9 = v9 * oneOverDuration;
        v10 = v10 * oneOverDuration;
        v11 = oneOverDuration * v11;
      }
      result = v9 * time + v5->mEnvOffset.x;
      ((void (__stdcall *)(_DWORD))v6->vfptr[1].__vecDelDtor)(LODWORD(result));
      v7 = env;
      *dest = (env->mEnd.x - env->mStart.x) * result + env->mStart.x;
      enva = v10 * time + v5->mEnvOffset.y;
      ((void (__stdcall *)(_DWORD))v5->mTable->vfptr[1].__vecDelDtor)(LODWORD(enva));
      dest[1] = (v7->mEnd.y - v7->mStart.y) * enva + v7->mStart.y;
      timeb = v11 * time + v5->mEnvOffset.z;
      ((void (__stdcall *)(_DWORD))v5->mTable->vfptr[1].__vecDelDtor)(LODWORD(timeb));
      dest[2] = (v7->mEnd.z - v7->mStart.z) * timeb + v7->mStart.z;
    }
    else
    {
      *dest = env->mStart.x;
      dest[1] = env->mStart.y;
      dest[2] = env->mStart.z;
    }
  }
}
void __thiscall rvEnvParms::Finalize(rvEnvParms *this)
{
  bool v1; // zf

  v1 = this->mTable == 0;
  this->mFastLookUp = 0;
  if ( !v1 )
    this->mFastLookUp = this->mRate.y == this->mRate.x
                     && this->mRate.z == this->mRate.x
                     && this->mEnvOffset.y == this->mEnvOffset.x
                     && this->mEnvOffset.z == this->mEnvOffset.x;
}
void __thiscall rvEnvParms3Particle::Rotate(rvEnvParms3Particle *this, rvAngles *angles)
{
  this->mStart.x = angles->pitch + this->mStart.x;
  this->mStart.y = angles->yaw + this->mStart.y;
  this->mStart.z = angles->roll + this->mStart.z;
  this->mEnd.x = angles->pitch + this->mEnd.x;
  this->mEnd.y = this->mEnd.y + angles->yaw;
  this->mEnd.z = angles->roll + this->mEnd.z;
}
void __thiscall rvEnvParms::SetDefaultType(rvEnvParms *this)
{
  rvEnvParms *v1; // esi
  sdDeclTypeHolder *v2; // eax

  v1 = this;
  if ( !this->mTable )
  {
    v2 = sdSingleton<sdDeclTypeHolder>::GetInstance();
    v1->mTable = (idDeclTable *)((int (__stdcall *)(int, const char *, signed int))declManager->vfptr->FindType)(
                                  v2->declTableType.declTypeHandle,
                                  "linear",
                                  1);
    v1->mIsCount = 1;
    v1->mFastLookUp = 1;
  }
}
int dynamic_initializer_for__singletonLock___113()
{
  sdLock::sdLock(&singletonLock_113);
  return atexit(dynamic_atexit_destructor_for__singletonLock___113);
}
void __cdecl dynamic_atexit_destructor_for__singletonLock___113()
{
  sdLock::~sdLock(&singletonLock_113);
}
