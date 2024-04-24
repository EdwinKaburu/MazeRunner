/**
 * @author: Edwin Kaburu, Zhuojing Xie
 * @date: 5/28/2022
 * @file: MazeRunnerHeuristics.cpp - AI Heuristics Solver Implementation
 * @changes
 *      @date: 6/28/2022 Graphics API Integration
 */

#include "MazeRunnerHeuristics.h"


// --------- Public Interface ------------
// ---------------------------------------

/**
 * MazeRunnerHeuristics() Constructor To Create Instance of Maze Runner Heuristics
 * @param maze_input 2D Matrix of Generated Maze
 * @param start Starting Position
 * @param end Ending Position
 */
MazeRunnerHeuristics::MazeRunnerHeuristics(const vector<vector<int>> &maze_input, Cell start, Cell end) {
    starting_cell = start;
    ending_cell = end;
    MAZE_MATRIX = maze_input;
    DIM = maze_input.size();
    num_levels = 0;
    total_time = 0;

    // Calculate the initial cost of the starting cell to the goal
    starting_cell.g_n = 0.0;
    starting_cell.h_n = GetManhattanDist(starting_cell, ending_cell);
    starting_cell.f_n = starting_cell.g_n + starting_cell.h_n;
}

MazeRunnerHeuristics::MazeRunnerHeuristics(const vector<vector<int>> &maze_input, Cell start, Cell end,
                                           vector<MazeAnimationFrames> &frames_list) {
    starting_cell = start;
    ending_cell = end;
    MAZE_MATRIX = maze_input;
    DIM = maze_input.size();
    num_levels = 0;
    total_time = 0;
    ANIMATION_MATRIX_PTR = &frames_list;

    // Calculate the initial cost of the starting cell to the goal
    starting_cell.g_n = 0.0;
    starting_cell.h_n = GetManhattanDist(starting_cell, ending_cell);
    starting_cell.f_n = starting_cell.g_n + starting_cell.h_n;
}

/**
 * GetSolvedMatrix() Return A Matrix, Traversed by AI, with a Optimal Solution To the Goal
 * @return A Solved Matrix
 */
vector<vector<int>> MazeRunnerHeuristics::GetSolvedMatrix() {
    // Get Current Time
    auto start_time = std::chrono::high_resolution_clock::now();
    FindHeuristicPath(starting_cell, {-1, -1}, num_levels);
    // Get Termination Time
    auto end_time = std::chrono::high_resolution_clock::now();

    // Total Time
    total_time = chrono::duration_cast<chrono::microseconds>(end_time - start_time).count();
    return MAZE_MATRIX;
}

/**
 * MazeRunnerHeuristics()- Default Destructor for this object
 */
MazeRunnerHeuristics::~MazeRunnerHeuristics() = default;

// -------- Private Interface --------------
// -----------------------------------------

/**
 * GetDistanceFormula() Get the Distance between two cells
 * @param from Cell 1
 * @param to Cell 2
 * @return Double Value Distance
 */
double MazeRunnerHeuristics::GetDistanceFormula(Cell from, Cell to) {
    return sqrt(pow(from.col - to.col, 2) + pow(from.row - to.row, 2));
}

/**
 * GetManhattanDist() Get the Manhattan Distance between two cells
 * @param from Cell 1
 * @param to Cell 2
 * @return Double Value Distance
 */
double MazeRunnerHeuristics::GetManhattanDist(Cell from, Cell to) {
    return abs(from.col - to.col) + abs(from.row - to.row);
}

/**
 * isCellEqual() Conditional Check for Equivalence
 * @param cell1 Cell1
 * @param cell2 Cell2
 * @return Boolean Condition on Equivalence
 */
bool MazeRunnerHeuristics::isCellEqual(Cell cell1, Cell cell2) {
    if (cell1.col == cell2.col && cell1.row == cell2.row) {
        return true;
    }
    return false;
}

/**
 * compareCells() Predicate Comparison on whether the f(n) in cell1 is smaller than cell2
 * @param cell1 Cell 1
 * @param cell2 Cell 2
 * @return Boolean Condition
 */
bool MazeRunnerHeuristics::compareCells(Cell cell1, Cell cell2) {
    return (cell1.f_n < cell2.f_n);
}

/**
 * PossibleLink() Boolean Condition On Whether A link Exists Between Two Points
 * @param from Cell 1
 * @param to Cell 2
 * @return Boolean Condition
 */
bool MazeRunnerHeuristics::PossibleLink(Cell from, Cell to) {
    // Get the Intermediate Cell's col and row
    int intermediate_row = (from.row + to.row) / 2, intermediate_col = (from.col + to.col) / 2;
    if (MAZE_MATRIX.at(intermediate_row).at(intermediate_col) >= 1) {
        // 1 Marks A Wall, 2 already traversed
        return false;
    }
    return true;
}

/**
 * SetLink() Set A Link Between Two Cells, 2 will be written as value to set link
 * @param from Cell 1
 * @param to Cell 2
 * @param cell_code Encoding For Grid
 */
