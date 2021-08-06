// -----------------------------------------------------------------------------
// This file is part of vAmiga
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#pragma once

#import "Constants.h"
#import "AgnusTypes.h"
#import "AmigaTypes.h"
#import "AmigaFileTypes.h"
#import "BlitterTypes.h"
#import "BootBlockImageTypes.h"
#import "ControlPortTypes.h"
#import "CopperTypes.h"
#import "CPUTypes.h"
#import "CIATypes.h"
#import "DeniseTypes.h"
#import "DiskTypes.h"
#import "DiskControllerTypes.h"
#import "DmaDebuggerTypes.h"
#import "DriveTypes.h"
#import "ErrorTypes.h"
#import "EventTypes.h"
#import "FSTypes.h"
#import "HardwareComponentTypes.h"
#import "JoystickTypes.h"
#import "KeyboardTypes.h"
#import "MemoryTypes.h"
#import "MsgQueueTypes.h"
#import "MuxerTypes.h"
#import "MouseTypes.h"
#import "PaulaTypes.h"
#import "PixelEngineTypes.h"
#import "RomFileTypes.h"
#import "RTCTypes.h"
#import "SerialPortTypes.h"
#import "StateMachineTypes.h"
#import "UARTTypes.h"

#import <Cocoa/Cocoa.h>
#import <MetalKit/MetalKit.h>

@interface ExceptionWrapper : NSObject {
    
    ErrorCode errorCode;
    NSString *what;
}

@property ErrorCode errorCode;
@property NSString *what;

@end

//
// Forward declarations
//

@class ADFFileProxy;
@class AgnusProxy;
@class AmigaFileProxy;
@class BlitterProxy;
@class CIAProxy;
@class ControlPortProxy;
@class CopperProxy;
@class CPUProxy;
@class DeniseProxy;
@class DiskControllerProxy;
@class DiskFileProxy;
@class DMSFileProxy;
@class DmaDebuggerProxy;
@class DriveProxy;
@class EXEFileProxy;
@class ExtendedRomFileProxy;
@class FolderProxy;
@class GuardsProxy;
@class HDFFileProxy;
@class IMGFileProxy;
@class JoystickProxy;
@class KeyboardProxy;
@class MemProxy;
@class MouseProxy;
@class PaulaProxy;
@class RetroShellProxy;
@class RomFileProxy;
@class RtcProxy;
@class RecorderProxy;
@class ScriptProxy;
@class SerialPortProxy;
@class SnapshotProxy;


//
// Base proxies
//

@interface Proxy : NSObject {
    
    // Reference to the wrapped C++ object
    @public void *obj;    
}

@end

@interface HardwareComponentProxy : Proxy { }

- (void)dump;
- (void)dumpConfig;

@end

//
// Amiga proxy
//

@interface AmigaProxy : HardwareComponentProxy {
        
    AgnusProxy *agnus;
    GuardsProxy *breakpoints;
    CIAProxy *ciaA;
    CIAProxy *ciaB;
    ControlPortProxy *controlPort1;
    ControlPortProxy *controlPort2;
    CopperProxy *copper;
    CPUProxy *cpu;
    DeniseProxy *denise;
    DiskControllerProxy *diskController;
    DmaDebuggerProxy *dmaDebugger;
    DriveProxy *df0;
    DriveProxy *df1;
    DriveProxy *df2;
    DriveProxy *df3;
    KeyboardProxy *keyboard;
    MemProxy *mem;
    PaulaProxy *paula;
    RetroShellProxy *retroShell;
    RtcProxy *rtc;
    RecorderProxy *recorder;
    SerialPortProxy *serialPort;
    GuardsProxy *watchpoints;
}

@property (readonly, strong) AgnusProxy *agnus;
@property (readonly, strong) BlitterProxy *blitter;
@property (readonly, strong) CIAProxy *ciaA;
@property (readonly, strong) CIAProxy *ciaB;
@property (readonly, strong) ControlPortProxy *controlPort1;
@property (readonly, strong) ControlPortProxy *controlPort2;
@property (readonly, strong) CopperProxy *copper;
@property (readonly, strong) CPUProxy *cpu;
@property (readonly, strong) DeniseProxy *denise;
@property (readonly, strong) DiskControllerProxy *diskController;
@property (readonly, strong) DmaDebuggerProxy *dmaDebugger;
@property (readonly, strong) DriveProxy *df0;
@property (readonly, strong) DriveProxy *df1;
@property (readonly, strong) DriveProxy *df2;
@property (readonly, strong) DriveProxy *df3;
@property (readonly, strong) GuardsProxy *breakpoints;
@property (readonly, strong) GuardsProxy *watchpoints;
@property (readonly, strong) KeyboardProxy *keyboard;
@property (readonly, strong) MemProxy *mem;
@property (readonly, strong) PaulaProxy *paula;
@property (readonly, strong) RetroShellProxy *retroShell;
@property (readonly, strong) RtcProxy *rtc;
@property (readonly, strong) RecorderProxy *recorder;
@property (readonly, strong) SerialPortProxy *serialPort;

