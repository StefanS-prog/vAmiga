// -----------------------------------------------------------------------------
// This file is part of vAmiga
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

extension ConfigurationController {
    
    func refreshHardwareTab() {

        let poweredOff = amiga.poweredOff
        
        // Chipset
        hwAgnusRevisionPopup.selectItem(withTag: config.agnusRev)
        hwDeniseRevisionPopup.selectItem(withTag: config.deniseRev)
        hwCiaRevisionPopup.selectItem(withTag: config.ciaRev)
        hwRealTimeClock.selectItem(withTag: config.rtClock)

        // Audio filter
        hwFilterType.selectItem(withTag: config.filterType)
        hwFilterAlwaysOn.state = config.filterAlwaysOn ? .on : .off
        hwFilterAlwaysOn.isEnabled = config.filterType != FilterType.NONE.rawValue

        // Memory
        hwChipRamPopup.selectItem(withTag: config.chipRam)
        hwSlowRamPopup.selectItem(withTag: config.slowRam)
        hwFastRamPopup.selectItem(withTag: config.fastRam)
        hwRamInitPattern.selectItem(withTag: config.ramInitPattern)
        hwBankMap.selectItem(withTag: config.bankMap)
        hwUnmappingType.selectItem(withTag: config.unmappingType)

        // Lock some controls if emulator is powered on
        hwAgnusRevisionPopup.isEnabled = poweredOff
        hwDeniseRevisionPopup.isEnabled = poweredOff
        hwCiaRevisionPopup.isEnabled = poweredOff
        hwRealTimeClock.isEnabled = poweredOff
        hwChipRamPopup.isEnabled = poweredOff
        hwSlowRamPopup.isEnabled = poweredOff
        hwFastRamPopup.isEnabled = poweredOff
        hwRamInitPattern.isEnabled = poweredOff
        hwFactorySettingsPopup.isEnabled = poweredOff

        // Lock symbol and explanation
        hwLockImage.isHidden = poweredOff
        hwLockText.isHidden = poweredOff
        hwLockSubText.isHidden = poweredOff

        // Buttons
        hwPowerButton.isHidden = !bootable
    }
    
    @IBAction func hwAgnusRevAction(_ sender: NSPopUpButton!) {

        config.agnusRev = sender.selectedTag()
        refresh()
    }

    @IBAction func hwDeniseRevAction(_ sender: NSPopUpButton!) {

        config.deniseRev = sender.selectedTag()
        refresh()
    }

    @IBAction func hwFilterTypeAction(_ sender: NSPopUpButton!) {
        
        config.filterType = sender.selectedTag()
        refresh()
    }

    @IBAction func hwFilterAlwaysOnAction(_ sender: NSButton!) {

        config.filterAlwaysOn = sender.state == .on
        refresh()
    }
    
    @IBAction func hwCiaRevAction(_ sender: NSPopUpButton!) {

        config.ciaRev = sender.selectedTag()
        refresh()
    }

    @IBAction func hwRealTimeClockAction(_ sender: NSPopUpButton!) {
        
        config.rtClock = sender.selectedTag()
        refresh()
    }
    
    @IBAction func hwChipRamAction(_ sender: NSPopUpButton!) {

        config.chipRam = sender.selectedTag()
        refresh()
    }

    @IBAction func hwSlowRamAction(_ sender: NSPopUpButton!) {
        
        config.slowRam = sender.selectedTag()
        refresh()
    }

    @IBAction func hwFastRamAction(_ sender: NSPopUpButton!) {
        
        config.fastRam = sender.selectedTag()
        refresh()
    }

    @IBAction func hwBankMapAction(_ sender: NSPopUpButton!) {

        config.bankMap = sender.selectedTag()
        refresh()
    }

    @IBAction func hwUnmappingTypeAction(_ sender: NSPopUpButton!) {

        config.unmappingType = sender.selectedTag()
        refresh()
    }

    @IBAction func hwRamInitPatternAction(_ sender: NSPopUpButton!) {

        config.ramInitPattern = sender.selectedTag()
        refresh()
    }

    @IBAction func hwPresetAction(_ sender: NSPopUpButton!) {
        
        switch sender.selectedTag() {
        case 0: config.loadHardwareDefaults(HardwareDefaults.A500)
        case 1: config.loadHardwareDefaults(HardwareDefaults.A1000)
        case 2: config.loadHardwareDefaults(HardwareDefaults.A2000)
        default: fatalError()
        }
        refresh()
    }

    @IBAction func hwDefaultsAction(_ sender: NSButton!) {
        
        config.saveHardwareUserDefaults()
    }
}
