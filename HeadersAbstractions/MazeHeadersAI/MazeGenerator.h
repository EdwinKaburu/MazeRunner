/**
 * @author: Edwin Kaburu, Zhuojing Xie
 * @date: 5/20/2022
 * @file: MazeGenerator.h - Generates A Maze
 *
 * Created by EdwinK on 6/28/2022.
 */

#ifndef AI_MAZE_GRAPHICS_MAZEGENERATOR_H
#define AI_MAZE_GRAPHICS_MAZEGENERATOR_H

#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <functional>
#include "MazeShared.h"
#include "MazePrimitiveGraphics.h"

class MazeGenerator {
private:
    std::vector<std::vector<int>> MAZE_MATRIX;
    unsigned int DIM;
    int same_rand_counter = 0;

    void GenerateMatrix();

    std::vector<Cell> GetUnVisitedNeighbours(Cell center_node);

    std::vector<Cell> dead_end_list;

    bool NodeLink(Cell &from, Cell &to);

    bool isDeadEnd(Cell center_node);

    void EstablishNodeConnections(Cell start_node, unsigned int prev_rand = 0);

public:
    MazeGenerator(unsigned int dim);

    std::vector<std::vector<int>> GetMazeMatrix();

    void GenerateRandomMaze(Cell start_node);

    Cell GetEndPoint(Cell start_node);

    ~MazeGenerator();
};


#endif //AI_MAZE_GRAPHICS_MAZEGENERATOR_H
