/**
 * Rarc-dumping tool based on work by thakis and updated to modern C++
 * 
 * @author Brenden Davidson <davidson.brenden15@gmail.com>
 * @date 11-21-19
 * @version 1.0
*/

#ifndef FILE_ENTRY_HXX
#define FILE_ENTRY_HXX

#include <fstream>
#include <iostream>

#include "RarcHeader.hxx"
#include "szstools.hxx"

namespace szstools {
    class FileEntry {
    public:
        uint16_t id;                // File ID. If 0xFFFF, then the entry is a subdirectory link.
        uint16_t unknown;
        uint16_t unknown2;
        uint16_t filenameOffset;    // file/subdir name; offset from beginning of string table.
        uint32_t dataOffset;        // offset to file data (for subdirs: index of Node representing the subdir)
        uint32_t dataSize;          // Size of the data
        uint32_t zero;              // Exactly that.

        FileEntry(uint32_t index, const RarcHeader& header, std::vector<char>& data);
    };
};

#endif // FILE_ENTRY_HXX