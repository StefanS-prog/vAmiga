// -----------------------------------------------------------------------------
// This file is part of vAmiga
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#include "Amiga.h"

int dirk = 0;

Denise::Denise()
{
    setDescription("Denise");
    
    subComponents = vector<HardwareComponent *> {
        
        &pixelEngine,
    };
}

Denise::~Denise()
{
    debug(2, "Destroying Denise[%p]\n", this);
}

void
Denise::_initialize()
{
    agnus = &amiga->agnus;
}

void
Denise::_powerOn()
{
    memset(bBuffer, 0, sizeof(bBuffer));
    memset(iBuffer, 0, sizeof(iBuffer));
    memset(zBuffer, 0, sizeof(zBuffer));
}

void
Denise::_inspect()
{
    // Prevent external access to variable 'info'
    pthread_mutex_lock(&lock);
    
    // Biplane information
    info.bplcon0 = bplcon0;
    info.bplcon1 = bplcon1;
    info.bplcon2 = bplcon2;
    info.bpu = (bplcon0 >> 12) & 0b111;

    info.diwstrt = agnus->diwstrt;
    info.diwstop = agnus->diwstop;
    info.diwHstrt = agnus->diwHstrt;
    info.diwHstop = agnus->diwHstop;
    info.diwVstrt = agnus->diwVstrt;
    info.diwVstop = agnus->diwVstop;

    info.joydat[0] = amiga->controlPort1.joydat();
    info.joydat[1] = amiga->controlPort2.joydat();
    info.clxdat = 0;

    for (unsigned i = 0; i < 6; i++) {
        info.bpldat[i] = bpldat[i];
    }
    for (unsigned i = 0; i < 32; i++) {
        info.colorReg[i] = pixelEngine.getColor(i);
        info.color[i] = pixelEngine.getRGBA(i);
    }
    
    // Sprite information
    
    for (int i = 0; i < 8; i++) {

        /* The sprite info is extracted from the pos and ctl values that are
         * recorded by the hsync handler at the beginning of rasterline 26.
         *
         * pos:  15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0  (Hx = HSTART)
         *       E7 E6 E5 E4 E3 E2 E1 E0 H8 H7 H6 H5 H4 H3 H2 H1  (Ex = VSTART)
         * ctl:  L7 L6 L5 L4 L3 L2 L1 L0 AT  -  -  -  - E8 L8 H0  (Lx = VSTOP)
         */
        uint16_t pos = info.sprite[i].pos;
        uint16_t ctl = info.sprite[i].ctl;
        info.sprite[i].ptr = agnus->sprpt[i]; 
        info.sprite[i].hstrt = ((pos & 0x00FF) << 1) | (ctl & 0b001);
        info.sprite[i].vstrt = ((pos & 0xFF00) >> 8) | ((ctl & 0b100) << 6);
        info.sprite[i].vstop = ((ctl & 0xFF00) >> 8) | ((ctl & 0b010) << 7);
        info.sprite[i].attach = GET_BIT(ctl, 7);

        // debug("%d: hstrt = %d vstsrt = %d vstop = %d\n", i, info.sprite[i].hstrt, info.sprite[i].vstrt, info.sprite[i].vstop);
    }

    pthread_mutex_unlock(&lock);
}

void
Denise::_dump()
{
    plainmsg(" CollisionCheck: %d\n", collisionCheck);
}

DeniseInfo
Denise::getInfo()
{
    DeniseInfo result;
    
    pthread_mutex_lock(&lock);
    result = info;
    pthread_mutex_unlock(&lock);
    
    return result;
}

SpriteInfo
Denise::getSprInfo(int nr)
{
    SpriteInfo result;
    
    pthread_mutex_lock(&lock);
    result = info.sprite[nr];
    pthread_mutex_unlock(&lock);
    
    return result;
}

uint16_t
Denise::peekJOY0DATR()
{
    uint16_t result = amiga->controlPort1.joydat();
    debug(2, "peekJOY0DATR() = $%04X (%d)\n", result, result);

    return result;
}

