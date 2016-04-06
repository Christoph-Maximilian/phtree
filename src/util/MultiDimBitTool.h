/*
 * MultiDimBitTool.h
 *
 *  Created on: Mar 16, 2016
 *      Author: max
 */

#ifndef SRC_UTIL_MULTIDIMBITTOOL_H_
#define SRC_UTIL_MULTIDIMBITTOOL_H_

#include <vector>
#include "../Entry.h"

class MultiDimBitTool {
public:

	static unsigned long bitsetToLong(const std::vector<bool>* bitset);

	static std::vector<unsigned long>* bitsetsToLongs(std::vector<std::vector<bool>>* bitsets);

	static std::vector<bool> longToBitset(unsigned long value, size_t bitLength);

	static void longsToBitsets(std::vector<std::vector<bool>>& target,
			const std::vector<long>& values, size_t bitLength);

	static long interleaveBits(unsigned int index, const Entry* e);

	static long interleaveBits(unsigned int index,
			const std::vector<std::vector<bool>>* values);

	static void removeFirstBits(unsigned int nBitsToRemove,
			std::vector<std::vector<bool>> *values);

	static void removeFirstBits(unsigned int nBitsToRemove,
			const std::vector<std::vector<bool>> *valuesFrom,
			std::vector<std::vector<bool>>* valuesTo);

	static void pushBitsToBack(std::vector<std::vector<bool>> *valuesToPushTo,
			const std::vector<std::vector<bool>> *valuesToAdd);

	static void pushValueToBack(std::vector<std::vector<bool>> *pushTo,
			unsigned long newValue);

	static void duplicateFirstBits(unsigned int nBitsToDuplicate,
			const std::vector<std::vector<bool>>* from,
			std::vector<std::vector<bool>>* to);

	static unsigned int setLongestCommonPrefix(std::vector<std::vector<bool>>* entryToSetTo,
			unsigned int startIndex,
			const std::vector<std::vector<bool>>* entry1,
			const std::vector<std::vector<bool>>* entry2);
	static Entry createEntryFrom(const std::vector<std::vector<bool>>* value1,
			unsigned long hcAddress,
			const std::vector<std::vector<bool>>* value2,
			int id);

};

#endif /* SRC_UTIL_MULTIDIMBITTOOL_H_ */