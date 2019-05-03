//
// Created by christoph on 02.05.19.
//

#include "gtest/gtest.h"
#include <stdlib.h>
#include <fstream>
#include <algorithm>

#include "../src/Entry.h"
#include "../src/PHTree.h"
#include "../src/util/PlotUtil.h"
#include "../src/util/rdtsc.h"
#include "../src/visitors/CountNodeTypesVisitor.h"
#include "../src/iterators/RangeQueryIterator.h"
#include "../src/visitors/SizeVisitor.h"


class UnitTest : public ::testing::Test {
public:
    virtual void SetUp() {
        miniS2Cells.emplace_back(0b1001100001111000);
        miniS2Cells.emplace_back(0b1001100001101000);
        miniS2Cells.emplace_back(0b1001100000100000);

        miniS2QueryCells.emplace_back(0b1001100001110110);
        miniS2QueryCells.emplace_back(0b1001100001101010);
        miniS2QueryCells.emplace_back(0b1001100000000010);

        miniS2QueryCellsNotContained.emplace_back(0b1001100001010010);
        miniS2QueryCellsNotContained.emplace_back(0b1001110001101000);
        miniS2QueryCellsNotContained.emplace_back(0b1001100001000010);
    }

    virtual void TearDown() {}

    std::vector<__uint16_t> miniS2Cells;
    std::vector<__uint16_t> miniS2QueryCells;
    std::vector<__uint16_t> miniS2QueryCellsNotContained;

};

TEST_F(UnitTest, S2Test16bits) {

    auto sizeVisitor = new SizeVisitor<1>();

    const unsigned int bitLength = 16;
    auto phtree = new PHTree<1, bitLength>();
    phtree->accept(sizeVisitor);

    uint64_t counter = 0;

    for (auto &s2cell: miniS2Cells) {
        phtree->insert({s2cell}, static_cast<int>(counter));
        counter++;
    }

    std::cout << *phtree << std::endl;
    // NO errors expected here!
    counter = 0;
    for (auto &s2cell: miniS2Cells) {
        ASSERT_TRUE(phtree->lookup({s2cell}).first);
        ASSERT_EQ(phtree->lookup({s2cell}).second, counter);
        if (phtree->lookup({s2cell}).second != counter) { std::cerr << "ERROR!!!" << std::endl; }
        counter++;
    }

    counter = 0;
    for (auto &s2cell: miniS2QueryCells) {
        ASSERT_TRUE(phtree->lookup({s2cell}).first);
        ASSERT_EQ(phtree->lookup({s2cell}).second, counter);
        if (phtree->lookup({s2cell}).second != counter) { std::cerr << "ERROR!!!" << std::endl; }
        counter++;
    }

    for (auto &s2cell: miniS2QueryCellsNotContained) {
        ASSERT_FALSE(phtree->lookup({s2cell}).first);
    }

    std::cout << sizeVisitor->getTotalByteSize() << " Bytes" << std::endl;

}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
