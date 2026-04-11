#include "../OpenGL.hpp"


enum class CaseState { Inactive, Hoverable, Selected };

class Case {
public:
    Case(glm::vec3 position, bool isWhite) 
        : m_position(position), m_isWhite(isWhite), m_state(CaseState::Inactive) {}

    void draw(const glm::mat4& viewProj) {
    }

    void setState(CaseState state) { m_state = state; }

private:
    glm::vec3 m_position;
    bool m_isWhite;
    CaseState m_state;
};