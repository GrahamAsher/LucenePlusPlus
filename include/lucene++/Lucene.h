/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2009-2014 Alan Wright. All rights reserved.
// Distributable under the terms of either the Apache License (Version 2.0)
// or the GNU Lesser General Public License.
/////////////////////////////////////////////////////////////////////////////

#ifndef LUCENE_H
#define LUCENE_H

#include "Config.h"

#include <cwctype>
#include <cwchar>
#include <sys/types.h>
#include <sstream>
#include <string>
#include <algorithm>
#include <limits>
#include <stdexcept>
#include <variant>
#include <any>
#include <functional>
#include <thread>
#include <filesystem>
#include <fstream>
#include <assert.h>

#define SIZEOF_ARRAY(arr) (sizeof(arr) / sizeof((arr)[0]))

#include "LuceneTypes.h"
#include "LuceneAllocator.h"

namespace Lucene
    {
    typedef std::basic_string< char, std::char_traits<char> > SingleString;
    typedef std::basic_ostringstream< char, std::char_traits<char> > SingleStringStream;
    typedef std::basic_string< wchar_t, std::char_traits<wchar_t> > String;
    typedef std::basic_ostringstream< wchar_t, std::char_traits<wchar_t> > StringStream;

    const std::basic_string< wchar_t, std::char_traits<wchar_t> > EmptyString;

    //typedef std::shared_ptr<boost::interprocess::file_lock> filelockPtr;
    typedef std::shared_ptr<std::thread> threadPtr;

    namespace boost_copy
        {
        template<class charT,class traits = std::char_traits<charT>> class basic_filebuf: public std::basic_filebuf<charT,traits>
            {
            public:
            basic_filebuf() = default;
            basic_filebuf(basic_filebuf const&) = delete;
            basic_filebuf const& operator=(basic_filebuf const&) = delete;

            basic_filebuf<charT,traits>* open(std::filesystem::path const& p,std::ios_base::openmode mode)
                {
                return std::basic_filebuf<charT,traits>::open(p.c_str(),mode) ? this : nullptr;
                }
            };

        template<class charT,class traits = std::char_traits<charT>> class basic_ifstream: public std::basic_ifstream<charT,traits>
            {
            public:
            basic_ifstream() = default;

            explicit basic_ifstream(std::filesystem::path const& p): std::basic_ifstream<charT,traits>(p.c_str(),std::ios_base::in) { }
            basic_ifstream(std::filesystem::path const& p,std::ios_base::openmode mode): std::basic_ifstream<charT,traits>(p.c_str(),mode) { }

            basic_ifstream(basic_ifstream const&) = delete;
            basic_ifstream const& operator=(basic_ifstream const&) = delete;

            public:
            void open(std::filesystem::path const& p)
                {
                std::basic_ifstream<charT,traits>::open(p.c_str(),std::ios_base::in);
                }

            void open(std::filesystem::path const& p,std::ios_base::openmode mode)
                {
                std::basic_ifstream<charT,traits>::open(p.c_str(),mode);
                }
            };

        template< class charT,class traits = std::char_traits<charT>> class basic_ofstream:  public std::basic_ofstream<charT,traits>
            {
            public:
            basic_ofstream() = default;

            explicit basic_ofstream(std::filesystem::path const& p): std::basic_ofstream<charT,traits>(p.c_str(),std::ios_base::out) { }
            basic_ofstream(std::filesystem::path const& p,std::ios_base::openmode mode): std::basic_ofstream<charT,traits>(p.c_str(),mode) { }

            basic_ofstream(basic_ofstream const&) = delete;
            basic_ofstream const& operator=(basic_ofstream const&) = delete;

            public:
            void open(std::filesystem::path const& p)
                {
                std::basic_ofstream<charT,traits>::open(p.c_str(),std::ios_base::out);
                }

            void open(std::filesystem::path const& p,std::ios_base::openmode mode)
                {
                std::basic_ofstream<charT,traits>::open(p.c_str(),mode);
                }
            };

        typedef basic_ifstream<char> ifstream;
        typedef basic_ofstream<char> ofstream;
        typedef basic_ofstream<wchar_t> wofstream;

        template<class T> inline void ltrim(std::basic_string<T>& s)
            {
            auto iter = s.begin();
            while (iter != s.end() && std::isspace(*iter))
                ++iter;
            s.erase(s.begin(),iter);
            }

        template<class T> inline void rtrim(std::basic_string<T>& s)
            {
            auto iter = s.rbegin();
            while (iter != s.rend() && std::isspace(*iter))
                ++iter;
            s.erase(iter.base(),s.end());
            }

        template<class T> inline void trim(std::basic_string<T>& s)
            {
            ltrim(s);
            rtrim(s);
            }

        template<class T> inline bool starts_with(const std::basic_string<T>& s,const std::basic_string<T>& t)
            {
            return s.rfind(t,0) == 0;
            }

        template<class T> inline bool ends_with(const std::basic_string<T>& s,const std::basic_string<T>& t)
            {
            return s.length() >= t.length() && s.find(t,s.length() - t.length()) == s.length() - t.length();
            }

        using ptime = double;
        }

    using ifstreamPtr = std::shared_ptr<boost_copy::ifstream>;
    using ofstreamPtr = std::shared_ptr<boost_copy::ofstream>;
    using localePtr = std::shared_ptr<std::locale>;
    }

