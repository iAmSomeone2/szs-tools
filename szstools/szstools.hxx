/**
 * Rarc-dumping tool based on work by thakis and updated to modern C++
 * 
 * @author Brenden Davidson <davidson.brenden15@gmail.com>
 * @date 11-21-19
 * @version 1.0
*/

#ifndef SZSTOOLS_HXX
#define SZSTOOLS_HXX

#include <iostream>
#include <vector>

namespace szstools {
    const static uint8_t HEADER_SIZE = 64;
    const static uint8_t NODE_SIZE = 16;
    const static uint8_t FILE_ENTRY_SIZE = 20;

    static uint32_t min(uint32_t a, uint32_t b) {
        return a < b?a:b;
    }

    static uint32_t max(uint32_t a, uint32_t b) {
        return a > b?a:b;
    }

    /**
     * Converts a big-endian word to little-endian.
     * 
     * @param word pointer to big-endian 16-bit value.
    */
    void toLittleEndian(uint16_t& word);

    /**
     * Converts a big-endian dword to little-endian.
     * 
     * @param dword pointer to big-endian 32-bit value.
    */
    void toLittleEndian(uint32_t& dword);

    /**
     * Converts a big-endian dword to little-endian.
     * 
     * This version only affects a single dword of the character vector. No other parts
     * are changed.
     * 
     * @param dwordVec pointer to a character vector.
     * @param pos first index of dword
    */
    void toLittleEndian(std::vector<char>& dwordVec, uint32_t pos);

    /**
     * Converts a 4-byte section of the vector into a little-endian uint32
     * 
     * @param dwordVec vector to read from
     * @param pos position in vector to read from
     * @return 32-bit little-endian value. 
    */
    uint32_t toUInt32(std::vector<char>& dwordVec, uint32_t pos);
};

#endif // SZSTOOLS_HXX