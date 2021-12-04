// -----------------------------------------------------------------------------
// This file is part of vAmiga
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#include "config.h"
#include "GdbServer.h"
#include "IOUtils.h"
#include "MemUtils.h"
#include "MsgQueue.h"
#include "RetroShell.h"
#include <unistd.h>

GdbServer::GdbServer(Amiga& ref) : SubComponent(ref)
{
}

void
GdbServer::_dump(dump::Category category, std::ostream& os) const
{
    using namespace util;
    
    if (category & dump::Config) {
        
        os << tab("Port");
        os << dec(config.port) << std::endl;
        os << tab("Verbose");
        os << bol(config.verbose) << std::endl;
    }
    
    if (category & dump::State) {
        
        os << tab("Port") << dec(port) << std::endl;
    }
}

GdbServerConfig
GdbServer::getDefaultConfig()
{
    GdbServerConfig defaults;

    defaults.port = 8080;
    defaults.verbose = true;

    return defaults;
}

void
GdbServer::resetConfig()
{
    auto defaults = getDefaultConfig();
    
    setConfigItem(OPT_GDB_PORT, defaults.port);
    setConfigItem(OPT_GDB_VERBOSE, defaults.verbose);
}

i64
GdbServer::getConfigItem(Option option) const
{
    switch (option) {
            
        case OPT_GDB_PORT:       return config.port;
        case OPT_GDB_VERBOSE:    return config.verbose;
            
        default:
            fatalError;
    }
}

void
GdbServer::setConfigItem(Option option, i64 value)
{
    switch (option) {
            
        case OPT_GDB_PORT:
            
            config.port = (isize)value;
            return;
                        
        case OPT_GDB_VERBOSE:
            
            config.verbose = (bool)value;
            return;

        default:
            fatalError;
    }
}

void
GdbServer::start()
{
    debug(GDB_DEBUG, "start\n");
    
    // Error out if the server is already running
    if (this->port) throw VAError(ERROR_GDB_RUNNING);
        
    port = config.port;
    ackMode = true;
    
    // Spawn a new thread
    if (serverThread.joinable()) serverThread.join();
    serverThread = std::thread(&GdbServer::main, this);
}

void
GdbServer::stop()
{
    debug(GDB_DEBUG, "stop\n");
 
    if (port) {
        
        // Trigger an exception inside the server thread
        connection.close();
        listener.close();
        
        // Wait until the server thread has terminated
        serverThread.join();
        
        debug(GDB_DEBUG, "stopped\n");
    }
}

string
GdbServer::receive()
{
    auto packet = connection.recv();

    debug(GDB_DEBUG, "Received %s\n", packet.c_str());

    if (config.verbose) {
        
        retroShell << packet << '\n';
    }

    msgQueue.put(MSG_GDB_RECEIVE);
    return packet;
}

void
GdbServer::send(const string &cmd)
{
    string packet = "$";
                
    packet += cmd;
    packet += "#";
    packet += checksum(cmd);
    
    connection.send(packet);
    msgQueue.put(MSG_GDB_SEND);
}

void
GdbServer::main()
{
    debug(GDB_DEBUG, "main\n");

    try {
        
        // Create a port listener
        listener = PortListener(port);
        
        // Wait for a client to connect
        connection = listener.accept();
        
        debug(GDB_DEBUG, "Entering main loop\n");
        msgQueue.put(MSG_GDB_START);

        while (1) {
            
            auto cmd = receive();
            process(cmd);
        }
                
    } catch (...) {
        
        port = 0;
        connection.close();
        listener.close();
        
        msgQueue.put(MSG_GDB_STOP);
        debug(GDB_DEBUG, "Leaving main\n");
    }
}

string
GdbServer::checksum(const string &s)
{
    uint8_t chk = 0;
    for(auto &c : s) chk += (uint8_t)c;

    return util::hexstr <2> (chk);
}

std::vector<string>
GdbServer::split(const string &s, char delimiter)
{
    std::stringstream ss(s);
    std::vector<std::string> result;
    string substr;
    
    while(std::getline(ss, substr, delimiter)) {
        result.push_back(substr);
    }
    
    return result;
}