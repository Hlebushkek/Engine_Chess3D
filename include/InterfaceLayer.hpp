#pragma once
#include <Engine.hpp>

class InterfaceLayer : public Engine::Layer
{
public:
	InterfaceLayer();
	virtual ~InterfaceLayer() = default;

	virtual void OnImGuiRender() override;
private:
	Engine::Button buttonGit;
	bool isWindow1Open; 
	bool isWindow2Open;
};