void MazeRunnerHeuristics::SetLink(Cell from, Cell to, short int cell_code) {
    // Get the Intermediate Cell's col and row
    int intermediate_row = (from.row + to.row) / 2, intermediate_col = (from.col + to.col) / 2;
    // Set Link Between from and to
    MAZE_MATRIX.at(from.row).at(from.col) = cell_code;
    MAZE_MATRIX.at(intermediate_row).at(intermediate_col) = cell_code;
    MAZE_MATRIX.at(to.row).at(to.col) = cell_code;
}

/**
 * CalculateHeuristicCost() Calculate the Heuristics Cost of a Potential Cell, f(n) = g(n) + h(n)
 * @param potential Potential Cell
 * @param ignore Ignore Cell or Skip Cell
 * @param center Center Cell or Caller Cell or Parent Cell
 * @return Boolean Condition on Cost Calculation
 */
bool MazeRunnerHeuristics::CalculateHeuristicCost(Cell &potential, Cell &ignore, Cell &center) {

    if (!isCellEqual(potential, ignore) && PossibleLink(center, potential)) {
        // Get the Heuristic Cost To Goal
        potential.h_n = GetManhattanDist(potential, ending_cell);
        // Get the g_n Cost from Starting Node
        potential.g_n = center.g_n + GetDistanceFormula(starting_cell, potential);
        // Get The Total Cost
        potential.f_n = potential.g_n + potential.h_n;
        return true;
    }
    return false;
}

/**
 * GetUnvisitedNeighbours() Get the Unvisited Neighbours or Adjacencies/adjacents of a Cell
 * @param list Collection To Save Unvisited Neighbours or Cells
 * @param center_node Current Node or Center Node needed to get its unvisited cells
 * @param ignore_node Ignore Cell or Skip Cell
 */
void MazeRunnerHeuristics::GetUnvisitedNeighbours(vector<Cell> &list, Cell center_node, Cell ignore_node) {
    // The Neighbouring Cols and Rows
    int upper_row = center_node.row - 2;
    int lower_row = center_node.row + 2;
    int left_col = center_node.col - 2;
    int right_col = center_node.col + 2;

    Cell potential_node = {};

    // Check if within Bounds
    if (upper_row > -1 && upper_row < DIM) {
        // Set Potential Node and Calculate Its Heuristics Costs
        potential_node = {upper_row, center_node.col, 0.0, 0.0, 0.0};
        if (CalculateHeuristicCost(potential_node, ignore_node, center_node)) {
            list.push_back(potential_node);
        }
    }

    // Check if within Bounds
    if (lower_row > -1 && lower_row < DIM) {
        // Set Potential Node and Calculate Its Heuristics Costs
        potential_node = {lower_row, center_node.col, 0.0, 0.0, 0.0};
        if (CalculateHeuristicCost(potential_node, ignore_node, center_node)) {
            list.push_back(potential_node);
        }
    }

    // Check if within Bounds
    if (left_col > -1 && left_col < DIM) {
        // Set Potential Node and Calculate Its Heuristics Costs
        potential_node = {center_node.row, left_col, 0.0, 0.0, 0.0};
        if (CalculateHeuristicCost(potential_node, ignore_node, center_node)) {
            list.push_back(potential_node);
        }
    }

    // Check if within Bounds
    if (right_col > -1 && right_col < DIM) {
        // Set Potential Node and Calculate Its Heuristics Costs
        potential_node = {center_node.row, right_col, 0.0, 0.0, 0.0};
        if (CalculateHeuristicCost(potential_node, ignore_node, center_node)) {
            list.push_back(potential_node);
        }
    }

    // Sort this List to have the smallest cell with the lowest cost at front of list
    sort(list.begin(), list.end(), compareCells);
}

/**
 * FindHeuristicPath() Finds the Optimal Path to the Goal (End Point) from Starting Point
 * @param current_node Current Node or Starting Node
 * @param ignore_node Ignore Cell or Skip Cell, default is {-1,-1}
 * @return Boolean Condition on Optimal Path Found
 */
bool MazeRunnerHeuristics::FindHeuristicPath(Cell current_node, Cell ignore_node, int &level) {
    // Increment Level
    level +=1;

    // Get Unvisited Neighbours
    vector<Cell> unvisited_neighbours = {};
    if (isCellEqual(current_node, ending_cell)) {
        // We Have reached the goal, Stop
        return true;
    }
    // Get Unvisited Neighbours
    GetUnvisitedNeighbours(unvisited_neighbours, current_node, ignore_node);

    while (!unvisited_neighbours.empty()) {
        // Extract The First Cell from Vector
        Cell next_current_node = unvisited_neighbours.at(0);
        // Set A link between current node and next node
        SetLink(current_node, next_current_node, 2);

        // Save This Frame
        ANIMATION_MATRIX_PTR->push_back({MAZE_MATRIX});

        // Call Find Heuristic Path
        bool result = FindHeuristicPath(next_current_node, current_node, level);
        // Remove Node From Unvisited
        unvisited_neighbours.erase(unvisited_neighbours.begin() + 0);
        // Re-Sort List
        sort(unvisited_neighbours.begin(), unvisited_neighbours.end(), compareCells);
        if (result) {
            // erase all data
            unvisited_neighbours.clear();
            // Path Was Found
            return result;
        }

        SetLink(current_node, next_current_node, 3);
    }
    return false;
}


