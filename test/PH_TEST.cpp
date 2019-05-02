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


class UnitTest : public ::testing::Test {
public:
    virtual void SetUp() {}

    virtual void TearDown() {}
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

    cout << *phtree;
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

TEST_F(UnitTest, TestOrdering) {

}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
