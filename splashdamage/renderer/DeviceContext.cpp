// Copyright (C) 2007 Id Software, Inc.
//

#include "idlib/precompiled.h"

#include "DeviceContext_local.h"

sdDeviceContextLocal::sdDeviceContextLocal() {

}

void sdDeviceContextLocal::Reset() {
}

void sdDeviceContextLocal::BeginEmitToCurrentView( const float modelMatrix[16], const int allowInViewID, const bool weaponDepthHack ) {
}

void sdDeviceContextLocal::BeginEmitFullScreen() {
}

void sdDeviceContextLocal::End() {
}

void sdDeviceContextLocal::SetColor( const idVec4& color ) {
}

void sdDeviceContextLocal::SetColor( const float r, const float g, const float b, const float a ) {
}

idVec4 sdDeviceContextLocal::SetColorMultiplier( const idVec4& c ) {
    return vec4_one;
}

void sdDeviceContextLocal::SetRegister( const int index, const float value ) {
}

void sdDeviceContextLocal::SetRegisters( const float* values ) {
}

void sdDeviceContextLocal::EnableClipping( bool enable ) {
}

void sdDeviceContextLocal::PushClipRect( const sdBounds2D& bounds ) {
}

void sdDeviceContextLocal::PopClipRect() {
}

void sdDeviceContextLocal::DrawRect( float x, float y, float w, float h, float s1, float t1, float s2, float t2, const idMaterial* material, float angle ) {
}

void sdDeviceContextLocal::DrawClippedRect( float x, float y, float w, float h, float s1, float t1, float s2, float t2, const idMaterial* material, float angle ) {
}

void sdDeviceContextLocal::DrawMaskedClippedRect( float x, float y, float w, float h, float s01, float t01, float s02, float t02, float s11, float t11, float s12, float t12, const idMaterial* material, float angle ) {
}

void sdDeviceContextLocal::DrawCinematic( float x, float y, float w, float h, float s1, float t1, float s2, float t2, const idMaterial* material, idSoundEmitter* referenceSound, float angle ) {
}

void sdDeviceContextLocal::DrawClippedWinding( const idWinding2D& winding, const idMaterial* material ) {
}

void sdDeviceContextLocal::DrawClippedWindingMasked( const idWinding2D& winding, const idMaterial* material, float minx, float miny, float width, float height ) {
}

void sdDeviceContextLocal::DrawMaskedMaterial( float x, float y, float w, float h, float u0, float v0, float u1, float v1, const idMaterial* material, const idVec4 &color, float scaleX, float scaleY, float offsetX, float offsetY, float angle ) {
}

void sdDeviceContextLocal::DrawMaterial( float x, float y, float w, float h, const idMaterial* material, const idVec4 &color, float scaleX, float scaleY, float offsetX, float offsetY, float angle ) {
}

void sdDeviceContextLocal::DrawMaterial( const idVec4& rect, const idMaterial *material, const idVec4 &color, const idVec2& scale, const idVec2& offset, float angle ) {
}

void sdDeviceContextLocal::DrawMaterial( const sdBounds2D& rect, const idMaterial *material, const idVec4 &color, const idVec2& scale, const idVec2& offset, float angle ) {
}

void sdDeviceContextLocal::DrawMaterial( float x, float y, float w, float h, const idMaterial* material, const idVec4 &color, const idVec2& st0, const idVec2& st1 ) {
}

void sdDeviceContextLocal::DrawRotatedMaterial( float angle, idVec2 topLeft, idVec2 extents, const idMaterial* material, const idVec4& color ) {
}

void sdDeviceContextLocal::DrawWindingMaterial( const idWinding2D& winding, const idMaterial* material, const idVec4& color ) {
}

void sdDeviceContextLocal::DrawRect( float x, float y, float w, float h, const idVec4 &color ) {
}

void sdDeviceContextLocal::DrawClippedRect( float x, float y, float w, float h, const idVec4 &color ) {
}

void sdDeviceContextLocal::DrawBox( float x, float y, float w, float h, float size, const idVec4 &color ) {
}

void sdDeviceContextLocal::DrawClippedBox( float x, float y, float w, float h, float size, const idVec4 &color ) {
}

void sdDeviceContextLocal::DrawCircleMaterial( const float x, const float y, const idVec2& radius, const int numSides, const idVec4& tcInfo, const idMaterial* material, const idVec4& color, float rotation ) {
}

void sdDeviceContextLocal::DrawCircleMaterialMasked( const float x, const float y, const idVec2& radius, const int numSides, const idVec4& tcInfo, const idMaterial* material, const idVec4& color, float rotation, float s11, float t11, float s12, float t12 ) {
}

void sdDeviceContextLocal::DrawCircle( const float x, const float y, const idVec2& radius, const float width, const int numSides, const idVec4& color ) {
}

void sdDeviceContextLocal::DrawLineMaterial( const idVec2& start, const idVec2& end, const float width, const idMaterial* material, const idVec4& color ) {
}

void sdDeviceContextLocal::DrawLine( const idVec2& start, const idVec2& end, const float width, const idVec4 &color ) {
}

void sdDeviceContextLocal::DrawFilledArc( const float x, const float y, const float radius, int numSides, float percent, const idVec4 &color, float startAngle, const idMaterial *material ) {
}

void sdDeviceContextLocal::DrawFilledArcMasked( const float x, const float y, const float radius, int numSides, float percent, const idVec4 &color, float s11, float t11, float s12, float t12, float startAngle, const idMaterial *material ) {
}

void sdDeviceContextLocal::DrawArc( const float x, const float y, const float radius, const float width, const int numSides, const float percent, const idVec4 &color, const float startAngle ) {
}

void sdDeviceContextLocal::DrawTimer( const float x, const float y, const float w, const float h, float percent, const idVec4 &color, const idMaterial* material, bool invert, const idVec2& st0, const idVec2& st1 ) {
}

qhandle_t sdDeviceContextLocal::FindFont( const char* fontName ) {
    return -1;
}

void sdDeviceContextLocal::FreeFont( const qhandle_t font ) {
}

const int sdDeviceContextLocal::GetFontHeight( const qhandle_t font, const int pointSize ) {
    return pointSize;
}

void sdDeviceContextLocal::SetFont( const qhandle_t font ) {
}

void sdDeviceContextLocal::SetFontSize( const int pointSize ) {
}

void sdDeviceContextLocal::DrawText( const wchar_t* text, const sdBounds2D& rect, unsigned int flags ) {
}

void sdDeviceContextLocal::GetTextDimensions( const wchar_t* text, const sdBounds2D& rect, unsigned int flags, const qhandle_t font, const int pointSize, int& width, int& height, float* scale, int** charAdvances, idList< int >* lineBreaks ) {
}

void sdDeviceContextLocal::OverrideAspectRationCorrection( bool setOverride ) {
}

float sdDeviceContextLocal::GetAspectRatioCorrection() const {
    return 0.0f;
}

static sdDeviceContextLocal deviceContextLocal;

sdDeviceContext* deviceContext = &deviceContextLocal;
