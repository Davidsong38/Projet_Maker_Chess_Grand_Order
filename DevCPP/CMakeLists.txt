cmake_minimum_required(VERSION 3.30)
project(Chess_Grand_Order)

set(CMAKE_CXX_STANDARD 20)

add_executable(Chess_Grand_Order
        src/main.cpp

)

add_compile_options(-Wno-int-to-pointer-cast)

target_link_libraries(Chess_Grand_Order
        opengl32
)

add_definitions(
        -DAPIENTRY
        -DGL_GLEXT_PROTOTYPES
)
target_sources(Chess_Grand_Order PRIVATE
        src/third_party/glcorearb.h
)

target_sources(Chess_Grand_Order PRIVATE
        src/game/GameEngine.cpp
        src/game/board/Chessboard.cpp
        src/game/board/piece_loader.cpp
        src/game/characters/characters_List.cpp
        src/game/characters/pieces/Pieces.cpp
        src/game/characters/pieces/pieces_List.cpp
        src/game/characters/pieces/bishop/Bishop.cpp
        src/game/characters/pieces/bishop/Medusa_Saber.cpp
        src/game/characters/pieces/king/King.cpp
        src/game/characters/pieces/king/Artoria.cpp
        src/game/characters/pieces/knight/Knight.cpp
        src/game/characters/pieces/knight/Ushiwakamaru.cpp
        src/game/characters/pieces/pawn/Pawn.cpp
        src/game/characters/pieces/pawn/Nemo_Marine.cpp
        src/game/characters/pieces/queen/Queen.cpp
        src/game/characters/pieces/queen/Sesshoin_Kiara.cpp
        src/game/characters/pieces/rook/Rook.cpp
        src/game/characters/pieces/rook/Arceuid.cpp
        src/game/characters/pieces/bishop/Merlin.cpp
        src/game/characters/pieces/knight/Okita.cpp
        src/game/characters/pieces/rook/Xu_Fu.cpp
        src/game/characters/pieces/queen/Nitocris_Alter.cpp
        src/game/characters/pieces/king/Gilgamesh.cpp
        src/game/effects/effect_List.cpp
        src/game/effects/EffectHandler.cpp
        src/game/inputs/mouseInputs.cpp
        src/game/inputs/Key.cpp
        src/game/inputs/keys.cpp
        src/game/inputs/clickables/ClickableArea.cpp
        src/game/inputs/clickables/CheckBox.cpp
        src/game/inputs/clickables/Button.cpp
        src/game/inputs/clickables/RadioButton.cpp
        src/game/inputs/clickables/clickables.cpp
        src/physics/PhysicsEngine.cpp
        src/rendering/RenderEngine.cpp
        src/rendering/opengl/glFunctions.cpp
        src/rendering/opengl/glRenderer.cpp
        src/rendering/opengl/shading/Shader.cpp
        src/rendering/opengl/shading/shaders.cpp
        src/rendering/opengl/textures/CubeMap.cpp
        src/rendering/opengl/textures/cubemaps.cpp
        src/rendering/opengl/textures/Texture.cpp
        src/rendering/opengl/textures/textures.cpp
        src/rendering/opengl/vao/VAO.cpp
        src/rendering/opengl/vao/vaos.cpp
        src/rendering/opengl/displayables/Sprite.cpp
        src/rendering/opengl/displayables/SpriteWithFilter.cpp
        src/rendering/opengl/displayables/SpriteGroup.cpp
        src/rendering/opengl/ui/uiElements.cpp
        src/utils/logging/log.cpp
        src/utils/file_io/fileIO.cpp
)

if (WIN32)
    target_sources(Chess_Grand_Order PRIVATE
            src/platform/windows.cpp
    )
endif ()

if (UNIX)
    target_sources(Chess_Grand_Order PRIVATE
            src/platform/unix.cpp
    )
endif (UNIX)

target_include_directories(Chess_Grand_Order PRIVATE
        src/game/
        src/game/board/
        src/game/characters/
        src/game/characters/pieces/
        src/game/characters/pieces/bishop/
        src/game/characters/pieces/king/
        src/game/characters/pieces/knight/
        src/game/characters/pieces/pawn/
        src/game/characters/pieces/queen/
        src/game/characters/pieces/rook/
        src/game/effects/
        src/game/inputs/
        src/game/inputs/clickables/
        src/physics/
        src/rendering/
        src/rendering/opengl/
        src/rendering/opengl/shading/
        src/rendering/opengl/textures/
        src/rendering/opengl/vao/
        src/rendering/opengl/displayables/
        src/rendering/opengl/ui/
        src/utils/
        src/utils/config/
        src/utils/logging/
        src/utils/file_io/
        src/platform/
        src/third_party/
        src/third_party/glm/
        src/third_party/glm/gtc/
)

add_custom_target(copy_assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets
)
add_dependencies(Chess_Grand_Order copy_assets)
