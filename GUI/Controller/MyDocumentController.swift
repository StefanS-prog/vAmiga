// -----------------------------------------------------------------------------
// This file is part of vAmiga
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

import Foundation

class MyDocumentController: NSDocumentController {
    
    override func makeDocument(withContentsOf url: URL,
                               ofType typeName: String) throws -> NSDocument {
        
        log()
        
        // For media files, attach the file to a new untitled document
        if typeName.uppercased() != "VAMIGA" {

            let doc = try super.makeUntitledDocument(ofType: typeName)
            if let mydoc = doc as? MyDocument {
                // try? mydoc.createAttachment(from: url)
                try? mydoc.addMedia(url: url, allowedTypes: FileType.all)
                return mydoc
            }
        }
        
        // For snapshot files, follow the standard procedure
        return try super.makeDocument(withContentsOf: url, ofType: typeName)
    }
}
