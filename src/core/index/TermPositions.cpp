/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2009-2014 Alan Wright. All rights reserved.
// Distributable under the terms of either the Apache License (Version 2.0)
// or the GNU Lesser General Public License.
/////////////////////////////////////////////////////////////////////////////

#include "LuceneInc.h"
#include "TermPositions.h"

namespace Lucene {

TermPositions::TermPositions() {
}

TermPositions::~TermPositions() {
}

int32_t TermPositions::nextPosition() {
    assert(false);
    return 0; // override
}

int32_t TermPositions::getPayloadLength() {
    assert(false);
    return 0; // override
}

ByteArray TermPositions::getPayload(ByteArray data, int32_t offset) {
    assert(false);
    return ByteArray(); // override
}

bool TermPositions::isPayloadAvailable() {
    assert(false);
    return false; // override
}

}
