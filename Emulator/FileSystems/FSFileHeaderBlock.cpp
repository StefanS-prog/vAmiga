// -----------------------------------------------------------------------------
// This file is part of vAmiga
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#include "FSVolume.h"

FSFileHeaderBlock::FSFileHeaderBlock(FSVolume &ref, u32 nr) : FSBlock(ref, nr)
{
    data = new u8[ref.bsize]();
   
    // Setup constant values
    
    set32(0, 2);                   // Type
    set32(1, nr);                  // Block pointer to itself
    setCreationDate(time(NULL));   // Creation date
    set32(-1, (u32)-3);            // Sub type
}

FSFileHeaderBlock::FSFileHeaderBlock(FSVolume &ref, u32 nr, const char *name) :
FSFileHeaderBlock(ref, nr)
{
    setName(FSName(name));
}

FSItemType
FSFileHeaderBlock::itemType(u32 byte)
{
    // Intercept some special locations
    if (byte == 328) return FSI_BCPL_STRING_LENGTH;
    if (byte == 432) return FSI_BCPL_STRING_LENGTH;

    // Translate the byte index to a (signed) long word index
    i32 word = byte / 4; if (word >= 6) word -= volume.bsize / 4;

    switch (word) {
        case 0:   return FSI_TYPE_ID;
        case 1:   return FSI_SELF_REF;
        case 2:   return FSI_DATA_BLOCK_REF_COUNT;
        case 3:   return FSI_UNUSED;
        case 4:   return FSI_FIRST_DATA_BLOCK_REF;
        case 5:   return FSI_CHECKSUM;
        case -50:
        case -49: return FSI_UNUSED;
        case -48: return FSI_PROT_BITS;
        case -47: return FSI_FILESIZE;
        case -23: return FSI_CREATED_DAY;
        case -22: return FSI_CREATED_MIN;
        case -21: return FSI_CREATED_TICKS;
        case -4:  return FSI_NEXT_HASH_REF;
        case -3:  return FSI_PARENT_DIR_REF;
        case -2:  return FSI_EXT_BLOCK_REF;
        case -1:  return FSI_SUBTYPE_ID;
    }
    
    if (word <= -51)                return FSI_DATA_BLOCK_REF;
    if (word >= -46 && word <= -24) return FSI_BCPL_COMMENT;
    if (word >= -20 && word <= -5)  return FSI_BCPL_FILE_NAME;

    assert(false);
    return FSI_UNKNOWN;
}

FSError
FSFileHeaderBlock::check(u32 byte, u8 *expected, bool strict)
{
    /* Note: At locations -4 and -3, many disks reference the bitmap block
     * which is wrong. We ignore to report this common inconsistency if
     * 'strict' is set to false.
     */

    // Translate the byte index to a (signed) long word index
    i32 word = byte / 4; if (word >= 6) word -= volume.bsize / 4;
    u32 value = get32(word);
    
    switch (word) {
        case   0: EXPECT_LONGWORD(2);                    break;
        case   1: EXPECT_SELFREF;                        break;
        case   3: EXPECT_BYTE(0);                        break;
        case   4: EXPECT_DATABLOCK_REF;                  break;
        case   5: EXPECT_CHECKSUM;                       break;
        case -50: EXPECT_BYTE(0);                        break;
        case  -4: if (strict) EXPECT_OPTIONAL_HASH_REF;  break;
        case  -3: if (strict) EXPECT_PARENT_DIR_REF;     break;
        case  -2: EXPECT_OPTIONAL_FILELIST_REF;          break;
        case  -1: EXPECT_LONGWORD(-3);                   break;
    }
        
    // Data block reference area
    if (word <= -51 && value) EXPECT_DATABLOCK_REF;
    if (word == -51) {
        if (value == 0 && getNumDataBlockRefs() > 0) {
            return FS_EXPECTED_REF;
        }
        if (value != 0 && getNumDataBlockRefs() == 0) {
            return FS_EXPECTED_NO_REF;
        }
    }
    
    return FS_OK;
}

void
FSFileHeaderBlock::dump()
{
    printf("           Name : %s\n", getName().c_str());
    printf("        Comment : %s\n", getComment().c_str());
    printf("        Created : ");    getCreationDate().print(); printf("\n");
    printf("           Next : %d\n", getNextHashRef());
    printf("      File size : %d\n", getFileSize());

    printf("    Block count : %d / %d\n", getNumDataBlockRefs(), getMaxDataBlockRefs());
    printf("          First : %d\n", getFirstDataBlockRef());
    printf("     Parent dir : %d\n", getParentDirRef());
    printf(" FileList block : %d\n", getNextListBlockRef());
    
    printf("    Data blocks : ");
    for (u32 i = 0; i < getNumDataBlockRefs(); i++) printf("%d ", getDataBlockRef(i));
    printf("\n");
}

