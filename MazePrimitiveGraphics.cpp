//
// Created by EdwinK on 6/26/2022.
//

#include "MazePrimitiveGraphics.h"

// -------------- Public Methods ---------------------//

MazePrimitiveGraphics::MazePrimitiveGraphics(unsigned short int dim_input, unsigned short int vertices_offset_input): DIM(dim_input),VERTICES_OFFSET(vertices_offset_input) {
    this->DIFFERENCE = (1.0f / DIM) * 2.0f;
    // Calculate Space Between Grid Boxes
    this->GRID_SPACER = DIFFERENCE * 0.05f;
    this->WEIGHTED_DIFFERENCE = DIFFERENCE - GRID_SPACER;
}


vector<GLfloat> MazePrimitiveGraphics::GetTriangleVertices() {
    return Grid_Vertices;
}

vector<GLuint> MazePrimitiveGraphics::GetTriangleIndices() {
    return Grid_Triangle_Indices;
}

void MazePrimitiveGraphics::GenerateGridPrimitive() {

    vector<float> default_coordinate_values = {};
    vector<int> default_indices_value = {};
    vector<int> get_neighbour_coord = {};

    float starting_x = -1.0f, starting_y = 1.0f;

    bool begin_indices = false, x_axis_spacer = false, y_axis_space = false;

    int global_index = 0, global_indices_index = 0, symmetric_point_offset = -1;

    while (starting_y > -0.99999) {
        float inner_x_axis = starting_x;

        while (inner_x_axis < 0.99999) {
            // Get Index-Offset place of this coordinate in the Vertices
            int current_placement_index = global_index * VERTICES_OFFSET;
            // Form This Coordinate With Default Colors
            default_coordinate_values = {inner_x_axis, starting_y, 0.0f, BLACK_COLOR.red, BLACK_COLOR.green, BLACK_COLOR.blue};

            // Insert Coordinates To Vertices List/Vector
            Grid_Vertices.insert(Grid_Vertices.begin() + Grid_Vertices.size(),
                                 default_coordinate_values.begin(), default_coordinate_values.end());

            // Condition Criteria To Form Rectangle Triangle
            if (begin_indices && x_axis_spacer) {
                // Get This Vertices Coordinate Three Neighbours
                get_neighbour_coord = GetNeighboursCoordinates(current_placement_index, symmetric_point_offset);

                // Link Top Left and Bottom Left Points/Vertices
                default_indices_value = {current_placement_index / VERTICES_OFFSET, get_neighbour_coord.at(1) / VERTICES_OFFSET,
                                         get_neighbour_coord.at(2) / VERTICES_OFFSET};

                // Insert Indices To Indices List/Vector
                Grid_Triangle_Indices.insert(Grid_Triangle_Indices.begin() + Grid_Triangle_Indices.size(),
                                             default_indices_value.begin(), default_indices_value.end());

                global_indices_index += 1;

                // Link Top Symmetric Point and Top Left Point
                default_indices_value = {current_placement_index / VERTICES_OFFSET, get_neighbour_coord.at(0) / VERTICES_OFFSET,
                                         get_neighbour_coord.at(1) / VERTICES_OFFSET};
                Grid_Triangle_Indices.insert(Grid_Triangle_Indices.begin() + Grid_Triangle_Indices.size(),
                                             default_indices_value.begin(), default_indices_value.end());
                global_indices_index += 1;

            }

            // Next Vertices Coordinate
            global_index += 1;

            if (x_axis_spacer) {
                // True For Spacer Need in X-Axis
                inner_x_axis = inner_x_axis + GRID_SPACER;
                x_axis_spacer = false;
            } else {
                // No Space Needed
                inner_x_axis += WEIGHTED_DIFFERENCE;
                x_axis_spacer = true;
            }
        }

        if (y_axis_space) {
            // Spacer Needed
            starting_y = starting_y - GRID_SPACER;
            // Form Indices To False
            begin_indices = false;
            // Invert Boolean Condition
            y_axis_space = false;
        } else {
            // No Spacer Needed
            // Decrement Down A Level by Wighted Difference
            starting_y -= WEIGHTED_DIFFERENCE;

            // Form Indices To True
            begin_indices = true;

            // Capture Symmetric Point Offset Once
            if (symmetric_point_offset < 0) {
                // Difference Between Two Perpendicular Elements in different two rows
                symmetric_point_offset = global_index * VERTICES_OFFSET;
            }
            // Invert Boolean Condition
            y_axis_space = true;

        }
    }
}

void MazePrimitiveGraphics::DefaultChessBoard() {
    // Default White and Purple Color
    ColorMap WHITE_COLOR(1.0f, 1.0f, 1.0f);
    ColorMap PURPLE_COLOR(0.87f, 0.0f, 1.0f);

    bool isWhite = true;
    vector<GLuint> primitive_shape = {};

    int counter = 0, i = 0;
    while (counter < (DIM * DIM)) {
        // Get Indices That Makes Up A Rectangle
        // i = counter * 6;
        // Primitive Shape
        /*primitive_shape = {Grid_Triangle_Indices[i], Grid_Triangle_Indices[(i + 1)], Grid_Triangle_Indices[(i + 2)],
                           Grid_Triangle_Indices[(i + 3)], Grid_Triangle_Indices[(i + 4)],
                           Grid_Triangle_Indices[(i + 5)]};*/
        primitive_shape = GetPrimitiveShape(counter);

        if (!isWhite) {
            ModifyPrimitiveColor(primitive_shape, WHITE_COLOR, true);
            isWhite = true;
        } else {
            ModifyPrimitiveColor(primitive_shape, PURPLE_COLOR, true);
            isWhite = false;
        }
        counter += 1;
    }
}

