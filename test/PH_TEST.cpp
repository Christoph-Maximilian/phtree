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
    }

    virtual void TearDown() {}

    std::vector<__uint16_t > miniS2Cells;
    std::vector<__uint16_t > miniS2QueryCells;

};


TEST_F(UnitTest, ScanTest) {
    const unsigned int bitLength = 8;
    auto phtree = new PHTree<1, bitLength>();

    const unsigned long upperBoundary = (1uL << bitLength);
    for (unsigned long i = 0; i < upperBoundary; ++i) {
        phtree->insert({i}, static_cast<int>(i*33));
        assert (phtree->lookup({i}).first);
        if (phtree->lookup({i}).second != i* 33) {
            std::cerr << "wrong!" << std::endl;
        }
    }


    //cout << *phtree;
    for (unsigned long width = 1; width < upperBoundary; ++width) {
        for (unsigned long lower = 0; lower < upperBoundary - width; ++lower) {
            RangeQueryIterator<1, bitLength>* it = phtree->rangeQuery({lower}, {lower + width});
            unsigned int nIntersects = 0;
            while (it->hasNext()) {
                it->next();
                nIntersects++;
            }

            assert (nIntersects == (1 + width));
            delete it;
        }
    }
    delete phtree;
}

TEST_F(UnitTest, S2Test16bits) {

    auto sizeVisitor = new SizeVisitor<1>();

    const unsigned int bitLength = 16;
    auto phtree = new PHTree<1, bitLength>();
    phtree->accept(sizeVisitor);

    auto counter = 0;

    for (auto& s2cell: miniS2Cells) {
        phtree->insert({s2cell}, static_cast<int>(counter));
        counter++;
    }

    std::cout << *phtree << std::endl;
    // NO errors expected here!
    counter = 0;
    for (auto& s2cell: miniS2Cells) {
        ASSERT_TRUE(phtree->lookup({s2cell}).first);
        ASSERT_EQ(phtree->lookup({s2cell}).second, static_cast<int>(counter));
        if (phtree->lookup({s2cell}).second != static_cast<int>(counter)) {std::cerr << "ERROR!!!" << std::endl;}
        counter++;
    }

    counter = 0;
    for (auto& s2cell: miniS2QueryCells) {
        ASSERT_TRUE(phtree->lookup({s2cell}).first);
        ASSERT_EQ(phtree->lookup({s2cell}).second, static_cast<int>(counter));
        if (phtree->lookup({s2cell}).second != static_cast<int>(counter)) {std::cerr << "ERROR!!!" << std::endl;}
        counter++;
    }

    std::cout << sizeVisitor->getTotalByteSize() << " Bytes" << std::endl;

}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
