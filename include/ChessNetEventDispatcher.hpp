#pragma once

#include <net/NetMessage.hpp>
#include "NetworkTypes.hpp"

class ChessNetMessageListener {
public:
    virtual void HandleNetMessage(const net::Message<ChessMessage>& msg) = 0;
};

class ChessNetMessageDispatcher {
public:
    void AddListener(ChessNetMessageListener* listener)
    {
        listeners.push_back(listener);
    }

    void DispatchMessage(const net::Message<ChessMessage>& msg)
    {
        for (ChessNetMessageListener* listener : listeners)
            listener->HandleNetMessage(msg);
    }

private:
    std::vector<ChessNetMessageListener*> listeners;
};