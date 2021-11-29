/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2009-2014 Alan Wright. All rights reserved.
// Distributable under the terms of either the Apache License (Version 2.0)
// or the GNU Lesser General Public License.
/////////////////////////////////////////////////////////////////////////////

#ifndef LUCENEFACTORY_H
#define LUCENEFACTORY_H

#include <memory>

namespace Lucene
{

template <class T,class ... Types> std::shared_ptr<T> newInstance(Types... args) { return std::make_shared<T>(const_cast<const Types&>(args)...); }

template <class T,class ... Types> std::shared_ptr<T> newLucene(Types... args)
    {
    std::shared_ptr<T> instance = std::make_shared<T>(const_cast<const Types&>(args)...);
    instance->initialize();
    return instance;
    }

}

#endif