uint16_t
Denise::peekJOY1DATR()
{
    uint16_t result = amiga->controlPort2.joydat();
    debug(2, "peekJOY1DATR() = $%04X (%d)\n", result, result);

    return result;
}

void
Denise::pokeJOYTEST(uint16_t value)
{
    debug(2, "pokeJOYTEST(%04X)\n", value);

    amiga->controlPort1.pokeJOYTEST(value);
    amiga->controlPort2.pokeJOYTEST(value);
}

void
Denise::pokeBPLCON0(uint16_t value)
{
    debug(BPL_DEBUG, "pokeBPLCON0(%X)\n", value);

    if (bplcon0 != value) {

        pokeBPLCON0(bplcon0, value);
        bplcon0 = value;
    }
}

void
Denise::pokeBPLCON0(uint16_t oldValue, uint16_t newValue)
{
    // Record the register change
    conRegHistory.recordChange(BPLCON0, newValue, 4 * agnus->pos.h - 4);
}

void
Denise::pokeBPLCON1(uint16_t value)
{
    debug(BPL_DEBUG, "pokeBPLCON1(%X)\n", value);

    bplcon1 = value & 0xFF;

    // Compute scroll values
    scrollLoresOdd  = (bplcon1 & 0b00001111);
    scrollLoresEven = (bplcon1 & 0b11110000) >> 4;
    scrollHiresEven = (bplcon1 & 0b00000111) << 1;
    scrollHiresOdd  = (bplcon1 & 0b01110000) >> 3;
}

void
Denise::pokeBPLCON2(uint16_t value)
{
    debug(BPL_DEBUG, "pokeBPLCON2(%X)\n", value);

    bplcon2 = value;

    // Record the pixel coordinate where the change takes place
    conRegHistory.recordChange(BPLCON2, value, 4 * agnus->pos.h + 4);
}

template <int x> void
Denise::pokeBPLxDAT(uint16_t value)
{
    assert(x < 6);
    debug(BPL_DEBUG, "pokeBPL%dDATA(%X)\n", x + 1, value);
    
    bpldat[x] = value;
}

template <int x> void
Denise::pokeSPRxPOS(uint16_t value)
{
    assert(x < 8);
    debug(SPR_DEBUG, "pokeSPR%dPOS(%X)\n", x, value);

    // 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0  (Ex = VSTART)
    // E7 E6 E5 E4 E3 E2 E1 E0 H8 H7 H6 H5 H4 H3 H2 H1  (Hx = HSTART)

    // Note: Denise only picks up the horizontal coordinate. Only Agnus knows
    // about the vertical coordinate.

    sprhstrt[x] = ((value & 0xFF) << 1) | (sprhstrt[x] & 0x01);
    
    // Update debugger info
    if (agnus->pos.v == 26) {
        info.sprite[x].pos = value;
    }
}

template <int x> void
Denise::pokeSPRxCTL(uint16_t value)
{
    assert(x < 8);
    debug(SPR_DEBUG, "pokeSPR%dCTL(%X)\n", x, value);
    
    // 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
    // L7 L6 L5 L4 L3 L2 L1 L0 AT  -  -  -  - E8 L8 H0  (Lx = VSTOP)

    // Note: Denise only picks up the horizontal coordinate. Only Agnus knows
    // about the vertical coordinate.
    
    sprhstrt[x] = (sprhstrt[x] & 0x1FE) | (value & 0x01);
    WRITE_BIT(attach, x, GET_BIT(value, 7));
    
    // Update debugger info
    if (agnus->pos.v == 26) {
        info.sprite[x].ctl = value;
        info.sprite[x].ptr = agnus->sprpt[x];
        assert(IS_EVEN(info.sprite[x].ptr));
    }
}

template <int x> void
Denise::pokeSPRxDATA(uint16_t value)
{
    assert(x < 8);
    debug(SPR_DEBUG, "pokeSPR%dDATA(%X)\n", x, value);
    
    sprdata[x] = value;
    armSprite(x);
}

template <int x> void
Denise::pokeSPRxDATB(uint16_t value)
{
    assert(x < 8);
    debug(SPR_DEBUG, "pokeSPR%dDATB(%X)\n", x, value);
    
    sprdatb[x] = value;
}

