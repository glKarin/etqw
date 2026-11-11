#include "../idlib/precompiled.h"
#pragma hdrstop

#include "../tr_local.h"

typedef struct VKImageFilter_s
{
	bool minLinear;
	bool magLinear;
	bool mipmapLinear;
	bool usingMipmap;
} VKImageFilter_t;

typedef struct VKImageRepeat_s
{
	int repeatS;
	int repeatT;
	int repeatU;
} VKImageRepeat_t;

typedef struct VKActuallyLoadImage_data_s
{
	idImage *image;
	bool checkForPrecompressed;
	const byte *cinData;
	int cols;
	int rows;

	VKActuallyLoadImage_data_s( idImage *image, bool checkForPrecompressed )
			: image( image ),
			  checkForPrecompressed( checkForPrecompressed ),
			  cinData(NULL),
			  cols(NULL),
			  rows(NULL)
	{}
	VKActuallyLoadImage_data_s( idImage *image, const byte *data, int cols, int rows )
			: image( image ),
			  checkForPrecompressed( false ),
			  cinData(data),
			  cols(cols),
			  rows(rows)
	{
		//cinData = malloc(cols * rows * 4);
		//memcpy(cinData, data, cols * rows * 4);
	}

	VKActuallyLoadImage_data_s()
			: image( NULL ),
			  checkForPrecompressed( false ),
			  cinData(NULL),
			  cols(NULL),
			  rows(NULL)
	{}

#if 0
	~VKActuallyLoadImage_data_s() {
		if(cinData)
			free(cinData);
	}
#endif
} VKActuallyLoadImage_data_t;

static idList<VKActuallyLoadImage_data_t> loadImages[NUM_INFLIGHT_FRAME];
static bool imageHandled[NUM_INFLIGHT_FRAME];

void R_VK_AddLoadImageList( idImage * image, bool checkForPrecompressed )
{
    loadImages[vk_context.frameIndex].Append( VKActuallyLoadImage_data_t( image, checkForPrecompressed ) );
}

void R_VK_AddUploadScratchImageList( idImage * image, const byte *data, int cols, int rows )
{
    loadImages[vk_context.frameIndex].Append( VKActuallyLoadImage_data_t( image, data, cols, rows ) );
}

void R_VK_HandleLoadImageList( void )
{
	if (loadImages[vk_context.frameIndex].Num() > 0)
	{
		imageHandled[vk_context.frameIndex] = true;
		VKActuallyLoadImage_data_t *loadImg = loadImages[vk_context.frameIndex].Ptr();
		for(int i = 0; i < loadImages[vk_context.frameIndex].Num(); i++, loadImg++)
		{
			if(loadImg->cinData)
				loadImg->image->UploadScratch_VK(loadImg->cinData, loadImg->cols, loadImg->rows);
			else
				loadImg->image->ActuallyLoadImage(loadImg->checkForPrecompressed, false);
		}
		loadImages[vk_context.frameIndex].SetNum(0);
		imageHandled[vk_context.frameIndex] = false;
	}
}

bool R_VK_DisableUploadImage(void)
{
	return vk_context.inRenderPass && !imageHandled[vk_context.frameIndex];
}

extern byte	mipBlendColors[16][4];

typedef struct VKImageMipmap_s
{
	int width;
	int height;
	int offset;
	int length;
} VKImageMipmap_t;

typedef idList<VKImageMipmap_t> VKImageMipmapList;

static void ByteListAppend(idList<byte> &list, const void *data, int length)
{
	int oldOffset = list.Num();
	idList<byte> tmp;
	tmp.SetNum(oldOffset + length);
	memcpy(tmp.Ptr(), list.Ptr(), oldOffset);
	memcpy(tmp.Ptr() + oldOffset, data, length);
	list.Swap(tmp);
}

#if 0
static int Image_CalcMipmapLevels(int scaled_width, int scaled_height, int *sizes)
{
	int miplevel = 1;
	if(sizes)
	{
		sizes[0] = scaled_width;
		sizes[1] = scaled_height;
	}

	while (scaled_width > 1 || scaled_height > 1) {
		scaled_width >>= 1;
		scaled_height >>= 1;

		if (scaled_width < 1) {
			scaled_width = 1;
		}

		if (scaled_height < 1) {
			scaled_height = 1;
		}

		if(scaled_width + scaled_height == 2)
			break;

		if(sizes)
		{
			sizes[miplevel * 2] = scaled_width;
			sizes[miplevel * 2 + 1] = scaled_height;
		}
		miplevel++;
	}

	return miplevel;
}

static int Image_CalcMipmapSize(int scaled_width, int scaled_height, bool cube, int *offsets)
{
	int side = (cube ? 6 : 1);
	int mipSize = scaled_width * scaled_height * 4 * side;
	if(offsets)
		*(offsets++) = 0;

	while (scaled_width > 1 || scaled_height > 1) {

		scaled_width >>= 1;
		scaled_height >>= 1;

		if (scaled_width < 1) {
			scaled_width = 1;
		}

		if (scaled_height < 1) {
			scaled_height = 1;
		}

		if(scaled_width + scaled_height == 2)
			break;

		if(offsets)
			*(offsets++) = mipSize;

		int size = scaled_width * scaled_height * 4 * side;

		mipSize += size;
	}

	return mipSize;
}

