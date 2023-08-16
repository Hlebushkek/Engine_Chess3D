#pragma once

#include "NetworkTypes.hpp"
#include "ChessGameInterface.hpp"

class ChessGameOnline : public ChessGameInterface
{
public:
    ChessGameOnline(const Lobby& lobby);

    uint32_t GetID() const { return this->m_id; }

protected:
    uint32_t m_id;

};