- (void)dealloc;
- (void)kill;

@property (readonly) BOOL isReleaseBuild;
@property BOOL warp;
@property BOOL debugMode;
@property EventID inspectionTarget;
- (void) removeInspectionTarget;
- (BOOL)isReady:(ErrorCode *)ec;
- (BOOL)isReady;
- (void)powerOn:(ErrorCode *)ec;
- (void)powerOff;
- (void)hardReset;
- (void)softReset;
- (void)shutdown;

- (AmigaInfo) getInfo;

@property (readonly) BOOL poweredOn;
@property (readonly) BOOL poweredOff;
@property (readonly) BOOL running;
@property (readonly) BOOL paused;

- (void)run:(ExceptionWrapper *)exc;
- (void)pause;
- (void)suspend;
- (void)resume;
- (void)continueScript;

- (void)requestAutoSnapshot;
- (void)requestUserSnapshot;
@property (readonly) SnapshotProxy *latestAutoSnapshot;
@property (readonly) SnapshotProxy *latestUserSnapshot;
- (void) loadFromSnapshot:(SnapshotProxy *)proxy;

- (NSInteger)getConfig:(Option)opt;
- (NSInteger)getConfig:(Option)opt id:(NSInteger)id;
- (NSInteger)getConfig:(Option)opt drive:(NSInteger)id;
- (BOOL)configure:(Option)opt value:(NSInteger)val;
- (BOOL)configure:(Option)opt enable:(BOOL)val;
- (BOOL)configure:(Option)opt id:(NSInteger)id value:(NSInteger)val;
- (BOOL)configure:(Option)opt id:(NSInteger)id enable:(BOOL)val;
- (BOOL)configure:(Option)opt drive:(NSInteger)id value:(NSInteger)val;
- (BOOL)configure:(Option)opt drive:(NSInteger)id enable:(BOOL)val;

- (void)setListener:(const void *)sender function:(Callback *)func;

- (void)stopAndGo;
- (void)stepInto;
- (void)stepOver;

@end


//
// Guards proxy (Breakpoints, Watchpoints)
//

@interface GuardsProxy : Proxy { }
    
@property (readonly) NSInteger count;
- (NSInteger)addr:(NSInteger)nr;
- (BOOL)isEnabled:(NSInteger)nr;
- (BOOL)isDisabled:(NSInteger)nr;
- (void)enable:(NSInteger)nr;
- (void)disable:(NSInteger)nr;
- (void)remove:(NSInteger)nr;
- (void)replace:(NSInteger)nr addr:(NSInteger)addr;

- (BOOL)isSetAt:(NSInteger)addr;
- (BOOL)isSetAndEnabledAt:(NSInteger)addr;
- (BOOL)isSetAndDisabledAt:(NSInteger)addr;
- (void)enableAt:(NSInteger)addr;
- (void)disableAt:(NSInteger)addr;
- (void)addAt:(NSInteger)addr;
- (void)removeAt:(NSInteger)addr;

@end


//
// CPU proxy
//

@interface CPUProxy : HardwareComponentProxy { }
    
- (CPUInfo) getInfo;

@property (readonly) i64 clock;
@property (readonly) i64 cycles;
@property (readonly, getter=isHalted) bool halted;

@property (readonly) NSInteger loggedInstructions;
- (void)clearLog;

- (NSString *)disassembleRecordedInstr:(NSInteger)i length:(NSInteger *)len;
- (NSString *)disassembleRecordedBytes:(NSInteger)i length:(NSInteger)len;
- (NSString *)disassembleRecordedFlags:(NSInteger)i;
- (NSString *)disassembleRecordedPC:(NSInteger)i;

- (NSString *)disassembleInstr:(NSInteger)addr length:(NSInteger *)len;
- (NSString *)disassembleWords:(NSInteger)addr length:(NSInteger)len;
- (NSString *)disassembleAddr:(NSInteger)addr;

