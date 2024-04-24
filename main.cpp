// Edwin Kaburu
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ShaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "MazePrimitiveGraphics.h"
#include "MazeGenerator.h"
#include "MazeRunnerHeuristics.h"

using namespace std;

const unsigned int WIDTH = 1000;
const unsigned int HEIGHT = 900;

const unsigned short int DIM = 65;
const unsigned short int VERTICES_OFFSET = 6;

Cell START_POINT = {(DIM / 2), (DIM / 2)};
Cell END_POINT = {0,0};

vector<GLfloat> Grid_Vertices = {};
vector<GLuint> Grid_Triangle_Indices = {};
vector<MazeAnimationFrames> ANIMATION_FRAMES = {};

void PrintMatrix(vector<vector<int>> &matrix) {
    for (auto &row: matrix) {
        for (int col = 0; col < row.size(); col++) {
            cout << row.at(col) << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}

void UpdateGraphicBuffer(MazePrimitiveGraphics *sp, VBO &VBO1, VAO &VAO1) {
    Grid_Vertices = sp->GetTriangleVertices();
    Grid_Triangle_Indices = sp->GetTriangleIndices();

    // Update VBO
    VBO1.UpdateBuffers(Grid_Vertices, Grid_Vertices.size() * sizeof(GLfloat));

    // Link VBO Again to VAO
    // Links VBO to VAO
    // Links VBO attributes such as coordinates and colors or textures to the VAO
    // Coordinates
    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void *) 0);
    //Colors
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void *) (3 * sizeof(float)));
}

void RunMazeHeuristicsRunner(vector<vector<int>>& generated_maze)
{
    // ---------- Heuristics AI runner Solver Stage --------------------
    // Pass the Generated Maze To the Heuristics, A * runner solver AI
    MazeRunnerHeuristics heuristics_maze(generated_maze, START_POINT, END_POINT, ANIMATION_FRAMES);
    // Get a solved maze with an optimal path
    generated_maze = heuristics_maze.GetSolvedMatrix();
    cout << "\nHeuristic Result, '2' represent Path Traversed\nSteps: " << heuristics_maze.num_levels
         << "\nTime in Microseconds: " << heuristics_maze.total_time << "\n";
    PrintMatrix(generated_maze);
    // ---------- End of Heuristics AI runner Solver Stage --------------------
}

void ToggleMazeGenerator()
{
    // Complete Deletion of Animation Frame
    ANIMATION_FRAMES.clear();

    vector<vector<int>> generated_maze = {};
    MazeGenerator MAZE_OBJECT(DIM);
    generated_maze = MAZE_OBJECT.GetMazeMatrix();
    PrintMatrix(generated_maze);

    MAZE_OBJECT.GenerateRandomMaze(START_POINT);

    cout << "\nGenerated Randomized Maze Result\n";
    generated_maze = MAZE_OBJECT.GetMazeMatrix();
    PrintMatrix(generated_maze);
    // Get The End Point
    END_POINT = MAZE_OBJECT.GetEndPoint(START_POINT);
    cout << "\nStarting Point: [" << START_POINT.row << ", " << START_POINT.col << "]\t Ending Point: ["
         << END_POINT.row << ", " << END_POINT.col << "]\n";

    ANIMATION_FRAMES.push_back({generated_maze});

    RunMazeHeuristicsRunner(generated_maze);

    // ---------- Heuristics AI runner Solver Stage --------------------
    // Pass the Generated Maze To the Heuristics, A * runner solver AI
    // MazeRunnerHeuristics heuristics_maze(generated_maze, START_POINT, END_POINT, ANIMATION_FRAMES);
    // Get a solved maze with an optimal path
    //    generated_maze = heuristics_maze.GetSolvedMatrix();
    //    cout << "\nHeuristic Result, '2' represent Path Traversed\nSteps: " << heuristics_maze.num_levels
    //         << "\nTime in Microseconds: " << heuristics_maze.total_time << "\n";
    //    PrintMatrix(generated_maze);
    // ---------- End of Heuristics AI runner Solver Stage --------------------
}

