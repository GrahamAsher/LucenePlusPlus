/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2009-2014 Alan Wright. All rights reserved.
// Distributable under the terms of either the Apache License (Version 2.0)
// or the GNU Lesser General Public License.
/////////////////////////////////////////////////////////////////////////////

#ifndef VARIANTUTILS_H
#define VARIANTUTILS_H

#include <any>
#include "Lucene.h"
#include "MiscUtils.h"

namespace Lucene {

class LPPAPI VariantUtils {
public:
    template <typename TYPE> static TYPE get(const std::any& var)
        {
        return var.type() == typeid(TYPE) ? std::any_cast<TYPE>(var) : TYPE();
        }

    template <typename TYPE> static bool typeOf(const std::any& var)
        {
        return var.type() == typeid(TYPE);
        }

    template <typename TYPE, typename... VARIANT_TYPES> static TYPE get(const std::variant<VARIANT_TYPES...>& var)
        {
        if (std::holds_alternative<TYPE>(var))
            return std::get<TYPE,VARIANT_TYPES...>(var);
        return TYPE();
        }

    template <typename TYPE,typename... VARIANT_TYPES> static bool typeOf(const std::variant<VARIANT_TYPES...>& var)
        {
        return std::holds_alternative<TYPE>(var);
        }

    static VariantNull null() {
        return VariantNull();
    }

    static bool isNull(const std::any& var) {
        return !var.has_value();
    }

    template <typename VAR>
    static bool isNull(VAR var) {
        return typeOf<VariantNull>(var);
    }


    template<typename... VARIANT_TYPES> static int32_t hashCode(const std::any& var)
        {
        const auto& T = var.type();
        if (T == typeid(String))
            return StringUtils::hashCode(std::any_cast<String>(var));
        if (T == typeid(int32_t))
            return std::any_cast<int32_t>(var);
        if (T == typeid(int64_t))
            return (int32_t)std::any_cast<int64_t>(var);
        if (T == typeid(double))
            {
            int64_t longBits = MiscUtils::doubleToLongBits(std::any_cast<double>(var));
            return (int32_t)(longBits ^ (longBits >> 32));
            }
        if (T == typeid(Collection<uint8_t>))
            return std::any_cast<Collection<uint8_t>>(var).hashCode();
        if (T == typeid(Collection<int32_t>))
            return std::any_cast<Collection<int32_t>>(var).hashCode();
        if (T == typeid(Collection<int64_t>))
            return std::any_cast<Collection<int64_t>>(var).hashCode();
        if (T == typeid(Collection<double>))
            return std::any_cast<Collection<double>>(var).hashCode();
        if (T == typeid(Collection<String>))
            return std::any_cast<Collection<String>>(var).hashCode();
        if (T == typeid(LuceneObjectPtr))
            return std::any_cast<LuceneObjectPtr>(var)->hashCode();

        return 0;
        }

    template<typename... VARIANT_TYPES> static int32_t hashCode(const std::variant<VARIANT_TYPES...>& aValue)
        {
        auto visitor = [](auto&& var)->int32_t
            {
            using T = std::decay_t<decltype(var)>;
            if constexpr (std::is_same_v<T,String>)
                return StringUtils::hashCode(var);
            if constexpr (std::is_same_v<T,int32_t>)
                return var;
            if constexpr (std::is_same_v<T,int64_t>)
                return (int32_t)var;
            if constexpr (std::is_same_v<T,double>)
                {
                int64_t longBits = MiscUtils::doubleToLongBits(var);
                return (int32_t)(longBits ^ (longBits >> 32));
                }
            if constexpr (std::is_same_v<T,Collection<uint8_t>>)
                return var.hashCode();
            if constexpr (std::is_same_v<T,Collection<int32_t>>)
                return var.hashCode();
            if constexpr (std::is_same_v<T,Collection<int64_t>>)
                return var.hashCode();
            if constexpr (std::is_same_v<T,Collection<double>>)
                return var.hashCode();
            if constexpr (std::is_same_v<T,Collection<String>>)
                return var.hashCode();
            if constexpr (std::is_same_v<T,LuceneObjectPtr>)
                return var->hashCode();

            return 0;
            };
        return std::visit(visitor,aValue);
        }

    template <typename FIRST, typename SECOND>
    static bool equalsType(FIRST first, SECOND second) {
        return (first.type() == second.type());
    }

    template <typename FIRST, typename SECOND>
    static bool equals(FIRST first, SECOND second) {
        return first.type() == second.type() ? (first == second) : false;
    }

    template <typename VAR>
    static int32_t compareTo(VAR first, VAR second) {
        return first < second ? -1 : (first == second ? 0 : 1);
    }
};

}

#endif