static int Image_CalcMapmapLevels(int scaled_width, int scaled_height, int *sizes)
{
	int miplevel = 1;
	if(sizes)
	{
		sizes[0] = scaled_width;
		sizes[1] = scaled_height;
	}

	while (scaled_width > 1 || scaled_height > 1) {
		scaled_width >>= 1;
		scaled_height >>= 1;

		if (scaled_width < 1) {
			scaled_width = 1;
		}

		if (scaled_height < 1) {
			scaled_height = 1;
		}

		if(scaled_width + scaled_height == 2)
			break;

		if(sizes)
		{
			sizes[miplevel * 2] = scaled_width;
			sizes[miplevel * 2 + 1] = scaled_height;
		}
		miplevel++;
	}

	return miplevel;
}
#endif

static VkSamplerAddressMode R_VK_ImageSamplerRepeatMode(textureRepeat_t repeat)
{
	switch (repeat) {
		case TR_REPEAT:
			return VK_SAMPLER_ADDRESS_MODE_REPEAT;
		case TR_CLAMP_TO_BORDER:
			return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
		case TR_CLAMP_TO_ZERO:
		case TR_CLAMP_TO_ZERO_ALPHA:
		case TR_CLAMP:
#ifdef _SPLASHDAMAGE
		case TR_CLAMP_X:
		case TR_CLAMP_Y:
#endif
			return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
#ifdef _SPLASHDAMAGE
		case TR_MIRROR:
			return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
		case TR_MIRROR_X:
		case TR_MIRROR_Y:
			return VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE;
#endif
		default:
			common->Warning("R_CreateImage: bad texture repeat %d\n", repeat);
			return VK_SAMPLER_ADDRESS_MODE_REPEAT;
	}
}

static void R_VK_ImageSamplerRepeatModes(textureRepeat_t repeat, VKImageRepeat_t *ret)
{
	ret->repeatS = ret->repeatT = ret->repeatU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	switch (repeat) {
	case TR_REPEAT:
	case TR_CLAMP_TO_BORDER:
	case TR_CLAMP_TO_ZERO:
	case TR_CLAMP_TO_ZERO_ALPHA:
	case TR_CLAMP:
#ifdef _SPLASHDAMAGE
	case TR_MIRROR:
#endif
		ret->repeatS = ret->repeatT = ret->repeatU = R_VK_ImageSamplerRepeatMode(repeat);
		break;
#ifdef _SPLASHDAMAGE
	case TR_CLAMP_X:
	case TR_MIRROR_X:
		ret->repeatS = R_VK_ImageSamplerRepeatMode(repeat);
		break;
	case TR_CLAMP_Y:
	case TR_MIRROR_Y:
		ret->repeatT = R_VK_ImageSamplerRepeatMode(repeat);
		break;
#endif
	default:
		common->Warning("R_CreateImage: bad texture repeat %d\n", repeat);
		break;
	}
}

static void R_VK_ImageSamplerGLFilterMode(int minMode, int magMode, VKImageFilter_t *ret)
{
	switch (minMode) {
		case GL_LINEAR:
			ret->minLinear = true;
			ret->mipmapLinear = false;
			ret->usingMipmap = false;
			break;
		case GL_NEAREST:
			ret->minLinear = false;
			ret->mipmapLinear = false;
			ret->usingMipmap = false;
			break;
		case GL_NEAREST_MIPMAP_NEAREST:
			ret->minLinear = false;
			ret->mipmapLinear = false;
			ret->usingMipmap = true;
			break;
		case GL_NEAREST_MIPMAP_LINEAR:
			ret->minLinear = false;
			ret->mipmapLinear = true;
			ret->usingMipmap = true;
			break;
		case GL_LINEAR_MIPMAP_NEAREST:
			ret->minLinear = true;
			ret->mipmapLinear = false;
			ret->usingMipmap = true;
			break;
		case GL_LINEAR_MIPMAP_LINEAR:
			ret->minLinear = true;
			ret->mipmapLinear = true;
			ret->usingMipmap = true;
			break;
		default:
			common->Warning("R_CreateImage: bad texture GL filter %d\n", minMode);
			ret->minLinear = false;
			ret->mipmapLinear = true;
			ret->usingMipmap = true;
			break;
	}

	switch (magMode) {
		case GL_LINEAR:
			ret->magLinear = true;
			break;
		case GL_NEAREST:
			ret->magLinear = false;
			break;
		default:
			common->Warning("R_CreateImage: bad texture GL filter %d\n", magMode);
			ret->magLinear = true;
			break;
	}
}

static void R_VK_ImageSamplerFilterMode(textureFilter_t filter, VKImageFilter_t *ret)
{
	switch (filter) {
		case TF_LINEAR:
			ret->minLinear = true;
			ret->magLinear = true;
			ret->mipmapLinear = false;
			ret->usingMipmap = false;
			break;
		case TF_NEAREST:
			ret->minLinear = false;
			ret->magLinear = false;
			ret->mipmapLinear = false;
			ret->usingMipmap = false;
			break;
		case TF_DEFAULT:
			R_VK_ImageSamplerGLFilterMode(globalImages->textureMinFilter, globalImages->textureMaxFilter, ret);
			break;
		default:
			common->Warning("R_CreateImage: bad texture filter %d\n", filter);
			ret->minLinear = false;
			ret->magLinear = true;
			ret->mipmapLinear = true;
			ret->usingMipmap = true;
			break;
	}
}