void
Denise::pokeColorReg(uint32_t addr, uint16_t value)
{
    assert(addr >= 0x180 && addr <= 0x1BE && IS_EVEN(addr));
    debug(COL_DEBUG, "pokeColorReg(%X, %X)\n", addr, value);

    pixelEngine.colRegHistory.recordChange(addr, value, 4 * agnus->pos.h);
}

bool
Denise::attached(int x) {

    assert(x >= 1 && x <= 7);
    assert(IS_ODD(x));

    return GET_BIT(attach,x) && sprhstrt[x-1] == sprhstrt[x];
}

void
Denise::armSprite(int x)
{
    SET_BIT(armed, x);
}

void
Denise::updateSpritePriorities(uint16_t bplcon2)
{
    switch (bplcon2 & 0b111) {

        case 0: prio1 = Z_0; break;
        case 1: prio1 = Z_1; break;
        case 2: prio1 = Z_2; break;
        case 3: prio1 = Z_3; break;
        case 4: prio1 = Z_4; break;

        default:
            // TODO: Check the effect of illegal values
            assert(false);
            break;
    }

    switch ((bplcon2 >> 3) & 0b111) {

        case 0: prio2 = Z_0; break;
        case 1: prio2 = Z_1; break;
        case 2: prio2 = Z_2; break;
        case 3: prio2 = Z_3; break;
        case 4: prio2 = Z_4; break;

        default:
            // TODO: Check the effect of illegal values
            assert(false);
            break;
    }

    prio12 = MAX(prio1, prio2);
    // debug("bplcon2 = %X prio1 = %d prio2 = %d prio12 = %d\n", bplcon2, prio1, prio2, prio12);
}

template <int HIRES> void
Denise::draw(int pixels)
{
    uint8_t index;

    currentPixel = ppos(agnus->pos.h);

    uint32_t maskOdd = 0x8000 << scrollLoresOdd;
    uint32_t maskEven = 0x8000 << scrollLoresEven;

    for (int i = 0; i < pixels; i++) {

        // Read a bit slice
        index =
        (!!(shiftReg[0] & maskOdd)  << 0) |
        (!!(shiftReg[1] & maskEven) << 1) |
        (!!(shiftReg[2] & maskOdd)  << 2) |
        (!!(shiftReg[3] & maskEven) << 3) |
        (!!(shiftReg[4] & maskOdd)  << 4) |
        (!!(shiftReg[5] & maskEven) << 5);

        maskOdd >>= 1;
        maskEven >>= 1;

        if (HIRES) {

            // Synthesize one hires pixel
            assert(currentPixel < sizeof(bBuffer));
            bBuffer[currentPixel++] = index;

        } else {

            // Synthesize two lores pixels
            assert(currentPixel + 1 < sizeof(bBuffer));
            bBuffer[currentPixel++] = index;
            bBuffer[currentPixel++] = index;
        }
    }

    // Shift out drawn bits
    for (int i = 0; i < 6; i++) shiftReg[i] <<= pixels;

#ifdef PIXEL_DEBUG
    rasterline[currentPixel - 2 * pixels] = 64;
#endif
}

void
Denise::translate()
{
    int pixel = 0;

    uint16_t bplcon0 = initialBplcon0;
    bool dual = dbplf(bplcon0);

    uint16_t bplcon2 = initialBplcon2;
    updateSpritePriorities(bplcon2);

    // Add a dummy register change to ensure we draw until the line ends
    conRegHistory.recordChange(0, 0, sizeof(bBuffer));

    // Iterate over all recorded register changes
    for (int i = 0; i < conRegHistory.count; i++) {

        RegisterChange &change = conRegHistory.change[i];

        // Translate a chunk of bitplane data
        if (dual) {
            translateDPF(pixel, change.pixel);
        } else {
            translateSPF(pixel, change.pixel);
        }
        pixel = change.pixel;

        // Apply the register change
        switch (change.addr) {

            case BPLCON0:
                bplcon0 = change.value;
                dual = dbplf(bplcon0);
                break;

            case BPLCON2:
                bplcon2 = change.value;
                updateSpritePriorities(bplcon2);
                break;

            default:
                assert(change.addr == 0);
                break;
        }
    }

    // Clear the history cache
    conRegHistory.init();
}

