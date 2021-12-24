// -----------------------------------------------------------------------------
// This file is part of vAmiga
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#pragma once

#include "RemoteServerTypes.h"
#include "SubComponent.h"
#include "Socket.h"
#include <thread>

class RemoteServer : public SubComponent {
        
    friend class RemoteManager;
    
protected:
    
    // The server thread
    std::thread serverThread;

    // The port number
    isize port = 0;
    
    // The launch arguments (if any)
    std::vector <string> args;
    
    // The current server state
    SrvState state = SRV_STATE_OFF;
    
    // The port listener and it's associated connection
    PortListener listener;
    Socket connection;

    // Number of sent and received packets
    isize numSent = 0;
    isize numReceived = 0;
        
    
    //
    // Initializing
    //
    
public:
    
    RemoteServer(Amiga& ref);
    ~RemoteServer();
    
    
    //
    // Methods from AmigaObject
    //
    
public:
    
    const char *getDescription() const override { return "RemoteServer"; }
    void _dump(dump::Category category, std::ostream& os) const override;
    
    
    //
    // Methods from AmigaComponent
    //
    
private:
    
    void _reset(bool hard) override { }
    
    isize _size() override { return 0; }
    u64 _checksum() override { return 0; }
    isize _load(const u8 *buffer) override {return 0; }
    isize _save(u8 *buffer) override { return 0; }

    
    //
    // Running the server
    //
    
private:
    
    // The main thread function
    void main();

    // Inner loops (called from main)
    void mainLoop() throws;
    void sessionLoop();

    
    //
    // Examining state
    //
    
public:
        
    isize getPort() const { return port; }
    bool isOff() const { return state == SRV_STATE_OFF; }
    bool isLaunching() const { return state == SRV_STATE_LAUNCHING; }
    bool isListening() const { return state == SRV_STATE_LISTENING; }
    bool isConnected() const { return state == SRV_STATE_CONNECTED; }

    
    //
    // Changing state
    //
    
public:
        
    // Launch the remote server
    void start(isize port, const std::vector <string> &args) throws;
    void start() throws { start(_defaultPort(), { }); }
    void start(isize port) throws { start(port, { }); }
    void start(const std::vector <string> &args) throws { start(_defaultPort(), args); }

    // Shuts down the remote server
    void stop() throws;
        
    // Disconnects the client
    void disconnect() throws;
         
private:
    
    // Switches the internal state and informs the GUI
    void switchState(SrvState newState);
    
    
    //
    // Transmitting and processing packets
    //
    
public:
    
    // Receives or transmits a string
    string receive();
    void send(const string &payload);
    
    // Convenience wrappers
    void send(char payload) throws;
    void send(int payload) throws;
    void send(long payload) throws;
    void send(std::stringstream &payload) throws;
    
    // Operator overloads
    RemoteServer &operator<<(char payload) { send(payload); return *this; }
    RemoteServer &operator<<(const string &payload) { send(payload); return *this; }
    RemoteServer &operator<<(int payload) { send(payload); return *this; }
    RemoteServer &operator<<(long payload) { send(payload); return *this; }
    RemoteServer &operator<<(std::stringstream &payload) { send(payload); return *this; }
 
    // Processes a package
    void process(const string &payload) throws;
    
private:
        
    // Reports an error to the GUI
    void handleError(const char *description);
    
    
    //
    // Subclass specific routines
    //

private:
    
    virtual isize _defaultPort() const = 0;
    virtual bool _launchable() = 0;
    virtual void _connect() throws = 0;
    virtual string _receive() throws = 0;
    virtual void _send(const string &payload) throws = 0;
    virtual void _process(const string &payload) throws = 0;
};
