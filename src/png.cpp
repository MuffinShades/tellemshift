#include "png.hpp"

const u64 sig[] = { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A };
const size_t sigSz = 8;

//chunk types
//#define IDHR 0x49484452
//#define IDAT 0x49444152
//#define IEND 0x49454e44

enum chunk_type {
	NULL_CHUNK = 0x0,
	IHDR = 0x49484452,
	IDAT = 0x49444154,
	IEND = 0x49454e44,
	sRGB = 0x73524742
};

struct png_chunk {
	size_t len;
	chunk_type type;
	byte* dat;
	u32 checksum;
	bool good = false;
};

chunk_type int_to_cType(int ty) {
	switch (ty) {
	case IHDR: {
		return IHDR;
	}
	case IDAT: {
		return IDAT;
	}
	case IEND: {
		return IEND;
	}
	case sRGB: {
		return sRGB;
	}
	}

	return NULL_CHUNK;
}

//read chunk
png_chunk readNextChunk(ByteStream& stream) {
	png_chunk res;
	res.len = stream.readInt32();
	int iTy = stream.readInt32();
	res.type = int_to_cType(iTy);

	std::cout << iTy << " <- Chunk type" << std::endl;
	std::cout << "Chunk info: " << res.len << std::endl;

	if (res.len > 0)
		res.dat = new byte[res.len];
	else
		res.dat = nullptr;

	for (int i = 0; i < res.len; i++)
		res.dat[i] = stream.readByte();

	res.checksum = stream.readUInt32();
	res.good = true;
	return res;
};

arr_container<byte> *DecodeIDAT(png_chunk c) {

	//inflate everything
	u32* uBytes = ptrconverter::convertTo<byte, u32>(c.dat, c.len);
	ZResult* dat = Zlib::Inflate(uBytes, c.len);
	delete[] uBytes;
	arr_container<byte> *res = new arr_container<byte>();
	res->dat = ptrconverter::convertTo<u32, byte>(dat->bytes, dat->len);
	res->sz = dat->len;
	//std::cout << "PNG INFLATE DAT: " << dat->len << "   " << std::endl;
	//for (size_t i = 0; i < dat->len; i++) {
		//res.dat[i] = (byte)dat->bytes[i];
		//std::cout << (int) res.dat[i] << " ";
	//}

	delete[] dat->bytes;

	return res;
}

byte *defilterDat(byte* i_dat, const size_t datSz, i32 w, i32 h) {
	//do nothing for now
	const u32 bbp = 4;
	const u32 scanline = w * bbp;

	auto df_sub = [](byte *i_dat, byte *out, size_t curScaneline, size_t scanline, const u32 rowOff, const u32 bpp, size_t &p, u32 i) {
		return (i_dat[p++] + (i >= bbp ? out[rowOff + (i - bbp)] : 0)) & 0xff;
	};

	auto df_up = [](byte* i_dat, byte* out, size_t curScanline, size_t scanline, const u32 rowOff, const u32 bpp, size_t& p, u32 i) {
		return (i_dat[p++] + ((curScanline > 0) ? out[(rowOff - scanline) + i] : 0)) & 0xff;
	};

	byte* out = new byte[datSz - h], *c_out = out;
	ZeroMem(out, datSz - h);
	size_t p = 0;
	for (size_t sline = 0; sline < h; sline++) { //current scane line (y)
		const u32 filterMethod = i_dat[p++], rowOff = sline * scanline;
		for (u32 i = 0; i < scanline; i++) { // current position in scale line (x)
			switch (filterMethod) {
			//0 -> no filter just put data back how it is
			case 0: {
				(*c_out++) = i_dat[p++] &  0xff;
				break;
			}
			//1 -> sub filter
			case 1: {
				//current = current data + prev
				(*c_out++) = df_sub(i_dat, out, sline, scanline, rowOff, bbp, p, i);
				break;
			}
			//2 -> up filter
			case 2: {
				(*c_out++) = df_up(i_dat, out, sline, scanline, rowOff, bbp, p, i);
				break;
			}
			//3 -> average filter
			case 3: {
				(*c_out++) = i_dat[p++] + (df_sub(i_dat, out, sline, scanline, rowOff, bbp, p, i) + df_up(i_dat, out, sline, scanline, rowOff, bbp, p, i));
				break;
			}
			//4 paeth filter
			case 4: {
				(*c_out++) = (i_dat[p++] & 0xff);
				break;
			}
			  deafult: {
				  std::cout << "Error invalid filter type!" << std::endl;
				  return nullptr;
			}
			}
		}
	}

	delete[] i_dat;
	return out;
};