int main() {

    MazePrimitiveGraphics sp(DIM, VERTICES_OFFSET);
    sp.GenerateGridPrimitive();

    Grid_Vertices = sp.GetTriangleVertices();
    Grid_Triangle_Indices = sp.GetTriangleIndices();

    const int INDICES_SIZE = Grid_Triangle_Indices.size(); //((DIM * DIM) * 2) * INDICES_OFFSET;
    const int VERTICES_SIZE = Grid_Vertices.size();

    cout << "Num of Indices: " << INDICES_SIZE << "\nNum of Vertices: " << VERTICES_SIZE << "\n";

    // Initialize GLFW
    glfwInit();

    // Informs GLFW version of OpenGl. Core is Modern Functionalities not Legacy
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create A Glfw Window with 700 by 700 pixels
    GLFWwindow *window = glfwCreateWindow((WIDTH + 500), (HEIGHT + 20), "AI Maze Project", NULL, NULL);

    if (window == NULL) {
        cout << "Window Failed To Create GLFW Window\n";
        return -1;
    }

    // Set the Glfw Window as Current Context
    glfwMakeContextCurrent(window);
    // Load Glad to configure OpenGl
    gladLoadGL();

    // Specify Viewport of Opengl in the Window
    // ViewPort goes from x = 0, y = 0, to x = 700 , y = 700
    glViewport(0, 0, WIDTH, HEIGHT);

    ShaderClass Shader_Program("../Shaders/default.vert", "../Shaders/default.frag");

    // Generates Vertex Array Object and binds it
    VAO VAO1;
    VAO1.Bind();

    // Generates Vertex Buffer Object and links it to vertices
    VBO VBO1(Grid_Vertices, Grid_Vertices.size() * sizeof(GLfloat));
    // Generates Element Buffer Object and links it to indices
    EBO EBO1(Grid_Triangle_Indices, Grid_Triangle_Indices.size() * sizeof(GLuint));

    // Links VBO to VAO
    // Links VBO attributes such as coordinates and colors or textures to the VAO
    // Coordinates
    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void *) 0);
    //Colors
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void *) (3 * sizeof(float)));

    // Unbind all to prevent accidentally modifying them
    VAO1.UnBind();
    VBO1.UnBind();
    EBO1.UnBind();

    // Distinguish which Triangle To Render Onto/From the Other Triangles
    glEnable(GL_DEPTH_TEST);

    ToggleMazeGenerator();

    int frame_number = 0, slow_down = 1;
    int new_start_point[2] = {START_POINT.row,START_POINT.col};
    bool toggle_animation = false, reset_animation = false;

    float DefaultStartPointColor[3] = {0.0f, 0.0f, 1.0f};
    float DefaultEndPointColor[3] = {1.0f, 0.0f, 0.83f};

    float DefaultOpenColor[3] = {1.0f, 1.0f, 1.0f};
    float DefaultCloseColor[3] = {0.0f, 0.0f, 0.0f};
    float DefaultExploredColor[3] = {0.0f, 1.0f, 0.0f};
    float DefaultBackTrackColor[3] = {1.0f, 0.62f, 0.0f};

    vector<ColorMap> maze_composition_color = {DefaultOpenColor, DefaultCloseColor, DefaultExploredColor, DefaultBackTrackColor};

    // ImGui Initialization
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Main While Loop
    while (!glfwWindowShouldClose(window)) {

        // Specify Color of Background
        glClearColor(0.07f, 0.13f, 0.17f, 1);
        // Clean Back Buffer and overwrite with new Color
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Tell ImGui We are Working On A New Frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (!io.WantCaptureMouse) {

        }

        // --------------------- OPENGL Drawing --------------------------
        // Inform Opengl With Shader Program To Use
        Shader_Program.Activate();

        // Bind VAO, so OpenGL knows to use it
        VAO1.Bind();
        // Draw glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, Grid_Triangle_Indices.size(), GL_UNSIGNED_INT, 0);

        ImGui::Begin("AI Runner Controller Window",NULL,ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Text("Select Animation Settings"); ImGui::SameLine(); ImGui::Text(":DIMS: %d", DIM);
        ImGui::Checkbox("Toggle Animation", &toggle_animation);
        ImGui::SliderInt("Slow Down", &slow_down, 1, 100);
        ImGui::SliderInt("Frame Number", &frame_number, 0.0f, (ANIMATION_FRAMES.size() - 1.0f));

        ImGui::Text("\nSelect Maze Composition Colors");
        ImGui::ColorEdit3("Starting Point Color", DefaultStartPointColor);
        ImGui::ColorEdit3("End Point Color", DefaultEndPointColor);
        ImGui::Spacing();
        ImGui::ColorEdit3("Open Space Color", DefaultOpenColor);
        ImGui::ColorEdit3("Wall Space Color", DefaultCloseColor);
        ImGui::ColorEdit3("Explored Space Color", DefaultExploredColor);
        ImGui::ColorEdit3("BackTracked Color", DefaultBackTrackColor);

        ImGui::Text("\nStarting Point");
        ImGui::InputInt2("[R,C]",new_start_point); ImGui::SameLine(); ImGui::Text("Enacted On Random Button Click");

        ImGui::Text("\nButton Controls");

        if (ImGui::Button("New Random Maze"))
        {
            if (new_start_point[0] < DIM && new_start_point[1] < DIM)
            {
                START_POINT.row = new_start_point[0];
                START_POINT.col = new_start_point[1];
            }
            ToggleMazeGenerator();
            frame_number = 0;
        }

        ImGui::SameLine();
        reset_animation =  ImGui::Button("Restart Animation");

        double fps = ImGui::GetIO().Framerate;
        ImGui::Text("Current Fps: %f", fps);

        ImGui::End();

        // Render The UI Elements
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap Back Buffer with the Front Buffer
        glfwSwapBuffers(window);
        // Handle Events
        glfwPollEvents();

        // Start Point Color
        ColorMap StartSpecifiedColor(DefaultStartPointColor);
        // End Point Color
        ColorMap EndSpecifiedColor(DefaultEndPointColor);

        maze_composition_color = { DefaultOpenColor, DefaultCloseColor, DefaultExploredColor, DefaultBackTrackColor};

        // Update The Buffers
        // Print the Generated Matrix, Add Traversed Color
        if (frame_number < ANIMATION_FRAMES.size()) {
            sp.GenerateMazeAnimationFrame(ANIMATION_FRAMES.at(frame_number).frame_vector, maze_composition_color);

            if (!toggle_animation) {
                frame_number += 1;
            }

            // End Point Color
            sp.UpdateAnimation(END_POINT.row, END_POINT.col, EndSpecifiedColor);

            UpdateGraphicBuffer(&sp, VBO1, VAO1);
        } else {

            // Set The Starting Point As Blue
            sp.UpdateAnimation(START_POINT.row, START_POINT.col, StartSpecifiedColor);

            // Set The End Point As Pink
            sp.UpdateAnimation(END_POINT.row, END_POINT.col, EndSpecifiedColor);

            UpdateGraphicBuffer(&sp, VBO1, VAO1);
        }

        // Unbind all to prevent accidentally modifying them
        VAO1.UnBind();

        // --------------------- End of OPENGL Drawing --------------------------
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS || reset_animation) {
            // Repeat the Matrix Again
            frame_number = 0;
            reset_animation = false;
        }
        // Sleep For 2 seconds
        this_thread::sleep_for(chrono::milliseconds(slow_down));
    }

    // Delete All ImGui Objects or End ImGui Processes
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // Delete All Objects
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    Shader_Program.Delete();
    // Destroy Glfw Window before GLFW termination
    glfwDestroyWindow(window);
    // Terminates GLFW before program termination
    glfwTerminate();
    return 0;
}
