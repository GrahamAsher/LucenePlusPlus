/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2009-2014 Alan Wright. All rights reserved.
// Distributable under the terms of either the Apache License (Version 2.0)
// or the GNU Lesser General Public License.
/////////////////////////////////////////////////////////////////////////////

#include "LuceneInc.h"
#include "LuceneException.h"
#include "StringUtils.h"

namespace Lucene {

LuceneException::LuceneException(const String& error, ExceptionType type) throw() {
    this->error = error;
    this->type = type;
    SingleStringStream ss;
    ss << "LuceneException[" << type << "]: " << StringUtils::toUTF8(error);
    this->_what = ss.str();
}

LuceneException::~LuceneException() throw() {
}

LuceneException::ExceptionType LuceneException::getType() const {
    return type;
}

String LuceneException::getError() const {
    return error;
}

bool LuceneException::isNull() const {
    return (type == Null);
}

void LuceneException::throwException() {
    switch (type) {
    case LuceneException::AlreadyClosed:
        throw (AlreadyClosedException(error, type));
    case LuceneException::Compression:
        throw (CompressionException(error, type));
    case LuceneException::CorruptIndex:
        throw (CorruptIndexException(error, type));
    case LuceneException::FieldReader:
        throw (FieldReaderException(error, type));
    case LuceneException::FileNotFound:
        throw (FileNotFoundException(error, type));
    case LuceneException::IllegalArgument:
        throw (IllegalArgumentException(error, type));
    case LuceneException::IllegalState:
        throw (IllegalStateException(error, type));
    case LuceneException::IndexOutOfBounds:
        throw (IndexOutOfBoundsException(error, type));
    case LuceneException::IO:
        throw (IOException(error, type));
    case LuceneException::LockObtainFailed:
        throw (LockObtainFailedException(error, type));
    case LuceneException::LockReleaseFailed:
        throw (LockReleaseFailedException(error, type));
    case LuceneException::Lookahead:
        throw (LookaheadSuccess(error, type));
    case LuceneException::MergeAborted:
        throw (MergeAbortedException(error, type));
    case LuceneException::Merge:
        throw (MergeException(error, type));
    case LuceneException::NoSuchDirectory:
        throw (NoSuchDirectoryException(error, type));
    case LuceneException::NullPointer:
        throw (NullPointerException(error, type));
    case LuceneException::NumberFormat:
        throw (NumberFormatException(error, type));
    case LuceneException::OutOfMemory:
        throw (OutOfMemoryError(error, type));
    case LuceneException::Parse:
        throw (ParseException(error, type));
    case LuceneException::QueryParser:
        throw (QueryParserError(error, type));
    case LuceneException::Runtime:
        throw (RuntimeException(error, type));
    case LuceneException::StaleReader:
        throw (StaleReaderException(error, type));
    case LuceneException::StopFillCache:
        throw (StopFillCacheException(error, type));
    case LuceneException::Temporary:
        throw (TemporaryException(error, type));
    case LuceneException::TimeExceeded:
        throw (TimeExceededException(error, type));
    case LuceneException::TooManyClauses:
        throw (TooManyClausesException(error, type));
    case LuceneException::UnsupportedOperation:
        throw (UnsupportedOperationException(error, type));
    case LuceneException::Null:
        // silence static analyzer
        break;
    }
}

const char* LuceneException::what() const throw()
{
    return _what.c_str();
}

}
