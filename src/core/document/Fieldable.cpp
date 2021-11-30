/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2009-2014 Alan Wright. All rights reserved.
// Distributable under the terms of either the Apache License (Version 2.0)
// or the GNU Lesser General Public License.
/////////////////////////////////////////////////////////////////////////////

#include "LuceneInc.h"
#include "Fieldable.h"
#include <assert.h>

namespace Lucene {

void Fieldable::setBoost(double boost) {
    assert(false);
    // override
}

double Fieldable::getBoost() {
    assert(false);
    return 0; // override
}

String Fieldable::name() {
    assert(false);
    return L""; // override
}

String Fieldable::stringValue() {
    assert(false);
    return L""; // override
}

ReaderPtr Fieldable::readerValue() {
    assert(false);
    return ReaderPtr(); // override
}

TokenStreamPtr Fieldable::tokenStreamValue() {
    assert(false);
    return TokenStreamPtr(); // override
}

bool Fieldable::isStored() {
    assert(false);
    return false; // override
}

bool Fieldable::isIndexed() {
    assert(false);
    return false; // override
}

bool Fieldable::isTokenized() {
    assert(false);
    return false; // override
}

bool Fieldable::isTermVectorStored() {
    assert(false);
    return false; // override
}

bool Fieldable::isStoreOffsetWithTermVector() {
    assert(false);
    return false; // override
}

bool Fieldable::isStorePositionWithTermVector() {
    assert(false);
    return false; // override
}

bool Fieldable::isBinary() {
    assert(false);
    return false; // override
}

bool Fieldable::getOmitNorms() {
    assert(false);
    return false; // override
}

void Fieldable::setOmitNorms(bool omitNorms) {
    assert(false);
    // override
}

bool Fieldable::isLazy() {
    assert(false);
    return false; // override
}

int32_t Fieldable::getBinaryOffset() {
    assert(false);
    return 0; // override
}

int32_t Fieldable::getBinaryLength() {
    assert(false);
    return 0; // override
}

ByteArray Fieldable::getBinaryValue() {
    assert(false);
    return ByteArray(); // override
}

ByteArray Fieldable::getBinaryValue(ByteArray result) {
    assert(false);
    return ByteArray(); // override
}

bool Fieldable::getOmitTermFreqAndPositions() {
    assert(false);
    return false; // override
}

void Fieldable::setOmitTermFreqAndPositions(bool omitTermFreqAndPositions) {
    assert(false);
    // override
}

}
