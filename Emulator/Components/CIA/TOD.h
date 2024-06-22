// -----------------------------------------------------------------------------
// This file is part of vAmiga
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the Mozilla Public License v2
//
// See https://mozilla.org/MPL/2.0 for license information
// -----------------------------------------------------------------------------

#pragma once

#include "TODTypes.h"
#include "SubComponent.h"

namespace vamiga {

typedef union
{
    struct
    {
        u8 lo;
        u8 mid;
        u8 hi;
    };
    u32 value;
}
Counter24;

class TOD : public SubComponent, public Inspectable<TODInfo> {

    Descriptions descriptions = {{

        .name           = "TOD",
        .description    = "Time-of-day Clock",
        .shell          = "tod"
    }};

    ConfigOptions options = {

    };

    friend class CIA;

    // Reference to the connected CIA
    class CIA &cia;

    // Result of the latest inspection
    mutable TODInfo info = {};

    // The 24 bit counter
    Counter24 tod;
    
    // Counter value before the latest increment
    Counter24 preTod;
    
    // Time stamp of the last increment
    Cycle lastInc;

    // The counter latch
    Counter24 latch;
    
    // Alarm value
    Counter24 alarm;
    
    /* Indicates if the TOD registers are frozen. The CIA chip freezes the
     * registers when the counter's high byte (bits 16 - 23) is read and
     * reactivates them, when the low byte (bits 0 - 7) is read. Although the
     * values stay constant, the internal clock continues to run.
     */
    bool frozen;
    
    /* Indicates if the TOD clock is halted. The CIA chip stops the TOD clock
     * when the counter's high byte (bits 16 - 23) is written and restarts it,
     * when the low byte (bits 0 - 7) is written.
     */
    bool stopped;
    
    /* Indicates if tod time matches the alarm value. This value is read in
     * checkIrq() for edge detection.
     */
    bool matching;
    
    
    //
    // Initializing
    //

public:

    TOD(CIA &ciaref, Amiga& ref);

    
    //
    // Methods from CoreObject
    //
    
private:
    
    void _dump(Category category, std::ostream& os) const override;

    
    //
    // Methods from CoreComponent
    //
    
private:
    
    void _reset(bool hard) override;

    template <class T>
    void serialize(T& worker)
    {
        worker

        << tod.value
        << preTod.value
        << lastInc
        << latch.value
        << alarm.value
        << frozen
        << stopped
        << matching;

    } SERIALIZERS(serialize);

    u64 _checksum() override { COMPUTE_SNAPSHOT_CHECKSUM }

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

    void cacheInfo(TODInfo &result) const override;


    //
    // Accessing
    //
    
    // Returns the counter's high byte (bits 16 - 23).
    u8 getCounterHi(Cycle timeStamp = INT64_MAX) const;

    // Returns the counter's intermediate byte (bits 8 - 15).
    u8 getCounterMid(Cycle timeStamp = INT64_MAX) const;
    
    // Returns the counter's low byte (bits 0 - 7).
    u8 getCounterLo(Cycle timeStamp = INT64_MAX) const;

    // Returns the alarm value's high byte (bits 16 - 23).
    u8 getAlarmHi() const;

    // Returns the alarm value's intermediate byte (bits 8 - 15).
    u8 getAlarmMid() const;

    // Returns the alarm value's low byte (bits 0 - 7).
    u8 getAlarmLo() const;

    // Sets the counter's high byte (bits 16 - 23).
    void setCounterHi(u8 value);

    // Sets the counter's intermediate byte (bits 8 - 15).
    void setCounterMid(u8 value);

    // Sets the counter's low byte (bits 0 - 7).
    void setCounterLo(u8 value);

    // Sets the alarm value's high byte (bits 16 - 23).
    void setAlarmHi(u8 value);

    // Sets the alarm value's intermediate byte (bits 8 - 15).
    void setAlarmMid(u8 value);

    // Sets the alarm value's low byte (bits 0 - 7).
    void setAlarmLo(u8 value);


    //
    // Executing
    //

public:
    
    // Increments the counter
    void increment();
    
private:

    bool incLoNibble(u8 &counter);
    bool incHiNibble(u8 &counter);

    // Updates variable 'matching'. A positive edge triggers an interrupt.
    void checkIrq();

    // Freezes the counter
    void freeze() { if (!frozen) { latch.value = tod.value; frozen = true; } }
    
    // Unfreezes the counter
    void defreeze() { frozen = false; }
    
    // Stops the counter
    void stop() { stopped = true; }
    
    // Starts the counter
    void cont() { stopped = false; }
};

}