static VkSampler R_VK_GetSampler(textureFilter_t linear, textureRepeat_t repeat, bool depthCmp, float lod)
{
	VKImageFilter_t filters;
	VKImageRepeat_t repeats;

	R_VK_ImageSamplerFilterMode(linear, &filters);
	R_VK_ImageSamplerRepeatModes(repeat, &repeats);
	return R_VK_AllocSampler(filters.minLinear, filters.magLinear, filters.mipmapLinear, repeats.repeatS, repeats.repeatT, repeats.repeatU, depthCmp, filters.usingMipmap ? lod : 0.0f);
}

static int R_VK_AllocImageBuffer(VKImage_t *image, int size, const uint8_t *data, const VKImageMipmap_t *mipmaps, int numLods)
{
	if(!data)
		return 0;

	if (numLods > 1)
	{
		// 生成mipmap数据
		idList<int> sizes;
		idList<int> offsets;
		sizes.SetNum(numLods * 2);
		offsets.SetNum(numLods);

		for (int i = 0; i < numLods; i++)
		{
			sizes[i * 2] = mipmaps[i].width;
			sizes[i * 2 + 1] = mipmaps[i].height;
			offsets[i] = mipmaps[i].offset;
		}

		// 创建缓冲区
		R_VK_UploadImageData(image, size, data, offsets.Ptr(), sizes.Ptr());

		return size;
	}
	else
	{
		// 创建缓冲区
		R_VK_UploadImageData(image, size, data, NULL, NULL);
		return size;
	}
}

