/**
 * @author: Edwin Kaburu, Zhuojing Xie
 * @date: 5/28/2022
 * @file: MazeGenerator.cpp - Generates A Maze
 *
 * @changes
 *      @date: 5/28/2022 Modify Cell struct to be called from one header
 *      @date: 6/28/2022 Graphics API Integration
 */

#include "MazeGenerator.h"

using namespace std;

// ---------------------- Public Section --------------------------
/**
 * MazeGenerator() Constructor to create instance of MazeGenerator
 * @param dim Dimensions Of Matrix
 */
MazeGenerator::MazeGenerator(unsigned int dim) {
    // Set the Dimension of Matrix
    DIM = dim;
    // Generate Random Matrix
    GenerateMatrix();
}

/**
 * GetMazeMatrix() Return the MazeMatrix
 * @return 2D Maze Matrix
 */
std::vector<std::vector<int>> MazeGenerator::GetMazeMatrix() {
    // return the Maze
    return MAZE_MATRIX;
}

/**
 * GenerateRandomMaze() Maze Matrix Generator specifying Starting Cell/Node
 * @param start_node Starting Cell/Node
 */
void MazeGenerator::GenerateRandomMaze(Cell start_node) {
    // Recursive Maze Generator
    EstablishNodeConnections(start_node);
}

/**
 * GetEndPoint() Get A Certain EndPoint with largest distance
 * @param start_node Specific Cell/Node
 * @return Furthest Cell/Node
 */
Cell MazeGenerator::GetEndPoint(Cell start_node) {
    Cell furthest_cell{};
    // Previous Distance
    double prev_distance = 0.0;

    // Apply Distance Formula
    for (auto potential_end_node: dead_end_list) {
        // Distance Formula
        double distance_formula = sqrt(pow(start_node.col - potential_end_node.col, 2)
                                       + pow(start_node.row - potential_end_node.row, 2));
        // Get If this
        if (distance_formula >= prev_distance) {
            // Set The Furthest Cell
            furthest_cell = potential_end_node;
            // Set This As Previous Distance
            prev_distance = distance_formula;
        }
    }
    return furthest_cell;
}

MazeGenerator::~MazeGenerator() = default;

// ---------------------- Private Section --------------------------

/**
 * GenerateMatrix() - Generates A Maze Matrix with all ones
 */
void MazeGenerator::GenerateMatrix() {
    for (int i = 0; i < DIM; i++) {
        // Generate a row of all ones
        vector<int> matrix_row(DIM, 1);
        // pushes this row to the matrix
        MAZE_MATRIX.push_back(matrix_row);
    }
}

/**
 * GetUnVisitedNeighbours() Get the Unvisited Adjacent Neighbours of a Cell
 * @param center_node Specific Cell
 * @return Collection/List of Neighbours of A Cell
 */
std::vector<Cell> MazeGenerator::GetUnVisitedNeighbours(Cell center_node) {
    // Vector of Unvisited Adjacent Neighbours
    vector<Cell> adjacent_neighbours = {};

    // Get Top Node/Cell, Above Current Cell
    int up_index = center_node.row - 2;
    if (up_index > -1) {
        // In Between the Bounds of Matrix
        int value = MAZE_MATRIX.at(up_index).at(center_node.col);
        if (value != 0) {
            // Add Unvisited Node/Cell To Collection
            adjacent_neighbours.push_back({up_index, center_node.col});
        }
    }
    // Get Bottom Node/Cell, Below Current Cell
    int bt_index = center_node.row + 2;
    if (bt_index < DIM) {
        // In Between the Bounds of Matrix
        int value = MAZE_MATRIX.at(bt_index).at(center_node.col);
        if (value != 0) {
            // Add Unvisited Node/Cell To Collection
            adjacent_neighbours.push_back({bt_index, center_node.col});
        }
    }

    // Get Left Node/Cell, Of The Current Cell
    int left_index = center_node.col - 2;
    if (left_index > -1) {
        // In Between the Bounds of Matrix
        int value = MAZE_MATRIX.at(center_node.row).at(left_index);
        if (value != 0) {
            // Add Unvisited Node/Cell To Collection
            adjacent_neighbours.push_back({center_node.row, left_index});
        }
    }

    // Get Right Node, Of The Current Cell
    int right_index = center_node.col + 2;
    if (right_index < DIM) {
        // In Between the Bounds of Matrix
        int value = MAZE_MATRIX.at(center_node.row).at(right_index);
        if (value != 0) {
            // Add Unvisited Node/Cell To Collection
            adjacent_neighbours.push_back({center_node.row, right_index});
        }
    }

    return adjacent_neighbours;
}