@end


//
// CIA proxy
//

@interface CIAProxy : HardwareComponentProxy { }
    
- (CIAInfo) getInfo;

@end


//
// Memory proxy
//

@interface MemProxy : HardwareComponentProxy { }

@property (readonly) MemoryConfig config;
- (MemoryStats) getStats;

- (BOOL) isBootRom:(RomIdentifier)rev;
- (BOOL) isArosRom:(RomIdentifier)rev;
- (BOOL) isDiagRom:(RomIdentifier)rev;
- (BOOL) isCommodoreRom:(RomIdentifier)rev;
- (BOOL) isHyperionRom:(RomIdentifier)rev;

@property (readonly) BOOL hasRom;
@property (readonly) BOOL hasBootRom;
@property (readonly) BOOL hasKickRom;
- (void)deleteRom;
- (BOOL)isRom:(NSURL *)url;
- (void)loadRom:(RomFileProxy *)proxy exception:(ExceptionWrapper *)exc;
- (void)loadRomFromBuffer:(NSData *)buffer exception:(ExceptionWrapper *)exc;
- (void)loadRomFromFile:(NSURL *)url exception:(ExceptionWrapper *)exc;
@property (readonly) u64 romFingerprint;
@property (readonly) RomIdentifier romIdentifier;
@property (readonly, copy) NSString *romTitle;
@property (readonly, copy) NSString *romVersion;
@property (readonly, copy) NSString *romReleased;

- (BOOL)hasExt;
- (void)deleteExt;
- (BOOL)isExt:(NSURL *)url;
- (void)loadExt:(ExtendedRomFileProxy *)proxy exception:(ExceptionWrapper *)exc;
- (void)loadExtFromBuffer:(NSData *)buffer exception:(ExceptionWrapper *)exc;
- (void)loadExtFromFile:(NSURL *)url exception:(ExceptionWrapper *)exc;
@property (readonly) u64 extFingerprint;
@property (readonly) RomIdentifier extIdentifier;
@property (readonly, copy) NSString *extTitle;
@property (readonly, copy) NSString *extVersion;
@property (readonly, copy) NSString *extReleased;
@property (readonly) NSInteger extStart;

/*
- (void)saveRom:(NSURL *)url error:(ErrorCode *)ec;
- (void)saveWom:(NSURL *)url error:(ErrorCode *)ec;
- (void)saveExt:(NSURL *)url error:(ErrorCode *)ec;
 */
- (void)saveRom:(NSURL *)url exception:(ExceptionWrapper *)exc;
- (void)saveWom:(NSURL *)url exception:(ExceptionWrapper *)exc;
- (void)saveExt:(NSURL *)url exception:(ExceptionWrapper *)exc;

- (MemorySource)memSrc:(Accessor)accessor addr:(NSInteger)addr;
- (NSInteger)spypeek16:(Accessor)accessor addr:(NSInteger)addr;

- (NSString *)ascii:(Accessor)accessor addr:(NSInteger)addr;
- (NSString *)hex:(Accessor)accessor addr:(NSInteger)addr bytes:(NSInteger)bytes;

@end


//
// Agnus
//

@interface AgnusProxy : HardwareComponentProxy { }

@property (readonly) NSInteger chipRamLimit;

- (AgnusInfo)getInfo;
- (EventSlotInfo)getEventSlotInfo:(NSInteger)slot;
- (EventInfo)getEventInfo;
- (AgnusStats)getStats;

@end


//
// Copper
//

@interface CopperProxy : HardwareComponentProxy { }

- (CopperInfo)getInfo;

- (BOOL)isIllegalInstr:(NSInteger)addr;
- (NSString *)disassemble:(NSInteger)addr;
- (NSString *)disassemble:(NSInteger)list instr:(NSInteger)offset;

@end


//
// Blitter
//

@interface BlitterProxy : HardwareComponentProxy { }

- (BlitterInfo)getInfo;

@end


//
// DMA Debugger
//

@interface DmaDebuggerProxy : Proxy { }

- (DMADebuggerInfo)getInfo;

@end

//
// Denise
//

@interface DeniseProxy : HardwareComponentProxy { }

- (DeniseInfo)getInfo;
- (SpriteInfo)getSpriteInfo:(NSInteger)nr;

- (NSInteger)sprDataLines:(NSInteger)nr;
- (u64)sprData:(NSInteger)nr line:(NSInteger)line;
- (u16)sprColor:(NSInteger)nr reg:(NSInteger)reg;