/*
================
GenerateImage_VK

The alpha channel bytes should be 255 if you don't
want the channel.

We need a material characteristic to ask for specific texture modes.

Designed limitations of flexibility:

No support for texture borders.

No support for texture border color.

No support for texture environment colors or GL_BLEND or GL_DECAL
texture environments, because the automatic optimization to single
or dual component textures makes those modes potentially undefined.

No non-power-of-two images.

No palettized textures.

There is no way to specify separate wrap/clamp values for S and T

There is no way to specify explicit mip map levels

================
*/
void idImage::GenerateImage_VK(const byte *pic, int width, int height,
							textureFilter_t filterParm, bool allowDownSizeParm,
							textureRepeat_t repeatParm, textureDepth_t depthParm)
{
	bool	preserveBorder;
	byte		*scaledBuffer;
	int			scaled_width, scaled_height;
	byte		*shrunk;

	PurgeImage_VK();

	filter = filterParm;
	allowDownSize = allowDownSizeParm;
	repeat = repeatParm;
	depth = depthParm;

	// if we don't have a rendering context, just return after we
	// have filled in the parms.  We must have the values set, or
	// an image match from a shader before OpenGL starts would miss
	// the generated texture

	if (!glConfig.isInitialized) {
		return;
	}

	// don't let mip mapping smear the texture into the clamped border
	if (repeat == TR_CLAMP_TO_ZERO) {
		preserveBorder = true;
	} else {
		preserveBorder = false;
	}

	// make sure it is a power of 2
	scaled_width = MakePowerOfTwo(width);
	scaled_height = MakePowerOfTwo(height);
#ifdef _SPLASHDAMAGE
	sourceWidth = scaled_width;
	sourceHeight = scaled_height;
#endif

	if (scaled_width != width || scaled_height != height) {
		common->Error("GenerateImage_VK: not a power of 2 image");
	}

	// Optionally modify our width/height based on options/hardware
	GetDownsize(scaled_width, scaled_height);

	scaledBuffer = NULL;

	// generate the texture number
	texnum = VK_AllocImage(&vk_image);

	// select proper internal format before we resample
	internalFormat = SelectInternalFormat(&pic, 1, width, height, depth);

	// copy or resample data as appropriate for first MIP level
	if ((scaled_width == width) && (scaled_height == height)) {
		// we must copy even if unchanged, because the border zeroing
		// would otherwise modify const data
		scaledBuffer = (byte *)R_StaticAlloc(sizeof(unsigned) * scaled_width * scaled_height);
		memcpy(scaledBuffer, pic, width*height*4);
	} else {
		// resample down as needed (FIXME: this doesn't seem like it resamples anymore!)
		// scaledBuffer = R_ResampleTexture( pic, width, height, width >>= 1, height >>= 1 );
		scaledBuffer = R_MipMap(pic, width, height, preserveBorder);
		width >>= 1;
		height >>= 1;

		if (width < 1) {
			width = 1;
		}

		if (height < 1) {
			height = 1;
		}

		while (width > scaled_width || height > scaled_height) {
			shrunk = R_MipMap(scaledBuffer, width, height, preserveBorder);
			R_StaticFree(scaledBuffer);
			scaledBuffer = shrunk;

			width >>= 1;
			height >>= 1;

			if (width < 1) {
				width = 1;
			}

			if (height < 1) {
				height = 1;
			}
		}

		// one might have shrunk down below the target size
		scaled_width = width;
		scaled_height = height;
	}

	uploadHeight = scaled_height;
	uploadWidth = scaled_width;
	type = TT_2D;

	// zero the border if desired, allowing clamped projection textures
	// even after picmip resampling or careless artists.
	if (repeat == TR_CLAMP_TO_ZERO) {
		byte	rgba[4];

		rgba[0] = rgba[1] = rgba[2] = 0;
		rgba[3] = 255;
		R_SetBorderTexels((byte *)scaledBuffer, width, height, rgba);
	}

	if (repeat == TR_CLAMP_TO_ZERO_ALPHA) {
		byte	rgba[4];

		rgba[0] = rgba[1] = rgba[2] = 255;
		rgba[3] = 0;
		R_SetBorderTexels((byte *)scaledBuffer, width, height, rgba);
	}

#ifdef _SPLASHDAMAGE //karin: image generator functor
	if (generatorFunction == NULL && generatorFunctor == NULL && ( ( depth == TD_BUMP && globalImages->image_writeNormalTGA.GetBool() ) || ( depth != TD_BUMP && globalImages->image_writeTGA.GetBool() ) ))
#else
	if (generatorFunction == NULL && ( ( depth == TD_BUMP && globalImages->image_writeNormalTGA.GetBool() ) || ( depth != TD_BUMP && globalImages->image_writeTGA.GetBool() ) ))
#endif
	{
		// Optionally write out the texture to a .tga
		char filename[MAX_IMAGE_NAME];
		ImageProgramStringToCompressedFileName(imgName, filename);
		char *ext = strrchr(filename, '.');

		if (ext) {
			strcpy(ext, ".tga");
			// swap the red/alpha for the write
			/*
			if ( depth == TD_BUMP ) {
				for ( int i = 0; i < scaled_width * scaled_height * 4; i += 4 ) {
					scaledBuffer[ i ] = scaledBuffer[ i + 3 ];
					scaledBuffer[ i + 3 ] = 0;
				}
			}
			*/
			R_WriteTGA(filename, scaledBuffer, scaled_width, scaled_height, false);

			// put it back
			/*
			if ( depth == TD_BUMP ) {
				for ( int i = 0; i < scaled_width * scaled_height * 4; i += 4 ) {
					scaledBuffer[ i + 3 ] = scaledBuffer[ i ];
					scaledBuffer[ i ] = 0;
				}
			}
			*/
		}
	}

	// swap the red and alpha for rxgb support
	// do this even on tga normal maps so we only have to use
	// one fragment program
	// if the image is precompressed ( either in palletized mode or true rxgb mode )
	// then it is loaded above and the swap never happens here
	if (depth == TD_BUMP && globalImages->image_useNormalCompression.GetInteger() != 1) {
#if 1 //k2023 texture2D().agb
		if(1)
#else
		if (glConfig.textureCompressionAvailable)
#endif
		{
		for (int i = 0; i < scaled_width * scaled_height * 4; i += 4) {
			scaledBuffer[ i + 3 ] = scaledBuffer[ i ];
			scaledBuffer[ i ] = 0;
		}
		}
		else // texture2D().rgb
		{
		for (int i = 0; i < scaled_width * scaled_height * 4; i += 4) {
			scaledBuffer[ i + 3 ] = 255;
		}
		}
	}

	// upload the main image level
	Bind();

	// create and upload the mip map levels, which we do in all cases, even if we don't think they are needed
	int		miplevel;

	miplevel = 0;

	idList<byte> imgData;
	ByteListAppend(imgData, scaledBuffer, scaled_width * scaled_height * 4);
	VKImageMipmapList mipmapList;
	VKImageMipmap_t &base = mipmapList.Alloc();
	base.width = scaled_width;
	base.height = scaled_height;
	base.offset = 0;
	base.length = scaled_width * scaled_height * 4;
	int baseWidth = scaled_width;
	int baseHeight = scaled_height;

	while (scaled_width > 1 || scaled_height > 1) {
		// preserve the border after mip map unless repeating
		shrunk = R_MipMap(scaledBuffer, scaled_width, scaled_height, preserveBorder);
		R_StaticFree(scaledBuffer);
		scaledBuffer = shrunk;

		scaled_width >>= 1;
		scaled_height >>= 1;

		if (scaled_width < 1) {
			scaled_width = 1;
		}

		if (scaled_height < 1) {
			scaled_height = 1;
		}

		miplevel++;

#ifdef _SPLASHDAMAGE
		if(mipmapState != defaultMipmapState)
		SetMipmapLevel( scaledBuffer, scaled_width, scaled_height, miplevel, mipmapState );
#endif

		// this is a visualization tool that shades each mip map
		// level with a different color so you can see the
		// rasterizer's texture level selection algorithm
		// Changing the color doesn't help with lumminance/alpha/intensity formats...
		if (depth == TD_DIFFUSE && globalImages->image_colorMipLevels.GetBool()) {
			R_BlendOverTexture((byte *)scaledBuffer, scaled_width * scaled_height, mipBlendColors[miplevel]);
		}

		// upload the mip map
		VKImageMipmap_t &lod = mipmapList.Alloc();
		lod.width = scaled_width;
		lod.height = scaled_height;
		lod.offset = imgData.Num();
		lod.length = scaled_width * scaled_height * 4;
		ByteListAppend(imgData, scaledBuffer, scaled_width * scaled_height * 4);

		//R_WritePNG(va("list/%s_%d.png", imgName.c_str(), miplevel + 1), scaledBuffer, scaled_width, scaled_height, 4);
	}

	if (scaledBuffer != 0) {
		R_StaticFree(scaledBuffer);
	}

	VK_CreateImage(vk_image, baseWidth, baseHeight, VK_FORMAT_R8G8B8A8_UNORM, false, 1, miplevel + 1, IU_SAMPLER);
	SetDebugInfo_VK(false);
	VkSampler sampler = R_VK_GetSampler(filter, repeat, false, vk_image->mipLevels - 1);
	vk_image->sampler = sampler;

	assert(mipmapList.Num() == vk_image->mipLevels);

	R_VK_AllocImageBuffer(vk_image, imgData.Num(), imgData.Ptr(), mipmapList.Ptr(), mipmapList.Num());
}

