/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2009-2014 Alan Wright. All rights reserved.
// Distributable under the terms of either the Apache License (Version 2.0)
// or the GNU Lesser General Public License.
/////////////////////////////////////////////////////////////////////////////

#include "LuceneInc.h"
#include "QueryParserCharStream.h"

namespace Lucene {

wchar_t QueryParserCharStream::readChar() {
    assert(false);
    return 0; // override
}

int32_t QueryParserCharStream::getColumn() {
    assert(false);
    return 0; // override
}

int32_t QueryParserCharStream::getLine() {
    assert(false);
    return 0; // override
}

int32_t QueryParserCharStream::getEndColumn() {
    assert(false);
    return 0; // override
}

int32_t QueryParserCharStream::getEndLine() {
    assert(false);
    return 0; // override
}

int32_t QueryParserCharStream::getBeginColumn() {
    assert(false);
    return 0; // override
}

int32_t QueryParserCharStream::getBeginLine() {
    assert(false);
    return 0; // override
}

void QueryParserCharStream::backup(int32_t amount) {
    assert(false);
    // override
}

wchar_t QueryParserCharStream::BeginToken() {
    assert(false);
    return 0; // override
}

String QueryParserCharStream::GetImage() {
    assert(false);
    return L""; // override
}

CharArray QueryParserCharStream::GetSuffix(int32_t length) {
    assert(false);
    return CharArray(); // override
}

void QueryParserCharStream::Done() {
    assert(false);
    // override
}

}
