// -----------------------------------------------------------------------------
// This file is part of vAmiga
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

import Foundation

class DropZone: Layer {
 
    let controller: MyController
    
    var window: NSWindow { return controller.window! }
    var contentView: NSView { return window.contentView! }
        
    var zones = [NSImageView(), NSImageView(), NSImageView(), NSImageView()]
    var ul = [NSPoint(x: 0, y: 0), NSPoint(x: 0, y: 0),
              NSPoint(x: 0, y: 0), NSPoint(x: 0, y: 0)]
    var lr = [NSPoint(x: 0, y: 0), NSPoint(x: 0, y: 0),
              NSPoint(x: 0, y: 0), NSPoint(x: 0, y: 0)]
    
    static let unconnected = 0.92
    static let unselected = 0.92
    static let selected = 0.92
    
    var enabled = [false, false, false, false]
    var inside = [false, false, false, false]
    var currentAlpha = [0.0, 0.0, 0.0, 0.0]
    var targetAlpha = [unselected, unselected, unselected, unselected]
    var maxAlpha = [0.0, 0.0, 0.0, 0.0]
        
    //
    // Initializing
    //
    
    override init(renderer: Renderer) {
        
        controller = renderer.parent
        
        for i in 0...3 { zones[i].unregisterDraggedTypes() }
        super.init(renderer: renderer)
        resize()
    }

    private func setType(_ type: FileType) {
    
        let connected = amiga.diskController.getConfig().connected
        
        switch type {
        
        case .ADF, .EXT, .IMG, .DMS, .EXE, .DIR:
            enabled = [connected.0, connected.1, connected.2, connected.3]
            
        default:
            enabled = [false, false, false, false]
        }
        
        for i in 0...3 {
            
            let imgZone = enabled[i] ? "dropZoneDf\(i)Empty" : "dropZoneDf\(i)Disabled"
            zones[i].image = NSImage.init(named: imgZone)
        }
    }

    func open(type: FileType, delay: Double) {

        track()
        setType(type)
        open(delay: delay)
        resize()
    }
    
    override func update(frames: Int64) {
        
        super.update(frames: frames)

        if alpha.current > 0 { updateAlpha() }
    }
    
    func isInside(_ sender: NSDraggingInfo, zone i: Int) -> Bool {

        assert(i >= 0 && i <= 3)

        if !enabled[i] { return false }
        
        let x = sender.draggingLocation.x
        let y = sender.draggingLocation.y

        return x > ul[i].x && x < lr[i].x && y > ul[i].y && y < lr[i].y
    }
    
    func draggingUpdated(_ sender: NSDraggingInfo) {
                        
        for i in 0...3 {

            if !enabled[i] {
                targetAlpha[i] = DropZone.unconnected
                return
            }
            
            let isIn = isInside(sender, zone: i)

            if isIn && !inside[i] {
                
                inside[i] = true
                zones[i].image = NSImage.init(named: "dropZoneDf\(i)Selected")
                targetAlpha[i] = DropZone.selected
            }

            if !isIn && inside[i] {
                
                inside[i] = false
                zones[i].image = NSImage.init(named: "dropZoneDf\(i)Empty")
                targetAlpha[i] = DropZone.unselected
            }
        }
    }
    
    override func alphaDidChange() {
                
        for i in 0...3 {
            
            maxAlpha[i] = Double(alpha.current)
        
            if alpha.current > 0 && zones[i].superview == nil {
                contentView.addSubview(zones[i])
            }
            if alpha.current == 0 && zones[i].superview != nil {
                zones[i].removeFromSuperview()
            }
        }
    }
    
    func updateAlpha() {
            
        for i in 0...3 {

            let current = currentAlpha[i]
            var delta = 0.0

            if current < targetAlpha[i] && current < maxAlpha[i] { delta = 0.05 }
            if current > targetAlpha[i] || current > maxAlpha[i] { delta = -0.05 }

            if delta != 0.0 {
                
                currentAlpha[i] += delta
                zones[i].alphaValue = CGFloat(currentAlpha[i])
            }
        }
    }
    
    func resize() {
                          
        let size = controller.metal.frame.size
        let origin = controller.metal.frame.origin
        let midx = origin.x + (size.width / 2)
        let midy = origin.y + (size.height / 2)

        let w = size.width / 6
        let h = w * 1.2
        let margin = w / 8
        let iconSize = NSSize.init(width: w, height: h)

        ul[0] = CGPoint.init(x: midx - 2 * w - 1.5 * margin, y: midy - (h / 2))
        lr[0] = CGPoint.init(x: ul[0].x + w, y: ul[0].y + h)

        ul[1] = CGPoint.init(x: midx - w - 0.5 * margin, y: midy - (h / 2))
        lr[1] = CGPoint.init(x: ul[1].x + w, y: ul[1].y + h)

        ul[2] = CGPoint.init(x: midx + 0.5 * margin, y: midy - (h / 2))
        lr[2] = CGPoint.init(x: ul[2].x + w, y: ul[2].y + h)

        ul[3] = CGPoint.init(x: midx + w + 1.5 * margin, y: midy - (h / 2))
        lr[3] = CGPoint.init(x: ul[3].x + w, y: ul[3].y + h)

        for i in 0...3 {

            zones[i].setFrameSize(iconSize)
            zones[i].frame.origin = ul[i]
        }
    }
}
