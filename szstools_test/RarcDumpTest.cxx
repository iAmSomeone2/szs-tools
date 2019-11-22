//
// Created by bdavidson on 11/21/19.
//

#include <filesystem>
#include <string>

#include <gtest/gtest.h>

#include "../szstools/RarcDump.hxx"

class RarcDumpTest : public ::testing::Test {
protected:
    std::filesystem::path rarcPath;
    std::filesystem::path outDir;

    RarcDumpTest() {
        rarcPath = "../car.rarc";
        outDir = rarcPath.filename().append("_out");
    }
};

TEST_F(RarcDumpTest, DecompressRarcFile) {
    szstools::RarcDump dumper(rarcPath);
    dumper.dump();
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}