void
Denise::translateSPF(int from, int to)
{
    for (int i = from; i < to; i++) {

        uint8_t s = bBuffer[i];
        bBuffer[i] = 0;

        assert(PixelEngine::isRgbaIndex(s));
        iBuffer[i] = s;
        zBuffer[i] = s ? prio2 : 0;
    }
}

void
Denise::translateDPF(int from, int to)
{
    if (PF2PRI()) {
        translateDPF<true>(from, to);
    } else {
        translateDPF<false>(from, to);
    }
}

template <bool pf2pri> void
Denise::translateDPF(int from, int to)
{
    for (int i = from; i < to; i++) {

        uint8_t s = bBuffer[i];
        bBuffer[i] = 0;

        // Determine color indices for both playfields
        uint8_t index1 = ((s & 1) >> 0) | ((s & 4) >> 1) | ((s & 16) >> 2);
        uint8_t index2 = ((s & 2) >> 1) | ((s & 8) >> 2) | ((s & 32) >> 3);

        if (pf2pri) {

            // Playfield 2 appears in front
            if (index2) {
                iBuffer[i] = index2 | 0b1000;
                zBuffer[i] = prio2 | Z_DPF;
            } else if (index1) {
                iBuffer[i] = index1;
                zBuffer[i] = prio1 | Z_DPF;
            } else {
                iBuffer[i] = 0;
                zBuffer[i] = Z_DPF;
            }

        } else {

            // Playfield 1 appears in front
            if (index1) {
                iBuffer[i] = index1;
                zBuffer[i] = prio1 | Z_DPF;
            } else if (index2) {
                iBuffer[i] = index2 | 0b1000;
                zBuffer[i] = prio2 | Z_DPF;
            } else {
                iBuffer[i] = 0;
                zBuffer[i] = Z_DPF;
            }
        }

        /*
        if (index1) {

            if (index2) { // Case 1: PF1 is solid, PF2 is solid

                if (pf2pri) {
                    colorIndex[i] = index2 | 0b1000;
                    zBuffer[i] = prio2;
                } else {
                    colorIndex[i] = index1;
                    zBuffer[i] = prio1;
                }
                zBuffer[i] |= (Z_DPF | Z_PF1 | Z_PF2);

            } else { // Case 2: PF1 is solid, PF2 is transparent

                colorIndex[i] = index1;
                zBuffer[i] = prio1 | (Z_DPF | Z_PF2);
            }

        } else {
            if (index2) { // Case 3: PF1 is transparent, PF2 is solid

                colorIndex[i] = index2 | 0b1000;
                zBuffer[i] = prio2 | (Z_DPF | Z_PF2);

            } else { // Case 4: PF1 is transparent, PF2 is transparent

                colorIndex[i] = 0;
                zBuffer[i] = Z_DPF;
            }
        }
        */
    }
}

void
Denise::drawSprites()
{
    // Only proceed if we are not inside the upper or lower border area
    if (!agnus->inBplDmaArea()) return;

    // Sprites 6 and 7
    if (armed & 0b11000000) {
        if (attached(7)) {
            drawSpritePair<7>();
        } else {
            if (armed & 0b10000000) drawSprite<7>();
            if (armed & 0b01000000) drawSprite<6>();
        }
    }

    // Sprites 4 and 5
    if (armed & 0b00110000) {
        if (attached(5)) {
            drawSpritePair<5>();
        } else {
            if (armed & 0b00100000) drawSprite<5>();
            if (armed & 0b00010000) drawSprite<4>();
        }
    }

    // Sprites 2 and 3
    if (armed & 0b00001100) {
        if (attached(3)) {
            drawSpritePair<3>();
        } else {
            if (armed & 0b00001000) drawSprite<3>();
            if (armed & 0b00000100) drawSprite<2>();
        }
    }

    // Sprites 1 and 0
    if (armed & 0b00000011) {
        if (attached(1)) {
            drawSpritePair<1>();
        } else {
            if (armed & 0b00000010) drawSprite<1>();
            if (armed & 0b00000001) drawSprite<0>();
        }
    }

    armed = 0;
}

