#pragma once
#include <iostream>
#include "glad.h"
#include "util.hpp"
#include "assetManager.hpp"
#include "vec.hpp"

class Texture {
private:
	u64 handle;
	static u32 next_open_slot;
	u32 texId;
	float invW = 0.0f, invH = 0.0f;
public:
	i32 w = 0, h = 0;
	i32 id = -1;
	float fid = -1.0f;
	Texture(std::string texAssetId, struct graphics *g);
	vec2 NormalizeCoord(vec2 p);
	u32 GetHandle();
};