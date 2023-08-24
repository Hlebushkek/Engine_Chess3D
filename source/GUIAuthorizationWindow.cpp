#include "GUIAuthorizationWindow.hpp"

GUIAuthorizationWindow::GUIAuthorizationWindow()
    : GUIWindow("Authorization", true, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize),
      m_errorText(), m_inputEmail("##email", 32), m_inputUser("##user", 32), m_inputPassword("##pass", 32)
{
    m_inputEmail.SetTitle("max " + std::to_string(m_inputEmail.GetBufferSize() - 1) + m_inputEmail.GetTitle());
    m_inputUser.SetTitle("max " + std::to_string(m_inputUser.GetBufferSize() - 1) + m_inputUser.GetTitle());
    m_inputPassword.SetTitle("max " + std::to_string(m_inputUser.GetBufferSize() - 1) + m_inputPassword.GetTitle());
    m_inputPassword.hideInput = true;
    m_errorText.visible = false;
}

void GUIAuthorizationWindow::InnerRender()
{
    m_errorText.Render();

    if (m_state != AuthorizationState::Login)
    {
        ImGui::Text("Email: ");
        m_inputEmail.Render();
    }

    ImGui::Text("User: ");
    m_inputUser.Render();
    ImGui::Text("Password: ");
    m_inputPassword.Render();

    bool showPass = !m_inputPassword.hideInput;
    ImGui::Checkbox("Show Password", &showPass);
    m_inputPassword.hideInput = !showPass;

    if (m_state != AuthorizationState::Authorized)
    {
        if (ImGui::Button(m_state == AuthorizationState::Register ? "Register" : "Login"))
        {
            switch (m_state)
            {
            case AuthorizationState::Register:
                if (auto sharedDelegate = delegate.lock())
                    sharedDelegate->OnRegister(m_inputEmail.GetText(), m_inputUser.GetText(), m_inputPassword.GetText());
                break;
            case AuthorizationState::Login:
                if (auto sharedDelegate = delegate.lock())
                    sharedDelegate->OnLogin(m_inputUser.GetText(), m_inputPassword.GetText());
                break;
            default:
                break;
            }
        }

        ImGui::SetCursorPos(ImVec2(8, ImGui::GetWindowHeight() - 32));
        if(ImGui::Button(m_state == AuthorizationState::Register ? "Back to Login" : "Create a new account"))
            SetState(m_state == AuthorizationState::Register ? AuthorizationState::Login : AuthorizationState::Register);
    }
    else
    {
        if (ImGui::Button("Logout"))
        {
            if (auto sharedDelegate = delegate.lock())
                sharedDelegate->OnLogout();

            SetState(AuthorizationState::Login);
        }
    }
    
    GUIWindow::InnerRender();
}

void GUIAuthorizationWindow::PreRenderSetup()
{
    ImGui::SetNextWindowSize(ImVec2(250,300));
}

void GUIAuthorizationWindow::SetAuthorizationSuccess()
{
    m_errorText.visible = false;
    SetState(AuthorizationState::Authorized);
}

void GUIAuthorizationWindow::SetAuthorizationError(std::string errorMsg)
{
    m_errorText.SetText(errorMsg);
    m_errorText.visible = true;
}

void GUIAuthorizationWindow::SetState(AuthorizationState state)
{
    m_state = state;

    m_inputEmail.visible = m_state != AuthorizationState::Login;
    m_inputEmail.readOnly = m_inputUser.readOnly = m_inputPassword.readOnly = m_state == AuthorizationState::Authorized;
}