/*
====================
GenerateCubeImage_VK

Non-square cube sides are not allowed
====================
*/
void idImage::GenerateCubeImage_VK(const byte *pic[6], int size,
                                textureFilter_t filterParm, bool allowDownSizeParm,
                                textureDepth_t depthParm)
{
	int			scaled_width, scaled_height;
	int			width, height;
	int			i;

	PurgeImage_VK();

	filter = filterParm;
	allowDownSize = allowDownSizeParm;
	depth = depthParm;

	type = TT_CUBIC;

	// if we don't have a rendering context, just return after we
	// have filled in the parms.  We must have the values set, or
	// an image match from a shader before OpenGL starts would miss
	// the generated texture
	if (!glConfig.isInitialized) {
		return;
	}

#if 0
	if (! glConfig.cubeMapAvailable) {
		return;
	}
#endif

	width = height = size;
#ifdef _SPLASHDAMAGE
	sourceWidth = width;
	sourceHeight = height;
#endif

	// generate the texture number
	texnum = VK_AllocImage(&vk_image);

	// select proper internal format before we resample
	internalFormat = SelectInternalFormat(pic, 6, width, height, depth);

	// don't bother with downsample for now
	scaled_width = width;
	scaled_height = height;

	uploadHeight = scaled_height;
	uploadWidth = scaled_width;

	Bind();

	// no other clamp mode makes sense
	idList<byte> imgData;
	VKImageMipmapList mipmapList;

	// upload the base level
	VKImageMipmap_t &base = mipmapList.Alloc();
	base.width = scaled_width;
	base.height = scaled_height;
	base.offset = 0;
	base.length = scaled_width * scaled_height * 4;
	int baseWidth = scaled_width;
	int baseHeight = scaled_height;
	int byteSize = scaled_width * scaled_height * 4;
	for (i = 0 ; i < 6 ; i++) {
		ByteListAppend(imgData, pic[i], byteSize);
	}

	// create and upload the mip map levels
	int		miplevel;
	byte	*shrunk[6];

	for (i = 0 ; i < 6 ; i++) {
		shrunk[i] = R_MipMap(pic[i], scaled_width, scaled_height, false);
	}

	miplevel = 1;

	while (scaled_width > 1) {
		int lodWidth = scaled_width / 2;
		int lodHeight = scaled_height / 2;
		VKImageMipmap_t &lod = mipmapList.Alloc();
		lod.width = lodWidth;
		lod.height = lodHeight;
		lod.offset = imgData.Num();
		lod.length = lodWidth * lodHeight * 4;
		byteSize = lodWidth * lodHeight * 4;
		for (i = 0 ; i < 6 ; i++) {
			ByteListAppend(imgData, shrunk[i], byteSize);
			//R_WritePNG(va("cubelist/%s_%d_%d.png", imgName.c_str(), i, miplevel), shrunk[i], lodWidth, lodHeight, 4);
		}

		for (i = 0 ; i < 6 ; i++) {
			byte	*shrunken;

			if (scaled_width > 2) {
				shrunken = R_MipMap(shrunk[i], scaled_width/2, scaled_height/2, false);
			} else {
				shrunken = NULL;
			}

			R_StaticFree(shrunk[i]);
			shrunk[i] = shrunken;
		}

		scaled_width >>= 1;
		scaled_height >>= 1;
		miplevel++;
	}

	VK_CreateImage(vk_image, baseWidth, baseHeight, VK_FORMAT_R8G8B8A8_UNORM, true, 1, miplevel, IU_SAMPLER);
	SetDebugInfo_VK(false);
	VkSampler sampler = R_VK_GetSampler(filter, TR_REPEAT, false, vk_image->mipLevels - 1);
	vk_image->sampler = sampler;

	assert(mipmapList.Num() == vk_image->mipLevels);

	R_VK_AllocImageBuffer(vk_image, imgData.Num(), imgData.Ptr(), mipmapList.Ptr(), mipmapList.Num());
}

void idImage::GenerateDepthStencilImage_VK( int width, int height, bool allowDownSizeParm, textureFilter_t filterParm, textureRepeat_t repeatParm, int component, int stencilComponent, bool compare )
{
	int			scaled_width, scaled_height;

	PurgeImage_VK();

	filter = TF_NEAREST;
	allowDownSize = allowDownSizeParm;
	repeat = repeatParm;
	depth = TD_HIGH_QUALITY;
	cubeFiles = CF_2D;

	type = TT_2D;

	// if we don't have a rendering context, just return after we
	// have filled in the parms.  We must have the values set, or
	// an image match from a shader before the render starts would miss
	// the generated texture
	if (!glConfig.isInitialized) {
		return;
	}

	// make sure it is a power of 2
	scaled_width = MakePowerOfTwo(width);
	scaled_height = MakePowerOfTwo(height);

	if (scaled_width != width || scaled_height != height) {
		common->Error("GenerateDepthStencilImage_VK: not a power of 2 image");
	}

#ifdef _SPLASHDAMAGE
	sourceWidth = scaled_width;
	sourceHeight = scaled_height;
#endif
	// Optionally modify our width/height based on options/hardware
	//GetDownsize(scaled_width, scaled_height);

	scaled_width = width;
	scaled_height = height;
	uploadWidth = scaled_width;
	uploadHeight = scaled_height;

	int dataType;
	switch (component) {
		case 32:
			internalFormat = GL_DEPTH32F_STENCIL8;
			dataType = VK_FORMAT_D32_SFLOAT_S8_UINT;
			break;
		case 24:
		default:
			internalFormat = GL_DEPTH24_STENCIL8;
			dataType = VK_FORMAT_D24_UNORM_S8_UINT;
			break;
	}

	texnum = VK_AllocImage(&vk_image);

	Bind();

	// no other clamp mode makes sense

	// set the minimize / maximize filtering

	// upload the base level

	//GL_CheckErrors("GenerateDepthStencilImage::start");
	VK_CreateImage(vk_image, scaled_width, scaled_height, dataType, false, 1, 1, IU_SAMPLER | IU_ATTACHMENT);
	SetDebugInfo_VK(false);
	//GL_CheckErrors("GenerateDepthStencilImage::end");

	VkSampler sampler = R_VK_GetSampler(TF_NEAREST, repeat, compare, vk_image->mipLevels - 1);
	vk_image->sampler = sampler;

	// see if we messed anything up
	if(compare)
	{
		qglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE );
		qglTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL );
	}
	else
	{
        VK_ChooseImageView(vk_image, IA_STENCIL);
	}
}

