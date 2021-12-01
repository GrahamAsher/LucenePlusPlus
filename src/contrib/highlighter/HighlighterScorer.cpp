/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2009-2014 Alan Wright. All rights reserved.
// Distributable under the terms of either the Apache License (Version 2.0)
// or the GNU Lesser General Public License.
/////////////////////////////////////////////////////////////////////////////

#include "ContribInc.h"
#include "HighlighterScorer.h"

namespace Lucene {

HighlighterScorer::~HighlighterScorer() {
}

TokenStreamPtr HighlighterScorer::init(const TokenStreamPtr& tokenStream) {
    assert(false);
    return TokenStreamPtr(); // override
}

void HighlighterScorer::startFragment(const TextFragmentPtr& newFragment) {
    assert(false);
    // override
}

double HighlighterScorer::getTokenScore() {
    assert(false);
    return 0; // override
}

double HighlighterScorer::getFragmentScore() {
    assert(false);
    return 0; // override
}

}
