#include "Menu.hpp"
#include "../App.hpp"
#include <imgui.h>

void Menu::draw(App* app)
{
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->GetCenter(), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(500, 300), ImGuiCond_Always);
    
    ImGui::Begin("Nouvelle Partie", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
    
    ImGui::SetWindowFontScale(1.5f);
    ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize("Choisissez votre mode de jeu :").x) * 0.5f);
    ImGui::Text("Choisissez votre mode de jeu :");
    ImGui::SetWindowFontScale(1.0f);
    
    ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

    float btn_width = 300.0f;
    float btn_height = 60.0f;
    ImGui::SetCursorPosX((ImGui::GetWindowSize().x - btn_width) * 0.5f);
    
    if (ImGui::Button("Mode Classique", ImVec2(btn_width, btn_height))) {
        app->start_game(GameMode::Classic);
    }
    
    ImGui::Spacing(); ImGui::Spacing();
    ImGui::SetCursorPosX((ImGui::GetWindowSize().x - btn_width) * 0.5f);
    
    if (ImGui::Button("Mode Chaos", ImVec2(btn_width, btn_height))) {
        app->start_game(GameMode::Chaos);
    }
    
    ImGui::End();
}