template <int x> void
Denise::drawSprite()
{
    assert(x >= 0 && x <= 7);

    const uint16_t depth[8] = { Z_SP0, Z_SP1, Z_SP2, Z_SP3, Z_SP4, Z_SP5, Z_SP6, Z_SP7 };
    uint16_t z = depth[x];

    uint32_t d1 = (uint32_t)sprdata[x] << 1;
    uint32_t d0 = (uint32_t)sprdatb[x] << 0;

    int baseCol = 16 + 2 * (x & 6);
    // int16_t pos = 2 * sprhstrt[x] + 32;

    int start = 2 * sprhstrt[x];
    int end = MIN(start + 31, LAST_PIXEL);

    for (int pos = end; pos >= start; pos -= 2) {

        int col = (d1 & 0b0010) | (d0 & 0b0001);

        if (col) {
            if (z > zBuffer[pos]) {
                iBuffer[pos] = baseCol | col;
                zBuffer[pos] |= z;
            }
            if (z > zBuffer[pos-1]) {
                iBuffer[pos-1] = baseCol | col;
                zBuffer[pos-1] |= z;
            }
        }

        d1 >>= 1;
        d0 >>= 1;
    }

    /*
    for (int i = 15; i >= 0; i--) {

        int col = (d1 & 0b0010) | (d0 & 0b0001);

        if (col) {
            if (pos < LAST_PIXEL && z > zBuffer[pos]) {
                iBuffer[pos] = baseCol | col;
                zBuffer[pos] |= z;
            } 
        if (pos+1 < LAST_PIXEL && z > zBuffer[pos+1]) {
                iBuffer[pos+1] = baseCol | col;
                zBuffer[pos+1] |= z;
            }
        }

        d1 >>= 1;
        d0 >>= 1;
        pos -= 2;
    }
    */
}

void
Denise::checkSpriteCollisions(int start)
{

}

template <int x> void
Denise::drawSpritePair()
{
    assert(x >= 1 && x <= 7);
    assert(IS_ODD(x));

    const uint16_t depth[8] = { Z_SP0, Z_SP1, Z_SP2, Z_SP3, Z_SP4, Z_SP5, Z_SP6, Z_SP7 };
    uint16_t z = depth[x];

    uint32_t d3 = (uint32_t)sprdata[x]   << 3;
    uint32_t d2 = (uint32_t)sprdatb[x]   << 2;
    uint32_t d1 = (uint32_t)sprdata[x-1] << 1;
    uint32_t d0 = (uint32_t)sprdatb[x-1] << 0;

    int start = 2 * sprhstrt[x];
    int end = MIN(start + 31, LAST_PIXEL);

    for (int pos = end; pos >= start; pos -= 2) {

        int col = (d3 & 0b1000) | (d2 & 0b0100) | (d1 & 0b0010) | (d0 & 0b0001);

        if (col) {
            if (z > zBuffer[pos]) {
                iBuffer[pos] = 0b10000 | col;
                zBuffer[pos] |= z;
            }
            if (z > zBuffer[pos-1]) {
                iBuffer[pos-1] = 0b10000 | col;
                zBuffer[pos-1] |= z;
            }
        }

        d3 >>= 1;
        d2 >>= 1;
        d1 >>= 1;
        d0 >>= 1;
    }
    /*
    int16_t pos = 2 * sprhstrt[x] + 32;

    for (int i = 15; i >= 0; i--) {

        int col = (d3 & 0b1000) | (d2 & 0b0100) | (d1 & 0b0010) | (d0 & 0b0001);

        if (col) {
            if (pos < LAST_PIXEL && z > zBuffer[pos]) {
                iBuffer[pos] = 0b10000 | col;
                zBuffer[pos] |= z;
            }
            if (pos+1 < LAST_PIXEL && z > zBuffer[pos+1]) {
                iBuffer[pos+1] = 0b10000 | col;
                zBuffer[pos+1] |= z;
            }
        }

        d3 >>= 1;
        d2 >>= 1;
        d1 >>= 1;
        d0 >>= 1;
        pos -= 2;
    }
    */
}

