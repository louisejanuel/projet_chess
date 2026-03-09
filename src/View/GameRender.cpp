#include "GameRender.hpp"
#include <imgui.h>
#include <string>
#include <algorithm>

void GameRender::render(Chessboard& chessboard) {
    float size = 50.0f;
    
    ImGui::Begin("ChessBoard");

    Piece* selected_piece = chessboard.get_piece(m_selected_index);

    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            int index = y * 8 + x;
            
            //colors
            bool is_dark = (x + y) % 2 != 0;
            ImVec4 bg_color = is_dark ? ImVec4{0.45f, 0.25f, 0.15f, 1.0f} : ImVec4{0.9f, 0.8f, 0.65f, 1.0f};

            //highlight possible moves
            if (is_highlighted(index)) {
                bg_color = ImVec4{0.2f, 0.8f, 0.2f, 1.0f}; 

                Piece* target_piece = chessboard.get_piece(index);
                
                if (selected_piece != nullptr && target_piece != nullptr) {
                    if (selected_piece->get_color() != target_piece->get_color()) {
                        bg_color = ImVec4{0.8f, 0.2f, 0.2f, 1.0f};
                    }
                }
            }
            
            if (index == m_selected_index) {
                bg_color = ImVec4{0.2f, 0.5f, 1.0f, 1.0f};
            }

            ImGui::PushStyleColor(ImGuiCol_Button, bg_color);
            ImGui::PushID(index);

            //show piece
            std::string label = " ";
            Piece* p = chessboard.get_piece(index);
            
            if (p != nullptr) {
                if (p->get_type() == Type::Pawn) label = "P";
                else if (p->get_type() == Type::King) label = "K";
                else if (p->get_type() == Type::Queen) label = "Q";
                else if (p->get_type() == Type::Rook) label = "R";
                else if (p->get_type() == Type::Bishop) label = "B";
                else if (p->get_type() == Type::Knight) label = "Kn";
                
                label += (p->get_color() == Color::White) ? " (W)" : " (B)";
            }
            
            //interaction
            if (ImGui::Button(label.c_str(), ImVec2{size, size})) {
                handle_click(chessboard, index);
            }

            ImGui::PopID();
            ImGui::PopStyleColor();

            //grid layout
            if (x < 7) ImGui::SameLine();
        }
    }

    // affichage victoire
    GameState state = chessboard.get_state();
    
    if (state != GameState::Playing) {
        // On saute une ligne pour aérer
        ImGui::Spacing(); 
        ImGui::Separator();
        ImGui::Spacing();

        if (state == GameState::WhiteWins) {
            // Un joli texte vert pour la victoire
            ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "VICTOIRE : Les blancs ont gagne !");
        } 
        else if (state == GameState::BlackWins) {
            ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "VICTOIRE : Les noirs ont gagne !");
        }
    }

    ImGui::End();
}

void GameRender::handle_click(Chessboard& chessboard, int index) {
    if (chessboard.get_state() != GameState::Playing) {
        return;
    }
    //click handling logic
    if (is_highlighted(index)) {
        chessboard.move_piece(m_selected_index, index);
        
        //reset selection after move
        m_selected_index = -1;
        m_possible_moves.clear();
        return;
    }

    //select piece
    Piece* p = chessboard.get_piece(index);
    if (p != nullptr) {
        m_selected_index = index;

        Position currentPos = { index % 8, index / 8 };
        m_possible_moves = p->get_available_moves(chessboard, currentPos);
    } 
    //deselect piece (click on another square)
    else {
        m_selected_index = -1;
        m_possible_moves.clear();
    }
}

bool GameRender::is_highlighted(int index) const {
    for (const Move& move : m_possible_moves) {
        if (move.end.to_index() == index) return true;
    }
    return false;
}