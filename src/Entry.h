/*
 * Entry.h
 *
 *  Created on: Feb 25, 2016
 *      Author: max
 */

#ifndef SRC_ENTRY_H_
#define SRC_ENTRY_H_

#include <vector>
#include <iostream>
#include "util/MultiDimBitset.h"

template <unsigned int DIM, unsigned int WIDTH>
class Entry {
	template <unsigned int D, unsigned int W>
	friend std::ostream& operator <<(std::ostream &out, const Entry<D, W> &entry);
	template <unsigned int D, unsigned int W>
	friend bool operator ==(const Entry<D, W> &entry1, const Entry<D, W> &entry2);
	template <unsigned int D, unsigned int W>
	friend bool operator !=(const Entry<D, W> &entry1, const Entry<D, W> &entry2);

public:

	Entry(std::vector<unsigned long> &values, int id);
	Entry(unsigned long* startBlock, unsigned int nBits, int id);
	~Entry();

	size_t getBitLength() const;
	size_t getDimensions() const;

	int id_;
	unsigned int nBits_;
	unsigned long values_[1 + (DIM * WIDTH - 1) / (sizeof (unsigned long) * 8)];
};

#include <string>
#include <assert.h>

using namespace std;

template <unsigned int DIM, unsigned int WIDTH>
Entry<DIM, WIDTH>::Entry(vector<unsigned long> &values, int id) : id_(id), nBits_(DIM * WIDTH), values_() {
	assert (values.size() == DIM);
	MultiDimBitset<DIM>::template toBitset<WIDTH>(values, values_);
	assert (nBits_ == getBitLength() * getDimensions());
}

template <unsigned int DIM, unsigned int WIDTH>
Entry<DIM, WIDTH>::Entry(unsigned long* startBlock, unsigned int nBits, int id) : id_(id), nBits_(nBits) {
	const size_t nBlocks = 1 + nBits / (sizeof (unsigned long) * 8);
	assert (nBlocks == sizeof(values_) / sizeof(unsigned long));
	for (int i = 0; i < nBlocks; ++i) {
		values_[i] = *(startBlock + i);
	}

	assert (nBits_ == getBitLength() * getDimensions());
}

template <unsigned int DIM, unsigned int WIDTH>
Entry<DIM, WIDTH>::~Entry() {
}

template <unsigned int DIM, unsigned int WIDTH>
size_t Entry<DIM, WIDTH>::getBitLength() const {
	return WIDTH;
}

template <unsigned int DIM, unsigned int WIDTH>
size_t Entry<DIM, WIDTH>::getDimensions() const {
	return DIM;
}

template <unsigned int D, unsigned int W>
ostream& operator <<(ostream& os, const Entry<D, W> &e) {
	vector<unsigned long> converted = MultiDimBitset<D>::toLongs(e.values_, e.nBits_);
	os << "(";
	unsigned currentD = 0;
	for (const auto c : converted) {
		os << c;
		currentD++;
		if (currentD != D) os << ", ";
	}
	os << ") = ";

	MultiDimBitset<D>::output(os, e.values_, e.nBits_);
	os << " | ID: " << e.id_;
	return os;
}

template <unsigned int D, unsigned int W>
bool operator ==(const Entry<D, W> &entry1, const Entry<D, W> &entry2) {
	return entry1.values_ == entry2.values_;
}

template <unsigned int D, unsigned int W>
bool operator !=(const Entry<D, W> &entry1, const Entry<D, W> &entry2) {
	return !(entry1 == entry2);
}

#endif /* SRC_ENTRY_H_ */
