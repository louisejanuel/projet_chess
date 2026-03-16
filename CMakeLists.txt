cmake_minimum_required(VERSION 3.20)

# Nom du projet (tu peux le changer en "ChessProject" si tu veux)
project(ChessProject)
add_executable(${PROJECT_NAME})

# C++ 20 est requis par le template
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

# --- CONFIGURATION DES RÉPERTOIRES ---

# Définition des dossiers de sortie
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}
    CXX_EXTENSIONS OFF
)

# Inclusion récursive de TOUS les fichiers .cpp dans src/ (Rendering, Board, etc.)
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS src/*.cpp src/*.hpp)
target_sources(${PROJECT_NAME} PRIVATE ${MY_SOURCES})

# Ajout des chemins d'inclusion pour éviter les erreurs "file not found"
target_include_directories(${PROJECT_NAME} PRIVATE 
    src 
    src/Rendering 
    src/Board
    ${CMAKE_SOURCE_DIR}/lib
    ${CMAKE_SOURCE_DIR}/lib/quick_imgui/lib/glad/include
)

# --- BIBLIOTHÈQUES EXTERNES ---

# --- BIBLIOTHÈQUES EXTERNES ---

# 1. Quick-ImGui (déjà configuré)
add_subdirectory(lib/quick_imgui)
target_link_libraries(${PROJECT_NAME} PRIVATE quick_imgui::quick_imgui)

# 2. GLM (Approche manuelle si find_package échoue)
# On cherche GLM dans les dossiers standards de l'université
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/lib)

# 3. GLAD (Pour reconnaître GLuint, GL_ARRAY_BUFFER, etc.)
# Si GLAD est dans ton dossier lib/, utilise add_subdirectory. 
# Sinon, s'il est déjà géré par quick_imgui, cette ligne peut être optionnelle.
# find_package(glad REQUIRED) 
# target_link_libraries(${PROJECT_NAME} PRIVATE glad)

# --- OPTIONS DE COMPILATION ---
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
endif()