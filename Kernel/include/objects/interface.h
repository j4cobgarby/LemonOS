#pragma once

#include <objects/kobject.h>
#include <objects/message.h>

#include <list.h>

class MessageInterface final : public KernelObject{
protected:
    char* name;
    List<FancyRefPtr<MessageEndpoint>> connections;

    uint16_t msgSize;
    lock_t incomingLock = 0;
    List<Pair<bool, FancyRefPtr<MessageEndpoint>>*> incoming;

    friend class Service;
public:
    MessageInterface(const char* _name, uint16_t msgSize);

    /////////////////////////////
    /// \brief Accept an incoming connection
    ///
    /// Creates a message channel and populateds endpoint with a MessageEndpoint
    ///
    /// \return 0 on success, 1 when no incoming connections, negative error code on failure
    /////////////////////////////
    long Accept(FancyRefPtr<MessageEndpoint>& endpoint);

    /////////////////////////////
    /// \brief Connect to interface
    ///
    /// Initiates a connection, blocks until it is accepted by the interface owner and returns a message endpoint. Should never fail.
    ///
    /// \return Pointer to MessageEndpoint
    /////////////////////////////
    FancyRefPtr<MessageEndpoint> Connect();
    
    inline static constexpr kobject_id_t TypeID() { return KOBJECT_ID_INTERFACE; }
    kobject_id_t InstanceTypeID() const { return TypeID(); }
};