png_file PngParse::Decode(std::string src) {
	png_file rs;
	if (src == "")
		return rs;
	arr_container<byte> fDat = FileWriter::ReadBytesFromBinFile(src);
	if (fDat.dat == nullptr)
		return rs;
	rs = PngParse::DecodeBytes(fDat.dat, fDat.sz);
	delete[] fDat.dat;
	return rs;
}

#define MSFL_PNG_DEBUG

png_file PngParse::DecodeBytes(byte* bytes, size_t sz) {
	std::cout << "Decoding Png: " << sz << std::endl;
	FileWriter::WriteToBinFile("C:\\TestStuff\\decodedpng.png", bytes, sz);

	auto freeChunk = [](png_chunk p) {
		if (p.dat != nullptr)
			delete[] p.dat;
		};

	ByteStream stream = ByteStream(bytes, sz);
	stream.mode = bmode_BigEndian; //set stream mode to big endian since that's what pngs use

	png_file rpng;

	//get for signature
	for (auto sig_byte : sig)
		if (stream.readByte() != sig_byte) {
			std::cout << "Error invalid png sig!" << std::endl;
			return rpng;
		}

	//read in header chunk
	png_chunk headChunk = readNextChunk(stream);

	if (headChunk.type != IHDR) {
		std::cout << "[png error] first chunk was not a header chunk!" << std::endl;
		return rpng;
	}

	//parse header chunk
	ByteStream hStream = ByteStream(headChunk.dat, headChunk.len);
	size_t iWidth = hStream.readUInt32(),
		iHeight = hStream.readUInt32();


	//read some header stuff
	i32 bitDepth = hStream.readByte(), 
		colorSpace = hStream.readByte(), 
		compressionMethod = hStream.readByte(), 
		_filterMethod = hStream.readByte();

	bool interlacing = (bool)hStream.readByte();

#ifdef MSFL_PNG_DEBUG
	std::cout << "Png Header: \n\tWidth: " << iWidth << "\n\tHeight: " << iHeight << "\n\tBit Depth: " << bitDepth << "\n\tColor Space" << colorSpace << "\n\tFilter Method: " << _filterMethod << "\n\tInterlacing: " << interlacing << std::endl;
#endif

	freeChunk(headChunk);

	const size_t decodeDatSz = iWidth * iHeight * 4 + iHeight;
	byte* imgDat = new byte[decodeDatSz];
	size_t iDPos = 0;

	//parse idat chunks
	for (;;) {
		png_chunk dat = readNextChunk(stream);

		switch (dat.type) {
		case IHDR: break;
		case sRGB: { //eh idk whats in this chunk anyway
			break;
		}
		case IDAT: {
			std::cout << "Decoding IDAT..." << std::endl;
			arr_container<byte> *i_dat = DecodeIDAT(dat);

			//std::cout << "Dat: " << imgDat << " " << iDPos << " " << i_dat.dat << " " << i_dat.sz << std::endl;

			//now add the data to the pixel stuff
			memcpy(imgDat + iDPos, i_dat->dat, i_dat->sz);
			iDPos += i_dat->sz;

			delete[] i_dat->dat;
			delete i_dat;
			break;
		}
		default: {
			if (dat.len > 0)
				freeChunk(dat);
			break;
		}
		}

		if (dat.type != NULL_CHUNK)
			freeChunk(dat);
		//if (stream.eos()) break;
		// 

		if (stream.tell() >= stream.getSize()) break;
	}

	//defilter the data
	imgDat = defilterDat(imgDat, decodeDatSz, iWidth, iHeight);

	/*std::cout << "IMAGE DAT: " << std::endl;
	for (int i = 0; i < iWidth * iHeight * 4; i++) {
		std::cout << imgDat[i] << " ";
	}
	std::cout << std::endl;*/

	//for testing just write data to a bitmap
	Bitmap testOut;
	testOut.header.w = iWidth;
	testOut.header.h = iHeight;
	testOut.header.fSz = iWidth * iHeight * 4;
	testOut.data = imgDat;
	BitmapParse::WriteToFile("C:\\TestStuff\\testpngread.bmp", &testOut);

	return {
		.data = imgDat,
		.sz = iWidth * iHeight * 4,
		.width = iWidth,
		.height = iHeight,
		.channels = bitDepth,
		.src = ""
	};
}

