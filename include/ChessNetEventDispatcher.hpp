#pragma once

#include <net/NetMessage.hpp>
#include "NetworkTypes.hpp"

class ChessNetMessageListener {
public:
    virtual void HandleNetMessage(const net::Message<ChessMessage>& msg) = 0;
};

class ChessNetMessageDispatcher {
public:
    void AddListener(std::weak_ptr<ChessNetMessageListener> listener)
    {
        listeners.push_back(listener);
    }

    void DispatchMessage(const net::Message<ChessMessage>& msg)
    {
        listeners.erase(std::remove_if(listeners.begin(), listeners.end(),
            [](const std::weak_ptr<ChessNetMessageListener>& wp) { return wp.expired(); }), listeners.end());

        for (auto weakListener : listeners)
            if (auto listener = weakListener.lock())
                listener->HandleNetMessage(msg);
    }

private:
    std::vector<std::weak_ptr<ChessNetMessageListener>> listeners;
};