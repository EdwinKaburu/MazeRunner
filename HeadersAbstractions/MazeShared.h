//
// Created by EdwinK on 6/26/2022.
//

#ifndef AI_MAZE_GRAPHICS_MAZESHARED_H
#define AI_MAZE_GRAPHICS_MAZESHARED_H

#include <vector>

using namespace std;

/**
 * ColorMap() A Representation Of A RGBA Color
 */
struct ColorMap {
    float red = 0.0f;
    float green = 0.0f;
    float blue = 0.0f;
    float alpha = 1.0f;
    ColorMap() = default;
    ColorMap(float r_input, float g_input, float b_input) {
        red = r_input;
        green = g_input;
        blue = b_input;
    }
    ColorMap(const float* rgb_input)
    {
        red = rgb_input[0];
        green = rgb_input[1];
        blue = rgb_input[2];
    }
};

/**
 * Cell Struct() A Representation of A Cell In a Grid
 */
struct Cell {
    int row;
    int col;
    double g_n;
    double h_n;
    double f_n;
};

enum grid_state{
    OPEN_GRID_SPACE,
    CLOSED_GRID_SPACE,
    EXPLORED_GRID_SPACE,
    EXPLORED_BACKTRACK_GRID_SPACE,
};

struct MazeAnimationFrames{
    vector<vector<int>> frame_vector;
};

#endif //AI_MAZE_GRAPHICS_MAZESHARED_H
