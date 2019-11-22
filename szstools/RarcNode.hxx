//
// Created by bdavidson on 11/21/19.
//

#ifndef SZSTOOLS_RARCNODE_HXX
#define SZSTOOLS_RARCNODE_HXX

namespace szstools {
  class RarcNode {
  public:
      std::string type;
      uint32_t filenameOffset; // Name of directory; offset from beginning of string table.
      uint16_t unknown;
      uint16_t numFileEntries; // Number of files belonging to node.
      uint32_t firstFileEntryOffset;
  };
};

#endif //SZSTOOLS_RARCNODE_HXX
