/*
 * LHC.h
 *
 *  Created on: Feb 25, 2016
 *      Author: max
 */

#ifndef LHC_H_
#define LHC_H_

#include <map>
#include <vector>
#include "Node.h"
#include "LHCAddressContent.h"

class LHC: public Node {
	friend class LHCIterator;
	friend class AssertionVisitor;
public:
	LHC(size_t dim, size_t valueLength);
	virtual ~LHC();
	NodeIterator* begin() override;
	NodeIterator* end() override;
	std::ostream& output(std::ostream& os, size_t depth) override;
	virtual void accept(Visitor* visitor, size_t depth) override;
	virtual void recursiveDelete() override;

protected:
	std::map<long, LHCAddressContent> sortedContents_;
	size_t longestSuffix_;

	NodeAddressContent lookup(long address) override;
	void insertAtAddress(long hcAddress, std::vector<std::vector<bool>>* suffix) override;
	void insertAtAddress(long hcAddress, Node* subnode) override;
	Node* adjustSize() override;

private:
	LHCAddressContent* lookupReference(long hcAddress);
};

#endif /* LHC_H_ */
