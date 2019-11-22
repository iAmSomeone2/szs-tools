/**
 * Rarc-dumping tool based on work by thakis and updated to modern C++
 * 
 * @author Brenden Davidson <davidson.brenden15@gmail.com>
 * @date 11-21-19
 * @version 1.0
*/

#include <array>

#include "szstools.hxx"


void szstools::toLittleEndian(uint16_t& word) {
    uint8_t upperByte = word & static_cast<uint8_t>(0xFF);
    uint8_t lowerByte = word >> static_cast<uint8_t>(8);
    
    word = static_cast<uint16_t>((upperByte << static_cast<uint8_t>(8)) | lowerByte);
}

void szstools::toLittleEndian(uint32_t& dword) {
    std::array<uint8_t, 4> bytes{};
    bytes[0] = dword & static_cast<uint8_t>(0xFF);
    bytes[1] = (dword >> static_cast<uint8_t>(8)) & static_cast<uint8_t>(0xFF);
    bytes[2] = (dword >> static_cast<uint8_t>(16)) & static_cast<uint8_t>(0xFF);
    bytes[3] = dword >> static_cast<uint8_t>(24);

    dword = static_cast<uint32_t>((bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3]);
}

/**
 * Converts a big-endian dword to little-endian.
 * 
 * This version only affects a single dword of the character vector. No other parts
 * are changed.
 * 
 * @param dwordVec pointer to a character vector.
 * @param pos first index of dword
*/
void szstools::toLittleEndian(std::vector<char>& dwordVec, uint32_t pos) {
    char temp = dwordVec.at(pos+3);
    dwordVec.at(pos) = temp;
    temp = dwordVec.at(pos+2);
    dwordVec.at(pos+1) = temp;
}

/**
 * Converts a 4-byte section of the vector into a little-endian uint32
 * 
 * @param dwordVec vector to read from
 * @param pos position in vector to read from
 * @return 32-bit little-endian value. 
*/
uint32_t szstools::toUInt32(std::vector<char>& dwordVec, uint32_t pos) {
    std::array<uint8_t, 4> temp{};
    temp[0] = static_cast<uint8_t>(dwordVec[pos]);
    temp[1] = static_cast<uint8_t>(dwordVec[pos + 1]);
    temp[2] = static_cast<uint8_t>(dwordVec[pos + 2]);
    temp[3] = static_cast<uint8_t>(dwordVec[pos + 3]);

    return static_cast<uint32_t>((temp[0] << 24) | (temp[1] << 16) | (temp[2] << 8) | temp[3]);
}