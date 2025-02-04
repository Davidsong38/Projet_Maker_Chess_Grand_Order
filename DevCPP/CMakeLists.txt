cmake_minimum_required(VERSION 3.30)
project(LTR_Engine_II)

set(CMAKE_CXX_STANDARD 20)

add_executable(LTR_Engine_II
        src/main.cpp
)

add_compile_options(-Wno-int-to-pointer-cast)

target_link_libraries(LTR_Engine_II
        opengl32
)

add_definitions(
        -DAPIENTRY
        -DGL_GLEXT_PROTOTYPES
)
target_sources(LTR_Engine_II PRIVATE
        src/third_party/glcorearb.h
)

target_sources(LTR_Engine_II PRIVATE
        src/game/GameEngine.cpp
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
        src/rendering/opengl/ui/uiElements.cpp
        src/utils/logging/log.cpp
        src/utils/file_io/fileIO.cpp
)

if (WIN32)
    target_sources(LTR_Engine_II PRIVATE
            src/platform/windows.cpp
    )
endif ()

if (UNIX)
    target_sources(LTR_Engine_II PRIVATE
            src/platform/unix.cpp
    )
endif (UNIX)

target_include_directories(LTR_Engine_II PRIVATE
        src/game/
        src/physics/
        src/rendering/
        src/rendering/opengl/
        src/rendering/opengl/shading/
        src/rendering/opengl/textures/
        src/rendering/opengl/vao/
        src/rendering/opengl/displayables/
        src/rendering/opengl/ui/
        src/utils/
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
add_dependencies(LTR_Engine_II copy_assets)
