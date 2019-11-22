/**
 * Yay0 Decoder
 * 
 * Based on thakis' yay0dec software.
 * 
 * @author Brenden Davidson
 * @date 11-21-19
 * @version 0.1
*/

#include <filesystem>
#include <iostream>
#include <array>
#include <vector>
#include <string>

namespace fs = std::filesystem;

namespace yay0dec {
    const static std::string HELP_TEXT = "Usage: ./yay0dec [input_file]";

    struct DataPositions {
        uint32_t srcPos;
        uint32_t dstPos;
    };

    /**
     * Converts the Yay0 big-endian format to the more standard little-endian format.
     * 
     * @param input big-endian DWORD from the Yay0 file
     * @return little-endian DWORD
    */
    uint32_t getDWORD(const std::vector<uint8_t>& data, const uint32_t& location);
    
    /**
     * Decodes a Yay0-encoded block of data
     * 
     * @param codes
     * @param counts
     * @param srcData
     * @param uncompressedSize
     * @return DataPositions struct. 
    */
    DataPositions decodeYay0(const std::vector<uint8_t>& codes, const std::vector<uint8_t>& counts, const std::vector<uint8_t>& srcData, std::vector<uint8_t> *destData, const uint32_t& uncompressedSize);

    void decodeAll(const std::vector<uint8_t>& srcData, const uint32_t& srcSize, fs::path& srcPath);
};