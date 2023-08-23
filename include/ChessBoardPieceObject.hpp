#pragma once

#include <Engine.hpp>
#include "ChessTypes.hpp"

class ChessBoardPieceObject : public Engine::GameObject
{
public:
    ChessBoardPieceObject(Engine::Texture* baseTexture, glm::vec3 position = glm::vec3(0.f), glm::vec3 rotation = glm::vec3(0.f), glm::vec3 scale = glm::vec3(1.f));

    virtual void OnMouseDown(int button) override;

    void HighlightFor(MoveType type);
    void Reset();

private:
    bool isHighlighted;

    Engine::Texture *baseTexture;
    Engine::Texture *moveTexture;
    Engine::Texture *attackTexture;

};