#include <iostream>
#include "util.hpp"
#include "assetManager.hpp"
#include <msfl.hpp>
#include "path.hpp"
#include "window.hpp"
#include "graphics.hpp"
#include "fps.hpp"

graphics *g = nullptr;

int main(void) {
    std::cout << "Uninteresting Game" << std::endl;
    Path::SetProjectDir(PROJECT_DIR); //set project directory
    i32 asset_res = 0;
    if (asset_res = AssetManager::compileDat(GLOBAL_MAP_PATH)) std::cout << "Failed to compile assets! Err code " << asset_res << std::endl; //compile assets
    std::cout << "Asset Compiel Status: " << asset_res << std::endl;
    std::cout << "INI: " << Window::winIni() << std::endl;

    Window testWindow;

    testWindow = Window("Uninteresting Game", 700, 700);

    glfwSwapInterval(0);

    g = new graphics(&testWindow);

    std::cout << "GMem Usage: " << g->getEstimatedMemoryUsage() << std::endl;

    g->Load("Graphics.Shaders.Default.Vert", "Graphics.Shaders.Default.Frag");
    std::cout << "Loaded graphics api :3" << std::endl;
    g->WinResize(testWindow.w, testWindow.h);

    int idf = 0;

    Texture testTexture = Texture(Path::GetOSPath("/dat_src/graphics/Spritesheets/"), g);

    while (testWindow.isRunning()) {
        testWindow.Update();
        graphics::FrameStart();
        g->SetClearColor(255.0f, 0.0f, 0.0f, 255.0f);
        /*const float clearChannel = (idf % 255) * 1.0f;
        graphics::SetClearColor(clearChannel, clearChannel, clearChannel);
        //glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        //glClear(GL_COLOR_BUFFER_BIT);
        fps_counter::start();
        for (size_t i = 0; i < 100000; i++) {
            g->SetFillColor(i % 255, (i + 99) % 255, (i + 219) % 255, 255.0f);
            g->FillRect(i % 600, (i ^ (int)(fps_counter::fps) ^ (idf++) + idf / (i + 1)) % 600, 3, 3);
        }*/


        g->DrawImage(testTexture, 0.0f, 0.0f, 500.0f, 100.0f, 0.0f, 0.0f, 100.0f, 100.0f);

        g->render_flush();

        if (fps_counter::f_tick()) {
            std::cout << "FPS: " << fps_counter::fps << " | Delta: " << fps_counter::GetTimeDelta() << " | Frame Time: " << (1000.0f / fps_counter::fps) << "ms" << std::endl;
        }
    }

    //old testing code now to actually code stuff
    //msfl testing
    /*arr_container<byte> jBytes = FileWriter::ReadBytesFromBinFile("C:\\mooptest\\testAssetMap.json");
    std::string j_map = std::string(const_cast<const char*>(reinterpret_cast<char*>(jBytes.dat)));

    AssetParse::WriteToFile("C:\\mooptest\\assetout.pak", j_map);
    delete[] jBytes.dat;*/


    //try parsing something
    /*Asset extractedAsset = AssetParse::ExtractAssetFromFile("C:\\mooptest\\assetout.pak", "L");
    std::cout << "Extracted Sz: " << extractedAsset.sz << std::endl;
    std::cout << "Name: " << extractedAsset.inf.fname << "." << extractedAsset.inf.fileType << std::endl;
    std::cout << "Sz: " << extractedAsset.inf.sz << " " << extractedAsset.sz << std::endl;
    FileWriter::WriteToBinFile("C:\\mooptest\\out2.jpeg", extractedAsset.bytes, extractedAsset.inf.sz);

    delete[] extractedAsset.bytes;
    std::cout << "..." << std::endl;*/

    AssetManager::free();
    g->free();
    delete g;

    return 0;
}