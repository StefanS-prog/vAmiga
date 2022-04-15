// -----------------------------------------------------------------------------
// This file is part of vAmiga
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#pragma once

#include "AmigaComponent.h"
#include "IOUtils.h"

class Properties : public AmigaObject {

    mutable util::ReentrantMutex mutex;
    
    // Key-value storage
    std::map <string, string> values;

    // Default values (used if no value is set)
    std::map <string, string> defaults;

    
    //
    // Initializing
    //
    
public:
    
    Properties();
    Properties(Properties const&) = delete;
    void operator=(Properties const&) = delete;
    
    
    //
    // Methods from AmigaObject
    //
    
public:

    // void prefix() const override;

private:
    
    const char *getDescription() const override { return "Properties"; }
    void _dump(Category category, std::ostream& os) const override;

    
    //
    // Loading and saving
    //
    
public:
    
    // Loads a properties file from disk
    void load(const fs::path &path) throws;
    void load(std::ifstream &stream) throws;
    void load(std::stringstream &stream) throws;

    // Saves a properties file to disk
    void save(const fs::path &path) throws;
    void save(std::ofstream &stream) throws;
    void save(std::stringstream &stream) throws;

    
    //
    // Working with key-value pairs
    //

public:
    
    string get(const string &key) throws;
    i64 get(Option option) throws;
    i64 get(Option option, isize nr) throws;
    
    string getFallback(const string &key) throws;
    i64 getFallback(Option option) throws;
    i64 getFallback(Option option, isize nr) throws;

    void set(const string &key, const string &value);
    void set(Option option, i64 value);
    void set(Option option, isize nr, i64 value);
    void set(Option option, std::vector <isize> nrs, i64 value);

    void setFallback(const string &key, const string &value);
    void setFallback(Option option, i64 value);
    void setFallback(Option option, isize nr, i64 value);
    void setFallback(Option option, std::vector <isize> nrs, i64 value);

    void remove();
    void remove(const string &key) throws;
    void remove(Option option) throws;
    void remove(Option option, isize nr) throws;
    void remove(Option option, std::vector <isize> nrs) throws;
};
