#include "GameRender.hpp"
#include <imgui.h>
#include <algorithm>
#include <string>

void GameRender::render(Chessboard& chessboard)
{
    ImGui::Begin("ChessBoard");

    // Clic droit pour annuler la sélection
    if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
        m_selected_index = -1;
        m_possible_moves.clear();
    }

    draw_board(chessboard);

    // affichage popup de promotion
    if (m_awaiting_promotion) {
        ImGui::OpenPopup("Choix Promotion");
    }
    draw_promotion_popup(chessboard);

    // affichage victoire
    GameState state = chessboard.get_state();

    if (state != GameState::Playing)
    {
        // On saute une ligne pour aérer
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        if (state == GameState::WhiteWins)
        {
            // Un joli texte vert pour la victoire
            ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "VICTOIRE : Les blancs ont gagne !");
        }
        else if (state == GameState::BlackWins)
        {
            ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "VICTOIRE : Les noirs ont gagne !");
        }
    }

    ImGui::End();
}

void GameRender::draw_board(Chessboard& chessboard)
{
    float size = 90.0f;
    Piece* selected_piece = chessboard.get_piece(m_selected_index);

    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            int index = y * 8 + x;

            // --- 1. COULEURS DE FOND ---
            bool is_dark = (x + y) % 2 != 0;
            ImVec4 bg_color = is_dark ? ImVec4{0.45f, 0.25f, 0.15f, 1.0f} : ImVec4{0.9f, 0.8f, 0.65f, 1.0f};

            if (is_highlighted(index)) {
                bg_color = ImVec4{0.2f, 0.8f, 0.2f, 1.0f}; // Vert (mouvement possible)
                Piece* target_piece = chessboard.get_piece(index);
                if (selected_piece != nullptr && target_piece != nullptr && selected_piece->get_color() != target_piece->get_color()) {
                    bg_color = ImVec4{0.8f, 0.2f, 0.2f, 1.0f}; // Rouge (capture)
                }
            }

            if (index == m_selected_index) {
                bg_color = ImVec4{0.2f, 0.5f, 1.0f, 1.0f}; // Bleu (sélection)
            }

            ImGui::PushStyleColor(ImGuiCol_Button, bg_color);
            ImGui::PushID(index);

            // --- 2. PIÈCE ET COULEUR DU TEXTE ---
            Piece* p = chessboard.get_piece(index);
            std::string label = get_piece_label(p);

            ImVec4 text_color = ImVec4{0.0f, 0.0f, 0.0f, 1.0f}; // Noir par défaut
            if (p != nullptr && p->get_color() == Color::White) {
                text_color = ImVec4{1.0f, 1.0f, 1.0f, 1.0f}; // Blanc
            }
            
            ImGui::PushStyleColor(ImGuiCol_Text, text_color);

            // --- 3. AFFICHAGE DU BOUTON (AVEC LA POLICE) ---
            if (m_chess_font != nullptr) {
                ImGui::PushFont(m_chess_font);
            }

            if (ImGui::Button(label.c_str(), ImVec2{size, size})) {
                handle_click(chessboard, index);
            }

            if (m_chess_font != nullptr) {
                ImGui::PopFont();
            }

            ImGui::PopStyleColor(2); // Dépile Button_Bg et Text
            ImGui::PopID();

            // --- 4. MISE EN PAGE ---
            if (x < 7) {
                ImGui::SameLine();
            }
        }
    }
}

std::string GameRender::get_piece_label(Piece* p) const
{
    if (p == nullptr) return " ";

    switch (p->get_type()) {
        case Type::Pawn:   return "♟";
        case Type::Rook:   return "♜";
        case Type::Knight: return "♞";
        case Type::Bishop: return "♝";
        case Type::Queen:  return "♛";
        case Type::King:   return "♚";
        default:           return " ";
    }
}


void GameRender::handle_click(Chessboard& chessboard, int index)
{
    if (chessboard.get_state() != GameState::Playing)
    {
        return;
    }

    // click handling logic
    if (is_highlighted(index))
    {
        Piece* p = chessboard.get_piece(m_selected_index);
        bool is_promotion = false;

        // Si c'est un pion qui arrive sur la ligne 0 ou 7
        if (p != nullptr && p->get_type() == Type::Pawn) {
            int target_y = index / 8;
            if (target_y == 0 || target_y == 7) {
                is_promotion = true;
            }
        }

        // Si promotion, on gèle le jeu et on prépare la popup
        if (is_promotion) {
            m_awaiting_promotion = true;
            m_pending_promotion_move = { {m_selected_index % 8, m_selected_index / 8}, {index % 8, index / 8}, Type::None };
        } 
        // Sinon, mouvement normal
        else {
            chessboard.move_piece(m_selected_index, index);
            m_selected_index = -1;
            m_possible_moves.clear();
        }
        return;
    }

    // select piece
    Piece* p = chessboard.get_piece(index);
    if (p != nullptr && p->get_color() == chessboard.get_current_turn())
    {
        m_selected_index = index;

        Position currentPos = {index % 8, index / 8};
        m_possible_moves    = p->get_available_moves(chessboard, currentPos);
    }
    // deselect piece (click on another square)
    else
    {
        m_selected_index = -1;
        m_possible_moves.clear();
    }
}

bool GameRender::is_highlighted(int index) const
{
    for (const Move& move : m_possible_moves)
    {
        if (move.end.to_index() == index)
            return true;
    }
    return false;
}


void GameRender::draw_promotion_popup(Chessboard& chessboard) 
{
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal("Choix Promotion", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("En quoi voulez-vous transformer votre pion ?");
        ImGui::Separator();


        auto select_piece = [&](Type type, const char* label) {
            if (ImGui::Button(label, ImVec2(120, 0))) {
                chessboard.move_piece(m_pending_promotion_move.start.to_index(), m_pending_promotion_move.end.to_index(), type);

                m_selected_index = -1;
                m_possible_moves.clear();
                m_awaiting_promotion = false;
                
                ImGui::CloseCurrentPopup();
            }
        };

        select_piece(Type::Queen, "Reine");
        select_piece(Type::Rook, "Tour");
        select_piece(Type::Bishop, "Fou");
        select_piece(Type::Knight, "Cavalier");

        ImGui::Separator();

        if (ImGui::Button("Annuler", ImVec2(120, 0))) {
            m_awaiting_promotion = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}