
#include <asset_loader.h>

#include <fstream>
#include <iostream>
using namespace assets;
bool assets::save_binaryfile(const  char* path, const AssetFile& file)
{
	std::ofstream outfile;
	outfile.open(path, std::ios::binary | std::ios::out);
	if (!outfile.is_open())
	{
		std::cout << "Error when trying to write file: " << path << std::endl;
	}
	outfile.write(file.type, 4);
	uint32_t version = file.version;
	//version
	outfile.write((const char*)&version, sizeof(uint32_t));

	//json lenght
	uint32_t lenght = static_cast<uint32_t>(file.json.size());
	outfile.write((const char*)&lenght, sizeof(uint32_t));

	//blob lenght
	uint32_t bloblenght = static_cast<uint32_t>(file.binaryBlob.size());
	outfile.write((const char*)&bloblenght, sizeof(uint32_t));

	//json stream 
	outfile.write(file.json.data(), lenght);
	//pixel data
	outfile.write(file.binaryBlob.data(), file.binaryBlob.size());

	outfile.close();

	return true;
}

bool assets::load_binaryfile(const  char* path, AssetFile& outputFile)
{
	std::ifstream infile;
	infile.open(path, std::ios::binary);

	if (!infile.is_open()) {
		std::cout << "Error when load assert binary file: " << path << std::endl;
		return false;
	}
	infile.seekg(0);


	infile.read(outputFile.type, 4);
	
	infile.read((char*)&outputFile.version, sizeof(uint32_t));

	//get json length
	uint32_t jsonlen = 0;
	infile.read((char*)&jsonlen, sizeof(uint32_t));
	//get binary blob length
	uint32_t bloblen = 0;
	infile.read((char*)&bloblen, sizeof(uint32_t));

	outputFile.json.resize(jsonlen);

	infile.read(outputFile.json.data(), jsonlen);

	outputFile.binaryBlob.resize(bloblen);
	infile.read(outputFile.binaryBlob.data(), bloblen);

	return true;
}

assets::CompressionMode assets::parse_compression(const char* f)
{
	if (strcmp(f, "LZ4") == 0)
	{
		return assets::CompressionMode::LZ4;
	}
	else {
		return assets::CompressionMode::None;
	}
}

bool assets::compareType(const char* type, const char* newtype) {
	for (int i = 0; i < 4; i++) {
		if (type[i] != newtype[i]) return false;
	}
	return true;
}