FSError
FSFileHeaderBlock::exportBlock(const char *exportDir)
{
    string path = exportDir;
    path += "/" + volume.getPath(this);

    printf("Creating file %s\n", path.c_str());
    
    FILE *file = fopen(path.c_str(), "w");
    if (file == nullptr) return FS_CANNOT_CREATE_FILE;
    
    writeData(file);
    fclose(file);
        
    return FS_OK;
}

size_t
FSFileHeaderBlock::writeData(FILE *file)
{
    long bytesRemaining = getFileSize();
    long bytesTotal = 0;
    long blocksTotal = 0;

    // Start here and iterate through all connected file list blocks
    FSBlock *block = this;

    while (block && blocksTotal < volume.capacity) {

        blocksTotal++;

        // Iterate through all data blocks references in this block
        u32 num = MIN(block->getNumDataBlockRefs(), block->getMaxDataBlockRefs());        
        for (u32 i = 0; i < num; i++) {
            
            u32 ref = getDataBlockRef(i);
            if (FSDataBlock *dataBlock = volume.dataBlock(getDataBlockRef(i))) {

                long bytesWritten = dataBlock->writeData(file, bytesRemaining);
                bytesTotal += bytesWritten;
                bytesRemaining -= bytesWritten;
                
            } else {
                
                printf("Ignoring block %d (no data block)\n", ref);
            }
        }
        
        // Continue with the next list block
        block = block->getNextListBlock();
    }
    
    if (bytesRemaining != 0) {
        printf("%ld remaining bytes. Expected 0.\n", bytesRemaining);
    }
    
    return bytesTotal;
}

size_t
FSFileHeaderBlock::addData(const u8 *buffer, size_t size)
{
    assert(getFileSize() == 0);
    
    // Compute the required number of DataBlocks
    u32 bytes = volume.getDataBlockCapacity();
    u32 numDataBlocks = (size + bytes - 1) / bytes;

    // Compute the required number of FileListBlocks
    u32 numDataListBlocks = 0;
    u32 max = getMaxDataBlockRefs();
    if (numDataBlocks > max) {
        numDataListBlocks = 1 + (numDataBlocks - max) / max;
    }

    printf("Required data blocks : %d\n", numDataBlocks);
    printf("Required list blocks : %d\n", numDataListBlocks);
    printf("         Free blocks : %d out of %d\n", volume.freeBlocks(), volume.numBlocks());
    
    if (volume.freeBlocks() < numDataBlocks + numDataListBlocks) {
        printf("Not enough free blocks\n");
        return 0;
    }
    
    for (u32 ref = nr, i = 0; i < numDataListBlocks; i++) {

        // Add a new file list block
        ref = volume.addFileListBlock(nr, ref);
    }
    
    for (u32 ref = nr, i = 1; i <= numDataBlocks; i++) {

        // Add a new data block
        ref = volume.addDataBlock(i, nr, ref);

        // Add references to the new data block
        addDataBlockRef(ref);
        
        // Add data
        FSBlock *block = volume.block(ref);
        if (block) {
            size_t written = block->addData(buffer, size);
            setFileSize(getFileSize() + written);
            buffer += written;
            size -= written;
        }
    }

    return getFileSize();
}


bool
FSFileHeaderBlock::addDataBlockRef(u32 ref)
{
    return addDataBlockRef(ref, ref);
}

bool
FSFileHeaderBlock::addDataBlockRef(u32 first, u32 ref)
{
    // If this block has space for more references, add it here
    if (getNumDataBlockRefs() < getMaxDataBlockRefs()) {

        if (getNumDataBlockRefs() == 0) setFirstDataBlockRef(first);
        setDataBlockRef(getNumDataBlockRefs(), ref);
        incNumDataBlockRefs();
        return true;
    }

    // Otherwise, add it to an extension block
    FSFileListBlock *item = getNextListBlock();
    
    for (int i = 0; item && i < searchLimit; i++) {
        
        if (item->addDataBlockRef(first, ref)) return true;
        item = item->getNextListBlock();
    }
    
    assert(false);
    return false;
}
