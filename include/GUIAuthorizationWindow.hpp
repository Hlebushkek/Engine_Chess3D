#pragma once

#include <Engine.hpp>

class GUIAuthorizationWindowDelegate
{
public:
    virtual void OnLogin(const std::string& user, const std::string& password) = 0;
    virtual void OnRegister(const std::string& email, const std::string& user, const std::string& password) = 0;
    virtual void OnLogout() = 0;
};

class GUIAuthorizationWindow : public Engine::GUIWindow
{
private:
    enum class AuthorizationState { Register, Login, Authorized };

public:
    GUIAuthorizationWindow();

    void SetAuthorizationSuccess();
    void SetAuthorizationError(std::string errorMsg);

    void InnerRender() override;

public:
    std::weak_ptr<GUIAuthorizationWindowDelegate> delegate;

protected:
    void PreRenderSetup() override;

private:
    void SetState(AuthorizationState state);

private:
    Engine::GUIInputText m_inputEmail;
    Engine::GUIInputText m_inputUser;
    Engine::GUIInputText m_inputPassword;

    AuthorizationState m_state = AuthorizationState::Login;

};