/*
===============
PurgeImage_VK
===============
*/
#ifdef _MULTITHREAD
void idImage::PurgeImage_VK(bool pending)
#else
void idImage::PurgeImage_VK()
#endif
{
#ifdef _MULTITHREAD
	if(multithreadActive && pending)
	{
		renderThread->AddPurgeList(this);
		return;
	}
#endif
	if (vk_image) {
		R_VK_DestroyImage(texnum);	// this should be the ONLY place it is ever called!
		SetDebugInfo_VK(true);
		texnum = TEXTURE_NOT_LOADED;
	}
	vk_image = NULL;

	// clear all the current binding caches, so the next bind will do a real one
	for (int i = 0 ; i < MAX_MULTITEXTURE_UNITS ; i++) {
		backEnd.glState.tmu[i].current2DMap = -1;
		backEnd.glState.tmu[i].current3DMap = -1;
		backEnd.glState.tmu[i].currentCubeMap = -1;
	}
}

/*
=============
RB_UploadScratchImage_VK

if rows = cols * 6, assume it is a cube map animation
=============
*/
void idImage::UploadScratch_VK(const byte *data, int cols, int rows)
{
	int			i;

	// if rows = cols * 6, assume it is a cube map animation
	if (rows == cols * 6) {
		if (type != TT_CUBIC) {
			type = TT_CUBIC;
			uploadWidth = -1;	// for a non-sub upload
		}

		Bind();

		rows /= 6;

		// if the scratchImage isn't in the format we want, specify it as a new texture
		if (cols != uploadWidth || rows != uploadHeight) {
#ifdef _SPLASHDAMAGExxx
			sourceWidth = cols;
			sourceHeight = rows;
#endif
			uploadWidth = cols;
			uploadHeight = rows;

			// upload the base level
			const int oldLayers = vk_image->layers;
			const int oldFormat = vk_image->format;
			RecreateImage_VK();
			VK_CreateImage(vk_image, uploadWidth, uploadHeight, oldFormat, true, oldLayers, 1, IU_SAMPLER);
			SetDebugInfo_VK(false);
			R_VK_AllocImageBuffer(vk_image, cols * rows * 4 * 6, data, NULL, NULL);
		}

		// no other clamp mode makes sense
		VkSampler sampler = R_VK_GetSampler(TF_LINEAR, TR_CLAMP_TO_ZERO, false, 0);
		vk_image->sampler = sampler;
	} else {
		// otherwise, it is a 2D image
		if (type != TT_2D) {
			type = TT_2D;
			uploadWidth = -1;	// for a non-sub upload
		}

		Bind();

		// if the scratchImage isn't in the format we want, specify it as a new texture
		if (cols != uploadWidth || rows != uploadHeight) {
#ifdef _SPLASHDAMAGExxx
			sourceWidth = cols;
			sourceHeight = rows;
#endif
			uploadWidth = cols;
			uploadHeight = rows;
			VKImage_t *newImage;
			VKHandle_t newHandle = VK_AllocImage(&newImage);
			VK_SwapImageHandle(texnum, newHandle);
			VK_CreateImage(newImage, uploadWidth, uploadHeight, vk_image->format, false, vk_image->layers, 1, IU_SAMPLER);
			SetDebugInfo_VK(false);
			R_VK_AllocImageBuffer(newImage, cols * rows * 4, data, NULL, NULL);
			R_VK_DestroyImage(newHandle);
			vk_image = newImage;
		} else {
			// otherwise, just subimage upload it so that drivers can tell we are going to be changing
			// it and don't try and do a texture compression
			int sizes[] = { cols, rows };
			int offsets[] = { 0 };
			R_VK_UploadImageSubData(vk_image, cols * rows * 4, data, 0, offsets, sizes);
		}

		// these probably should be clamp, but we have a lot of issues with editor
		// geometry coming out with texcoords slightly off one side, resulting in
		// a smear across the entire polygon
		VkSampler sampler = R_VK_GetSampler(TF_LINEAR, TR_REPEAT, false, 0);
		vk_image->sampler = sampler;
	}
}

