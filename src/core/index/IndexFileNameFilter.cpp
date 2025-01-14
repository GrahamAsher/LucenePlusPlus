/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2009-2014 Alan Wright. All rights reserved.
// Distributable under the terms of either the Apache License (Version 2.0)
// or the GNU Lesser General Public License.
/////////////////////////////////////////////////////////////////////////////

#include "LuceneInc.h"
#include "IndexFileNameFilter.h"
#include "IndexFileNames.h"
#include <regex>

namespace Lucene {

bool IndexFileNameFilter::accept(const String& directory, const String& name) {
    String::size_type i = name.find_last_of(L'.');
    if (i != String::npos) {
        String extension(name.substr(i+1));
        if (IndexFileNames::INDEX_EXTENSIONS().contains(extension)) {
            return true;
        } else if (!extension.empty()) {
            if (extension[0] == L'f' && std::regex_search(extension, std::wregex(L"f\\d+"))) {
                return true;
            }
            if (extension[0] == L's' && std::regex_search(extension, std::wregex(L"s\\d+"))) {
                return true;
            }
        }
    } else {
        if (name == IndexFileNames::DELETABLE()) {
            return true;
        }
        if (boost_copy::starts_with(name, IndexFileNames::SEGMENTS())) {
            return true;
        }
    }
    return false;
}

bool IndexFileNameFilter::isCFSFile(const String& name) {
    String::size_type i = name.find_last_of(L'.');
    if (i != String::npos) {
        String extension(name.substr(i+1));
        if (IndexFileNames::INDEX_EXTENSIONS_IN_COMPOUND_FILE().contains(extension)) {
            return true;
        } else if (!extension.empty() && extension[0] == L'f' && std::regex_search(extension, std::wregex(L"f\\d+"))) {
            return true;
        }
    }
    return false;
}

IndexFileNameFilterPtr IndexFileNameFilter::getFilter() {
    static IndexFileNameFilterPtr singleton;
    if (!singleton) {
        singleton = newLucene<IndexFileNameFilter>();
        CycleCheck::addStatic(singleton);
    }
    return singleton;
}

}
