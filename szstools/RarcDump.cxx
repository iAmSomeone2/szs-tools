/**
 * Rarc-dumping tool based on work by thakis and updated to modern C++
 * 
 * @author Brenden Davidson <davidson.brenden15@gmail.com>
 * @date 11-21-19
 * @version 1.0
*/

#include <fstream>
#include <sstream>
#include <boost/format.hpp>

#include "RarcDump.hxx"

szstools::RarcDump::RarcDump(const fs::path &inputPath) {
    std::ifstream inputFile(inputPath, std::ios::in | std::ios::binary | std::ios::ate);
    if (!inputFile.is_open()) {
        throw std::runtime_error("Failed to open input file for reading.");
    }
    size_t fileSize = inputFile.tellg();
    inputFile.seekg(std::ifstream::beg);

    fileData.resize(fileSize);
    inputFile.read(fileData.data(), fileSize);

    inputFile.close();

    header = new RarcHeader();

    // Set the appropriate vector sizes.
    header->unknown2.resize(4);
    header->unknown3.resize(2);
    header->unknown5.resize(2);

    // Set up the header struct
    initHeader();

    // Set up the root node
    rootNode = getNode(0);
}

szstools::RarcDump::~RarcDump() {
    // Clean up the RarcDump object
    delete header;
    delete rootNode;
}

/**
 * Initializes the instance header using a vector of bytes.
 * 
 * @param headerBytes a vector of bytes to read into the header.
*/
void szstools::RarcDump::initHeader() {
    uint32_t readIdx = 0;
    // Set up the type string.
    for (uint8_t i = 0; i < 4; i++) {
        header->type.push_back(fileData[i]);
        readIdx++;
    }

    // Grab the size value
    header->size = toUInt32(fileData, readIdx);
    readIdx += 4;

    // Fill first unknown
    header->unknown = toUInt32(fileData, readIdx);
    readIdx += 4;

    // Grab the data start offset
    header->dataStartOffset = toUInt32(fileData, readIdx);
    readIdx += 4;

    // Fill unknown2
    fillVector(&header->unknown2, fileData, &readIdx, 4);

    // Fill numNodes
    header->numNodes = toUInt32(fileData, readIdx);
    readIdx += 4;

    // Fill unknown3
    fillVector(&header->unknown3, fileData, &readIdx, 2);

    // Fill fileEntriesOffset
    header->fileEntriesOffset = toUInt32(fileData, readIdx);
    readIdx += 4;

    // Fill unknown4
    header->unknown4 = toUInt32(fileData, readIdx);
    readIdx += 4;

    // Fill stringTableOffset
    header->stringTableOffset = toUInt32(fileData, readIdx);
    readIdx += 4;

    // Fill unknown5
    fillVector(&header->unknown5, fileData, &readIdx, 2);
}

void szstools::RarcDump::fillVector(std::vector<uint32_t> *targetVec, std::vector<char>& inVec, uint32_t *readPos, size_t size) {
    for (uint32_t i = 0; i < (uint32_t)size; i++) {
        targetVec->at(i) = toUInt32(inVec, *readPos);
        *readPos += 4;
    }
}

szstools::RarcNode *szstools::RarcDump::getNode(uint32_t index) {
    // Move file seek pointer to the beginning of the node to read.
    uint32_t nodeLoc = HEADER_SIZE + (index * NODE_SIZE);

    // Read in the node data
    // Set up the node
    uint32_t readIdx = 0;
    auto *node = new RarcNode();
    // TODO: Find correct offset of root node.
    node->filenameOffset = toUInt32(fileData, nodeLoc+readIdx);
    readIdx += 4;
    node->unknown = toUInt32(fileData, nodeLoc+readIdx);
    readIdx += 4;
    node->numFileEntries = toUInt32(fileData, nodeLoc+readIdx);
    readIdx += 4;
    node->firstFileEntryOffset = toUInt32(fileData, nodeLoc+readIdx);

    return node;
}

void szstools::RarcDump::dumpNode(const RarcNode& node) {
    uint32_t offset = node.filenameOffset + header->stringTableOffset + 0x20;
    std::string nodeName = getString(offset);
    fs::create_directory(nodeName);
    fs::current_path(nodeName);

    for (uint32_t i = 0; i < node.numFileEntries; i++) {
        FileEntry currentFile = FileEntry(node.firstFileEntryOffset + i, *header, fileData);

        if (currentFile.id == 0xFFFF){ // file is a subdirectory
            if (currentFile.filenameOffset != 0 && currentFile.filenameOffset != 2) {
                // Avoids going to "." and ".."
                RarcNode *tempNode = getNode(currentFile.dataOffset);
                dumpNode(*tempNode);
                delete tempNode;
            }
        } else {
            // File entry is actually a file.
            offset = currentFile.filenameOffset + header->stringTableOffset + 0x20;
            fs::path currentFilePath = getString(offset);
            std::cout << boost::format("%s/%s\n") % nodeName % currentFilePath.string();

            std::ofstream outFile(currentFilePath, std::ios::binary | std::ios::trunc | std::ios::out);
            if (!outFile.is_open()) {
                throw std::runtime_error("Failed to open output file for writing.");
            }

            uint32_t read = 0;
            std::vector<char> buffer;
            uint32_t entryLoc = currentFile.dataOffset + header->dataStartOffset + 0x20;
            while (read < currentFile.dataSize) {
                size_t readSize = min(1024, currentFile.dataSize - read);
                buffer.assign(fileData.begin() + entryLoc, (fileData.begin() + entryLoc) + readSize);
                outFile.write(buffer.data(), readSize);
                read += readSize;
            }
            outFile.close();
        }
    }

    fs::current_path("..");
}

std::string szstools::RarcDump::getString(uint32_t offset) {
    std::string result;
    char c = -1;
    do {
        c = fileData[offset];
        result.append(1, c);
        offset++;
    } while (c != '\0');

    return result;
}

void szstools::RarcDump::dump() {
    dumpNode(*rootNode);
}