/*
====================
CopyFramebuffer_VK
====================
*/
void idImage::CopyFramebuffer_VK(int x, int y, int imageWidth, int imageHeight, bool useOversizedBuffer)
{
	Bind();

	if (cvarSystem->GetCVarBool("g_lowresFullscreenFX")) {
		imageWidth = 512;
		imageHeight = 512;
	}

	// if the size isn't a power of 2, the image must be increased in size
	int	potWidth, potHeight;

	potWidth = MakePowerOfTwo(imageWidth);
	potHeight = MakePowerOfTwo(imageHeight);

#ifdef _SPLASHDAMAGE
	int scaled_width = potWidth;
	int scaled_height = potHeight;
#endif
	//k: fix berserk/double/grabber/helltime vision
#if !defined(GL_ES_VERSION_2_0)
	GetDownsize(imageWidth, imageHeight);
	GetDownsize(potWidth, potHeight);
#endif

	// only resize if the current dimensions can't hold it at all,
	// otherwise subview renderings could thrash this
	if ((useOversizedBuffer && (uploadWidth < potWidth || uploadHeight < potHeight))
	    || (!useOversizedBuffer && (uploadWidth != potWidth || uploadHeight != potHeight))) {
#ifdef _SPLASHDAMAGE
		sourceWidth = scaled_width;
		sourceHeight = scaled_height;
#endif
		uploadWidth = potWidth;
		uploadHeight = potHeight;

		if (potWidth == imageWidth && potHeight == imageHeight) {
			RecreateImage_VK();
			VK_CreateImage(vk_image, imageWidth, imageHeight, VK_FORMAT_R8G8B8A8_UNORM, false, 1, 1, IU_SAMPLER);
			SetDebugInfo_VK(false);

			R_VK_CopyColorAttachmentImage(vk_image, x, y, vk_context.swapchain.width, vk_context.swapchain.height, 0, 0, imageWidth, imageHeight);
		} else {
			//byte	*junk;
			// we need to create a dummy image with power of two dimensions,
			// then do a glCopyTexSubImage2D of the data we want
			// this might be a 16+ meg allocation, which could fail on _alloca
			//junk = (byte *)Mem_Alloc(potWidth * potHeight * 4);
			//memset(junk, 0, potWidth * potHeight * 4);		//!@#
			RecreateImage_VK();
			VK_CreateImage(vk_image, potWidth, potHeight, VK_FORMAT_R8G8B8A8_UNORM, false, 1, 1, IU_SAMPLER);
			SetDebugInfo_VK(false);

			//Mem_Free(junk);

			R_VK_CopyColorAttachmentImage(vk_image, x, y, vk_context.swapchain.width, vk_context.swapchain.height, 0, 0, potWidth, potHeight);
		}
		R_VK_Bind(vk_image);

		VkSampler sampler = R_VK_GetSampler(TF_LINEAR, TR_CLAMP_TO_ZERO, false, 0);
		vk_image->sampler = sampler;
	} else {
		// otherwise, just subimage upload it so that drivers can tell we are going to be changing
		// it and don't try and do a texture compression or some other silliness
		R_VK_CopyColorAttachmentImage(vk_image, x, y, vk_context.swapchain.width, vk_context.swapchain.height, 0, 0, imageWidth, imageHeight);
	}

	// if the image isn't a full power of two, duplicate an extra row and/or column to fix bilerps
	if (imageWidth != potWidth) {
		VK_CopyColorAttachmentImage(vk_image, x+imageWidth-1, y, 1, imageHeight, imageWidth, 0);
	}

	if (imageHeight != potHeight) {
		VK_CopyColorAttachmentImage(vk_image, x, y+imageHeight-1, imageWidth, 1, 0, imageHeight);
	}

	backEnd.c_copyFrameBuffer++;
}

/*
====================
CopyDepthbuffer_VK

This should just be part of copyFramebuffer once we have a proper image type field
====================
*/
void idImage::CopyDepthbuffer_VK(int x, int y, int imageWidth, int imageHeight)
{
	Bind();

	// if the size isn't a power of 2, the image must be increased in size
	int	potWidth, potHeight;

	GLenum depthComponent = vk_context.swapchain.framebuffers[vk_context.swapchain.imageIndex].depthStencilImage.format;

	potWidth = MakePowerOfTwo(imageWidth);
	potHeight = MakePowerOfTwo(imageHeight);

	if (uploadWidth != potWidth || uploadHeight != potHeight) {
#ifdef _SPLASHDAMAGE
		sourceWidth = potWidth;
		sourceHeight = potHeight;
#endif
		uploadWidth = potWidth;
		uploadHeight = potHeight;

		if (potWidth == imageWidth && potHeight == imageHeight) {
			RecreateImage_VK();
			VK_CreateImage(vk_image, imageWidth, imageHeight, depthComponent, false, 1, 1, IU_SAMPLER);
			SetDebugInfo_VK(false);

			R_VK_CopyDepthStencilAttachmentImage(vk_image, x, y, vk_context.swapchain.width, vk_context.swapchain.height, 0, 0, imageWidth, imageHeight, IA_DEPTH_STENCIL);
			VK_ChooseImageView(vk_image, IA_DEPTH);
		} else {
			// we need to create a dummy image with power of two dimensions,
			// then do a glCopyTexSubImage2D of the data we want
			RecreateImage_VK();
			VK_CreateImage(vk_image, imageWidth, imageHeight, depthComponent, false, 1, 1, IU_SAMPLER);
			SetDebugInfo_VK(false);

			R_VK_CopyDepthStencilAttachmentImage(vk_image, x, y, vk_context.swapchain.width, vk_context.swapchain.height, 0, 0, imageHeight, imageHeight, IA_DEPTH_STENCIL);
			VK_ChooseImageView(vk_image, IA_DEPTH);
		}

		VkSampler sampler = R_VK_GetSampler(TF_NEAREST, TR_CLAMP_TO_ZERO, false, 0);
		vk_image->sampler = sampler;
	} else {
		// otherwise, just subimage upload it so that drivers can tell we are going to be changing
		// it and don't try and do a texture compression or some other silliness
		R_VK_CopyDepthStencilAttachmentImage(vk_image, x, y, vk_context.swapchain.width, vk_context.swapchain.height, 0, 0, imageWidth, imageHeight, IA_DEPTH_STENCIL);
	}
}