void
Denise::drawBorder()
{
    int borderL = 0;
    int borderR = 0;
    int borderV = 0;

#ifdef BORDER_DEBUG
    borderL = 64;
    borderR = 65;
    borderV = 66;
#endif

    // Check if the horizontal flipflop was set somewhere in this rasterline
    bool hFlopWasSet = agnus->hFlop || agnus->hFlopOn != -1;

    // Check if the whole line is blank (drawn in background color)
    bool lineIsBlank = !agnus->vFlop || !hFlopWasSet;

    // Draw the border
    if (lineIsBlank) {

        for (int i = 0; i <= LAST_PIXEL; i++) {
           iBuffer[i] = borderV;
        }

    } else {

        // Draw left border
        if (!agnus->hFlop && agnus->hFlopOn != -1) {
            for (int i = 0; i < 2 * agnus->hFlopOn; i++) {
                assert(i < sizeof(iBuffer));
                iBuffer[i] = borderL;
            }
        }

        // Draw right border
        if (agnus->hFlopOff != -1) {
            for (int i = 2 * agnus->hFlopOff; i <= LAST_PIXEL; i++) {
                assert(i < sizeof(iBuffer));
                iBuffer[i] = borderR;
            }
        }
    }

#ifdef LINE_DEBUG
    int16_t vpos = agnus->pos.v;
    bool lines = vpos == 300 || vpos == 0xA0; // vpos == 26 || vpos == 0x50 || vpos == 276 || vpos == 255;
    if (lines) for (int i = 0; i <= LAST_PIXEL / 2; colorIndex[i++] = 64);
#endif
}

void
Denise::beginOfLine(int vpos)
{
    // Reset the horizontal pixel counter
    currentPixel = (agnus->dmaFirstBpl1Event * 4) + 6;

    // Save the current values of the bitplane control registers
    initialBplcon0 = bplcon0;
    initialBplcon1 = bplcon1;
    initialBplcon2 = bplcon2;
}

void
Denise::endOfLine(int vpos)
{
    // debug("endOfLine pixel = %d HPIXELS = %d\n", pixel, HPIXELS);

    // Make sure we're below the VBLANK area
    if (vpos >= 26) {

        // Translate bitplane data to color register indices
        translate();

        // Draw sprites if at least one is armed
        if (armed) drawSprites();

        // Draw border pixels
        drawBorder();

        // Synthesize RGBA values and write the result into the frame buffer
        pixelEngine.colorize(iBuffer, vpos);

        /* Note that Denise has already synthesized pixels that belong to the
         * next DMA line (i.e., the pixels that have been written into the
         * rasterline array with offset values > $E2). We move them to the
         * beginning of the rasterline array to make them appear when the next
         * line is drawn.
         */
        // TODO: DO WE STILL NEED THIS???
        for (int i = 4 * 0xE3; i < sizeof(iBuffer); i++) {
            iBuffer[i - 4 * 0xE3] = iBuffer[i];
            iBuffer[i] = 0;
        }
    }

    // Invoke the DMA debugger
    agnus->dmaDebugger.computeOverlay();
}

void
Denise::debugSetBPU(int count)
{
    if (count < 0) count = 0;
    if (count > 6) count = 6;

    amiga->suspend();
    
    uint16_t value = bplcon0 & 0b1000111111111111;
    pokeBPLCON0(value | (count << 12));
    
    amiga->resume();
}

void
Denise::debugSetBPLCONx(unsigned x, uint16_t value)
{
    assert(x <= 2);

    amiga->suspend();

    switch (x) {
        case 0:
            pokeBPLCON0(value);
            break;
        case 1:
            pokeBPLCON1(value);
            break;
        case 2:
            pokeBPLCON2(value);
            break;
    }

    amiga->resume();
}

