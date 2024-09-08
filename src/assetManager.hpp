#pragma once
#include <iostream>
#include "util.hpp"
#include <msfl.hpp>
#include "settings.hpp"
#include "path.hpp"

#ifndef RELEASE_MODE
#if WIN32
#ifdef VS
#define PROJECT_DIR "C:/Users/chris/Downloads/moop-main/moop-main/"
#else
#define PROJECT_DIR "../../../../"
#endif
#else
#define PROJECT_DIR "../../../"
#endif
#else
#define PROJECT_DIR ""
#endif
#define GLOBAL_MAP_PATH Path::GetOSPath("dat_src/global_map.json")

#define GLOBAL_MAP_DATA_LABEL "_glb_output"
#define ASSET_OUTPUT_FORMAT ".dat"
#define GLBL_MAP_OUT "dat/glbl.map"

struct _AssetStore {
    std::string id;
    Asset* asset;
};

class AssetManager {
private:
    static std::vector<_AssetStore> _store;
    static Asset* _map;
    static JStruct map_struct;
public:
    static Asset *ReqAsset(std::string id, bool store = true);
    static i32 compileDat(std::string map);
    static void free();
};