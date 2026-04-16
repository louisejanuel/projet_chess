#include "GameRender.hpp"
#include <imgui.h>
#include <algorithm>
#include <string>

void GameRender::reset()
{
    m_selected_index = -1;
    m_possible_moves.clear();
    m_awaiting_promotion = false;
}

// main render loop for the 2D chessboard
void GameRender::render(Game& game, AmbianceMarkov& ambiance)
{
    ImGui::Begin("ChessBoard", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);

    // cancel selection on right-click
    if (ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
        m_selected_index = -1;
        m_possible_moves.clear();
    }

    draw_board(game, ambiance);

    // display promotion popup if needed
    if (m_awaiting_promotion) {
        ImGui::OpenPopup("Choix Promotion");
    }
    draw_promotion_popup(game, ambiance);

    ImGui::End();
}

// draw the 8x8 grid and the pieces
void GameRender::draw_board(Game& game, AmbianceMarkov& ambiance)
{
    // dynamically scale cell size to fit the panel
    ImVec2 avail          = ImGui::GetContentRegionAvail();
    float  max_board_size = std::min(avail.x, avail.y);
    float  spacing        = ImGui::GetStyle().ItemSpacing.x;
    float  padding        = ImGui::GetStyle().WindowPadding.x;
    float  size           = std::max(10.0f, (max_board_size - spacing * 7.0f - padding * 2.0f) / 8.0f);

    Piece* selected_piece = game.get_board().get_piece(m_selected_index);

    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            int index = y * 8 + x;

            // background colors
            bool   is_dark  = (x + y) % 2 != 0;
            ImVec4 bg_color = is_dark ? ImVec4{0.45f, 0.25f, 0.15f, 1.0f} : ImVec4{0.9f, 0.8f, 0.65f, 1.0f};

            if (index == game.get_collapsing_square_idx())
            {
                bg_color = ImVec4{0.4f, 0.1f, 0.6f, 1.0f};
            }

            if (is_highlighted(index))
            {
                bg_color = ImVec4{0.2f, 0.8f, 0.2f, 1.0f};
                Piece* target_piece = game.get_board().get_piece(index);
                if (selected_piece != nullptr && target_piece != nullptr && selected_piece->get_color() != target_piece->get_color())
                {
                    bg_color = ImVec4{0.8f, 0.2f, 0.2f, 1.0f};
                }
            }

            if (index == m_selected_index)
            {
                bg_color = ImVec4{0.2f, 0.5f, 1.0f, 1.0f};
            }

            ImGui::PushStyleColor(ImGuiCol_Button, bg_color);
            ImGui::PushID(index);

            // piece and texte color
            Piece* p     = game.get_board().get_piece(index);
            std::string label = get_piece_label(p);

            ImVec4 text_color = ImVec4{0.0f, 0.0f, 0.0f, 1.0f};
            if (p != nullptr && p->get_color() == Color::White)
            {
                text_color = ImVec4{1.0f, 1.0f, 1.0f, 1.0f};
            }

            ImGui::PushStyleColor(ImGuiCol_Text, text_color);

            // button display
            if (m_chess_font != nullptr) ImGui::PushFont(m_chess_font);

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

            if (ImGui::Button(label.c_str(), ImVec2{size, size}))
            {
                handle_click(game, index, ambiance);
            }

            ImGui::PopStyleVar();

            if (m_chess_font != nullptr) ImGui::PopFont();

            ImGui::PopStyleColor(2);
            ImGui::PopID();

            // layout
            if (x < 7) ImGui::SameLine();
        }
    }
}

// handle player interactions with the board
void GameRender::handle_click(Game& game, int index, AmbianceMarkov& ambiance)
{
    if (game.get_state() != GameState::Playing)
        return;

    if (is_highlighted(index))
    {
        Piece* p            = game.get_board().get_piece(m_selected_index);
        bool   is_promotion = false;

        if (p != nullptr && (p->get_type() == Type::Pawn || p->get_type() == Type::BerolinaPawn))
        {
            int target_y = index / 8;
            if (target_y == 0 || target_y == 7) is_promotion = true;
        }

        if (is_promotion)
        {
            if (game.get_game_mode() == GameMode::Chaos)
            {
                //chaos
                if (game.play_move(m_selected_index, index)) {
                    ambiance.transition();
                }
                m_selected_index = -1;
                m_possible_moves.clear();
            }
            else
            {
                //classic
                m_awaiting_promotion     = true;
                m_pending_promotion_move = {{m_selected_index % 8, m_selected_index / 8}, {index % 8, index / 8}, Type::None};
            }
        }
        else
        {
            if (game.play_move(m_selected_index, index)) {
                ambiance.transition();
            }
            m_selected_index = -1;
            m_possible_moves.clear();
        }
        return;
    }

    Piece* p = game.get_board().get_piece(index);
    if (p != nullptr && p->get_color() == game.get_current_turn())
    {
        m_selected_index = index;
        m_possible_moves = p->get_available_moves(game.get_board(), {index % 8, index / 8});
    }
    else
    {
        m_selected_index = -1;
        m_possible_moves.clear();
    }
}

// check if a specific cell is a valid destination for the selected piece
bool GameRender::is_highlighted(int index) const
{
    for (const Move& m : m_possible_moves)
    {
        if (m.end.to_index() == index)
        {
            return true;
        }
    }
    return false;
}

// get the unicode character corresponding to the piece type
std::string GameRender::get_piece_label(Piece* p) const
{
    if (p == nullptr)
        return " ";

    switch (p->get_type())
    {
        case Type::Pawn:         return "♟";
        case Type::BerolinaPawn: return "♟";
        case Type::Rook:         return "♜";
        case Type::Knight:       return "♞";
        case Type::Bishop:       return "♝";
        case Type::Queen:        return "♛";
        case Type::King:         return "♚";
        default:                 return " ";
    }
}

void GameRender::draw_promotion_popup(Game& game, AmbianceMarkov& ambiance)
{
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal("Choix Promotion", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("En quoi voulez-vous transformer votre pion ?");
        ImGui::Separator();

        auto select_piece = [&](Type type, const char* label) {
            if (ImGui::Button(label, ImVec2(120, 0)))
            {
                if (game.play_move(m_pending_promotion_move.start.to_index(), m_pending_promotion_move.end.to_index(), type)) {
                    ambiance.transition();
                }

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

        if (ImGui::Button("Annuler", ImVec2(120, 0)))
        {
            m_awaiting_promotion = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}