#pragma once
#include <Engine.hpp>

class InterfaceLayer : public Engine::Layer
{
public:
	InterfaceLayer();
	virtual ~InterfaceLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
    virtual void Update() override;
	virtual void OnImGuiRender() override;

private:

};