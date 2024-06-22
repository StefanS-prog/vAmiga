// -----------------------------------------------------------------------------
// This file is part of vAmiga
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the Mozilla Public License v2
//
// See https://mozilla.org/MPL/2.0 for license information
// -----------------------------------------------------------------------------

#pragma once

#include "SubComponent.h"
#include "RemoteManagerTypes.h"
#include "SerServer.h"
#include "RshServer.h"
#include "GdbServer.h"

namespace vamiga {

class RemoteManager : public SubComponent {

    Descriptions descriptions = {{

        .name           = "RemoteManager",
        .description    = "Remote Manager",
        .shell          = "server"
    }};

    ConfigOptions options = {

    };

public:
    
    // The remote servers
    SerServer serServer = SerServer(amiga);
    RshServer rshServer = RshServer(amiga);
    GdbServer gdbServer = GdbServer(amiga);
    
    // Convenience wrapper
    std::vector <RemoteServer *> servers = {
        &serServer, &rshServer, &gdbServer
    };

    
    //
    // Initializing
    //
    
public:
    
    RemoteManager(Amiga& ref);
    // ~RemoteManager();
    
    
    //
    // Methods from CoreObject
    //
    
protected:
    
    void _dump(Category category, std::ostream& os) const override;
    
    
    //
    // Methods from CoreComponent
    //
    
private:
    
    template <class T> void serialize(T& worker) { } SERIALIZERS(serialize);
    
    void _reset(bool hard) override { }
    isize _size() override { return 0; }
    u64 _checksum() override { return 0; }
    isize _load(const u8 *buffer) override {return 0; }
    isize _save(u8 *buffer) override { return 0; }
    
public:

    const Descriptions &getDescriptions() const override { return descriptions; }


    //
    // Methods from Configurable
    //

public:

    const ConfigOptions &getOptions() const override { return options; }

    /*
    i64 getConfigItem(Option option, long id) const;
    void setOption(Option option, i64 value) override;
    void setConfigItem(Option option, long id, i64 value);
    */
    
    //
    // Managing connections
    //
    
public:
    
    // Returns the number of servers being in a certain state
    isize numLaunching() const;
    isize numListening() const;
    isize numConnected() const;
    isize numErroneous() const;


    //
    // Servicing events
    //
    
public:
    
    void serviceServerEvent();
};

}
