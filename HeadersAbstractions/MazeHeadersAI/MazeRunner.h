/**
 * @author: Edwin Kaburu, Zhuojing Xie
 * @date: 5/28/2022
 * @file: MazeRunnerHeuristics.cpp - AI Heuristics Solver Implementation
 *
 * @changes
 *      @date: 6/28/2022 Graphics API Integration
 */

#ifndef AI_MAZE_GRAPHICS_MAZERUNNER_H
#define AI_MAZE_GRAPHICS_MAZERUNNER_H

#include <vector>
#include <queue>
#include <chrono>
#include "MazeShared.h"

class MazeRunner {
public:
    std::vector<std::vector<int>> board;
    int DFS(std::pair<int, int> st, std::pair<int, int>dest);
    int BFS(std::pair<int, int> st, std::pair<int, int>dest);
    MazeRunner(std::vector<std::vector<int>> board);
    MazeRunner(std::vector<std::vector<int>> board, vector<MazeAnimationFrames>&frames_list);
    void printMap();
    std::chrono::duration<double> elapsed_seconds ;

private:
    vector<MazeAnimationFrames>* ANIMATION_MATRIX_PTR;
    void flip_Path(std::queue<std::pair<int, int>> path);
    bool DFS(std::pair<int, int> st, std::pair<int, int>dest, int step, std::queue<std::pair<int, int>> path,
             std::vector<std::vector<bool>>& isVisited, std::vector<std::queue<std::pair<int, int>>>& found);
    std::queue<std::pair<int, int>> buildBFS_Path(std::pair<int, int> dest,std::pair<int, int> st, std::vector<std::vector<std::pair<int, int>>>& isVisited);
};


#endif //AI_MAZE_GRAPHICS_MAZERUNNER_H
