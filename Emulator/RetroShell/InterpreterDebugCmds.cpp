// -----------------------------------------------------------------------------
// This file is part of vAmiga
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#include "config.h"
#include "Interpreter.h"
#include "RetroShell.h"

namespace vamiga {

void
Interpreter::initDebugShell(Command &root)
{
    initCommons(root);

    //
    // Top-level commands
    //

    root.newGroup("Controlling the instruction stream");

    root.add({"pause"},
             "Pauses emulation",
             &RetroShell::exec <Token::pause>);

    root.add({"run"},
             "Continues emulation",
             &RetroShell::exec <Token::run>);

    root.add({"step"},
             "Steps into the next instruction",
             &RetroShell::exec <Token::step>);

    root.add({"next"},
             "Steps over the next instruction",
             &RetroShell::exec <Token::next>);

    root.add({"goto"}, { Arg::address },
             "Redirects the program counter",
             &RetroShell::exec <Token::jump>);

    root.add({"disassemble"}, { Arg::address },
             "Runs disassembler",
             &RetroShell::exec <Token::disassemble>);


    root.newGroup("Guarding the program execution");

    root.add({"break"},
             "Manages CPU breakpoints");

    root.add({"watch"},
             "Manages CPU watchpoints");

    root.add({"catch"},
             "Manages CPU catchpoints");

    root.add({"cbreak"},
             "Manages Copper breakpoints");

    root.add({"cwatch"},
             "Manages Copper watchpoints");


    root.newGroup("Exploring components");

    root.add({"amiga"},
             "Main computer");

    root.add({"memory"},
             "RAM and ROM");

    root.add({"cpu"},
             "Motorola 68k CPU");

    root.add({"ciaa"},
             "Complex Interface Adapter A");

    root.add({"ciab"},
             "Complex Interface Adapter B");

    root.add({"agnus"},
             "Custom Chipset");

    root.add({"blitter"},
             "Coprocessor");

    root.add({"copper"},
             "Coprocessor");

    root.add({"paula"},
             "Custom Chipset");

    root.add({"denise"},
             "Custom Chipset");

    root.add({"os"},
             "AmigaOS debugger");

    
    //
    // Breakpoints
    //

    root.newGroup("");

    root.add({"break", ""},
             "Lists all breakpoints",
             &RetroShell::exec <Token::bp>);

    root.add({"break", "at"}, { Arg::address },
             "Sets a breakpoint at the specified address",
             &RetroShell::exec <Token::bp, Token::at>);

    root.add({"break", "delete"}, { Arg::address },
             "Deletes a breakpoint",
             &RetroShell::exec <Token::bp, Token::del>);

    root.add({"break", "enable"}, { Arg::address },
             "Enables a breakpoint",
             &RetroShell::exec <Token::bp, Token::enable>);

    root.add({"break", "disable"}, { Arg::address },
             "Disables a breakpoint",
             &RetroShell::exec <Token::bp, Token::disable>);

    root.add({"break", "ignore"}, { Arg::address, Arg::value },
             "Ignores a breakpoint a certain number of times",
             &RetroShell::exec <Token::bp, Token::ignore>);

    //
    // Watchpoints
    //

    root.newGroup("");

    root.add({"watch", ""},
             "Lists all watchpoints",
             &RetroShell::exec <Token::wp>);

    root.add({"watch", "at"}, { Arg::address },
             "Sets a watchpoint at the specified address",
             &RetroShell::exec <Token::wp, Token::at>);

    root.add({"watch", "delete"}, { Arg::address },
             "Deletes a watchpoint",
             &RetroShell::exec <Token::wp, Token::del>);

    root.add({"watch", "enable"}, { Arg::address },
             "Enables a watchpoint",
             &RetroShell::exec <Token::wp, Token::enable>);

    root.add({"watch", "disable"}, { Arg::address },
             "Disables a watchpoint",
             &RetroShell::exec <Token::wp, Token::disable>);

    root.add({"watch", "ignore"}, { Arg::address, Arg::value },
             "Ignores a watchpoint a certain number of times",
             &RetroShell::exec <Token::wp, Token::ignore>);

    //
    // Catchpoints
    //

    root.newGroup("");

    root.add({"catch", ""},
             "Lists all catchpoints",
             &RetroShell::exec <Token::cp>);

    root.add({"catch", "vector"}, { Arg::value },
             "Catches an exception vector",
             &RetroShell::exec <Token::cp, Token::vector>);

    root.add({"catch", "interrupt"}, { Arg::value },
             "Catches an interrupt",
             &RetroShell::exec <Token::cp, Token::interrupt>);

    root.add({"catch", "trap"}, { Arg::value },
             "Catches a trap instruction",
             &RetroShell::exec <Token::cp, Token::trap>);

    root.add({"catch", "delete"}, { Arg::value },
             "Deletes a catchpoint",
             &RetroShell::exec <Token::cp, Token::del>);

    root.add({"catch", "enable"}, { Arg::value },
             "Enables a catchpoint",
             &RetroShell::exec <Token::cp, Token::enable>);

    root.add({"catch", "disable"}, { Arg::value },
             "Disables a catchpoint",
             &RetroShell::exec <Token::cp, Token::disable>);

    root.add({"catch", "ignore"}, { Arg::value, Arg::value },
             "Ignores a catchpoint a certain number of times",
             &RetroShell::exec <Token::cp, Token::ignore>);


    //
    // Copper breakpoints
    //

    root.add({"cbreak", ""},
             "Lists all breakpoints",
             &RetroShell::exec <Token::cbp>);

    root.add({"cbreak", "at"}, { Arg::value },
             "Sets a breakpoint at the specified address",
             &RetroShell::exec <Token::cbp, Token::at>);

    root.add({"cbreak", "delete"}, { Arg::value },
             "Deletes a breakpoint",
             &RetroShell::exec <Token::cbp, Token::del>);

    root.add({"cbreak", "enable"}, { Arg::value },
             "Enables a breakpoint",
             &RetroShell::exec <Token::cbp, Token::enable>);

    root.add({"cbreak", "disable"}, { Arg::value },
             "Disables a breakpoint",
             &RetroShell::exec <Token::cbp, Token::disable>);

    root.add({"cbreak", "ignore"}, { Arg::value, Arg::value },
             "Ignores a breakpoint a certain number of times",
             &RetroShell::exec <Token::cbp, Token::ignore>);


    //
    // Copper watchpoints
    //

    root.add({"cwatch", ""},
             "Lists all watchpoints",
             &RetroShell::exec <Token::cwp>);

    root.add({"cwatch", "at"}, { Arg::value },
             "Sets a watchpoint at the specified address",
             &RetroShell::exec <Token::cwp, Token::at>);

    root.add({"cwatch", "delete"}, { Arg::value },
             "Deletes a watchpoint",
             &RetroShell::exec <Token::cwp, Token::del>);

    root.add({"cwatch", "enable"}, { Arg::value },
             "Enables a watchpoint",
             &RetroShell::exec <Token::cwp, Token::enable>);

    root.add({"cwatch", "disable"}, { Arg::value },
             "Disables a watchpoint",
             &RetroShell::exec <Token::cwp, Token::disable>);

    root.add({"cwatch", "ignore"}, { Arg::value, Arg::value },
             "Ignores a watchpoint a certain number of times",
             &RetroShell::exec <Token::cwp, Token::ignore>);


    //
    // Components
    //

    root.newGroup("");

    root.add({"amiga", ""},
             "Lorem ipsum",
             &RetroShell::exec <Token::amiga>);

    root.add({"amiga", "state"},
             "Lorem ipsum",
             &RetroShell::exec <Token::amiga, Token::state>);

    root.add({"memory", ""},
             "Lorem ipsum",
             &RetroShell::exec <Token::memory>);

    root.add({"memory", "dump"}, { Arg::address },
             "Generates a memory hexdump",
             &RetroShell::exec <Token::memory, Token::memdump>);

    root.add({"memory", "banks"},
             "Dumps the memory bank map",
             &RetroShell::exec <Token::memory, Token::bankmap>);

    root.add({"memory", "checksum"},
             "Computes memory checksums",
             &RetroShell::exec <Token::memory, Token::checksums>);

    root.add({"cpu", ""},
             "Lorem ipsum",
             &RetroShell::exec <Token::cpu>);

    root.add({"cpu", "state"},
             "Inspects the internal state",
             &RetroShell::exec <Token::cpu, Token::state>);

    root.add({"cpu", "vectors"},
             "Dumps the vector table",
             &RetroShell::exec <Token::cpu, Token::vectors>);

    for (isize i = 0; i < 2; i++) {

        string cia = (i == 0) ? "ciaa" : "ciab";

        root.add({cia, ""},
                 "Displays the internal state",
                 &RetroShell::exec <Token::cia>, i);

        root.add({cia, "state"},
                 "Displays the internal state",
                 &RetroShell::exec <Token::cia, Token::state>, i);

        root.add({cia, "tod"},
                 "Displays the state of the 24-bit counter",
                 &RetroShell::exec <Token::cia, Token::tod>, i);
    }

    root.add({"agnus", ""},
             "Lorem ipsum",
             &RetroShell::exec <Token::agnus>);

    root.add({"agnus", "state"},
             "Inspects the internal state",
             &RetroShell::exec <Token::agnus, Token::state>);

    root.add({"agnus", "beam"},
             "Displays the current beam position",
             &RetroShell::exec <Token::agnus, Token::beam>);

    root.add({"agnus", "dma"},
             "Prints all scheduled DMA events",
             &RetroShell::exec <Token::agnus, Token::dma>);

    root.add({"agnus", "events"},
             "Inspects the event scheduler",
             &RetroShell::exec <Token::agnus, Token::events>);

    root.add({"blitter", ""},
             "Lorem ipsum",
             &RetroShell::exec <Token::blitter>);

    root.add({"blitter", "state"},
             "Inspects the internal state",
             &RetroShell::exec <Token::blitter, Token::state>);

    root.add({"copper", ""},
             "Lorem ipsum",
             &RetroShell::exec <Token::copper>);

    root.add({"copper", "state"},
             "Inspects the internal state",
             &RetroShell::exec <Token::copper, Token::state>);

    root.add({"copper", "list"}, { Arg::value },
             "Prints the Copper list",
             &RetroShell::exec <Token::copper, Token::list>);

    root.add({"paula", ""},
             "Custom Chipset",
             &RetroShell::exec <Token::paula>);

    root.add({"paula", "state"},
             "Inspects the internal state",
             &RetroShell::exec <Token::paula, Token::state>);

    root.add({"denise", ""},
             "Lorem ipsum",
             &RetroShell::exec <Token::denise>);

    root.add({"denise", "state"},
             "Inspects the internal state",
             &RetroShell::exec <Token::denise, Token::state>);

    root.add({"os", "info"},
             "Displays basic system information",
             &RetroShell::exec <Token::os, Token::info>);

    root.add({"os", "execbase"},
             "Displays information about the ExecBase struct",
             &RetroShell::exec <Token::os, Token::execbase>);

    root.add({"os", "interrupts"},
             "Lists all interrupt handlers",
             &RetroShell::exec <Token::os, Token::interrupts>);

    root.add({"os", "libraries"}, { }, { Arg::argument },
             "Lists all libraries",
             &RetroShell::exec <Token::os, Token::libraries>);

    root.add({"os", "devices"}, { }, { Arg::argument },
             "Lists all devices",
             &RetroShell::exec <Token::os, Token::devices>);

    root.add({"os", "resources"}, { }, { Arg::argument },
             "Lists all resources",
             &RetroShell::exec <Token::os, Token::resources>);

    root.add({"os", "tasks"}, { }, { Arg::argument },
             "Lists all tasks",
             &RetroShell::exec <Token::os, Token::tasks>);

    root.add({"os", "processes"}, { }, { Arg::argument },
             "Lists all processes",
             &RetroShell::exec <Token::os, Token::processes>);

    root.add({"os", "catch"}, { Arg::argument },
             "Pauses emulation on task launch",
             &RetroShell::exec <Token::os, Token::cp>);

    root.add({"os", "set"},
             "Configures the component");

    root.add({"os", "set", "diagboard" }, { Arg::boolean },
             "Attaches or detaches the debug expansion board",
             &RetroShell::exec <Token::os, Token::set, Token::diagboard>);
}

}
