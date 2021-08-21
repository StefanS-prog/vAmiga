// -----------------------------------------------------------------------------
// This file is part of vAmiga
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

class MyFormatter: Formatter {
    
    var radix: Int
    var minValue: Int
    var maxValue: Int
    var format: String
    
    init(radix: Int, min: Int, max: Int) {

        self.radix = radix
        self.minValue = min
        self.maxValue = max
        
        if radix == 10 {

            format =
                (maxValue < 10) ? "%01d" :
                (maxValue < 100) ? "%02d" :
                (maxValue < 1000) ? "%03d" :
                (maxValue < 10000) ? "%04d" :
                (maxValue < 100000) ? "%05d" :
                (maxValue < 1000000) ? "%06d" :
                (maxValue < 10000000) ? "%07d" :
                (maxValue < 100000000) ? "%08d" : "???"

        } else {
            
            format =
                (maxValue < 0x10) ? "%01X" :
                (maxValue < 0x100) ? "%02X" :
                (maxValue < 0x1000) ? "%03X" :
                (maxValue < 0x10000) ? "%04X" :
                (maxValue < 0x100000) ? "%05X" :
                (maxValue < 0x1000000) ? "%06X" :
                (maxValue < 0x10000000) ? "%07X" :
                (maxValue < 0x100000000) ? "%08X" : "???"
        }
  
        super.init()
    }
    
    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
    override func isPartialStringValid(_ partialString: String, newEditingString newString: AutoreleasingUnsafeMutablePointer<NSString?>?, errorDescription error: AutoreleasingUnsafeMutablePointer<NSString?>?) -> Bool {
        
        let number = (partialString == "") ? 0 : Int(partialString, radix: radix)
        if number == nil || number! < minValue || number! > maxValue {
            NSSound.beep()
            return false
        }
        return true
    }
    
    override func getObjectValue(_ obj: AutoreleasingUnsafeMutablePointer<AnyObject?>?, for string: String, errorDescription error: AutoreleasingUnsafeMutablePointer<NSString?>?) -> Bool {
        
        let result = (string == "") ? 0 : Int(string, radix: radix)
        obj?.pointee = result as AnyObject?
        return true
    }
        
    override func string(for obj: Any?) -> String? {
    
        guard let number = obj as? Int else {
            return nil
        }
        
        switch radix {
        case 2:
            let bits: [Character] = [number & 0x80 != 0 ? "1" : "0",
                                     number & 0x40 != 0 ? "1" : "0",
                                     number & 0x20 != 0 ? "1" : "0",
                                     number & 0x10 != 0 ? "1" : "0",
                                     number & 0x08 != 0 ? "1" : "0",
                                     number & 0x04 != 0 ? "1" : "0",
                                     number & 0x02 != 0 ? "1" : "0",
                                     number & 0x01 != 0 ? "1" : "0"]
            return String(bits)
            
        case 10, 16:
            return String(format: format, number)

        default:
            fatalError()
        }
    }
}
