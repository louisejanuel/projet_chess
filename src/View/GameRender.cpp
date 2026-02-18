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
                else if (p->get_type() == Type::Knight) label = "N";
                
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
    ImGui::End();
}

void GameRender::handle_click(Chessboard& chessboard, int index) {
    //case 1 : click handling logic
    if (is_highlighted(index)) {
        chessboard.move_piece(m_selected_index, index);
        
        //reset selection after move
        m_selected_index = -1;
        m_possible_moves.clear();
        return;
    }

    //case 2 : select piece
    Piece* p = chessboard.get_piece(index);
    if (p != nullptr) {
        // TODO : Vérifier ici si c'est au tour de cette couleur de jouer
        m_selected_index = index;
        m_possible_moves = p->get_available_moves(chessboard, index);
    } 
    //case 3 : deselect piece (click on another square)
    else {
        m_selected_index = -1;
        m_possible_moves.clear();
    }
}

bool GameRender::is_highlighted(int index) const {
    //index is highlighted if it's in the possible moves list
    for (int move : m_possible_moves) {
        if (move == index) return true;
    }
    return false;
}