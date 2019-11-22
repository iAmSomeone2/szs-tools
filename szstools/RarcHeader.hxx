/**
 * Rarc-dumping tool based on work by thakis and updated to modern C++
 * 
 * @author Brenden Davidson <davidson.brenden15@gmail.com>
 * @date 11-21-19
 * @version 1.0
*/

#ifndef RARC_HEADER_HXX
#define RARC_HEADER_HXX

#include <iostream>
#include <vector>

namespace szstools {
    class RarcHeader {
    public:
        std::vector<char> type;
        uint32_t size;
        uint32_t unknown;
        uint32_t dataStartOffset;
        std::vector<uint32_t> unknown2;

        uint32_t numNodes;
        std::vector<uint32_t> unknown3;
        uint32_t fileEntriesOffset;
        uint32_t unknown4;
        uint32_t stringTableOffset;
        std::vector<uint32_t> unknown5;
    };
};

#endif // RARC_HEADER_HXX