@property (readonly) ScreenBuffer stableBuffer;
@property (readonly) u32 *noise;

@end


//
// Recorder proxy
//

@interface RecorderProxy : Proxy { }

@property (readonly) BOOL hasFFmpeg;
@property (readonly) BOOL recording;
@property (readonly) double duration;
@property (readonly) NSInteger frameRate;
@property (readonly) NSInteger bitRate;
@property (readonly) NSInteger sampleRate;

- (BOOL)startRecording:(NSRect)rect
               bitRate:(NSInteger)rate
               aspectX:(NSInteger)aspectX
               aspectY:(NSInteger)aspectY;
- (void)stopRecording;
- (BOOL)exportAs:(NSString *)path;

@end


//
// Paula proxy
//

@interface PaulaProxy : HardwareComponentProxy { }

- (PaulaInfo)getInfo;
- (AudioInfo)getAudioInfo;
- (MuxerStats)getMuxerStats;
- (UARTInfo)getUARTInfo;

- (double)sampleRate;
- (void)setSampleRate:(double)rate;

- (void)readMonoSamples:(float *)target size:(NSInteger)n;
- (void)readStereoSamples:(float *)target1 buffer2:(float *)target2 size:(NSInteger)n;

- (void)rampUp;
- (void)rampUpFromZero;
- (void)rampDown;

- (float)drawWaveformL:(u32 *)buffer w:(NSInteger)w h:(NSInteger)h scale:(float)s color:(u32)c;
- (float)drawWaveformL:(u32 *)buffer size:(NSSize)size scale:(float)s color:(u32)c;
- (float)drawWaveformR:(u32 *)buffer w:(NSInteger)w h:(NSInteger)h scale:(float)s color:(u32)c;
- (float)drawWaveformR:(u32 *)buffer size:(NSSize)size scale:(float)s color:(u32)c;

@end


//
// Rtc proxy
//

@interface RtcProxy : HardwareComponentProxy { }

- (void)update;

@end


//
// ControlPort proxy
//

@interface ControlPortProxy : HardwareComponentProxy {
        
    MouseProxy *mouse;
    JoystickProxy *joystick;
}

@property (readonly, strong) MouseProxy *mouse;
@property (readonly, strong) JoystickProxy *joystick;

- (ControlPortInfo)getInfo;

@end


//
// SerialPort
//

@interface SerialPortProxy : HardwareComponentProxy { }

- (SerialPortInfo)getInfo;

@end


//
// Mouse proxy
//

@interface MouseProxy : HardwareComponentProxy { }

- (BOOL)detectShakeAbs:(NSPoint)pos;
- (BOOL)detectShakeRel:(NSPoint)pos;
- (void)setXY:(NSPoint)pos;
- (void)setDxDy:(NSPoint)pos;
- (void)trigger:(GamePadAction)event;

@end


//
// Joystick proxy
//

@interface JoystickProxy : HardwareComponentProxy { }

- (void)trigger:(GamePadAction)event;

@end


//
// Keyboard proxy
//

@interface KeyboardProxy : HardwareComponentProxy { }

- (BOOL)keyIsPressed:(NSInteger)keycode;
- (void)pressKey:(NSInteger)keycode;
- (void)releaseKey:(NSInteger)keycode;
- (void)releaseAllKeys;

@end


//
// DiskController
//

@interface DiskControllerProxy : HardwareComponentProxy { }

- (DiskControllerConfig)getConfig;
- (DiskControllerInfo)getInfo;
@property (readonly) NSInteger selectedDrive;
@property (readonly) DriveState state;
@property (readonly, getter=isSpinning) BOOL spinning;
- (void)eject:(NSInteger)nr;
- (void)insert:(NSInteger)nr file:(DiskFileProxy *)fileProxy;
 - (void)setWriteProtection:(NSInteger)nr value:(BOOL)value;

@end


//
// Drive
//

@interface DriveProxy : HardwareComponentProxy { }

- (DriveInfo) getInfo;

@property (readonly) NSInteger nr;
@property (readonly) BOOL hasDisk;
@property (readonly) BOOL hasDDDisk;
@property (readonly) BOOL hasHDDisk;
- (BOOL)hasWriteProtectedDisk;
- (void)setWriteProtection:(BOOL)value;
- (void)toggleWriteProtection;
- (BOOL)isInsertable:(DiskDiameter)type density:(DiskDensity)density;
@property (getter=isModifiedDisk) BOOL modifiedDisk;
@property (readonly) BOOL motor;
@property (readonly) NSInteger cylinder;
@property (readonly) u64 fnv;

