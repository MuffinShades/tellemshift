#include "bitmap.hpp"

/**
 *
 * Super simple bitmap graphics class
 * 
 * Used for drawing pixels and stuff to a bitmap.
 * Gonna use for testing and ttf rendering to a
 * bitmap.
 * 
 */

#ifdef __cplusplus
extern "C" {
#endif

#define MSFL_COMPILE_DLL
#define MSFL_EXPORTS

#ifdef WIN32
	#ifdef MSFL_COMPILE_DLL
		#ifdef MSFL_EXPORTS
			#define MSFL_EXP __declspec(dllexport)
		#else
			#define MSFL_EXP __declspec(dllimport)
		#endif
	#else
		#define MSFL_EXP
	#endif
#else
	#define MSFL_EXP
#endif

struct _bmpColor {
    byte r, g, b, a;
};

class BitmapGraphics {
private:
    Bitmap *bmp;
    _bmpColor tColor = {0,0,0,0};
public:
    BitmapGraphics(Bitmap *bmp = nullptr) : bmp(bmp) {}
    MSFL_EXP void SetColor(byte r = 0, byte g = 0, byte b = 0, byte a = 0);
    MSFL_EXP void DrawPixel(u32 x, u32 y);
    MSFL_EXP void ClearColor();
    MSFL_EXP void Clear();
};

#ifdef __cplusplus
}
#endif