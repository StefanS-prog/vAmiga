// -----------------------------------------------------------------------------
// This file is part of vAmiga
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#pragma once

#include "SubComponent.h"
#include "ZorroBoardTypes.h"

class ZorroBoard : public SubComponent {
    
    friend class ZorroManager;
    
protected:
    
    // Current state
    BoardState state;
    
    
    //
    // Constructing
    //
    
public:
    
    using SubComponent::SubComponent;
    
    
    //
    // Methods from AmigaObject
    //
    
protected:
        
    void _dump(dump::Category category, std::ostream& os) const override;


    //
    // Querying
    //
    
public:
    
    // Returns basic board properties
    virtual u8 type() const { return 0; }
    virtual u8 product() const { return 0; }
    virtual u8 flags() const { return 0; }
    virtual u16 manufacturer() const { return 0; }
    virtual u32 serialNumber() const { return 0; }
    
private:
    
    // Reads a single byte from configuration space
    u8 getDescriptorByte(isize offset) const;
    
    
    //
    // Accessing
    //
    
    virtual u8 peek8(u32 addr) const;
    virtual u8 spypeek8(u32 addr) const { return peek8(addr); }
    virtual void poke8(u32 addr, u8 value) = 0;
    
    
    //
    // Changing state
    //
    
    // Called when autoconfig is complete
    virtual void activate();
    
    // Called when the board is supposed to shut up by software
    virtual void shutup();
    
    // Updates the current memory map
    virtual void updateMemSrcTables() { };
};
