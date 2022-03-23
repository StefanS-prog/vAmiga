// -----------------------------------------------------------------------------
// This file is part of Moira - A Motorola 68k emulator
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#pragma once

#include "MoiraConfig.h"
#include "MoiraTypes.h"

namespace moira {

// Base structure for a single breakpoint or watchpoint
struct Guard {

    // The observed address
    u32 addr = 0;

    // Disabled guards never trigger
    bool enabled = true;

    // Ignore counter
    long ignore = 0;
    
public:

    // Returns true if the guard hits
    bool eval(u32 addr, Size S = Byte);

};

// Base class for a collection of guards
class Guards {

    friend class Debugger;
    
protected:

    // Reference to the connected CPU
    class Moira &moira;

    // Capacity of the guards array
    long capacity = 1;

    // Array holding all guards
    Guard *guards = new Guard[1];

    // Number of currently stored guards
    long count = 0;

public:
    
    // A copy of the latest match
    std::optional <Guard> hit;

    
    //
    // Constructing
    //

public:

    Guards(Moira& ref) : moira(ref) { }
    virtual ~Guards();
    
    
    //
    // Inspecting the guard list
    //

    long elements() const { return count; }
    Guard *guardNr(long nr) const;
    Guard *guardAt(u32 addr) const;

    std::optional<u32> guardAddr(long nr) const;

    
    //
    // Adding or removing guards
    //

    bool isSet(long nr) const { return guardNr(nr) != nullptr; }
    bool isSetAt(u32 addr) const { return guardAt(addr) != nullptr; }

    void setAt(u32 addr);

    void remove(long nr);
    void removeAt(u32 addr);
    void removeAll() { count = 0; setNeedsCheck(false); }

    void replace(long nr, u32 addr);

    
    //
    // Enabling or disabling guards
    //

    bool isEnabled(long nr) const;
    bool isEnabledAt(u32 addr) const;
    bool isDisabled(long nr) const;
    bool isDisabledAt(u32 addr) const;

    void enable(long nr) { setEnable(nr, true); }
    void enableAt(u32 addr) { setEnableAt(addr, true); }
    void disable(long nr) { setEnable(nr, false); }
    void disableAt(u32 addr) { setEnableAt(addr, false); }
    void setEnable(long nr, bool val);
    void setEnableAt(u32 addr, bool val);

    void ignore(long nr, long count);
    
    
    //
    // Checking guards
    //

    // Indicates if guard checking is necessary
    virtual void setNeedsCheck(bool value) = 0;

    // Evaluates all guards
    bool eval(u32 addr, Size S = Byte);
};

class Breakpoints : public Guards {

public:

    Breakpoints(Moira& ref) : Guards(ref) { }
    void setNeedsCheck(bool value) override;
};

class Watchpoints : public Guards {

public:

    Watchpoints(Moira& ref) : Guards(ref) { }
    void setNeedsCheck(bool value) override;
};

class Catchpoints : public Guards {

public:

    Catchpoints(Moira& ref) : Guards(ref) { }
    void setNeedsCheck(bool value) override;
};

class Debugger {

public:

    // Reference to the connected CPU
    class Moira &moira;

    // Guard storage
    Breakpoints breakpoints = Breakpoints(moira);
    Watchpoints watchpoints = Watchpoints(moira);
    Catchpoints catchpoints = Catchpoints(moira);
    
private:

    /* Soft breakpoint for implementing single-stepping. In contrast to a
     * standard (hard) breakpoint, a soft breakpoint is deleted when reached.
     * The CPU halts if softStop matches the CPU's program counter (used to
     * implement "step over") or if softStop equals UINT64_MAX (used to
     * implement "step into"). To disable soft stopping, simply set softStop
     * to an unreachable memory location such as UINT64_MAX - 1.
     */
    u64 softStop = UINT64_MAX - 1;

    // Buffer storing logged instructions
    static const int logBufferCapacity = 256;
    Registers logBuffer[logBufferCapacity];

    // Logging counter
    long logCnt = 0;


    //
    // Constructing
    //

public:

    Debugger(Moira& ref) : moira(ref) { }

    void reset();


    //
    // Working with breakpoints, watchpoints, and catchpoints
    //

    // Sets a soft breakpoint that will trigger immediately
    void stepInto();

    // Sets a soft breakpoint to the next instruction
    void stepOver();

    // Returns true if a breakpoint, watchpoint, or catchpoints hits in
    bool softstopMatches(u32 addr);
    bool breakpointMatches(u32 addr);
    bool watchpointMatches(u32 addr, Size S);
    bool catchpointMatches(u32 vectorNr);

    // Saved program counters (DEPRECATED)
    i64 breakpointPC = -1;
    i64 watchpointPC = -1;

    
    //
    // Working with the log buffer
    //

    // Turns instruction logging on or off
    void enableLogging();
    void disableLogging();

    // Returns the number of logged instructions
    int loggedInstructions();

    // Logs an instruction
    void logInstruction();
    
    /* Reads an item from the log buffer
     *
     *    xxxRel: n == 0 returns the most recently recorded entry
     *    xxxAbs: n == 0 returns the oldest entry
     */
    Registers &logEntryRel(int n);
    Registers &logEntryAbs(int n);

    // Clears the log buffer
    void clearLog() { logCnt = 0; }
    
    
    //
    // Providing textual representations
    //
    
    // Returns a human-readable name for an exception vector
    static std::string vectorName(u8 vector);
    
    
    //
    // Changing state
    //
    
    // Continues program execution at the specified address
    void jump(u32 addr);    
};

}
