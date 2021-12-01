/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2009-2014 Alan Wright. All rights reserved.
// Distributable under the terms of either the Apache License (Version 2.0)
// or the GNU Lesser General Public License.
/////////////////////////////////////////////////////////////////////////////

#include "LuceneInc.h"
#include "CompressionTools.h"
#include "MiscUtils.h"
#include "UnicodeUtils.h"
#include "StringUtils.h"

#pragma message("The CompressionTools class is unimplemented")

namespace Lucene {

const int32_t CompressionTools::COMPRESS_BUFFER = 4096;

//String ZLibToMessage(int32_t error) {
//    if (error == boost::iostreams::zlib::okay) {
//        return L"okay";
//    } else if (error == boost::iostreams::zlib::stream_end) {
//        return L"stream_end";
//    } else if (error == boost::iostreams::zlib::stream_error) {
//        return L"stream_error";
//    } else if (error == boost::iostreams::zlib::version_error) {
//        return L"version_error";
//    } else if (error == boost::iostreams::zlib::data_error) {
//        return L"data_error";
//    } else if (error == boost::iostreams::zlib::mem_error) {
//        return L"mem_error";
//    } else if (error == boost::iostreams::zlib::buf_error ) {
//        return L"buf_error";
//    } else {
//        return L"unknown";
//    }
//}
//
//class BufferArraySink : public boost::iostreams::sink {
//public:
//    BufferArraySink(ByteArray& _buffer, std::streamsize& _position, size_t allocSize) : buffer(_buffer), position(_position) {
//        this->allocSize = allocSize;
//        this->buffer.resize((int32_t)allocSize);
//    }
//
//public:
//    ByteArray& buffer;
//    std::streamsize& position;
//
//private:
//    size_t allocSize;
//
//public:
//    std::streamsize write(const char* s, std::streamsize n) {
//        if (position + n >= (std::streamsize)allocSize) {
//            // grow buffer
//            allocSize <<= 1;
//            buffer.resize((int32_t)allocSize);
//        }
//        MiscUtils::arrayCopy(s, 0, buffer.get(), position, n);
//        position += n;
//        return n;
//    }
//};

CompressionTools::~CompressionTools() {
}

ByteArray CompressionTools::compress(uint8_t* value, int32_t offset, int32_t length, int32_t compressionLevel)
    {
    throw std::runtime_error("unimplemented");
    }

ByteArray CompressionTools::compress(uint8_t* value, int32_t offset, int32_t length)
    {
    throw std::runtime_error("unimplemented");
    }

ByteArray CompressionTools::compress(ByteArray value)
    {
    throw std::runtime_error("unimplemented");
    }

ByteArray CompressionTools::compressString(const String& value)
    {
    throw std::runtime_error("unimplemented");
    }

ByteArray CompressionTools::compressString(const String& value, int32_t compressionLevel)
    {
    throw std::runtime_error("unimplemented");
    }

ByteArray CompressionTools::decompress(ByteArray value)
    {
    throw std::runtime_error("unimplemented");
    }

String CompressionTools::decompressString(ByteArray value)
    {
    throw std::runtime_error("unimplemented");
    }

}
