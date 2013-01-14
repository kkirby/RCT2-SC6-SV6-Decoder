#ifndef CONVERTER_H_MWUSGPP7
#define CONVERTER_H_MWUSGPP7

#include "../reader/reader.h"
#include <vector>

class CChunkReader {
	public:
		struct ChunkEncoding {
			enum Type {
				WithoutChange,
				Rle,
				RleCompressed,
				Rotate
			};
		};
		
		class SByteArrayContainer {
			public:
				byteArray bytes;
				size_t length;
				SByteArrayContainer(byteArray bytes, size_t length){
					this->bytes = bytes;
					this->length = length;
				}
				~SByteArrayContainer(){
					delete[] this->bytes;
				}
		};
		
		CChunkReader(const CReader * fileReader);
		~CChunkReader();
		
		SByteArrayContainer * readChunk() throw (CReadException);
		
		static void rotateBitsRight(byte & value, unsigned int rotationCount);
		static void rleDecode(byteArray & bytes, size_t & length);
		static void stringDecode(byteArray & bytes, size_t & length);
		static void copyVectorToByteArray(const std::vector<byte> & from, byteArray & to);
	protected:
		const CReader * _fileReader;
};

#endif /* end of include guard: CONVERTER_H_MWUSGPP7 */