void MazePrimitiveGraphics::UpdateAnimation(int row, int col, ColorMap color_input) {

    // Get Primitive Shape's indices of Box/Grid at row and col
    vector<GLuint> primitive_shape = GetPrimitiveIndices(row, col);

    // Update the Colors Of Primitive
    ModifyPrimitiveColor(primitive_shape, color_input, true);
}

void MazePrimitiveGraphics::GenerateMazeAnimationFrame(const vector<vector<int>>& generated_maze,  vector<ColorMap>& maze_composition) {

    for(int row = 0; row < DIM; row++)
    {
        for(int col = 0; col < DIM; col++)
        {
           int val1 = generated_maze.at(row).at(col);

            switch (val1) {
                case OPEN_GRID_SPACE:
                    UpdateAnimation(row, col, maze_composition.at(OPEN_GRID_SPACE));
                    continue;
                case CLOSED_GRID_SPACE:
                    // Wall
                    UpdateAnimation(row, col, maze_composition.at(CLOSED_GRID_SPACE));
                    continue;
                case EXPLORED_GRID_SPACE:
                    // Explored Grid
                    UpdateAnimation(row, col, maze_composition.at(EXPLORED_GRID_SPACE));
                    continue;
                case EXPLORED_BACKTRACK_GRID_SPACE:
                    // Explored Grid Space, But No Solution Found
                    UpdateAnimation(row, col, maze_composition.at(EXPLORED_BACKTRACK_GRID_SPACE));
                    continue;
                default:
                    // Error Code
                    continue;
            }
        }
    }
}


// -----------------------  Private Method --------------------------- //

float MazePrimitiveGraphics::CountDigit(long long int n) {
    if (n / 10 == 0)
        return 1;
    return 1 + CountDigit(n / 10);
}

vector<int> MazePrimitiveGraphics::GetNeighboursCoordinates(int cur_global_place_index, int sym_point_offset) {
    // Get Top Symmetric Vertices Point
    int top_symmetric_point = cur_global_place_index - sym_point_offset;
    // Get Bottom Left and Top Left to form a Right Triangle
    int bottom_left_point = cur_global_place_index - VERTICES_OFFSET, top_left_symmetric_point = top_symmetric_point - VERTICES_OFFSET;

    if(top_symmetric_point < 0 || bottom_left_point < 0 || top_left_symmetric_point < 0)
    {
        int stop = 0;
    }
    // Return Right Triangle Coordinates
    return {top_symmetric_point, top_left_symmetric_point, bottom_left_point};
}

vector<GLuint> MazePrimitiveGraphics::GetPrimitiveShape(int primitive_num) {
    // Multiply By 6, because 6 Indices or entries Makes Up A Rectangle of 2 right-triangles
    primitive_num = primitive_num * 6;
    // Primitive Shape or Indices That Makes Up a Rectangle
    return { Grid_Triangle_Indices[primitive_num], Grid_Triangle_Indices[(primitive_num + 1)], Grid_Triangle_Indices[(primitive_num + 2)],
             Grid_Triangle_Indices[(primitive_num + 3)], Grid_Triangle_Indices[(primitive_num + 4)],
             Grid_Triangle_Indices[(primitive_num + 5)]};
}

vector<GLuint> MazePrimitiveGraphics::GetPrimitiveIndices( int row,   int col) {
    int rectangle_num = col;
    while (row > 0)
    {
        rectangle_num = rectangle_num + DIM;
        row -= 1;
    }
    // Return The Shape or Primitive Occupied or matches the Entry's or Cell/Box's Number
    return GetPrimitiveShape(rectangle_num);
}

void MazePrimitiveGraphics::ModifyPrimitiveColor(const vector<GLuint> &triangle_coordinates, ColorMap color_map,
                                                 bool multiply_offset) {
    // Access Grid Vertices Array At Specified Index Point to set Specified Color
    int index_location = 0;
    for (int i = 0; i < triangle_coordinates.size(); i++) {
        // Get Location
        index_location = triangle_coordinates.at(i);
        if (multiply_offset) {
            // Multiply an Indices/Index Value By Offset To Get Specific Location in The Vertices
            index_location = index_location * VERTICES_OFFSET;
        }
        // Modify The Colors At This Vertices Coordinate
        Grid_Vertices[(index_location + 3)] = color_map.red;
        Grid_Vertices[(index_location + 4)] = color_map.green;
        Grid_Vertices[(index_location + 5)] = color_map.blue;
    }
}

MazePrimitiveGraphics::~MazePrimitiveGraphics() {

}




