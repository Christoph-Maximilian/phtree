/*
 * SpatialSelectionOperationsUtil.h
 *
 *  Created on: Apr 7, 2016
 *      Author: max
 */

#ifndef SRC_UTIL_SPATIALSELECTIONOPERATIONSUTIL_H_
#define SRC_UTIL_SPATIALSELECTIONOPERATIONSUTIL_H_
#define S2LOOKUP_OPTIMIZED
#include <vector>

template <unsigned int DIM>
class Node;
template <unsigned int DIM, unsigned int WIDTH>
class Entry;

template <unsigned int DIM, unsigned int WIDTH>
class SpatialSelectionOperationsUtil {
public:
	static std::pair<bool, int> lookup(const Entry<DIM, WIDTH>& e,
			const Node<DIM>* rootNode,
			std::vector<std::pair<unsigned long, const Node<DIM>*>>* visitedNodes);
};

#include <assert.h>
#include "../nodes/Node.h"
#include "../nodes/NodeAddressContent.h"

using namespace std;

template <unsigned int DIM, unsigned int WIDTH>
pair<bool, int> SpatialSelectionOperationsUtil<DIM, WIDTH>::lookup(
		const Entry<DIM, WIDTH>& e,
		const Node<DIM>* rootNode,
		vector<pair<unsigned long, const Node<DIM>*>>* visitedNodes) {

	const Node<DIM>* currentNode = rootNode;
	unsigned long lastHcAddress = 0;
	size_t index = 0;
	NodeAddressContent<DIM> content;

	while (true) {

		const size_t prefixLength = currentNode->getPrefixLength();
		if (prefixLength > 0) {
			// validate prefix
			const pair<bool, size_t> prefixComp = MultiDimBitset<DIM>::compare(e.values_, DIM * WIDTH,
					index, index + prefixLength,
					currentNode->getFixPrefixStartBlock(), prefixLength * DIM);

			if (!prefixComp.first) {
				#ifdef PRINT
					cout << "prefix mismatch at prefix index " << prefixComp.second << endl;
				#endif
				return {false, 0};
			}
		}

		if (visitedNodes)
			visitedNodes->push_back(pair<unsigned long, const Node<DIM>*>(lastHcAddress, currentNode));

		// validate HC address
		index += prefixLength;
		const unsigned long hcAddress = MultiDimBitset<DIM>::interleaveBits(e.values_, index, DIM * WIDTH);
		currentNode->lookup(hcAddress, content, true);
		assert (!content.exists || !content.hasSpecialPointer);

		if (!content.exists) {
			#ifdef PRINT
				cout << "HC address mismatch" << endl;
			#endif
			return {false, 0};
		}

		if (content.hasSubnode) {
			// recurse
			#ifdef PRINT
				cout << "ok up to index " << index << " > ";
			#endif
			++index;
			assert (content.subnode);
			currentNode = content.subnode;
			lastHcAddress = content.address;
		} else {
			const size_t suffixBits = DIM * (WIDTH - index - 1);
			if (suffixBits > 0) {
#ifndef S2LOOKUP_OPTIMIZED
				if (WIDTH == 16) {
					uint16_t postfix = *content.getSuffixStartBlock();
					uint16_t value = e.values_[0];
					uint16_t trailing_zeros = __builtin_ctzll(*content.getSuffixStartBlock());
					uint16_t bit_string = (value << (index + 1));
					bit_string = bit_string >>  (index + 1);
					bit_string = bit_string >> (1 + trailing_zeros);
					if (bit_string == postfix >> (trailing_zeros + 1)) {
						return {true, content.id};
					}
				}
				if (WIDTH == 64) {
#endif
					uint64_t postfix = *content.getSuffixStartBlock();
					uint64_t value = e.values_[0];
					uint64_t trailing_zeros = __builtin_ctzll(*content.getSuffixStartBlock());
					uint64_t bit_string = (value << (index + 1));
					bit_string = bit_string >>  (index + 1);
					bit_string = bit_string >> (1 + trailing_zeros);
					if (bit_string == postfix >> (trailing_zeros + 1)) {
						return {true, content.id};
					}
#ifndef S2LOOKUP_OPTIMIZED
				}
#endif
                return {false, 0};

				// validate suffix which is either directly stored or a reference
				const pair<bool, size_t> suffixComp = MultiDimBitset<DIM>::compare(e.values_, DIM * WIDTH,
								index + 1, WIDTH,
								content.getSuffixStartBlock(), suffixBits);
				if (!suffixComp.first) {
					#ifdef PRINT
						cout << "suffix mismatch at suffix index " << suffixComp.second << endl;
					#endif
					return {false, 0};
				}
			}

			#ifdef PRINT
				cout << "found" << endl;
			#endif

			return {true, content.id};
		}
	}
}

#endif /* SRC_UTIL_SPATIALSELECTIONOPERATIONSUTIL_H_ */
