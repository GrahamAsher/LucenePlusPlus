/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2009-2014 Alan Wright. All rights reserved.
// Distributable under the terms of either the Apache License (Version 2.0)
// or the GNU Lesser General Public License.
/////////////////////////////////////////////////////////////////////////////

#include "LuceneInc.h"
#include "BufferedIndexInput.h"
#include "MiscUtils.h"
#include "StringUtils.h"

namespace Lucene {

/// Default buffer size.
const int32_t BufferedIndexInput::BUFFER_SIZE = 1024 * 2;

BufferedIndexInput::BufferedIndexInput(int32_t bufferSize) {
    this->bufferSize = bufferSize;
    bufferStart = 0;
    bufferLength = 0;
    bufferPosition = 0;
}

BufferedIndexInput::~BufferedIndexInput() {
}

uint8_t BufferedIndexInput::readByte() {
    if (bufferPosition < bufferLength) {
       return __buffer[bufferPosition++];
    }
    refill();
    return __buffer[bufferPosition++];
}

static const int MAX_VARINT32_LENGHT = 5;

int32_t BufferedIndexInput::readVInt() {
    if (bufferPosition + MAX_VARINT32_LENGHT < bufferLength) {
        uint8_t b = __buffer[bufferPosition++];
        int32_t i = (b & 0x7f);
        for (int32_t shift = 7; (b & 0x80) != 0; shift += 7) {
            b = __buffer[bufferPosition++];
            i |= (b & 0x7f) << shift;
        }
        return i;
    }
    else {
        uint8_t b = readByte();
        int32_t i = (b & 0x7f);
        for (int32_t shift = 7; (b & 0x80) != 0; shift += 7) {
            b = readByte();
            i |= (b & 0x7f) << shift;
        }
        return i;
    }
}

void BufferedIndexInput::setBufferSize(int32_t newSize) {
    if (newSize != bufferSize) {
        bufferSize = newSize;
        if (buffer) {
            // Resize the existing buffer and carefully save as many bytes as possible starting from the current bufferPosition
            ByteArray _newBuffer(ByteArray::newInstance(newSize));
            int32_t leftInBuffer = bufferLength - bufferPosition;
            int32_t numToCopy = leftInBuffer > newSize ? newSize : leftInBuffer;

            MiscUtils::arrayCopy(buffer.get(), bufferPosition, _newBuffer.get(), 0, numToCopy);
            bufferStart += bufferPosition;
            bufferPosition = 0;
            bufferLength = numToCopy;
            newBuffer(_newBuffer);
        }
    }
}

void BufferedIndexInput::newBuffer(ByteArray newBuffer) {
    // Subclasses can do something here
    buffer = newBuffer;
    __buffer = newBuffer.get();
}

int32_t BufferedIndexInput::getBufferSize() {
    return bufferSize;
}

void BufferedIndexInput::checkBufferSize(int32_t bufferSize) {
    if (bufferSize <= 0) {
        throw (IllegalArgumentException(L"bufferSize must be greater than 0 (got " + StringUtils::toString(bufferSize) + L")"));
    }
}

void BufferedIndexInput::readBytes(uint8_t* b, int32_t offset, int32_t length) {
    readBytes(b, offset, length, true);
}

void BufferedIndexInput::readBytes(uint8_t* b, int32_t offset, int32_t length, bool useBuffer) {
    if (length <= (bufferLength - bufferPosition)) {
        // the buffer contains enough data to satisfy this request
        if (length > 0) { // to allow b to be null if length is 0
            MiscUtils::arrayCopy(buffer.get(), bufferPosition, b, offset, length);
        }
        bufferPosition += length;
    } else {
        // the buffer does not have enough data, first serve all we've got
        int32_t available = bufferLength - bufferPosition;
        if (available > 0) {
            MiscUtils::arrayCopy(buffer.get(), bufferPosition, b, offset, available);
            offset += available;
            length -= available;
            bufferPosition += available;
        }

        // and now, read the remaining 'length' bytes
        if (useBuffer && length < bufferSize) {
            // If the amount left to read is small enough, and we are allowed to use our buffer,
            // do it in the usual buffered way: fill the buffer and copy from it
            refill();
            if (bufferLength < length) {
                // throw an exception when refill() could not read length bytes
                MiscUtils::arrayCopy(buffer.get(), 0, b, offset, bufferLength);
                throw (IOException(L"Read past EOF"));
            } else {
                MiscUtils::arrayCopy(buffer.get(), 0, b, offset, length);
                bufferPosition = length;
            }
        } else {
            // The amount left to read is larger than the buffer or we've been asked to not use
            // our buffer - there's no performance reason not to read it all at once.
            // Note that unlike the previous code of this function, there is no need to do a seek
            // here, because there's no need to reread what we had in the buffer.
            int64_t after = bufferStart + bufferPosition + length;
            if (after > this->length()) {
                throw (IOException(L"Read past EOF"));
            }
            readInternal(b, offset, length);
            bufferStart = after;
            bufferPosition = 0;
            bufferLength = 0; // trigger refill() on read
        }
    }
}

void BufferedIndexInput::refill() {
    int64_t start = bufferStart + bufferPosition;
    int64_t end = start + bufferSize;
    if (end > length()) { // don't read past EOF
        end = length();
    }
    int32_t newLength = (int32_t)(end - start);
    if (newLength <= 0) {
        throw (IOException(L"Read past EOF"));
    }

    if (!buffer) {
        newBuffer(ByteArray::newInstance(bufferSize)); // allocate buffer lazily
        seekInternal(bufferStart);
    }
    readInternal(__buffer, 0, newLength);
    bufferLength = newLength;
    bufferStart = start;
    bufferPosition = 0;
}

void BufferedIndexInput::close() {
    bufferStart = 0;
    bufferLength = 0;
    bufferPosition = 0;
}

int64_t BufferedIndexInput::getFilePointer() {
    return bufferStart + bufferPosition;
}

void BufferedIndexInput::seek(int64_t pos) {
    if (pos >= bufferStart && pos < (bufferStart + bufferLength)) {
        bufferPosition = (int32_t)(pos - bufferStart);    // seek within buffer
    } else {
        bufferStart = pos;
        bufferPosition = 0;
        bufferLength = 0; // trigger refill() on read()
        seekInternal(pos);
    }
}

LuceneObjectPtr BufferedIndexInput::clone(const LuceneObjectPtr& other) {
    BufferedIndexInputPtr cloneIndexInput(std::dynamic_pointer_cast<BufferedIndexInput>(IndexInput::clone(other)));
    cloneIndexInput->bufferSize = bufferSize;
    cloneIndexInput->buffer.reset();
    cloneIndexInput->bufferLength = 0;
    cloneIndexInput->bufferPosition = 0;
    cloneIndexInput->bufferStart = getFilePointer();
    return cloneIndexInput;
}

}
