/**
 * Rarc-dumping tool based on work by thakis and updated to modern C++
 * 
 * @author Brenden Davidson <davidson.brenden15@gmail.com>
 * @date 11-21-19
 * @version 1.0
*/

#include "FileEntry.hxx"

szstools::FileEntry::FileEntry(uint32_t index, const RarcHeader& header, std::vector<char>& data) {
    uint32_t seek = header.fileEntriesOffset + index*FILE_ENTRY_SIZE + 0x20;

    uint32_t readIdx = 0;
    uint32_t tempDword = toUInt32(data, seek + readIdx);
    id = static_cast<uint16_t>(tempDword & 0xFFFF); // id should be the lower word
    unknown = static_cast<uint16_t>(tempDword >> 16);
    readIdx += 4;

    tempDword = toUInt32(data, seek + readIdx);
    unknown2 = static_cast<uint16_t>(tempDword & 0xFFFF);
    filenameOffset = static_cast<uint16_t>(tempDword >> 16);
    readIdx += 4;

    dataOffset = toUInt32(data, seek + readIdx);
    readIdx += 4;

    dataSize = toUInt32(data, seek + readIdx);
    readIdx += 4;

    zero = toUInt32(data, seek + readIdx);
}