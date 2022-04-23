// -----------------------------------------------------------------------------
// This file is part of vAmiga
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

class DialogWindow: NSWindow {

    // Delegation method for ESC and Cmd+
    override func cancelOperation(_ sender: Any?) {
                      
        if let controller = delegate as? DialogController {
            controller.cancelAction(sender)
        }
    }
}

/* Base class for all auxiliary windows. The class extends NSWindowController
 * by a reference to the controller of the connected emulator window (parent)
 * and a reference to the parents proxy object. It also provides some wrappers
 * around showing and hiding the window.
 */
protocol DialogControllerDelegate: AnyObject {
    
    // Called before beginSheet() is called
    func sheetWillShow()

    // Called after beginSheet() has beed called
    func sheetDidShow()

    // Called after the completion handler has been executed
    func cleanup()
}

class DialogController: NSWindowController, DialogControllerDelegate {

    var parent: MyController!
    var amiga: AmigaProxy!

    // List of open windows or sheets (to make ARC happy)
    static var active: [DialogController] = []
    
    // Remembers whether awakeFromNib has been called
    var awake = false
    
    convenience init?(with controller: MyController, nibName: NSNib.Name) {
    
        self.init(windowNibName: nibName)
        
        parent = controller
        amiga = parent.amiga
    }

    func register() {
        
        DialogController.active.append(self)
        log("Register: \(DialogController.active)", level: 2)
    }
    
    func unregister() {
        
        DialogController.active = DialogController.active.filter {$0 != self}
        log("Unregister: \(DialogController.active)", level: 2)
    }
    
    override func windowWillLoad() {
    }
    
    override func windowDidLoad() {
    }
    
    override func awakeFromNib() {
    
        awake = true
        window?.delegate = self
        sheetWillShow()
    }
    
    func sheetWillShow() {
        
    }
    
    func sheetDidShow() {
        
    }
    
    func cleanup() {
        
    }
    
    func showWindow(completionHandler handler:(() -> Void)? = nil) {

        register()
        if awake { sheetWillShow() }
        
        showWindow(self)
    }

    func showSheet(completionHandler handler:(() -> Void)? = nil) {

        register()
        if awake { sheetWillShow() }
        
        parent.window?.beginSheet(window!, completionHandler: { result in

            handler?()
            self.cleanup()
        })

        sheetDidShow()
    }
            
    func hideSheet() {
    
        if let win = window {
            parent.window?.endSheet(win, returnCode: .cancel)
        }
        unregister()
    }
    
    @IBAction func okAction(_ sender: Any!) {
        
        hideSheet()
    }
    
    @IBAction func cancelAction(_ sender: Any!) {
        
        hideSheet()
    }
}

extension DialogController: NSWindowDelegate {

    func windowWillClose(_ notification: Notification) {

        unregister()
    }
}
