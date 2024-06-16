// -----------------------------------------------------------------------------
// This file is part of vAmiga
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the Mozilla Public License v2
//
// See https://mozilla.org/MPL/2.0 for license information
// -----------------------------------------------------------------------------

#pragma once

#include "ControlPortTypes.h"
#include "SubComponent.h"
#include "Joystick.h"
#include "Mouse.h"

namespace vamiga {

class ControlPort : public SubComponent, public Inspectable<ControlPortInfo> {

    Descriptions descriptions = {
        {
            .name           = "Port1",
            .description    = "Control Port 1"
        },
        {
            .name           = "Port2",
            .description    = "Control Port 2"
        }
    };


    ConfigOptions options = {

    };

public:

    static constexpr isize PORT1 = 0;
    static constexpr isize PORT2 = 1;

private:
    
    // The connected device
    ControlPortDevice device = CPD_NONE;
    
    // The two mouse position counters
    i64 mouseCounterX = 0;
    i64 mouseCounterY = 0;

    // The position of the connected mouse
    i64 mouseX = 0;
    i64 mouseY = 0;
    
    // Resistances on the potentiometer lines (specified as a delta charge)
    double chargeDX;
    double chargeDY;
    
    
    //
    // Sub components
    //

public:
    
    Mouse mouse = Mouse(amiga, *this);
    Joystick joystick = Joystick(amiga, *this);


    //
    // Initializing
    //
    
public:
    
    ControlPort(Amiga& ref, isize nr);

    
    //
    // Methods from CoreObject
    //
    
private:
    
    void _dump(Category category, std::ostream& os) const override;
    
    
    //
    // Methods from CoreComponent
    //
    
private:
    
    void _reset(bool hard) override { RESET_SNAPSHOT_ITEMS(hard) }

    template <class T>
    void serialize(T& worker)
    {
        worker

        << mouseCounterX
        << mouseCounterY
        << chargeDX
        << chargeDY;
    }

    isize _size() override { COMPUTE_SNAPSHOT_SIZE }
    u64 _checksum() override { COMPUTE_SNAPSHOT_CHECKSUM }
    isize _load(const u8 *buffer) override { LOAD_SNAPSHOT_ITEMS }
    isize _save(u8 *buffer) override { SAVE_SNAPSHOT_ITEMS }

public:

    const Descriptions &getDescriptions() const override { return descriptions; }

    
    //
    // Methods from Configurable
    //

public:

    const ConfigOptions &getOptions() const override { return options; }


    //
    // Analyzing
    //

public:

    void cacheInfo(ControlPortInfo &result) const override;

    bool isPort1() const { return objid == PORT1; }
    bool isPort2() const { return objid == PORT2; }

    
    //
    // Accessing
    //

public:

    // Changes the connected device type
    void setDevice(ControlPortDevice value) { device = value; }
    
    // Getter for the delta charges
    i16 getChargeDX() const { return (i16)chargeDX; }
    i16 getChargeDY() const { return (i16)chargeDY; }

    // Called by the mouse when it's position has changed
    void updateMouseXY(i64 x, i64 y);
    
    // Returns the control port bits showing up in the JOYxDAT register
    u16 joydat() const;

    // Emulates a write access to JOYTEST
    void pokeJOYTEST(u16 value);

    // Modifies the POTGOR bits according to the connected device
    void changePotgo(u16 &potgo) const;

    // Modifies the PRA bits of CIA A according to the connected device
    void changePra(u8 &pra) const;
};

}
