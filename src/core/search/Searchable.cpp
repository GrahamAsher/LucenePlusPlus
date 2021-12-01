/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2009-2014 Alan Wright. All rights reserved.
// Distributable under the terms of either the Apache License (Version 2.0)
// or the GNU Lesser General Public License.
/////////////////////////////////////////////////////////////////////////////

#include "LuceneInc.h"
#include "Searchable.h"

namespace Lucene {

void Searchable::search(const WeightPtr& weight, const FilterPtr& filter, const CollectorPtr& collector) {
    assert(false);
    // override
}

void Searchable::close() {
    assert(false);
    // override
}

int32_t Searchable::docFreq(const TermPtr& term) {
    assert(false);
    return 0; // override
}

Collection<int32_t> Searchable::docFreqs(Collection<TermPtr> terms) {
    assert(false);
    return Collection<int32_t>(); // override
}

int32_t Searchable::maxDoc() {
    assert(false);
    return 0; // override
}

TopDocsPtr Searchable::search(const WeightPtr& weight, const FilterPtr& filter, int32_t n) {
    assert(false);
    return TopDocsPtr(); // override
}

DocumentPtr Searchable::doc(int32_t n) {
    assert(false);
    return DocumentPtr(); // override
}

DocumentPtr Searchable::doc(int32_t n, const FieldSelectorPtr& fieldSelector) {
    assert(false);
    return DocumentPtr(); // override
}

QueryPtr Searchable::rewrite(const QueryPtr& query) {
    assert(false);
    return QueryPtr(); // override
}

ExplanationPtr Searchable::explain(const WeightPtr& weight, int32_t doc) {
    assert(false);
    return ExplanationPtr(); // override
}

TopFieldDocsPtr Searchable::search(const WeightPtr& weight, const FilterPtr& filter, int32_t n, const SortPtr& sort) {
    assert(false);
    return TopFieldDocsPtr(); // override
}

}
