#pragma once
#include <Engine.hpp>

class ChessBoardLayer : public Engine::Layer
{
public:
	ChessBoardLayer();
	virtual ~ChessBoardLayer() = default;

private:
	glm::vec3 squareColor = { 0.2f, 0.3f, 0.8f };

};