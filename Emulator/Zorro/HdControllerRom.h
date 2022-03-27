// Auto boot driver from AmiEmu.
// Author: mras0
//
// https://github.com/mras0/AmiEmu/master/exprom.asm
// https://github.com/dirkwhoffmann/vAmiga/issues/651

#define EXPROM_SIZE isizeof(exprom)

const u8 exprom[1306] = {
    
    0x90, 0x00, 0x00, 0xf4, 0x00, 0xca, 0x00, 0xba, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x4a, 0xfc,
    0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0xf4, 0x81, 0x00, 0x03, 0x14, 0x00, 0x00, 0x00, 0x28,
    0x00, 0x00, 0x00, 0x41, 0x00, 0x00, 0x00, 0x64, 0x76, 0x69, 0x72, 0x74, 0x75, 0x61, 0x6c, 0x68,
    0x64, 0x2e, 0x64, 0x65, 0x76, 0x69, 0x63, 0x65, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x76, 0x69, 0x72, 0x74, 0x75, 0x61, 0x6c, 0x68, 0x64, 0x20, 0x30, 0x2e, 0x33, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x6f, 0x73, 0x2e, 0x6c, 0x69, 0x62, 0x72,
    0x61, 0x72, 0x79, 0x00, 0x00, 0x00, 0x00, 0x32, 0x00, 0x00, 0x00, 0x74, 0x00, 0x00, 0x00, 0x90,
    0x00, 0x00, 0x01, 0x58, 0x00, 0x00, 0x04, 0x08, 0x00, 0x00, 0x04, 0x88, 0x00, 0x00, 0x04, 0xca,
    0x00, 0x00, 0x04, 0xce, 0x00, 0x00, 0x04, 0xd2, 0x00, 0x00, 0x05, 0x00, 0xff, 0xff, 0xff, 0xff,
    0xe0, 0x00, 0x00, 0x08, 0x03, 0x00, 0xc0, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x28, 0xe0, 0x00,
    0x00, 0x0e, 0x06, 0x00, 0xd0, 0x00, 0x00, 0x14, 0x00, 0x00, 0xd0, 0x00, 0x00, 0x16, 0x00, 0x03,
    0xc0, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x41, 0x00, 0x00, 0x43, 0xfa, 0xff, 0x9c, 0x4e, 0xae,
    0xff, 0xa0, 0x20, 0x40, 0x20, 0x68, 0x00, 0x16, 0x4e, 0xd0, 0x22, 0x7c, 0x00, 0x00, 0x01, 0x34,
    0x93, 0xfc, 0x00, 0x00, 0x00, 0x00, 0xd3, 0xc8, 0x22, 0x0a, 0x30, 0x19, 0x6b, 0x06, 0xd3, 0xb2,
    0x00, 0x00, 0x60, 0xf6, 0x22, 0x08, 0x30, 0x19, 0x6b, 0x06, 0xd3, 0xb2, 0x00, 0x00, 0x60, 0xf6,
    0x70, 0x01, 0x4e, 0x75, 0x00, 0x10, 0x00, 0x14, 0x00, 0x1c, 0x00, 0x20, 0x00, 0x24, 0x00, 0x68,
    0x00, 0x6c, 0x00, 0x9a, 0x00, 0xb4, 0xff, 0xff, 0x00, 0x70, 0x00, 0x74, 0x00, 0x78, 0x00, 0x7c,
    0x00, 0x80, 0x00, 0x84, 0x00, 0x88, 0xff, 0xff, 0x48, 0xe7, 0x7f, 0xfe, 0x2a, 0x40, 0x2b, 0x4e,
    0x00, 0x22, 0x2b, 0x48, 0x00, 0x26, 0x99, 0xcc, 0x43, 0xfa, 0x02, 0x78, 0x70, 0x00, 0x4e, 0xae,
    0xfd, 0xd8, 0x4a, 0x80, 0x67, 0x00, 0x02, 0x4c, 0x28, 0x40, 0x41, 0xfa, 0x03, 0xde, 0x3c, 0x28,
    0x00, 0x02, 0x67, 0x00, 0x01, 0x0e, 0x43, 0xfa, 0x02, 0x6c, 0x4e, 0xae, 0xfe, 0x0e, 0x4a, 0x80,
    0x67, 0x00, 0x00, 0x1a, 0x41, 0xfa, 0x03, 0xc4, 0x20, 0x80, 0x31, 0x7c, 0xfe, 0xe0, 0x00, 0x04,
    0x3c, 0x28, 0x00, 0x02, 0x66, 0x00, 0x00, 0x10, 0x60, 0x00, 0x00, 0xe8, 0x61, 0x00, 0x03, 0x5a,
    0x4a, 0x80, 0x67, 0x00, 0x02, 0x0e, 0x2b, 0x40, 0x00, 0x2e, 0x9f, 0xfc, 0x00, 0x00, 0x00, 0x1a,
    0x7a, 0x00, 0x22, 0x4f, 0x33, 0x45, 0x00, 0x00, 0x41, 0xfa, 0x03, 0x90, 0x20, 0x89, 0x31, 0x7c,
    0xfe, 0xe1, 0x00, 0x04, 0x9f, 0xfc, 0x00, 0x00, 0x00, 0x10, 0x26, 0x4f, 0x26, 0x29, 0x00, 0x0a,
    0x45, 0xe9, 0x00, 0x0e, 0x74, 0x00, 0x20, 0x1a, 0x22, 0x00, 0x42, 0x41, 0x48, 0x41, 0xe0, 0x89,
    0xea, 0x89, 0x00, 0x81, 0x00, 0x01, 0x00, 0x00, 0x02, 0x80, 0x3f, 0xff, 0xff, 0xff, 0xe5, 0x88,
    0x50, 0x80, 0x4e, 0xae, 0xff, 0x3a, 0x22, 0x02, 0xe5, 0x89, 0x27, 0x80, 0x10, 0x00, 0x52, 0x82,
    0xb6, 0x82, 0x66, 0xd2, 0x27, 0x45, 0x00, 0x0c, 0x41, 0xfa, 0x03, 0x40, 0x20, 0x8b, 0x31, 0x7c,
    0xfe, 0xe2, 0x00, 0x04, 0x20, 0x2b, 0x00, 0x00, 0x58, 0x80, 0xe4, 0x88, 0x26, 0x40, 0xdf, 0xfc,
    0x00, 0x00, 0x00, 0x10, 0x20, 0x3c, 0x00, 0x00, 0x00, 0x3e, 0x22, 0x3c, 0x00, 0x01, 0x00, 0x01,
    0x4e, 0xae, 0xff, 0x3a, 0x4a, 0x80, 0x67, 0x00, 0x01, 0x7a, 0x20, 0x40, 0x22, 0x4f, 0x21, 0x69,
    0x00, 0x02, 0x00, 0x0e, 0x21, 0x69, 0x00, 0x06, 0x00, 0x12, 0x21, 0x7c, 0x00, 0x00, 0x01, 0x80,
    0x00, 0x16, 0x21, 0x7c, 0xff, 0xff, 0xff, 0xff, 0x00, 0x3a, 0x21, 0x4b, 0x00, 0x36, 0x47, 0xfa,
    0xfe, 0x11, 0x21, 0x4b, 0x00, 0x0a, 0x22, 0x48, 0x20, 0x6d, 0x00, 0x2e, 0x41, 0xe8, 0x00, 0x12,
    0x4e, 0xae, 0xff, 0x10, 0x52, 0x45, 0xbc, 0x45, 0x66, 0x00, 0xff, 0x38, 0xdf, 0xfc, 0x00, 0x00,
    0x00, 0x1a, 0x2c, 0x4c, 0x9f, 0xfc, 0x00, 0x00, 0x00, 0x10, 0x20, 0x4f, 0x70, 0x10, 0x4e, 0xae,
    0xff, 0x76, 0x2c, 0x2f, 0x00, 0x00, 0xdf, 0xfc, 0x00, 0x00, 0x00, 0x10, 0x4a, 0x86, 0x67, 0x00,
    0x01, 0x12, 0x20, 0x46, 0x2b, 0x68, 0x00, 0x20, 0x00, 0x2a, 0x08, 0xa8, 0x00, 0x01, 0x00, 0x0e,
    0x7a, 0x00, 0x9f, 0xfc, 0x00, 0x00, 0x00, 0x5c, 0x26, 0x4f, 0x9f, 0xfc, 0x00, 0x00, 0x00, 0x20,
    0x27, 0x4f, 0x00, 0x00, 0x41, 0xfa, 0xfd, 0x92, 0x27, 0x48, 0x00, 0x04, 0x27, 0x45, 0x00, 0x08,
    0x27, 0x7c, 0x00, 0x00, 0x00, 0x10, 0x00, 0x10, 0x41, 0xfa, 0x02, 0x70, 0x20, 0x8b, 0x31, 0x7c,
    0xfe, 0xdf, 0x00, 0x04, 0x20, 0x4b, 0x4e, 0xae, 0xff, 0x70, 0x4a, 0x80, 0x67, 0x00, 0x00, 0x98,
    0x20, 0x40, 0x21, 0x7c, 0xff, 0xff, 0xff, 0xff, 0x00, 0x24, 0x21, 0x6b, 0x00, 0x58, 0x00, 0x20,
    0x20, 0x2b, 0x00, 0x54, 0x08, 0x00, 0x00, 0x00, 0x66, 0x00, 0x00, 0x14, 0x70, 0x80, 0x72, 0x01,
    0x4e, 0xae, 0xff, 0x6a, 0x4a, 0x80, 0x67, 0x00, 0x00, 0x6e, 0x60, 0x00, 0x00, 0x7a, 0x0c, 0x6e,
    0x00, 0x24, 0x00, 0x14, 0x65, 0x16, 0x22, 0x46, 0x20, 0x2b, 0x00, 0x4c, 0x72, 0x01, 0x4e, 0xae,
    0xff, 0xdc, 0x4a, 0x80, 0x67, 0x00, 0x00, 0x50, 0x60, 0x00, 0x00, 0x5c, 0x2f, 0x08, 0x2c, 0x6d,
    0x00, 0x22, 0x70, 0x14, 0x22, 0x3c, 0x00, 0x01, 0x00, 0x01, 0x2c, 0x6d, 0x00, 0x22, 0x4e, 0xae,
    0xff, 0x3a, 0x20, 0x5f, 0x4a, 0x80, 0x67, 0x00, 0x00, 0x2e, 0x22, 0x40, 0x13, 0x7c, 0x00, 0x10,
    0x00, 0x08, 0x23, 0x46, 0x00, 0x0a, 0x33, 0x7c, 0x00, 0x01, 0x00, 0x0e, 0x23, 0x48, 0x00, 0x10,
    0x41, 0xec, 0x00, 0x4a, 0x4e, 0xae, 0xff, 0x7c, 0x4e, 0xae, 0xfe, 0xf2, 0x4e, 0xae, 0xff, 0x76,
    0x2c, 0x4c, 0x60, 0x00, 0x00, 0x12, 0xdf, 0xfc, 0x00, 0x00, 0x00, 0x20, 0xdf, 0xfc, 0x00, 0x00,
    0x00, 0x5c, 0x60, 0x00, 0x00, 0x1e, 0xdf, 0xfc, 0x00, 0x00, 0x00, 0x20, 0xdf, 0xfc, 0x00, 0x00,
    0x00, 0x5c, 0x52, 0x45, 0xba, 0x7a, 0x01, 0xa4, 0x66, 0x00, 0xff, 0x08, 0x20, 0x0d, 0x60, 0x00,
    0x00, 0x04, 0x70, 0x00, 0x2c, 0x6d, 0x00, 0x22, 0xb9, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x67, 0x00,
    0x00, 0x0c, 0x2f, 0x00, 0x22, 0x4c, 0x4e, 0xae, 0xfe, 0x62, 0x20, 0x1f, 0x4c, 0xdf, 0x7f, 0xfe,
    0x4e, 0x75, 0x65, 0x78, 0x70, 0x61, 0x6e, 0x73, 0x69, 0x6f, 0x6e, 0x2e, 0x6c, 0x69, 0x62, 0x72,
    0x61, 0x72, 0x79, 0x00, 0x46, 0x69, 0x6c, 0x65, 0x53, 0x79, 0x73, 0x74, 0x65, 0x6d, 0x2e, 0x72,
    0x65, 0x73, 0x6f, 0x75, 0x72, 0x63, 0x65, 0x00, 0x48, 0xe7, 0xff, 0xfe, 0x52, 0x6e, 0x00, 0x20,
    0x24, 0x49, 0x74, 0x00, 0x34, 0x3a, 0x01, 0x44, 0xb0, 0x82, 0x6c, 0x60, 0x28, 0x00, 0x20, 0x3c,
    0x00, 0x00, 0x00, 0x2e, 0x22, 0x3c, 0x00, 0x01, 0x00, 0x01, 0x26, 0x4e, 0x2c, 0x6b, 0x00, 0x22,
    0x4e, 0xae, 0xff, 0x3a, 0x2c, 0x4b, 0x4a, 0x80, 0x67, 0x00, 0x00, 0x42, 0x26, 0x40, 0x27, 0x4e,
    0x00, 0x26, 0x27, 0x44, 0x00, 0x2a, 0x17, 0x7c, 0x00, 0x02, 0x00, 0x0e, 0x17, 0x7c, 0x00, 0x04,
    0x00, 0x08, 0x41, 0xfa, 0xfc, 0x14, 0x27, 0x48, 0x00, 0x0a, 0x25, 0x4b, 0x00, 0x18, 0x52, 0x6e,
    0x00, 0x20, 0x52, 0x6b, 0x00, 0x24, 0x70, 0x00, 0x15, 0x40, 0x00, 0x1f, 0x15, 0x7c, 0x00, 0x07,
    0x00, 0x08, 0x53, 0x6e, 0x00, 0x20, 0x4c, 0xdf, 0x7f, 0xff, 0x4e, 0x75, 0x70, 0xff, 0x15, 0x40,
    0x00, 0x1f, 0x25, 0x40, 0x00, 0x14, 0x60, 0xea, 0x48, 0xe7, 0xff, 0xfe, 0x26, 0x69, 0x00, 0x18,
    0x70, 0x00, 0x23, 0x40, 0x00, 0x18, 0x23, 0x40, 0x00, 0x14, 0x53, 0x6b, 0x00, 0x24, 0x66, 0x00,
    0x00, 0x16, 0x2a, 0x4e, 0x22, 0x4b, 0x20, 0x3c, 0x00, 0x00, 0x00, 0x2e, 0x2c, 0x6e, 0x00, 0x22,
    0x4e, 0xae, 0xff, 0x2e, 0x2c, 0x4d, 0x70, 0x00, 0x53, 0x6e, 0x00, 0x20, 0x66, 0x00, 0x00, 0x06,
    0x61, 0x00, 0x00, 0x08, 0x4c, 0xdf, 0x7f, 0xff, 0x4e, 0x75, 0x70, 0x00, 0x4e, 0x75, 0x70, 0x00,
    0x4e, 0x75, 0x48, 0xe7, 0xff, 0xfe, 0x13, 0x7c, 0x00, 0x05, 0x00, 0x08, 0x41, 0xfa, 0x00, 0x7c,
    0x20, 0x89, 0x31, 0x7c, 0xfe, 0xde, 0x00, 0x04, 0x08, 0x29, 0x00, 0x00, 0x00, 0x1e, 0x66, 0x00,
    0x00, 0x0a, 0x2c, 0x6e, 0x00, 0x22, 0x4e, 0xae, 0xfe, 0x86, 0x4c, 0xdf, 0x7f, 0xff, 0x4e, 0x75,
    0x13, 0x7c, 0x00, 0x00, 0x00, 0x1f, 0x4e, 0x75, 0x20, 0x3c, 0x00, 0x00, 0x00, 0x20, 0x22, 0x3c,
    0x00, 0x01, 0x00, 0x01, 0x4e, 0xae, 0xff, 0x3a, 0x4a, 0x80, 0x67, 0x00, 0x00, 0x3c, 0x2f, 0x00,
    0x20, 0x40, 0x11, 0x7c, 0x00, 0x08, 0x00, 0x08, 0x43, 0xfa, 0xfe, 0xca, 0x21, 0x49, 0x00, 0x0a,
    0x43, 0xfa, 0xfb, 0x4f, 0x21, 0x49, 0x00, 0x0e, 0x41, 0xe8, 0x00, 0x12, 0x21, 0x48, 0x00, 0x00,
    0x58, 0xa8, 0x00, 0x00, 0x42, 0xa8, 0x00, 0x04, 0x21, 0x48, 0x00, 0x08, 0x41, 0xee, 0x01, 0x50,
    0x22, 0x40, 0x4e, 0xae, 0xff, 0x0a, 0x20, 0x1f, 0x4e, 0x75
};