/*
====================
CopyStencilbuffer_VK

This should just be part of copyFramebuffer once we have a proper image type field
====================
*/
void idImage::CopyStencilbuffer_VK(int x, int y, int imageWidth, int imageHeight)
{
	Bind();

	// if the size isn't a power of 2, the image must be increased in size
	int	potWidth, potHeight;

	int stencilComponent = vk_context.swapchain.framebuffers[vk_context.swapchain.imageIndex].depthStencilImage.format;

	potWidth = MakePowerOfTwo(imageWidth);
	potHeight = MakePowerOfTwo(imageHeight);

	if (uploadWidth != potWidth || uploadHeight != potHeight) {
#ifdef _SPLASHDAMAGE
		sourceWidth = potWidth;
		sourceHeight = potHeight;
#endif
		uploadWidth = potWidth;
		uploadHeight = potHeight;

		if (potWidth == imageWidth && potHeight == imageHeight) {
			RecreateImage_VK();
			VK_CreateImage(vk_image, imageWidth, imageHeight, stencilComponent, false, 1, 1, IU_SAMPLER);
			SetDebugInfo_VK(false);

			R_VK_CopyDepthStencilAttachmentImage(vk_image, x, y, vk_context.swapchain.width, vk_context.swapchain.height, 0, 0, imageWidth, imageHeight, IA_DEPTH_STENCIL);
			VK_ChooseImageView(vk_image, IA_STENCIL);
		} else {
			// we need to create a dummy image with power of two dimensions,
			// then do a glCopyTexSubImage2D of the data we want
			RecreateImage_VK();
			VK_CreateImage(vk_image, imageWidth, imageHeight, stencilComponent, false, 1, 1, IU_SAMPLER);
			SetDebugInfo_VK(false);

			R_VK_CopyDepthStencilAttachmentImage(vk_image, x, y, vk_context.swapchain.width, vk_context.swapchain.height, 0, 0, imageHeight, imageHeight, IA_DEPTH_STENCIL);
			VK_ChooseImageView(vk_image, IA_STENCIL);
		}

		VkSampler sampler = R_VK_GetSampler(TF_NEAREST, TR_CLAMP_TO_ZERO, false, 0);
		vk_image->sampler = sampler;
	} else {
		// otherwise, just subimage upload it so that drivers can tell we are going to be changing
		// it and don't try and do a texture compression or some other silliness
		R_VK_CopyDepthStencilAttachmentImage(vk_image, x, y, vk_context.swapchain.width, vk_context.swapchain.height, 0, 0, imageWidth, imageHeight, IA_DEPTH_STENCIL);
	}

}

void idImage::RecreateImage_VK()
{
	R_VK_RecreateImage(texnum, &vk_image);

	// re-bind the texture if is current
	tmu_t			*tmu = &backEnd.glState.tmu[backEnd.glState.currenttmu];
	if (type == TT_2D) {
		if (tmu->current2DMap == texnum)
			R_VK_BindType(TUT_2D, vk_image);
	} else if (type == TT_CUBIC) {
		if (tmu->currentCubeMap != texnum)
			R_VK_BindType(TUT_CUBEMAP, vk_image);
	}
#if !defined(GL_ES_VERSION_2_0)
	else if (type == TT_3D) {
		if (tmu->current3DMap != texnum)
			R_VK_BindTexture(GL_TEXTURE_3D, vk_image);
	}
#endif
#ifdef GL_ES_VERSION_3_0
	else if (type == TT_2D_ARRAY) {
		if (tmu->current2DArrayMap != texnum)
			R_VK_BindType(TUT_2D_ARRAY, vk_image);
	}
#endif
}

void idImage::SetDebugInfo_VK(bool purge)
{
	if (!vk_image)
		return;

	const char *typeName = type == TT_CUBIC ? "cube" : "2d";

	VKDEBUG_SET_NAME(VK_OBJECT_TYPE_IMAGE, (uint64_t)vk_image->image, "%s%s_%d::%d::%s", purge ? "free_image" : "image", typeName, vk_context.frameIndex, texnum, imgName.c_str());
	VKDEBUG_SET_NAME(VK_OBJECT_TYPE_IMAGE_VIEW, (uint64_t)vk_image->imageView, "%s%s_%d::%d::%s", purge ? "free_imageView" : "imageView", typeName, vk_context.frameIndex, texnum, imgName.c_str());
	VKDEBUG_SET_NAME(VK_OBJECT_TYPE_IMAGE_VIEW, (uint64_t)vk_image->imageView2, "%s%s_%d::%d::%s", purge ? "free_imageView2" : "imageView2", typeName, vk_context.frameIndex, texnum, imgName.c_str());
}
