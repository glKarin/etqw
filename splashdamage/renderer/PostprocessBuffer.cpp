#include "idlib/precompiled.h"
#pragma hdrstop

#include "renderer/tr_local.h"

#include "PostprocessBuffer.h"

static idCVar harm_r_clearPostprocessBuffer("harm_r_clearPostprocessBuffer", "0", CVAR_BOOL | CVAR_RENDERER | CVAR_ARCHIVE, "Clear postprocess buffer image on every draw");

sdPostprocessBuffer::sdPostprocessBuffer()
	: width(-1),
	height(-1),
	fb(NULL),
	currentBuffer(-1)
{
	for(int k = 0; k < sizeof(globalImages->postProcessBuffers) / sizeof(globalImages->postProcessBuffers[0]); k++)
    {
        images[k] = NULL;
    }
}

bool sdPostprocessBuffer::Init(int w, int h)
{
	if(fb)
	{
		if(w != width || h != height)
		{
			Shutdown();
		}
		else
		{
			return true;
		}
	}

	width = w;
	height = h;
	fb = new idFramebuffer("sdPostprocessBuffer", width, height);
	for(int k = 0; k < sizeof(globalImages->postProcessBuffers) / sizeof(globalImages->postProcessBuffers[0]); k++)
	{
		images[k] = globalImages->GetImage(va("_postProcessBuffer_%d", k));
	}
	fb->Bind();
    fb->AddDepthStencilBuffer(GL_DEPTH24_STENCIL8);
    fb->AddColorBuffer(GL_RGBA8, 0);
	fb->Check();
	fb->Unbind();

	return true;
}

void sdPostprocessBuffer::Shutdown(void)
{
	if(fb)
	{
		fb->Purge();
		delete fb;
		fb = NULL;
	}
	for(int k = 0; k < sizeof(globalImages->postProcessBuffers) / sizeof(globalImages->postProcessBuffers[0]); k++)
    {
        images[k] = NULL;
    }
	currentBuffer = -1;
}

void sdPostprocessBuffer::Begin(int index)
{
    assert(fb);
    assert(index >= 0 && index < sizeof(globalImages->postProcessBuffers) / sizeof(globalImages->postProcessBuffers[0]));
    fb->Bind();
	currentBuffer = index;
	UploadImage();
	fb->AttachImage2D(images[currentBuffer]);
	if(harm_r_clearPostprocessBuffer.GetBool())
		Clear();
}

void sdPostprocessBuffer::UploadImage(void) const
{
    assert(currentBuffer != -1 && images[currentBuffer]);
	if(images[currentBuffer]->uploadWidth < width || images[currentBuffer]->uploadHeight < height)
	{
		int nw = MakePowerOfTwo(width);
		int nh = MakePowerOfTwo(height);
		byte *pic = (byte *)Mem_ClearedAlloc(nw * nh * 4);
		images[currentBuffer]->GenerateImage(pic, nw, nh, TF_LINEAR, false, TR_CLAMP, TD_HIGH_QUALITY);
		images[currentBuffer]->sourceWidth = width;
		images[currentBuffer]->sourceHeight = height;
		Mem_Free(pic);
	}
}

void sdPostprocessBuffer::End(void)
{
    assert(fb);
	/*
	images[currentBuffer]->CopyFramebuffer(backEnd.viewDef->viewport.x1,
			backEnd.viewDef->viewport.y1,  backEnd.viewDef->viewport.x2 -  backEnd.viewDef->viewport.x1 + 1,
			backEnd.viewDef->viewport.y2 -  backEnd.viewDef->viewport.y1 + 1, true);
	*/
#if 0
	static idCVar ppp("ppp", "0", 0, "");
	int width = images[currentBuffer]->uploadWidth;
	int height = images[currentBuffer]->uploadHeight;
	if (ppp.GetBool())
	{
		GLint packAlign;
		qglGetIntegerv(GL_PACK_ALIGNMENT, &packAlign);
		qglPixelStorei(GL_PACK_ALIGNMENT, 1);	// otherwise small rows get padded to 32 bits

		byte *data = (byte *)malloc(width * height * 4);
		qglReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
		qglPixelStorei(GL_PACK_ALIGNMENT, packAlign);	// otherwise small rows get padded to 32 bits

		extern void R_WritePNG(const char *filename, const byte *data, int width, int height, int comp, bool flipVertical = false, int quality = 100, const char *basePath = NULL);
		//R_WritePNG(va("texturesxxx/%d_%d.png", tr.frameCount, currentBuffer), data, width, height,4);

		//fileSystem->WriteTGA(va("texturesxxx/%d_%d.tga", tr.frameCount, currentBuffer), data, width, height);
		free(data);
	}
#endif
	fb->AttachColorBuffer();
    fb->Unbind();
	currentBuffer = -1;
#if 0
	if (ppp.GetBool())
	{
		GLint packAlign;
		qglGetIntegerv(GL_PACK_ALIGNMENT, &packAlign);
		qglPixelStorei(GL_PACK_ALIGNMENT, 1);	// otherwise small rows get padded to 32 bits

		byte *data = (byte *)malloc(width * height * 4);
		qglReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
		qglPixelStorei(GL_PACK_ALIGNMENT, packAlign);	// otherwise small rows get padded to 32 bits

		extern void R_WritePNG(const char *filename, const byte *data, int width, int height, int comp, bool flipVertical = false, int quality = 100, const char *basePath = NULL);
		//R_WritePNG(va("texturesxxx/%d_%d.png", tr.frameCount, currentBuffer), data, width, height,4);

		fileSystem->WriteTGA(va("texturesxxx/%d_%d.tga", tr.frameCount, currentBuffer), data, width, height);
		free(data);
	}
#endif
}

int sdPostprocessBuffer::UploadWidth(void) const
{
    assert(currentBuffer != -1 && images[currentBuffer]);
    return images[currentBuffer]->uploadWidth;
}

int sdPostprocessBuffer::UploadHeight(void) const
{
    assert(currentBuffer != -1 && images[currentBuffer]);
    return images[currentBuffer]->uploadHeight;
}

void sdPostprocessBuffer::Clear(void) const
{
	qglClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

uint32_t sdPostprocessBuffer::GetFramebuffer(void) const
{
	assert(fb);
	return fb->GetFramebuffer();
}

void sdPostprocessBuffer::ClearAll(void) const
{
	if(harm_r_clearPostprocessBuffer.GetBool())
		return;
	assert(fb);
    fb->Bind();
	for(int k = 0; k < sizeof(globalImages->postProcessBuffers) / sizeof(globalImages->postProcessBuffers[0]); k++)
	{
		fb->AttachImage2D(images[k]);
		Clear();
		fb->AttachColorBuffer();
	}
    fb->Unbind();
}

sdPostprocessBuffer postprocessBuffer;
