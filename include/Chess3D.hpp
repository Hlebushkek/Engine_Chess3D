#pragma once

#include <Engine.hpp>
#include "ChessClient.hpp"

class Chess3D : public Engine::Application
{
public:
	Chess3D(const char* title, const int width, const int height);
	virtual ~Chess3D() = default;

	ChessClient client;

private:

};