@end


//
// FSDevice
//

@interface FSDeviceProxy : Proxy { }

+ (instancetype)makeWithADF:(ADFFileProxy *)adf;
+ (instancetype)makeWithHDF:(HDFFileProxy *)hdf;

@property (readonly) FSVolumeType dos;
@property (readonly) NSInteger numCyls;
@property (readonly) NSInteger numHeads;
@property (readonly) NSInteger numTracks;
@property (readonly) NSInteger numSectors;
@property (readonly) NSInteger numBlocks;

- (void)killVirus;
- (FSBlockType)blockType:(NSInteger)blockNr;
- (FSItemType)itemType:(NSInteger)blockNr pos:(NSInteger)pos;
- (FSErrorReport)check:(BOOL)strict;
- (ErrorCode)check:(NSInteger)nr pos:(NSInteger)pos expected:(unsigned char *)exp strict:(BOOL)strict;
- (BOOL)isCorrupted:(NSInteger)blockNr;
- (NSInteger)getCorrupted:(NSInteger)blockNr;
- (NSInteger)nextCorrupted:(NSInteger)blockNr;
- (NSInteger)prevCorrupted:(NSInteger)blockNr;
- (void)printDirectory:(BOOL) recursive;

- (NSInteger)readByte:(NSInteger)block offset:(NSInteger)offset;
- (ErrorCode)export:(NSString *)path;

@end


//
// RetroShell proxy
//

@interface RetroShellProxy : Proxy { }

@property (readonly) NSInteger cposRel;

- (NSString *)getText;
- (void)pressUp;
- (void)pressDown;
- (void)pressLeft;
- (void)pressRight;
- (void)pressHome;
- (void)pressEnd;
- (void)pressBackspace;
- (void)pressDelete;
- (void)pressReturn;
- (void)pressTab;
- (void)pressKey:(char)c;

@end


//
// F I L E   T Y P E   P R O X I E S
//

@protocol MakeWithFile
+ (instancetype)makeWithFile:(NSString *)path exception:(ExceptionWrapper *)exc;
@end

@protocol MakeWithBuffer
+ (instancetype)makeWithBuffer:(const void *)buf length:(NSInteger)len exception:(ExceptionWrapper *)exc;
@end

@protocol MakeWithDrive
+ (instancetype)makeWithDrive:(DriveProxy *)proxy exception:(ExceptionWrapper *)exc;
@end

@protocol MakeWithFileSystem
+ (instancetype)makeWithFileSystem:(FSDeviceProxy *)proxy exception:(ExceptionWrapper *)exc;
@end

//
// AmigaFile
//

@interface AmigaFileProxy : Proxy { }

+ (FileType) urlType:(NSURL *)url;
@property (readonly) FileType type;
- (void)setPath:(NSString *)path;
- (NSInteger)writeToFile:(NSString *)path error:(ErrorCode *)err;
@property (readonly) u64 fnv;

@end

//
// Snapshot proxy
//

@interface SnapshotProxy : AmigaFileProxy <MakeWithFile, MakeWithBuffer> {
    
    NSImage *preview;
}

+ (instancetype)makeWithFile:(NSString *)path error:(ErrorCode *)ec;
+ (instancetype)makeWithFile:(NSString *)path exception:(ExceptionWrapper *)ex;
+ (instancetype)makeWithBuffer:(const void *)buf length:(NSInteger)len error:(ErrorCode *)ec;
+ (instancetype)makeWithAmiga:(AmigaProxy *)proxy;

@property (readonly, strong) NSImage *previewImage;
@property (readonly) time_t timeStamp;

@end


//
// Script proxy
//

@interface ScriptProxy : AmigaFileProxy <MakeWithFile, MakeWithBuffer> { }

+ (instancetype)makeWithFile:(NSString *)path error:(ErrorCode *)err;
+ (instancetype)makeWithBuffer:(const void *)buf length:(NSInteger)len error:(ErrorCode *)err;
 
- (void)execute:(AmigaProxy *)proxy;

@end


//
// RomFile proxy
//

@interface RomFileProxy : AmigaFileProxy <MakeWithFile, MakeWithBuffer> { }

+ (instancetype)makeWithFile:(NSString *)path error:(ErrorCode *)err;
+ (instancetype)makeWithBuffer:(const void *)buf length:(NSInteger)len error:(ErrorCode *)err;
 
@end


//
// ExtendedRomFile proxy
//

