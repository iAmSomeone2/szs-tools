/**
 * Yay0 Decoder
 * 
 * Based on thakis' yay0dec software and rewritten in modern C++.
 * 
 * Original file can be found @ http://www.amnoid.de/gc/
 * 
 * @author Brenden Davidson
 * @date 11-21-19
 * @version 1.0
*/

#include <fstream>
#include <boost/format.hpp>

#include "yay0dec.hxx"


/**
 * Converts the Yay0 big-endian format to the more standard little-endian format.
 * 
 * @param input big-endian DWORD from the Yay0 file
 * @return little-endian DWORD
*/
uint32_t yay0dec::getDWORD(const std::vector<uint8_t>& data, const uint32_t& location) {
    std::array<uint8_t, 4> dword;
    for (uint8_t i = 0; i < 4; i++) {
        dword[i] = data[i + location];
    }

    return (dword[0] << 24) | (dword[1] << 16) | (dword[2] << 8) | dword[3];
}

/**
 * Decodes a Yay0-encoded block of data
 * 
 * @param codes
 * @param counts
 * @param srcData
 * @param uncompressedSize
 * @return DataPositions struct. 
*/
yay0dec::DataPositions yay0dec::decodeYay0(const std::vector<uint8_t>& codes, const std::vector<uint8_t>& counts, const std::vector<uint8_t>& srcData, std::vector<uint8_t> *destData, const uint32_t& uncompressedSize) {
    DataPositions dataPos = {0, 0};

    uint32_t codePos = 0, countPos = 0;
    // Number of valid bits left in the "code" byte.
    uint32_t validBitCount = 0;
    uint8_t currentCodeByte;

    while (dataPos.dstPos < uncompressedSize) {
        // Read in a new code byte if the current one has finished processing.
        if (validBitCount == 0) {
            currentCodeByte = codes[codePos];
            codePos++;
            validBitCount = 8;
        }

        if ((currentCodeByte & 0x80) != 0) {
            // Directly copy the byte.
            destData->at(dataPos.dstPos) = srcData[dataPos.srcPos];
            dataPos.dstPos++;
            dataPos.srcPos++;
        } else {
            // Deal with the byte blocks
            uint8_t byte1 = counts[countPos];
            uint8_t byte2 = counts[countPos + 1];
            countPos += 2;

            // TODO: Analyze this block to figure out what exactly it's doing.
            uint32_t dist = ((byte1 & 0xF) << 8) | byte2;
            uint32_t copySrc = dataPos.dstPos - (dist + 1);

            uint32_t numBytes = byte1 >> 4;
            if (numBytes == 0) {
                numBytes = srcData[dataPos.srcPos] + 0x12;
                dataPos.srcPos++;
            } else {
                numBytes += 2;
            }

            // Copy the chunk of bytes
            for (uint32_t i = 0; i < numBytes; i++) {
                destData->at(dataPos.dstPos) = destData->at(copySrc);
                copySrc++;
                dataPos.dstPos++;
            }
        }

        // Use the next bit from the "code" byte
        currentCodeByte <<= 1;
        validBitCount -= 1;
    }

    return dataPos;
}

void yay0dec::decodeAll(const std::vector<uint8_t>& srcData, const uint32_t& srcSize, fs::path& srcPath) {
    // Check if we actually have a Yay0 file
    std::string header = "1234";
    for (uint8_t i = 0; i < 4; i++) {
        header[i] = static_cast<char>(srcData[i]);
    }
    if (header != "Yay0") {
        throw std::runtime_error("Incorrect file type.");
    }

    uint32_t decodedSize = getDWORD(srcData, 4);
    uint32_t countOffset = getDWORD(srcData, 8);
    uint32_t dataOffset = getDWORD(srcData, 12);

    // Create destination file name.
    fs::path destPath = srcPath.replace_extension("rarc");

    std::ofstream destFile(destPath, std::ios::binary | std::ios::out | std::ios::trunc);
    if (!destFile.is_open()) {
        throw std::runtime_error("Failed to open output file for writing.");
    }

    std::cout << boost::format("Writing output to %s\n") % destPath.string();
    std::cout << boost::format("Writing 0x%X bytes\n") % decodedSize;

    std::vector<uint8_t> destData(decodedSize + 0x1000);
    std::vector<uint8_t> codes, counts, data;

    codes.assign(srcData.begin() + 16, srcData.end());
    counts.assign(srcData.begin() + countOffset, srcData.end());
    data.assign(srcData.begin() + dataOffset, srcData.end());

    // TODO: Deal with data offset for source data.
    DataPositions dataPos = decodeYay0(codes, counts, data, &destData, decodedSize);

    const char* outData = const_cast<const char*>(reinterpret_cast<char *>(destData.data()));
    destFile.write(outData, decodedSize);
    destFile.close();
}

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cout << boost::format("%s\n") % yay0dec::HELP_TEXT;
        return -1;
    }

    fs::path inputPath(argv[1]);
    std::ifstream inputFile(inputPath, std::ios::binary | std::ios::in | std::ios::ate);
    if (!inputFile.is_open()) {
        std::cerr << "Failed to open input file for reading." << std::endl;
        return -1;
    }

    // Determine size of file.
    uint32_t fileSize = inputFile.tellg();
    inputFile.seekg(inputFile.beg + 0);
    std::cout << boost::format("Input file size: 0x%X\n") % fileSize;

    // Read data from file into memory.
    std::vector<uint8_t> fileData(fileSize);
    char bufferByte;
    for (uint32_t i = 0; i < fileSize; i++) {
        inputFile.read(&bufferByte, 1);
        fileData[i] = static_cast<uint8_t>(bufferByte);
    }
    inputFile.close();

    try {
        yay0dec::decodeAll(fileData, fileSize, inputPath);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 2;
    }
    

    return 0;
}