#include "Sprite.h"

Sprite::Sprite(const wchar_t *filename, Graphics *graphics, float scaleWidth, float scaleHeight, int nFrameWidth, int nFrameHeight)
{
	this->graphics = graphics;

	IWICImagingFactory *factory = NULL;
	CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)&factory);

	IWICBitmapDecoder *decoder = NULL;
	factory->CreateDecoderFromFilename(filename, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder);

	IWICBitmapFrameDecode *frame = NULL;
	decoder->GetFrame(0, &frame);

	IWICFormatConverter *converter = NULL;
	factory->CreateFormatConverter(&converter);

	converter->Initialize(frame, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0, WICBitmapPaletteTypeCustom);

	graphics->GetRenderTarget()->CreateBitmapFromWicBitmap(converter, NULL, &bmp);

	if (factory) factory->Release();
	if (decoder) decoder->Release();
	if (frame) frame->Release();
	if (converter) converter->Release();

	this->nFrameWidth = nFrameWidth;
	this->nFrameHeight = nFrameHeight;
	this->scaleWidth = scaleWidth;
	this->scaleHeight = scaleHeight;
}

Sprite::~Sprite()
{
	if (bmp) bmp->Release();
}

void Sprite::Draw(int frameWidth, int frameHeight, int x, int y, float scaleWidth, float scaleHeight)
{
	this->scaleWidth = scaleWidth;
	this->scaleHeight = scaleHeight;

	float spriteWidth = bmp->GetSize().width / nFrameWidth;
	float spriteHeight = bmp->GetSize().height / nFrameHeight;
	D2D1_RECT_F source = D2D1::RectF(0.0f + frameWidth*spriteWidth, 0.0f + frameHeight*spriteHeight, (frameWidth + 1) * spriteWidth, (frameHeight + 1) * spriteHeight);
	D2D1_RECT_F dest = D2D1::RectF(x, y, x+(spriteWidth*scaleWidth), y+(spriteHeight*scaleHeight));
	graphics->GetRenderTarget()->DrawBitmap(bmp, dest, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, source);
}

std::pair<int, int> Sprite::GetSize()
{
	return std::make_pair<int, int>((int)((bmp->GetSize().width / nFrameWidth)*scaleWidth), (int)((bmp->GetSize().height / nFrameHeight)*scaleHeight));
}