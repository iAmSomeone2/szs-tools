/**
 * Rarc-dumping tool based on work by thakis and updated to modern C++
 * 
 * @author Brenden Davidson <davidson.brenden15@gmail.com>
 * @date 11-21-19
 * @version 1.0
*/

#ifndef RARC_DUMP_HXX
#define RARC_DUMP_HXX

#include <filesystem>
#include <string>
#include <iostream>
#include <array>
#include <vector>

#include "szstools.hxx"
#include "RarcHeader.hxx"
#include "RarcNode.hxx"
#include "FileEntry.hxx"

namespace fs = std::filesystem;

namespace szstools {
    class RarcDump {
    public:
        explicit RarcDump(const fs::path &inputPath);
        ~RarcDump();

        void dump();
    private:
        RarcHeader *header;
        RarcNode *rootNode;
        std::vector<char> fileData;

        /**
         * Initializes the instance header using a vector of bytes.
         * 
         * @param headerBytes a vector of bytes to read into the header.
        */
        void initHeader();

        static void fillVector(std::vector<uint32_t> *targetVec, std::vector<char>& inVec, uint32_t *readPos, size_t size);

        RarcNode *getNode(uint32_t index);

        void dumpNode(const RarcNode& node);

        std::string getString(uint32_t offset);
    };
};

#endif // RARC_DUMP_HXX