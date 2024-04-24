//
// Created by EdwinK on 6/26/2022.
//

#ifndef AI_MAZE_GRAPHICS_MAZEPRIMITIVEGRAPHICS_H
#define AI_MAZE_GRAPHICS_MAZEPRIMITIVEGRAPHICS_H

#include "vector"
#include "MazeShared.h"
#include <glad/glad.h>

using namespace std;

class MazePrimitiveGraphics{
private:
    unsigned short int DIM;
    unsigned short int VERTICES_OFFSET;
    float DIFFERENCE;
    float GRID_SPACER;
    float WEIGHTED_DIFFERENCE;

    // Default Colors
    ColorMap BLACK_COLOR;

    vector<GLfloat> Grid_Vertices = {};
    vector<GLuint> Grid_Triangle_Indices = {};

    float CountDigit(long long n);
    vector<int> GetNeighboursCoordinates(int cur_global_place_index, int sym_point_offset);

    vector<GLuint> GetPrimitiveShape(int primitive_shape_num);

    void ModifyPrimitiveColor(const vector<GLuint> &triangle_coordinates, ColorMap color_map, bool multiply_offset);

    vector<GLuint> GetPrimitiveIndices(int row, int col);

public:
    MazePrimitiveGraphics(unsigned short dim_input, unsigned short vertices_offset_input);

    vector<GLfloat> GetTriangleVertices();
    vector<GLuint> GetTriangleIndices();

    void GenerateGridPrimitive();
    void DefaultChessBoard();
    void UpdateAnimation(int row, int col, ColorMap color_input);

    void GenerateMazeAnimationFrame(const vector<vector<int>>& generated_maze, vector<ColorMap>& maze_composition);

    virtual ~MazePrimitiveGraphics();

};

#endif //AI_MAZE_GRAPHICS_MAZEPRIMITIVEGRAPHICS_H
