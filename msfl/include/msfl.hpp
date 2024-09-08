#pragma once

#define DLLVERSION 0.2

/*

	Msfl.

	muffinshades file library

	Written in C++ by James Weigand (muffinshades)
	Copyright muffinshades 2023-Present all right reserved.
	This library can be used by anyone but all credit for
	this library goes to muffinshades.

	Lines of code 10,000+

	Can read and write to:
		.txt
		.bin
		.cfg
		.properties
		.ini
		.inf
		.pak*
		.json
		.png  (no support yet but mostly done)
		.xml
		.zip  (no support yet)
		.wav  (no support yet)
		.yml  (no support yet)
		.yaml (no support yet)
		.msa
		.ap
*/


//all the includes
#include "asset.hpp"
#include "balloon.hpp"
#include "BinUtility.h"
#include "bitmap.hpp"
#include "bitmapRender.hpp"
#include "ByteStream.hpp"
#include "cfgParse.hpp"
#include "Date.hpp"
#include "FileWriter.h"
#include "jparse.hpp"
#include "pak.hpp"
#include "ptrconverter.hpp"
#include "msutil.hpp"
#include "RawBytes.hpp"
#include "ttf.hpp"
#include "ttfRender.hpp"
#include "xml.hpp"