@interface ExtendedRomFileProxy : AmigaFileProxy <MakeWithFile, MakeWithBuffer> { }

+ (instancetype)makeWithFile:(NSString *)path error:(ErrorCode *)err;
+ (instancetype)makeWithBuffer:(const void *)buf length:(NSInteger)len error:(ErrorCode *)err;
 
@end


//
// DiskFileProxy
//

@interface DiskFileProxy : AmigaFileProxy {
}

@property (readonly) FSVolumeType dos;
@property (readonly) DiskDiameter diskType;
@property (readonly) DiskDensity diskDensity;
@property (readonly) NSInteger numCyls;
@property (readonly) NSInteger numSides;
@property (readonly) NSInteger numTracks;
@property (readonly) NSInteger numSectors;
@property (readonly) NSInteger numBlocks;

@property (readonly) BootBlockType bootBlockType;
@property (readonly) NSString *bootBlockName;
@property (readonly) BOOL hasVirus;

- (void)killVirus;

- (NSInteger)readByte:(NSInteger)block offset:(NSInteger)offset;
- (void)readSector:(unsigned char *)dst block:(NSInteger)block;
- (void)readSectorHex:(char *)dst block:(NSInteger)block count:(NSInteger)count;

@end


//
// ADFFileProxy
//

@interface ADFFileProxy : DiskFileProxy <MakeWithFile, MakeWithBuffer, MakeWithDrive> {
}

+ (instancetype)makeWithBuffer:(const void *)buf length:(NSInteger)len exception:(ExceptionWrapper *)exc;
+ (instancetype)makeWithFile:(NSString *)path exception:(ExceptionWrapper *)exc;
+ (instancetype)makeWithDiameter:(DiskDiameter)type density:(DiskDensity)density;
+ (instancetype)makeWithDrive:(DriveProxy *)drive exception:(ExceptionWrapper *)exc;

- (void)formatDisk:(FSVolumeType)fs bootBlock:(NSInteger)bootBlockID;

@end


//
// HDFFile proxy
//

@interface HDFFileProxy : AmigaFileProxy <MakeWithFile, MakeWithBuffer> {
}

+ (instancetype)makeWithBuffer:(const void *)buf length:(NSInteger)len exception:(ExceptionWrapper *)exc;
+ (instancetype)makeWithFile:(NSString *)path exception:(ExceptionWrapper *)exc;

@property (readonly) NSInteger numBlocks;

@end


//
// EXTFileProxy
//

@interface EXTFileProxy : DiskFileProxy <MakeWithFile, MakeWithBuffer> {
}

+ (instancetype)makeWithBuffer:(const void *)buf length:(NSInteger)len exception:(ExceptionWrapper *)exc;
+ (instancetype)makeWithFile:(NSString *)path exception:(ExceptionWrapper *)exc;

@end


//
// IMGFileProxy
//

@interface IMGFileProxy : DiskFileProxy <MakeWithFile, MakeWithBuffer, MakeWithDrive> {
}

+ (instancetype)makeWithBuffer:(const void *)buf length:(NSInteger)len exception:(ExceptionWrapper *)exc;
+ (instancetype)makeWithFile:(NSString *)path exception:(ExceptionWrapper *)exc;
+ (instancetype)makeWithDrive:(DriveProxy *)proxy exception:(ExceptionWrapper *)exc;

@end


//
// DMSFileProxy
//

@interface DMSFileProxy : DiskFileProxy <MakeWithFile, MakeWithBuffer> {
}

+ (instancetype)makeWithBuffer:(const void *)buf length:(NSInteger)len exception:(ExceptionWrapper *)exc;
+ (instancetype)makeWithFile:(NSString *)path exception:(ExceptionWrapper *)exc;

@property (readonly) ADFFileProxy *adf;

@end


//
// EXEFileProxy
//

@interface EXEFileProxy : DiskFileProxy <MakeWithFile, MakeWithBuffer> {
}

+ (instancetype)makeWithBuffer:(const void *)buf length:(NSInteger)len exception:(ExceptionWrapper *)exc;
+ (instancetype)makeWithFile:(NSString *)path exception:(ExceptionWrapper *)exc;

@property (readonly) ADFFileProxy *adf;

@end


//
// Folder proxy
//

@interface FolderProxy : DiskFileProxy <MakeWithFile> {
}

+ (instancetype)makeWithFile:(NSString *)path exception:(ExceptionWrapper *)exc;

@property (readonly) ADFFileProxy *adf;

@end
