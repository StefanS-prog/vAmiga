// -----------------------------------------------------------------------------
// This file is part of vAmiga
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the Mozilla Public License v2
//
// See https://mozilla.org/MPL/2.0 for license information
// -----------------------------------------------------------------------------

#pragma once

#ifdef __cplusplus

#include "Types.h"
#include <map>

namespace util {

#define assert_enum(e,v) assert(e##Enum::isValid(v))

template <class T, typename E> struct Reflection {

    // Returns the shortened key as a C string
    static const char *key(long nr) { return T::key((E)nr); }

    // Returns the key without the section prefix (if any)
    static const char *plainkey(isize nr) {
        auto *p = key(nr);
        for (isize i = 0; p[i]; i++) if (p[i] == '.') return p + i + 1;
        return p;
    }
    
    // Collects all key / value pairs
    static std::map <string, long> pairs() {
        
        std::map <string,long> result;
                
        for (isize i = T::minVal; i <= T::maxVal; i++) {
            if (T::isValid(i)) result.insert(std::make_pair(key(i), i));
        }

        return result;
    }

    // Returns a list in form of a colon seperated string
    static string keyList(bool prefix = false, const string &delim = ", ") {
        
        string result;
        
        auto p = pairs();
        for(auto it = std::begin(p); it != std::end(p); ++it) {
            
            if (it != std::begin(p)) result += delim;
            if (prefix && T::prefix()) result += T::prefix();
            result += it->first;
        }
        
        return result;
    }

    // Convinience wrapper
    static string argList(bool prefix = false) {

        return "{ " + keyList(prefix, " | ") + " }";
    }
};

}

#endif
