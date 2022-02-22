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
#include "RamExpansion.h"
#include "HdrController.h"

class ZorroManager : public SubComponent {
    
public:

    // Number of emulated Zorro slots
    static constexpr isize slotCount = 6;
    
    // A FastRam expansion board
    RamExpansion ramExpansion = RamExpansion(amiga);

private:
    
    ZorroBoard *slots[slotCount + 1] = {
        
        &ramExpansion,
        &hdrController0,
        &hdrController1,
        &hdrController2,
        &hdrController3,
        nullptr
    };
    
            
    //
    // Initializing
    //
    
public:
    
    ZorroManager(Amiga& ref);
    
    
    //
    // Methods from AmigaObject
    //
    
private:
    
    const char *getDescription() const override { return "ZorroManager"; }
    void _dump(dump::Category category, std::ostream& os) const override;

    
    //
    // Methods from AmigaComponent
    //
    
private:
    
    void _reset(bool hard) override { RESET_SNAPSHOT_ITEMS(hard) }

    template <class T>
    void applyToPersistentItems(T& worker) { }
    template <class T>
    void applyToResetItems(T& worker, bool hard = true) { }

    isize _size() override { COMPUTE_SNAPSHOT_SIZE }
    u64 _checksum() override { COMPUTE_SNAPSHOT_CHECKSUM }
    isize _load(const u8 *buffer) override { LOAD_SNAPSHOT_ITEMS }
    isize _save(u8 *buffer) override { SAVE_SNAPSHOT_ITEMS }

        
    //
    // Accessing
    //
    
public:
    
    // Returns the device in the specified slot or nullptr
    ZorroBoard *getSlot(isize i);
    
    // Reads a value from Zorro board space
    u8 peek8(u32 addr);
    u16 peek16(u32 addr);
    u8 spypeek8(u32 addr) const;
    u16 spypeek16(u32 addr) const;

    // Writes a value into Zorro board space
    void poke8(u32 addr, u8 value);
    void poke16(u32 addr, u16 value);

    // Read a byte from autoconfig space
    u8 peekACF(u32 addr) const;
    u8 spypeekACF(u32 addr) const { return peekACF(addr); }
    
    // Write a byte into autoconfig space
    void pokeACF(u32 addr, u8 value);
    
    // Asks all boards to update the memory map
    void updateMemSrcTables();
    
private:
    
    // Returns the mapped in device for a given address
    ZorroBoard *mappedInDevice(u32 addr) const;
};
