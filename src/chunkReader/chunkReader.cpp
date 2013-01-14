#include "chunkReader.h"

CChunkReader::CChunkReader(const CReader * fileReader){
	this->_fileReader = fileReader;
}

CChunkReader::~CChunkReader(){
	
}

CChunkReader::CByteArrayContainer * CChunkReader::readChunk() throw(CReadException) {
	size_t chunkLength = 0;
	byteArray buffer = this->_fileReader->read(1);
	ChunkEncoding::Type chunkType = (ChunkEncoding::Type)buffer[0];
	delete[](buffer);
	buffer = this->_fileReader->read(4);
	memcpy(&chunkLength,buffer,4);
	delete[](buffer);
	buffer = this->_fileReader->read(chunkLength);
	//std::cout << (int)buffer[0] << "\n\n";
	// Start decoding
	if(chunkType == ChunkEncoding::Rotate){
		for(size_t i = 0; i < chunkLength; i++){
			CChunkReader::rotateBitsRight(
				buffer[i],
				(i << 1 & 7) + 1
			);
		}
	}
	else if(chunkType == ChunkEncoding::Rle){
		CChunkReader::rleDecode(
			buffer,
			chunkLength
		);
	}
	else if(chunkType == ChunkEncoding::RleCompressed){
		CChunkReader::rleDecode(
			buffer,
			chunkLength
		);
		CChunkReader::stringDecode(
			buffer,
			chunkLength
		);
	}
	CByteArrayContainer * returnContainer = new CByteArrayContainer(buffer,chunkLength);
	return returnContainer;
}

void CChunkReader::rotateBitsRight(byte & value, unsigned int rotationCount){
	value = (value >> rotationCount) | ((value & ((1 << rotationCount) - 1)) << sizeof(byte) - rotationCount);
}

void CChunkReader::rleDecode(byteArray & bytes, size_t & length){
	std::vector<byte> targetByteArray(0);
	targetByteArray.reserve(length * 2);
	for(size_t i = 0; i < length; i++){
		byte & observingByte = bytes[i];
		byte msb = (observingByte & 128) >> 7;
		if(msb == 0){
			for(size_t x = 0; x <= observingByte; x++){
				targetByteArray.push_back(bytes[++i]);
			}
		}
		else if(msb == 1){
			i++;
			size_t copyLength = 1 - observingByte + 256;
			for(size_t x = 0; x < copyLength; x++){
				targetByteArray.push_back(bytes[i]);
			}
		}
	}
	length = targetByteArray.size();
	CChunkReader::copyVectorToByteArray(
		targetByteArray,
		bytes
	);
}

void CChunkReader::stringDecode(byteArray & bytes, size_t & length){
	std::vector<byte> targetByteArray(0);
	targetByteArray.reserve(length * 2);
	for(size_t i = 0; i < length; i++){
		if(bytes[i] == 0xFF){
			targetByteArray.push_back(bytes[++i]);
		}
		else {
			size_t copyLength = bytes[i] & 7;
			size_t copyOffset = targetByteArray.size() + (int)(bytes[i] >> 3) - 32;
			for(size_t x = 0; x <= copyLength; x++){
				targetByteArray.push_back(targetByteArray[copyOffset + x]);
			}
		}
	}
	length = targetByteArray.size();
	CChunkReader::copyVectorToByteArray(
		targetByteArray,
		bytes
	);
}

void CChunkReader::copyVectorToByteArray(const std::vector<byte> & from, byteArray & to){
	size_t length = from.size() * sizeof(byte);
	to = (byteArray)realloc(to,length);
	memcpy(to,&from[0],length);
}