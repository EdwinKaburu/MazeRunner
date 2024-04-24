cmake_minimum_required(VERSION 3.22)
project(AI_MAZE_GRAPHICS)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_BUILD_TYPE Debug)

include_directories(Libraries/include)
include_directories(HeadersAbstractions)
include_directories(HeadersAbstractions/MazeHeadersAI)
include_directories(Libraries/lib)

include_directories(imgui/ImguiHeaders)
include_directories(imgui/ImguiSource)


link_directories(Libraries/include)
link_directories(HeadersAbstractions)
link_directories(Libraries/lib)

link_directories(imgui/ImguiHeaders)
link_directories(imgui/ImguiSource)


add_executable(AI_MAZE_GRAPHICS main.cpp glad.c stb.cpp ShaderClass.cpp EBO.cpp VBO.cpp VAO.cpp MazePrimitiveGraphics.cpp
        MazeGenerator.cpp MazeRunnerHeuristics.cpp MazeRunner.cpp imgui/ImguiSource/imgui.cpp
        imgui/ImguiSource/imgui_impl_glfw.cpp imgui/ImguiSource/imgui_impl_opengl3.cpp imgui/ImguiSource/imgui_draw.cpp
        imgui/ImguiSource/imgui_tables.cpp imgui/ImguiSource/imgui_widgets.cpp)

find_package(OpenGL REQUIRED)

target_link_libraries(AI_MAZE_GRAPHICS OpenGL::GL)
target_link_libraries(AI_MAZE_GRAPHICS glfw3)