#include "crc32.hpp"

/*
 * Function to write png headers aka IHDR chunks
 */
/*void WritePngHeader(ByteStream& stream, pngInfo pInf) {
	ByteStream hStream;

	//write chunk data
	hStream.writeUInt32(IHDR);
	hStream.writeUInt32(pInf.width);
	hStream.writeUInt32(pInf.height);
	hStream.writeByte(pInf.bitDepth);
	hStream.writeByte(6);
	hStream.writeByte(NULL); //color space set it to null
	hStream.writeByte(NULL); //compression method is always zlib so set to 0
	hStream.writeByte((byte)false); //no interlacing

	//add hStream to main stream
	u32 sSz = hStream.GetSize() - 4;
	stream.writeUInt32(sSz);
	u32 checkSum = crc32::GenFirstCrc();

	//crc32::ComputeNextU32(checkSum, sSz);

	for (size_t i = 0; i < hStream.GetSize(); i++) {
		stream.writeByte(hStream.bytes[i]);
		checkSum = crc32::ComputeNext(checkSum, hStream.bytes[i]);
	}

	//finally write the checksum
	crc32::Finish(checkSum);
	stream.writeUInt32(checkSum);
}

void WriteIDAT(ByteStream& stream, byte* dat, size_t sz) {
	u32* uDat = ptrconverter::convertTo<byte, u32>(dat, sz);
	ZResult compressedDat = Zlib::Deflate(uDat, sz, 15, 1); //compress le dat
	delete[] uDat;

	//now write chunk
	u32 checkSum = crc32::GenFirstCrc();
	stream.writeUInt32(compressedDat.len);
	//crc32::ComputeNextU32(checkSum, compressedDat.len);
	stream.writeUInt32(IDAT);
	crc32::ComputeNextU32(checkSum, IDAT);

	std::cout << "Compressed Size: " << compressedDat.len << " Og Size: " << sz << std::endl;

	for (size_t i = 0; i < compressedDat.len; i++) {
		stream.writeByte(compressedDat.bytes[i]);
		checkSum = crc32::ComputeNext(checkSum, compressedDat.bytes[i]);
	}

	delete[] compressedDat.bytes;

	//add checksum
	crc32::Finish(checkSum);
	stream.writeUInt32(checkSum);
}

//writes image data to a .png file
bool PngParse::WriteToFile(std::string src, byte* dat, size_t sz, pngInfo pInf) {
	if (src == "")
		return false;

	//open file
	std::ofstream oStream = std::ofstream(src);

	if (!oStream.good()) return false;
	oStream.close();

	//now encode and create le chunks
	if (dat == nullptr || sz <= 0) return false;

	ByteStream datStream;

	//write png sig
	for (auto s : sig)
		datStream.writeByte(s);

	//write header
	WritePngHeader(datStream, pInf);

	//write a sRGB chunk
	u32 cs = crc32::GenFirstCrc();
	datStream.writeUInt32(1);
	//crc32::ComputeNextU32(cs, 1);
	datStream.writeUInt32(sRGB);
	crc32::ComputeNextU32(cs, sRGB);
	datStream.writeByte(NULL);
	cs = crc32::ComputeNext(cs, 0);
	crc32::Finish(cs);
	datStream.writeUInt32(cs);

	//now write the main IDAT chunk, for now we wont apply filtering
	WriteIDAT(datStream, dat, sz);

	//now add iend chunk
	u32 endCrc = crc32::GenFirstCrc();
	datStream.writeUInt32(0);
	//crc32::ComputeNextU32(endCrc, 0);
	datStream.writeInt32(IEND);
	crc32::ComputeNextU32(endCrc, IEND);
	crc32::Finish(endCrc);
	datStream.writeUInt32(endCrc);

	//now write data to file
	FileWriter::WriteToBinFile(src, datStream.bytes, datStream.GetSize());
}*/