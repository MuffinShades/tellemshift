#include "util.hpp"
#include "assetManager.hpp"

std::vector<_AssetStore> AssetManager::_store;
Asset *AssetManager::_map = nullptr;
JStruct AssetManager::map_struct;

i32 AssetManager::compileDat(std::string map) {
    std::cout << "Map: " << map << std::endl;
    if (map.length() <= 0)
        return 1;

    //read map file
    arr_container<byte> map_fDat = FileWriter::ReadBytesFromBinFile(map);

    if (map_fDat.sz <= 0 || map_fDat.dat == nullptr) {
        if (map_fDat.dat != nullptr)
            delete[] map_fDat.dat;

        return 1;
    }

    //now parse file
    JStruct dMap = jparse::parseStr(const_cast<const char*>(reinterpret_cast<char*>(map_fDat.dat)));
    delete[] map_fDat.dat;

    if (dMap.body.size() <= 0)
        return 2;

    std::cout << "Found: " << dMap.body.size() << " items" << std::endl;

    std::cout << "MAp: " << backslashEscapeAll("{\"map\":\"" + map + "\"}") << std::endl;
    AssetParse::WriteToFile(Path::GetOSPath(GLBL_MAP_OUT), backslashEscapeAll("{\"map\":\"" + map + "\"}"));

    for (auto& itm : dMap.body) {

        //create asset file
        std::string baseDir = itm.body->FindToken("dir").rawValue;
        if (baseDir.length() <= 0) continue;
        std::string aMap = baseDir + itm.body->FindToken("map_target").rawValue;
        if (aMap.length() <= 0) continue;
        else aMap = Path::GetOSPath(aMap);
        std::string outputFile = itm.body->FindToken("output_file").rawValue;
        if (outputFile.length() <= 0) continue;
        else outputFile = Path::GetOSPath(outputFile);

        arr_container<byte> _map = FileWriter::ReadBytesFromBinFile(aMap);

        if (_map.dat == nullptr || _map.sz <= 0) {
            if (_map.dat != nullptr)
                delete[] _map.dat;

            continue;
        }

        //convert map data to string
        std::string map_dat = std::string(
            const_cast<const char*>(
                reinterpret_cast<char*>(_map.dat)
            )
        );

        _clipStr(map_dat, _map.sz);

        AssetParse::WriteToFile(outputFile, map_dat, Path::GetOSPath(baseDir));
        delete[] _map.dat;
    }

    return 0;
}

Asset* _create_asset_copy(Asset* a) {
    Asset* r = new Asset;
    memcpy(r, a, sizeof(Asset));
    memcpy(&r->inf, &a->inf, sizeof(AssetDescriptor));
    r->__nb_free = true;
    return r;
}

Asset *AssetManager::ReqAsset(std::string id, bool store) {
    if (_map == nullptr) {
        std::cout << "MAP PATH: " << Path::GetOSPath(GLBL_MAP_OUT) << std::endl;
        Asset e = AssetParse::ExtractAssetFromFile(
            Path::GetOSPath(GLBL_MAP_OUT),
            "map"
        );

        if (e.bytes == nullptr || e.sz <= 0) {
            std::cout << "Error couldnt load map, INVALID PATH" << std::endl;
            return nullptr;
        }

        _map = _create_asset_copy(&e);
        std::string mapDat = std::string(const_cast<const char*>(reinterpret_cast<char*>(_map->bytes)));
        _clipStr(mapDat, _map->sz);
        map_struct = jparse::parseStr(mapDat.c_str());
    }

    std::string file = "";

    const char* id_c = id.c_str();
    const size_t idLen = id.length();
    char* _c = const_cast<char*>(id_c);
    
    do { 
        file += *_c;
    } while ((size_t)(_c - id_c) < idLen  && *++_c != '.');

    const size_t subLen = (size_t)(_c - id_c) + 1;

    JToken fTok = map_struct.FindToken(file);

    if (fTok.body == nullptr)
        return nullptr;

    file = Path::GetOSPath(fTok.body->FindToken("output_file").rawValue);

    const size_t f_len = file.length();

    //check store first
    for (auto& _s : _store)
        if (_strCompare(_s.id, id, true, id.length()))
            return _create_asset_copy(_s.asset);

    std::cout << "Extracting: " << id.substr(subLen) << std::endl;

    Asset extracted = AssetParse::ExtractAssetFromFile(file, id.substr(subLen));

    //store assset if needed
    if (store) {
        Asset* re = nullptr;
        _store.push_back({
            .id = std::string(id),
            .asset = (re = _create_asset_copy(&extracted))
        });

        return re;
    }

    return _create_asset_copy(&extracted);
}

void AssetManager::free() {
    for (auto& a : _store)
        if (a.asset != nullptr) {
            delete[] a.asset->bytes;
            a.asset->bytes = nullptr;
            delete a.asset;
        }
}