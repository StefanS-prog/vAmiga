// -----------------------------------------------------------------------------
// This file is part of vAmiga
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#pragma once

#include "DmaDebuggerTypes.h"
#include "SubComponent.h"
#include "Colors.h"
#include "Constants.h"

class DmaDebugger : public SubComponent {

    // Current configuration
    DmaDebuggerConfig config = {};

    // Enable bits
    bool visualize[BUS_COUNT] = {};
    
    // Colors used for highlighting DMA (derived from config.debugColor)
    RgbColor debugColor[BUS_COUNT][5] = {};

    // Local copies of the corresponding Agnus arrays
    u16 busValue[HPOS_CNT];
    BusOwner busOwner[HPOS_CNT];


    //
    // Initializing
    //

public:

    DmaDebugger(Amiga &ref);

    
    //
    // Methods from AmigaObject
    //
    
private:
    
    const char *getDescription() const override { return "DmaDebugger"; }
    void _dump(Category category, std::ostream& os) const override { }

    
    //
    // Methods from AmigaComponent
    //
    
private:
    
    void _reset(bool hard) override { }


    //
    // Configuring
    //

public:
    
    const DmaDebuggerConfig &getConfig() const { return config; }
    void resetConfig() override;

    i64 getConfigItem(Option option) const;
    i64 getConfigItem(Option option, long id) const;
    void setConfigItem(Option option, i64 value);
    void setConfigItem(Option option, long id, i64 value);

private:

    void getColor(DmaChannel channel, double *rgb);
    void setColor(BusOwner owner, u32 rgba);

    
    //
    // Analyzing
    //
    
public:

    // Returns the result of the most recent call to inspect()
    DmaDebuggerInfo getInfo();

    
    //
    // Serializing
    //

private:

    isize _size() override { return 0; }
    u64 _checksum() override { return 0; }
    isize _load(const u8 *buffer) override {return 0; }
    isize _save(u8 *buffer) override { return 0; }
    

    //
    // Running the debugger
    //

public:
    
    // Called by Agnus at the end of each scanline
    void eolHandler();

    // Called by Agnus at the beginning of the HSYNC area
    void hsyncHandler();

    // Cleans by Agnus at the end of each frame
    void vSyncHandler();

private:

    // Visualizes DMA usage for a certain range of DMA cycles
    void computeOverlay(u32 *ptr, isize first, isize last); 
};
