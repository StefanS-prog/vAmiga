// -----------------------------------------------------------------------------
// This file is part of vAmiga
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#include "Amiga.h"

u16
Paula::peekINTREQR()
{
    u16 result = intreq;

    debug(INTREG_DEBUG, "peekINTREQR(): %x\n", result);

    return result;
}

void
Paula::pokeINTREQ(u16 value)
{
    debug(INTREG_DEBUG, "pokeINTREQ(%X)\n", value);

    paula.setINTREQ(value);
}

void
Paula::setINTREQ(bool setclr, u16 value)
{
    assert(!(value & 0x8000));

    debug(INTREG_DEBUG, "setINTREQ(%d,%x)\n", setclr, value);

    if (setclr) {
        intreq |= value;
    } else {
        intreq &= ~value;
    }

    if (ciaa.irqPin() == 0) SET_BIT(intreq, 3);
    if (ciab.irqPin() == 0) SET_BIT(intreq, 13);

    checkInterrupt();
}

void
Paula::pokeINTENA(u16 value)
{
    debug(INTREG_DEBUG, "pokeINTENA(%X)\n", value);

    paula.setINTENA(value);
}

void
Paula::setINTENA(bool setclr, u16 value)
{
    assert(!(value & 0x8000));

    debug(INTREG_DEBUG, "setINTENA(%d,%x)\n", setclr, value);

    if (setclr) intena |= value; else intena &= ~value;
    checkInterrupt();
}

void
Paula::pokeADKCON(u16 value)
{
    plaindebug(MAX(AUDREG_DEBUG, DSKREG_DEBUG), "pokeADKCON(%X)\n", value);

    bool set = value & 0x8000;
    bool clr = !set;
    
    // Report unusual values for debugging
    if (set && (GET_BIT(value, 13) || GET_BIT(value, 14))) {
        debug(XFILES, "XFILES (ADKCON): PRECOMP set (%x)\n", value);
    }
        if (clr && GET_BIT(value, 12)) {
        debug(XFILES, "XFILES (ADKCON): MFMPREC cleared (GCR) (%x)\n", value);
    }
        if (set && GET_BIT(value, 9)) {
        debug(XFILES, "XFILES (ADKCON): MSBSYNC set (GCR) (%x)\n", value);
    }
        if (clr && GET_BIT(value, 8)) {
        debug(XFILES, "XFILES (ADKCON): FAST cleared (GCR) (%x)\n", value);
    }
        
    if (set) adkcon |= (value & 0x7FFF); else adkcon &= ~value;

    // Take care of a possible change of the UARTBRK bit
    uart.updateTXD();

    if (adkcon & 0b1110111) debug(AUDREG_DEBUG, "ADKCON MODULATION: %x\n", adkcon);
}

template <int x> u16
Paula::peekPOTxDAT()
{
    assert(x == 0 || x == 1);

    u16 result = x ? HI_LO(potCntY1, potCntX1) : HI_LO(potCntY0, potCntX0);
    debug(POT_DEBUG, "peekPOT%dDAT() = %X\n", x, result);

    return result;
}

u16
Paula::peekPOTGOR()
{
    u16 result = 0;

    REPLACE_BIT(result, 14, chargeY1 >= 1.0);
    REPLACE_BIT(result, 12, chargeX1 >= 1.0);
    REPLACE_BIT(result, 10, chargeY0 >= 1.0);
    REPLACE_BIT(result,  8, chargeX0 >= 1.0);

    debug(POT_DEBUG, "charges: %f %f %f %f\n", chargeY1, chargeX1, chargeY0, chargeX0);
    
    // A connected device may force the output level to a specific value
    controlPort1.changePotgo(result);
    controlPort2.changePotgo(result);

    debug(POT_DEBUG, "peekPOTGOR = %X (potgo = %x)\n", result, potgo);
    return result;
}

void
Paula::pokePOTGO(u16 value)
{
    debug(POT_DEBUG, "pokePOTGO(%X)\n", value);

    potgo = value;

    // Take care of bits that are configured as outputs
    if (GET_BIT(value, 9))  chargeX0 = GET_BIT(value, 8)  ? 1.0 : 0.0;
    if (GET_BIT(value, 11)) chargeY0 = GET_BIT(value, 10) ? 1.0 : 0.0;
    if (GET_BIT(value, 13)) chargeX1 = GET_BIT(value, 12) ? 1.0 : 0.0;
    if (GET_BIT(value, 15)) chargeY1 = GET_BIT(value, 14) ? 1.0 : 0.0;

    // Check the START bit
    if (GET_BIT(value, 0)) {

        debug(POT_DEBUG, "Starting potentiometer scan procedure\n");

        // Clear potentiometer counters
        potCntX0 = 0;
        potCntY0 = 0;
        potCntX1 = 0;
        potCntY1 = 0;

        // Schedule the first DISCHARGE event
        agnus.schedulePos<POT_SLOT>(agnus.pos.v, HPOS_MAX, POT_DISCHARGE, 8);
    }
}

template u16 Paula::peekPOTxDAT<0>();
template u16 Paula::peekPOTxDAT<1>();