#include "LuceneFactory.h"
#include "LuceneException.h"
#include "Array.h"
#include "Collection.h"
#include "Map.h"
#include "Set.h"
#include "HashMap.h"
#include "HashSet.h"
#include "Constants.h"

namespace Lucene {

typedef Array<uint8_t> ByteArray;
typedef Array<int32_t> IntArray;
typedef Array<int64_t> LongArray;
typedef Array<wchar_t> CharArray;
typedef Array<double> DoubleArray;

template <class TYPE>
struct luceneEquals {
    inline bool operator()(const TYPE& first, const TYPE& second) const {
        return first ? first->equals(second) : (!first && !second);
    }
};

template <class TYPE>
struct luceneEqualTo {
    luceneEqualTo(const TYPE& type) : equalType(type) {}
    inline bool operator()(const TYPE& other) const {
        return equalType->equals(other);
    }
    const TYPE& equalType;
};

template <class TYPE>
struct luceneWeakEquals {
    inline bool operator()(const TYPE& first, const TYPE& second) const {
        if (first.expired() || second.expired()) {
            return (first.expired() && second.expired());
        }
        return first.lock()->equals(second.lock());
    }
};

template <class TYPE>
struct luceneHash {
    std::size_t operator()(const TYPE& type) const {
        return type ? type->hashCode() : 0;
    }
};

template <class TYPE>
struct luceneWeakHash {
    std::size_t operator()(const TYPE& type) const {
        return type.expired() ? 0 : type.lock()->hashCode();
    }
};

template <class TYPE>
struct luceneCompare {
    inline bool operator()(const TYPE& first, const TYPE& second) const {
        if (!second) {
            return false;
        }
        if (!first) {
            return true;
        }
        return (first->compareTo(second) < 0);
    }
};

typedef std::monostate VariantNull;
typedef std::variant<String, int32_t, int64_t, double, ReaderPtr, ByteArray, VariantNull> FieldsData;
typedef std::variant<String, uint8_t, int32_t, int64_t, double, VariantNull> ComparableValue;
typedef std::variant<int32_t, int64_t, double, VariantNull> NumericValue;
typedef std::variant<String, VariantNull> StringValue;
typedef std::variant<Collection<uint8_t>, Collection<int32_t>, Collection<double>, VariantNull> CollectionValue;

template<typename CHAR_TYPE> inline std::basic_ostream<CHAR_TYPE>& operator<<(std::basic_ostream<CHAR_TYPE>& os,const std::monostate&) { return os; }

// An ostream output operator for std::variant.
template<typename CHAR_TYPE,typename T,typename... Ts> inline std::basic_ostream<CHAR_TYPE>& operator<<(std::basic_ostream<CHAR_TYPE>& os,const std::variant<T,Ts...>& v)
    {
    std::visit([&os](auto&& arg){ os << arg; },v);
    return os;
    }

typedef HashSet< SegmentInfoPtr, luceneHash<SegmentInfoPtr>, luceneEquals<SegmentInfoPtr> > SetSegmentInfo;
typedef HashSet< MergeThreadPtr, luceneHash<MergeThreadPtr>, luceneEquals<MergeThreadPtr> > SetMergeThread;
typedef HashSet< OneMergePtr, luceneHash<OneMergePtr>, luceneEquals<OneMergePtr> > SetOneMerge;
typedef HashSet< QueryPtr, luceneHash<QueryPtr>, luceneEquals<QueryPtr> > SetQuery;
typedef HashSet< TermPtr, luceneHash<TermPtr>, luceneEquals<TermPtr> > SetTerm;
typedef HashSet< BooleanClausePtr, luceneHash<BooleanClausePtr>, luceneEquals<BooleanClausePtr> > SetBooleanClause;
typedef HashSet< ReaderFieldPtr, luceneHash<ReaderFieldPtr>, luceneEquals<ReaderFieldPtr> > SetReaderField;

template <class TYPE> struct luceneArrayHash { std::size_t operator()(const TYPE& type) const { return type.hashCode(); } };
typedef HashSet< ByteArray, luceneArrayHash<ByteArray> > SetByteArray;

typedef HashMap< String, String > MapStringString;
typedef HashMap< wchar_t, NormalizeCharMapPtr > MapCharNormalizeCharMap;
typedef HashMap< String, AnalyzerPtr > MapStringAnalyzer;
typedef HashMap< String, ByteArray > MapStringByteArray;
typedef HashMap< String, int32_t > MapStringInt;
typedef HashMap< String, FieldInfoPtr > MapStringFieldInfo;
typedef HashMap< String, Collection<TermVectorEntryPtr> > MapStringCollectionTermVectorEntry;
typedef HashMap< String, RefCountPtr > MapStringRefCount;
typedef HashMap< int32_t, TermVectorsPositionInfoPtr > MapIntTermVectorsPositionInfo;
typedef HashMap< String, MapIntTermVectorsPositionInfo > MapStringMapIntTermVectorsPositionInfo;
typedef HashMap< String, NormPtr > MapStringNorm;
typedef HashMap< String, TermVectorEntryPtr > MapStringTermVectorEntry;
typedef HashMap< String, RAMFilePtr > MapStringRAMFile;
typedef HashMap< int32_t, ByteArray > MapIntByteArray;
typedef HashMap< int32_t, FilterItemPtr > MapIntFilterItem;
typedef HashMap< int32_t, double > MapIntDouble;
typedef HashMap< int64_t, int32_t > MapLongInt;
typedef HashMap< String, double > MapStringDouble;
typedef HashMap< int32_t, CachePtr > MapStringCache;
typedef HashMap< String, LockPtr > MapStringLock;

typedef HashMap< SegmentInfoPtr, SegmentReaderPtr, luceneHash<SegmentInfoPtr>, luceneEquals<SegmentInfoPtr> > MapSegmentInfoSegmentReader;
typedef HashMap< SegmentInfoPtr, int32_t, luceneHash<SegmentInfoPtr>, luceneEquals<SegmentInfoPtr> > MapSegmentInfoInt;
typedef HashMap< DocFieldConsumerPerThreadPtr, Collection<DocFieldConsumerPerFieldPtr>, luceneHash<DocFieldConsumerPerThreadPtr>, luceneEquals<DocFieldConsumerPerThreadPtr> > MapDocFieldConsumerPerThreadCollectionDocFieldConsumerPerField;
typedef HashMap< InvertedDocConsumerPerThreadPtr, Collection<InvertedDocConsumerPerFieldPtr>, luceneHash<InvertedDocConsumerPerThreadPtr>, luceneEquals<InvertedDocConsumerPerThreadPtr> > MapInvertedDocConsumerPerThreadCollectionInvertedDocConsumerPerField;
typedef HashMap< InvertedDocEndConsumerPerThreadPtr, Collection<InvertedDocEndConsumerPerFieldPtr>, luceneHash<InvertedDocEndConsumerPerThreadPtr>, luceneEquals<InvertedDocEndConsumerPerThreadPtr> > MapInvertedDocEndConsumerPerThreadCollectionInvertedDocEndConsumerPerField;
typedef HashMap< TermsHashConsumerPerThreadPtr, Collection<TermsHashConsumerPerFieldPtr>, luceneHash<TermsHashConsumerPerThreadPtr>, luceneEquals<TermsHashConsumerPerThreadPtr> > MapTermsHashConsumerPerThreadCollectionTermsHashConsumerPerField;
typedef HashMap< FieldInfoPtr, Collection<NormsWriterPerFieldPtr>, luceneHash<FieldInfoPtr>, luceneEquals<FieldInfoPtr> > MapFieldInfoCollectionNormsWriterPerField;
typedef HashMap< IndexReaderPtr, HashSet<String>, luceneHash<IndexReaderPtr>, luceneEquals<IndexReaderPtr> > MapIndexReaderSetString;
typedef HashMap< TermPtr, int32_t, luceneHash<TermPtr>, luceneEquals<TermPtr> > MapTermInt;
typedef HashMap< QueryPtr, int32_t, luceneHash<QueryPtr>, luceneEquals<QueryPtr> > MapQueryInt;
typedef HashMap< EntryPtr, std::any, luceneHash<EntryPtr>, luceneEquals<EntryPtr> > MapEntryAny;
typedef HashMap< PhrasePositionsPtr, LuceneObjectPtr, luceneHash<PhrasePositionsPtr>, luceneEquals<PhrasePositionsPtr> > MapPhrasePositionsLuceneObject;
typedef HashMap< ReaderFieldPtr, SetReaderField, luceneHash<ReaderFieldPtr>, luceneEquals<ReaderFieldPtr> > MapReaderFieldSetReaderField;

typedef WeakHashMap< LuceneObjectWeakPtr, LuceneObjectPtr, luceneWeakHash<LuceneObjectWeakPtr>, luceneWeakEquals<LuceneObjectWeakPtr> > WeakMapObjectObject;
typedef WeakHashMap< LuceneObjectWeakPtr, MapEntryAny, luceneWeakHash<LuceneObjectWeakPtr>, luceneWeakEquals<LuceneObjectWeakPtr> > WeakMapLuceneObjectMapEntryAny;

typedef Map< String, AttributePtr > MapStringAttribute;
typedef Map< int64_t, DocumentsWriterThreadStatePtr > MapThreadDocumentsWriterThreadState;
typedef Map< String, IndexReaderPtr > MapStringIndexReader;
typedef Map< TermPtr, NumPtr, luceneCompare<TermPtr> > MapTermNum;

typedef std::function<bool (const TermVectorEntryPtr&, const TermVectorEntryPtr&)> TermVectorEntryComparator;

template < class KEY, class VALUE, class HASH = std::hash<KEY>, class EQUAL = std::equal_to<KEY> > class SimpleLRUCache;
typedef SimpleLRUCache< TermPtr, TermInfoPtr, luceneHash<TermPtr>, luceneEquals<TermPtr> > TermInfoCache;
typedef std::shared_ptr<TermInfoCache> TermInfoCachePtr;
}

#include "Synchronize.h"
#include "CycleCheck.h"
#if defined(LPP_BUILDING_LIB) || defined(LPP_EXPOSE_INTERNAL)
#define INTERNAL public
#else
#define INTERNAL protected
#endif

#endif
