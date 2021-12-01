/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2009-2014 Alan Wright. All rights reserved.
// Distributable under the terms of either the Apache License (Version 2.0)
// or the GNU Lesser General Public License.
/////////////////////////////////////////////////////////////////////////////

#include "LuceneInc.h"
#include "TermDocs.h"

namespace Lucene {

TermDocs::TermDocs() {
}

void TermDocs::seek(const TermPtr& term) {
    assert(false);
    // override
}

void TermDocs::seek(const TermEnumPtr& termEnum) {
    assert(false);
    // override
}

int32_t TermDocs::doc() {
    assert(false);
    return 0; // override
}

int32_t TermDocs::freq() {
    assert(false);
    return 0; // override
}

bool TermDocs::next() {
    assert(false);
    return false; // override
}

int32_t TermDocs::read(Collection<int32_t>& docs, Collection<int32_t>& freqs) {
    assert(false);
    return 0; // override
}

bool TermDocs::skipTo(int32_t target) {
    assert(false);
    return false; // override
}

void TermDocs::close() {
    assert(false);
    // override
}

}