/**
 * isDeadEnd() Checks if specified cell is a dead end
 * @param center_node Specific Cell/Node
 * @return Boolean Condition
 */
bool MazeGenerator::isDeadEnd(Cell center_node) {
    // The Neighbouring Cols and Rows
    int upper_row = center_node.row - 1;
    int lower_row = center_node.row + 1;
    int left_col = center_node.col - 1;
    int right_col = center_node.col + 1;

    // Counter To Count Potential Paths
    int counter = 0;

    if (upper_row > -1 && upper_row < DIM) {
        if (MAZE_MATRIX.at(upper_row).at(center_node.col) == 0) {
            counter += 1;
        }
    }
    if (lower_row > -1 && lower_row < DIM) {
        if (MAZE_MATRIX.at(lower_row).at(center_node.col) == 0) {
            counter += 1;
        }
    }
    if (left_col > -1 && left_col < DIM) {
        if (MAZE_MATRIX.at(center_node.row).at(left_col) == 0) {
            counter += 1;
        }
    }
    if (right_col > -1 && right_col < DIM) {
        if (MAZE_MATRIX.at(center_node.row).at(right_col) == 0) {
            counter += 1;
        }
    }
    if (counter == 1) {
        return true;
    }
    return false;
}

/**
 * NodeLink() Establishes A Path or Link Between the Current Node/Cell and Next Potential Node/Cell
 * @param from Current Node/Cell
 * @param to Potential Next Node/Cell
 * @return Boolean Condition on Link Establishment
 */
bool MazeGenerator::NodeLink(Cell &from, Cell &to) {
    // Get the Intermediate Cell (row, col), between the Two Cells/Nodes.
    int intermediate_row = (from.row + to.row) / 2, intermediate_col = (from.col + to.col) / 2;

    if (MAZE_MATRIX.at(to.row).at(to.col) == 0) {
        // No Establishment
        // We Leave the Intermediate Cell as Wall Between The Two Nodes/Cells
        // Some Other Node, already flip the To(Potential Node/Cell) to be open, therefore this Cell/Node cannot flip and
        // already flip Cell/Node
        // DeadEnd
        // Save This Cell/Node As A DeadEnd
        if (isDeadEnd(from)) {
            dead_end_list.push_back(from);
        }
        return false;
    }
    // Set the Path From Current Node/Cell To the Open
    MAZE_MATRIX.at(from.row).at(from.col) = 0;
    MAZE_MATRIX.at(intermediate_row).at(intermediate_col) = 0;
    MAZE_MATRIX.at(to.row).at(to.col) = 0;
    // Return True for Establishment
    return true;
}

/**
 * EstablishNodeConnections() Depth First Search To Establish a Random Path throughout the Matrix,
 *                           from the current or start Node/Cell
 * @param start_node Current Node/Cell
 * @param prev_rand Integer Previous Random Value
 */
void MazeGenerator::EstablishNodeConnections(Cell start_node, unsigned int prev_rand) {
    // Current Duration Time
    auto seed = chrono::high_resolution_clock::now().time_since_epoch().count();
    // A Mersenne Twister pseudo-random generator
    mt19937 gen(seed);
    // Uniform Distribution of Unsigned Integers.
    uniform_int_distribution<unsigned int> distribution;

    // Get Unvisited Neighbours,
    vector<Cell> unvisited_neighbours = GetUnVisitedNeighbours(start_node);

    if (unvisited_neighbours.empty() && isDeadEnd(start_node)) {
        dead_end_list.push_back(start_node);
    }

    while (!unvisited_neighbours.empty()) {
        // Get A Random Neighbour Index
        unsigned int next_index = distribution(gen) % unvisited_neighbours.size();
        // Make sure Total Number of generated duplicate numbers is below 2
        if (next_index == prev_rand && same_rand_counter > 1) {
            // Get Another Random Number
            next_index = distribution(gen) % unvisited_neighbours.size();
            // Reset duplicate random number counter
            same_rand_counter = 0;
        }
        // Increment random number counter
        same_rand_counter += 1;

        // Get the Unvisited Cell
        Cell next_node = unvisited_neighbours.at(next_index);
        // Establish A Link Between the Start Node/Cell and the Unvisited Neighbour of this Cell
        bool result = NodeLink(start_node, next_node);

        // Remove This Potential Node from Unvisited List
        unvisited_neighbours.erase(unvisited_neighbours.begin() + next_index);
        if (result) {
            // Not A DeadEnd continue
            EstablishNodeConnections(next_node, next_index);
        }
    }
}

