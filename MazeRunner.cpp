/**
 * @author: Edwin Kaburu, Zhuojing Xie
 * @date: 5/28/2022
 * @file: MazeRunner.cpp - BFS/ DFS AI Solver Implementation
 *
 * @changes
 *     @date: 6/28/2022 Graphics API Integration
 */


#include "MazeRunner.h"
#include <utility>
#include <iostream>
#include <unordered_map>
#include <queue>
#include <climits>

using namespace std;

/**
 * default constuctor. will take 2d vectors as map input
 * @param board
 */
MazeRunner::MazeRunner(std::vector<std::vector<int>> board) {
    this->board = std::move(board);
}

MazeRunner::MazeRunner(std::vector<std::vector<int>> board, vector<MazeAnimationFrames> &frames_list) {
    this->board = std::move(board);
    this->ANIMATION_MATRIX_PTR = &frames_list;
}


/**
 * DFS public function
 * @param st  starting point
 * @param dest ending point
 * @return total steps to achieve ending. -1 if there is no route.
 */
int MazeRunner::DFS(pair<int, int> st, pair<int, int>dest) {
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    queue<pair<int, int>> path;
    vector<vector<bool>> isVisited(board.size(), vector<bool>(board[0].size(), false));
    vector<queue<pair<int, int>>> found;
    DFS(st, dest, 0, path, isVisited,found);

    queue<pair<int, int>> bestPath;
    int bestSteps = INT_MAX;
    for(const auto& x:found){
        if(x.size()<bestSteps){
            bestPath = x;
            bestSteps = x.size();
        }
    }
    end = std::chrono::system_clock::now();
    if(bestSteps < INT_MAX){
        flip_Path(bestPath);
        elapsed_seconds = end -start;
        return bestSteps;
    }

    return -1;
}

/**
 * private helping DFS function
 * @param st starting point
 * @param dest ending point
 * @param step  current steps
 * @param path current path to this st point
 * @param isVisited 2d vector marked all the point already been visited
 * @param found 2d vector to record all the route that achieve ending point
 * @return true if achieve dest point. false otherwise
 */
bool MazeRunner::DFS(pair<int, int> st, pair<int, int>dest, int step, queue<pair<int, int>> path, vector<vector<bool>>& isVisited,vector<queue<pair<int, int>>>& found) {
    int r = board.size();
    int c = board[0].size();
    path.push(st);

    vector<int> directions = {-1,0,1,0,-1};

    int sti = st.first;
    int stj = st.second;
    int desti = dest.first;
    int destj = dest.second;


    if(sti == desti && stj == destj){
        found.push_back(path);
        return true;
    }

    isVisited[sti][stj] = true;
    for(int i = 0 ; i < 4 ; i++){
        int x = sti + directions[i];
        int y = stj + directions[i+1];
        if(x<r && x>=0 && y<c && y>=0 && !isVisited[x][y] && board[x][y] == 0){
            auto temp = make_pair(x,y);
            if(DFS(temp, dest, ++step, path, isVisited,found)){
                return true;
            }

        }
    }
    return false;
}

/**
 * Flip the map depending on the path.
 * 8 for starting point
 * 7 for ending point
 * 9 for the middle route.
 * @param path
 */
void MazeRunner::flip_Path(queue<pair<int, int>> path) {
    auto [r,c] = path.front();
    board[r][c] = 8;
    path.pop();

    while(!path.empty()){
        auto [r,c] = path.front();
        path.pop();
        if(path.empty()){
            board[r][c] = 7;
        }
        else {
            board[r][c] = 9;
        }
    }

}

/**
 * print the map nicer. will mark S as starting point, E as ending point. O as middle route, . as open route, X is wall or obstacle
 */
void MazeRunner::printMap() {
    unordered_map<int, char> marks;
    marks[1] = 'X';
    marks[0] = '.';
    marks[7] = 'S';
    marks[8] = 'E';
    marks[9] = 'o';

    for (int row = 0; row < board.size(); row++) {
        for (int col = 0; col < board[0].size(); col++) {
            cout << marks[board.at(row).at(col)]<<" ";
        }
        cout << "\n";
    }
    cout << "\n";

}

/**
 * BFS function for finding the route.
 * @param st starting point
 * @param dest ending point
 * @return steps from starting point to ending points
 */
int MazeRunner::BFS(std::pair<int, int> st, std::pair<int, int> dest) {
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();

    vector<int> directions = {-1,0,1,0,-1};
    vector<vector<pair<int, int>>> isVisited(board.size(),vector(board[0].size(), make_pair(-1, -1)));
    int step = 0;
    queue<pair<int, int>> nextMove;
    nextMove.push(st);

    while(!nextMove.empty()){
        step++;
        int size = nextMove.size();
        while(size--){
            auto [r,c] = nextMove.front();
            if(r == dest.first && c == dest.second){
                flip_Path(buildBFS_Path(dest,st, isVisited));
                return step;
            }
            nextMove.pop();
            for(int i = 0 ; i < 4 ; i++){
                int x = r + directions[i];
                int y = c + directions[i+1];
                if(x<board.size() && x>=0 && y< board[0].size() && y>=0 && isVisited[x][y].first == -1 && board[x][y] == 0){
                    isVisited[x][y] = make_pair(r,c);
                    nextMove.push(make_pair(x,y));
                }
            }
        }
    }
    end = std::chrono::system_clock::now();
    elapsed_seconds = end -start;

    return step;
}

/**
 * depending the source to current cells(), it flip the maps to mark route
 * @param dest destination point
 * @param st starting point
 * @param isVisited vector<std::vector<std::pair<int, int>>> &isVisited is record the which points comes to the current points.
 * @return
 */
std::queue<std::pair<int, int>>
MazeRunner::buildBFS_Path(std::pair<int, int> dest,std::pair<int, int> st, vector<std::vector<std::pair<int, int>>> &isVisited) {
    queue<pair<int, int>> path;
    auto [r, c] = dest;

    while(r != st.first || c!= st.second){
        path.push(make_pair(r,c));
        int _r = isVisited[r][c].first;
        int _c = isVisited[r][c].second;
        r = _r;
        c = _c;
    }

    path.push(st);

    return path;

}
