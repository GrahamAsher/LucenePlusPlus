/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2009-2014 Alan Wright. All rights reserved.
// Distributable under the terms of either the Apache License (Version 2.0)
// or the GNU Lesser General Public License.
/////////////////////////////////////////////////////////////////////////////

#include "ContribInc.h"
#include "Encoder.h"

namespace Lucene {

Encoder::~Encoder() {
}

String Encoder::encodeText(const String& originalText) {
    assert(false);
    return L""; // override
}

}
