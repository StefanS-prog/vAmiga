// -----------------------------------------------------------------------------
// This file is part of vAmiga
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

class PartitionSelector: DialogController {
        
    @IBOutlet weak var partitionPopup: NSPopUpButton!

    @IBOutlet weak var name: NSTextField!
    @IBOutlet weak var lowerCyl: NSTextField!
    @IBOutlet weak var upperCyl: NSTextField!

    var nr = 0
    var drive: HardDriveProxy { amiga.dh(nr)! }
    var partition: Int { return partitionPopup.selectedTag() }
    var userSelection: Int?
    
    //
    // Starting up
    //
    
    func showSheet(hardDrive nr: Int, completionHandler:(() -> Void)? = nil) {
                
        self.nr = nr
        super.showSheet(completionHandler: completionHandler)
    }
            
    override public func awakeFromNib() {
        
        super.awakeFromNib()
        
        partitionPopup.removeAllItems()
        
        let partitions = drive.partitions
        for i in 0 ..< partitions {
            
            partitionPopup.addItem(withTitle: "Partition \(i)")
            partitionPopup.item(at: i)!.tag = i
        }
        
        partitionPopup.autoenablesItems = false

        update()
    }
    
    override func windowDidLoad() {
        
    }
    
    override func sheetDidShow() {
        
        track()
    }
    
    //
    // Updating the displayed information
    //
    
    func update() {
          
        name.stringValue = drive.name(ofPartition: partition)
        lowerCyl.integerValue = drive.lowerCyl(ofPartition: partition)
        upperCyl.integerValue = drive.upperCyl(ofPartition: partition)
    }
        
    //
    // Action methods
    //

    @IBAction func selectAction(_ sender: Any!) {

        update()
    }

    @IBAction override func okAction(_ sender: Any!) {
        
        track()
        userSelection = partition
        super.okAction(sender)
    }
}
