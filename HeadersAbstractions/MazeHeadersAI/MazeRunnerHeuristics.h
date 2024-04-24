/**
 * @author: Edwin Kaburu, Zhuojing Xie
 * @date: 5/28/2022
 * @file: MazeRunnerHeuristics.h - AI Heuristics Solver Header
 *
 *  Created by EdwinK on 6/28/2022.
 */


#ifndef AI_MAZE_GRAPHICS_MAZERUNNERHEURISTICS_H
#define AI_MAZE_GRAPHICS_MAZERUNNERHEURISTICS_H


#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
#include "MazeShared.h"

using namespace std;

class MazeRunnerHeuristics {

public:
    int num_levels;
    long long total_time;

    MazeRunnerHeuristics(const vector<vector<int>>& maze_input, Cell start, Cell end);
    MazeRunnerHeuristics(const vector<vector<int>>& maze_input, Cell start, Cell end, vector<MazeAnimationFrames>&frames_list);

    vector<vector<int>> GetSolvedMatrix();

    ~MazeRunnerHeuristics();

private:
    Cell starting_cell;
    Cell ending_cell;
    vector<vector<int>> MAZE_MATRIX;
    vector<MazeAnimationFrames>* ANIMATION_MATRIX_PTR;
    unsigned int DIM;

    bool PossibleLink(Cell from, Cell to);

    void SetLink(Cell from, Cell to, short int cell_code);

    void GetUnvisitedNeighbours(vector<Cell>& list, Cell center_node, Cell ignore_node);

    bool CalculateHeuristicCost(Cell &potential, Cell& ignore, Cell& center);

    bool FindHeuristicPath(Cell current_node, Cell ignore_node, int& level);

    static bool compareCells(Cell cell1, Cell cell2);

    static bool isCellEqual(Cell cell1, Cell cell2);

    static double GetDistanceFormula(Cell from, Cell to);

    static double GetManhattanDist(Cell from, Cell to);

};

#endif //AI_MAZE_GRAPHICS_MAZERUNNERHEURISTICS_H
