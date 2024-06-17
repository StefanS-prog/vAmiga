// -----------------------------------------------------------------------------
// This file is part of vAmiga
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the Mozilla Public License v2
//
// See https://mozilla.org/MPL/2.0 for license information
// -----------------------------------------------------------------------------

#pragma once

#include "CPUTypes.h"
#include "SubComponent.h"
#include "RingBuffer.h"
#include "Moira.h"

namespace vamiga {

class CPU : public moira::Moira, public Inspectable<CPUInfo> 
{
    Descriptions descriptions = {{

        .name           = "CPU",
        .description    = "Central Processing Unit",
        .shell          = "cpu"
    }};

    ConfigOptions options = {

        OPT_CPU_REVISION,
        OPT_CPU_DASM_REVISION,
        OPT_CPU_DASM_SYNTAX,
        OPT_CPU_OVERCLOCKING,
        OPT_CPU_RESET_VAL
    };

    friend class Moira;

    // The current configuration
    CPUConfig config = {};

    // Result of the latest inspection
    mutable CPUInfo info = {};


    //
    // Overclocking
    //

public:

    // Sub-cycle counter
    i64 debt;

    // Number of cycles that should be executed at normal speed
    i64 slowCycles;


    //
    // Initializing
    //

public:

    CPU(Amiga& ref);

    
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
    void _trackOn() override;
    void _trackOff() override;
    
    template <class T>
    void serialize(T& worker)
    {
        if (util::isSoftResetter(worker)) return;

        worker

        // Items from CPU class
        << debt
        << slowCycles

        // Items from Moira class
        << clock
        << reg.pc
        << reg.pc0
        << reg.sr.t1
        << reg.sr.t0
        << reg.sr.s
        << reg.sr.m
        << reg.sr.x
        << reg.sr.n
        << reg.sr.z
        << reg.sr.v
        << reg.sr.c
        << reg.sr.ipl
        << reg.r
        << reg.usp
        << reg.isp
        << reg.msp
        << reg.ipl
        << reg.vbr
        << reg.sfc
        << reg.dfc
        << reg.cacr
        << reg.caar

        << queue.irc
        << queue.ird

        << ipl
        << fcl
        << fcSource
        << exception
        << cp
        << loopModeDelay
        << readBuffer
        << writeBuffer
        << flags;

        if (util::isResetter(worker)) return;

        worker

        // Persistent items
        << config.revision
        << config.dasmRevision
        << config.overclocking
        << config.regResetVal;
    }

    isize _size() override { COMPUTE_SNAPSHOT_SIZE }
    u64 _checksum() override { COMPUTE_SNAPSHOT_CHECKSUM }
    isize _load(const u8 *buffer) override { LOAD_SNAPSHOT_ITEMS }
    isize _save(u8 *buffer) override { SAVE_SNAPSHOT_ITEMS }
    isize didLoadFromBuffer(const u8 *buffer) override;
    

    //
    // Methods from CoreComponent
    //

public:

    const Descriptions &getDescriptions() const override { return descriptions; }
    

    //
    // Methods from Configurable
    //

public:
    
    const CPUConfig &getConfig() const { return config; }
    const ConfigOptions &getOptions() const override { return options; }
    i64 getOption(Option opt) const override;
    void setOption(Option opt, i64 value) override;

    
    //
    // Analyzing
    //
    
public:
    
    // CPUInfo getInfo() const { return CoreComponent::getInfo(info); }
    void cacheInfo(CPUInfo &result) const override;

    //
    // Working with the clock
    //

public:

    // Returns the clock in CPU cycles
    CPUCycle getCpuClock() const { return getClock(); }

    // Returns the clock in master cycles
    Cycle getMasterClock() const { return CPU_CYCLES(getClock()); }

    // Delays the CPU by a certain amout of master cycles
    void addWaitStates(Cycle cycles) { clock += AS_CPU_CYCLES(cycles); }
    
    // Resynchronizes an overclocked CPU with the Agnus clock
    void resyncOverclockedCpu();


    //
    // Running the disassembler
    //
    
    // Disassembles a recorded instruction from the log buffer
    const char *disassembleRecordedInstr(isize i, isize *len);
    const char *disassembleRecordedWords(isize i, isize len);
    const char *disassembleRecordedFlags(isize i);
    const char *disassembleRecordedPC(isize i);

    // Disassembles the instruction at the specified address
    const char *disassembleWord(u16 value);
    const char *disassembleAddr(u32 addr);
    const char *disassembleInstr(u32 addr, isize *len);
    const char *disassembleWords(u32 addr, isize len);

    // Disassembles the currently executed instruction
    const char *disassembleInstr(isize *len);
    const char *disassembleWords(isize len);
    const char *disassemblePC();

    // Dumps a portion of the log buffer
    void dumpLogBuffer(std::ostream& os, isize count);
    void dumpLogBuffer(std::ostream& os);

    // Disassembles a memory range
    void disassembleRange(std::ostream& os, u32 addr, isize count);
    void disassembleRange(std::ostream& os, std::pair<u32, u32> range, isize max = 255);


    //
    // Changing state
    //
    
    // Continues program execution at the specified address
    void jump(u32 addr);
    
    
    //
    // Instruction delegates
    //

    void willExecute(const char *func, moira::Instr I, moira::Mode M, moira::Size S, u16 opcode);
    void didExecute(const char *func, moira::Instr I, moira::Mode M, moira::Size S, u16 opcode);
    void willExecute(moira::ExceptionType exc, u16 vector);
    void didExecute(moira::ExceptionType exc, u16 vector);

    
    //
    // Debugging
    //
    
    // Manages the breakpoint list
    void setBreakpoint(u32 addr, isize ignores = 0) throws;
    void deleteBreakpoint(isize nr) throws;
    void enableBreakpoint(isize nr) throws;
    void disableBreakpoint(isize nr) throws;
    void toggleBreakpoint(isize nr) throws;
    void ignoreBreakpoint(isize nr, isize ignores) throws;

    // Manages the watchpoint list
    void setWatchpoint(u32 addr, isize ignores = 0) throws;
    void deleteWatchpoint(isize nr) throws;
    void enableWatchpoint(isize nr) throws;
    void disableWatchpoint(isize nr) throws;
    void toggleWatchpoint(isize nr) throws;
    void ignoreWatchpoint(isize nr, isize ignores) throws;

    // Manages the catchpoint list
    void setCatchpoint(u8 vector, isize ignores = 0) throws;
    void deleteCatchpoint(isize nr) throws;
    void enableCatchpoint(isize nr) throws;
    void disableCatchpoint(isize nr) throws;
    void toggleCatchpoint(isize nr) throws;
    void ignoreCatchpoint(isize nr, isize ignores) throws;
};

}
