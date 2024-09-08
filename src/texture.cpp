#include "texture.hpp"
#include "stb_image.h"
#include "png.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "graphics.hpp"

u32 Texture::next_open_slot = 0;

Texture::Texture(std::string texAssetId, graphics *g) {
	if (texAssetId.length() <= 0)
		return;

	//get the data from the asset
	Asset* tex_asset = AssetManager::ReqAsset(texAssetId);

	if (tex_asset == nullptr)
		return;


	//create texture
	glGenTextures(1, &this->texId);
	glBindTexture(GL_TEXTURE_2D, this->texId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//add data to texture
	png_file iDat = PngParse::DecodeBytes(tex_asset->bytes, tex_asset->sz);

	if (iDat.data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iDat.width, iDat.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, iDat.data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	this->handle = glGetTextureHandleARB(this->texId);
	glMakeTextureHandleResidentARB(this->handle);

	this->w = iDat.width;
	this->h = iDat.height;
	this->invW = 1.0f / this->w;
	this->invH = 1.0f / this->h;


	delete[] iDat.data;
	delete tex_asset;

	if (g != nullptr)
		this->id = g->AddTexture(this);

	std::cout << "Created texture handle: " << this->handle << std::endl;
}

u32 Texture::GetHandle() {
	return this->handle;
}

vec2 Texture::NormalizeCoord(vec2 pos) {
	return vec2(pos.x * this->invW, pos.y * this->invH);
}