void
Denise::debugSetBPLCONxBit(unsigned x, unsigned bit, bool value)
{
    assert(x <= 2);
    assert(bit <= 15);

    uint16_t mask = 1 << bit;

    amiga->suspend();

    switch (x) {
        case 0:
            pokeBPLCON0(value ? (bplcon0 | mask) : (bplcon0 & ~mask));
            break;
        case 1:
            pokeBPLCON1(value ? (bplcon1 | mask) : (bplcon1 & ~mask));
            break;
        case 2:
            pokeBPLCON2(value ? (bplcon2 | mask) : (bplcon2 & ~mask));
            break;
    }
    
    amiga->resume();
}

void
Denise::debugSetBPLCONxNibble(unsigned x, unsigned nibble, uint8_t value)
{
    assert(x <= 2);
    assert(nibble <= 4);

    uint16_t mask = 0b1111 << (4 * nibble);
    uint16_t bits = (value & 0b1111) << (4 * nibble);

    amiga->suspend();

    switch (x) {
        case 0:
            pokeBPLCON0((bplcon0 & ~mask) | bits);
            break;
        case 1:
            pokeBPLCON1((bplcon1 & ~mask) | bits);
            break;
        case 2:
            pokeBPLCON2((bplcon2 & ~mask) | bits);
            break;
    }

    amiga->resume();
}

template void Denise::pokeBPLxDAT<0>(uint16_t value);
template void Denise::pokeBPLxDAT<1>(uint16_t value);
template void Denise::pokeBPLxDAT<2>(uint16_t value);
template void Denise::pokeBPLxDAT<3>(uint16_t value);
template void Denise::pokeBPLxDAT<4>(uint16_t value);
template void Denise::pokeBPLxDAT<5>(uint16_t value);

template void Denise::pokeSPRxPOS<0>(uint16_t value);
template void Denise::pokeSPRxPOS<1>(uint16_t value);
template void Denise::pokeSPRxPOS<2>(uint16_t value);
template void Denise::pokeSPRxPOS<3>(uint16_t value);
template void Denise::pokeSPRxPOS<4>(uint16_t value);
template void Denise::pokeSPRxPOS<5>(uint16_t value);
template void Denise::pokeSPRxPOS<6>(uint16_t value);
template void Denise::pokeSPRxPOS<7>(uint16_t value);

template void Denise::pokeSPRxCTL<0>(uint16_t value);
template void Denise::pokeSPRxCTL<1>(uint16_t value);
template void Denise::pokeSPRxCTL<2>(uint16_t value);
template void Denise::pokeSPRxCTL<3>(uint16_t value);
template void Denise::pokeSPRxCTL<4>(uint16_t value);
template void Denise::pokeSPRxCTL<5>(uint16_t value);
template void Denise::pokeSPRxCTL<6>(uint16_t value);
template void Denise::pokeSPRxCTL<7>(uint16_t value);

template void Denise::pokeSPRxDATA<0>(uint16_t value);
template void Denise::pokeSPRxDATA<1>(uint16_t value);
template void Denise::pokeSPRxDATA<2>(uint16_t value);
template void Denise::pokeSPRxDATA<3>(uint16_t value);
template void Denise::pokeSPRxDATA<4>(uint16_t value);
template void Denise::pokeSPRxDATA<5>(uint16_t value);
template void Denise::pokeSPRxDATA<6>(uint16_t value);
template void Denise::pokeSPRxDATA<7>(uint16_t value);

template void Denise::pokeSPRxDATB<0>(uint16_t value);
template void Denise::pokeSPRxDATB<1>(uint16_t value);
template void Denise::pokeSPRxDATB<2>(uint16_t value);
template void Denise::pokeSPRxDATB<3>(uint16_t value);
template void Denise::pokeSPRxDATB<4>(uint16_t value);
template void Denise::pokeSPRxDATB<5>(uint16_t value);
template void Denise::pokeSPRxDATB<6>(uint16_t value);
template void Denise::pokeSPRxDATB<7>(uint16_t value);

template void Denise::draw<0>(int pixels);
template void Denise::draw<1>(int pixels);

template void Denise::translateDPF<true>(int from, int to);
template void Denise::translateDPF<false>(int from, int to);
