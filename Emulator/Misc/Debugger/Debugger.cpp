// -----------------------------------------------------------------------------
// This file is part of vAmiga
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the Mozilla Public License v2
//
// See https://mozilla.org/MPL/2.0 for license information
// -----------------------------------------------------------------------------

#include "config.h"
#include "Debugger.h"
#include "Amiga.h"
#include "IOUtils.h"
#include <sstream>

namespace vamiga {

const char *
Debugger::regName(u32 addr)
{
    return ChipsetRegEnum::key((addr >> 1) & 0xFF);
}

void
Debugger::stopAndGo()
{
    isRunning() ? amiga.pause() : amiga.run();
}

void
Debugger::stepInto()
{
    if (isRunning()) return;

    cpu.debugger.stepInto();
    amiga.run();

    // Inform the GUI
    msgQueue.put(MSG_STEP);
}

void
Debugger::stepOver()
{
    if (isRunning()) return;

    cpu.debugger.stepOver();
    amiga.run();

    // Inform the GUI
    msgQueue.put(MSG_STEP);
}

void
Debugger::jump(u32 addr)
{
    cpu.jump(addr);
}

template <Accessor A> const char *
Debugger::ascDump(u32 addr, isize numBytes) const
{
    assert(numBytes < 256);
    static char str[256];

    for (isize i = 0; i < numBytes; i += 2) {
        u16 word = mem.spypeek16 <A> ((u32)(addr + i));
        str[i] = isprint(HI_BYTE(word)) ? HI_BYTE(word) : '.';
        str[i+1] = isprint(LO_BYTE(word)) ? LO_BYTE(word) : '.';
    }
    str[numBytes] = 0;
    return str;
}

template <Accessor A> const char *
Debugger::hexDump(u32 addr, isize numBytes) const
{
    assert(numBytes % 2 == 0);
    static char str[256];
    char *p = str;

    for (isize i = 0; i < numBytes; i += 2, p += 5) {

        u16 word = mem.spypeek16 <A> ((u32)(addr + i));

        u8 digit1 = (word >> 12) & 0xF;
        u8 digit2 = (word >> 8) & 0xF;
        u8 digit3 = (word >> 4) & 0xF;
        u8 digit4 = (word >> 0) & 0xF;

        p[0] = digit1 < 10 ? '0' + digit1 : 'A' + digit1 - 10;
        p[1] = digit2 < 10 ? '0' + digit2 : 'A' + digit2 - 10;
        p[2] = digit3 < 10 ? '0' + digit3 : 'A' + digit3 - 10;
        p[3] = digit4 < 10 ? '0' + digit4 : 'A' + digit4 - 10;
        p[4] = i == numBytes - 2 ? char(0) : ' ';
    }

    return str;
}

template <Accessor A> void
Debugger::memDump(std::ostream& os, u32 addr, isize numLines) const
{
    addr &= ~0xF;

    for (isize i = 0; i < numLines; i++, addr += 16) {

        os << std::setfill('0') << std::hex << std::right << std::setw(6) << isize(addr);
        os << ":  ";
        os << hexDump<A>(addr, 16);
        os << "  ";
        os << ascDump<A>(addr, 16);
        os << std::endl;
    }
}

void
Debugger::convertNumeric(std::ostream& os, isize value) const
{
    using namespace util;

    auto chr = [&](u8 v) { return std::isprint(v) ? (char)v : '.'; };

    os << std::setw(10) << std::right << std::setfill(' ') << dec(u32(value)) << " | ";
    os << hex(u32(value)) << " | ";
    os << bin(u32(value)) << " | ";
    os << "\"";
    os << chr(BYTE3(value)) << chr(BYTE2(value)) << chr(BYTE1(value)) << chr(BYTE0(value));
    os << "\"";
    os << std::endl;
}

void 
Debugger::convertNumeric(std::ostream& os, string s) const
{
    u8 bytes[4];

    bytes[0] = s.length() >= 4 ? (u8)s[s.length() - 1] : 0;
    bytes[1] = s.length() >= 3 ? (u8)s[s.length() - 2] : 0;
    bytes[2] = s.length() >= 2 ? (u8)s[s.length() - 3] : 0;
    bytes[3] = s.length() >= 1 ? (u8)s[s.length() - 4] : 0;

    convertNumeric(os, HI_HI_LO_LO(bytes[0], bytes[1], bytes[2], bytes[3]));
}

 template const char *Debugger::ascDump <ACCESSOR_CPU> (u32 addr, isize numBytes) const;
 template const char *Debugger::ascDump <ACCESSOR_AGNUS> (u32 addr, isize numBytes) const;

 template const char *Debugger::hexDump <ACCESSOR_CPU> (u32 addr, isize numBytes) const;
 template const char *Debugger::hexDump <ACCESSOR_AGNUS> (u32 addr, isize numBytes) const;

 template void Debugger::memDump <ACCESSOR_CPU> (std::ostream& os, u32 addr, isize numLines) const;
 template void Debugger::memDump <ACCESSOR_AGNUS> (std::ostream& os, u32 addr, isize numLines) const;
}
