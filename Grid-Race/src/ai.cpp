#include <queue>
#include <vector>
#include <utility>
#include <iostream>
#include "ai.h"

DijkstraAlgorithm::DijkstraAlgorithm(int i_rows, int i_columns, int start[2], int end[2], 
                                     std::vector<pi> i_obstacles) {
    // Initialize adjacency matrix
    rows = i_rows;
    columns = i_columns;

    grid = new int*[rows];
    for (int i = 0; i < rows; i += 1)
        grid[i] = new int[columns];

    // Initialize start and end
    start_position[0] = start[0];
    start_position[1] = start[1];

    goal_position[0] = end[0];
    goal_position[1] = end[1];

    // Initialize Obstacles
    obstacles = std::vector<pi>(i_obstacles);
}

std::vector<pi> DijkstraAlgorithm::plan() {
    // Initialize priority queue
    std::priority_queue<node, std::vector<node>, std::greater<node>> min_heap;

    // Initialize matrix of distances
    int distances[rows][columns];
    for (int i = 0; i < rows; i += 1) {
        for (int j = 0; j < columns; j += 1)
            distances[i][j] = 1000000;
    }

    // Initialize parent array
    node **parent;
    parent = new node*[rows];
    for (int i = 0; i < rows; i += 1)
        parent[i] = new node[columns];

    // Insert start to queue
    node source = std::make_pair(0, std::make_pair(start_position[0], start_position[1]));
    distances[start_position[0]][start_position[1]] = 0;
    min_heap.push(source);

    // Search Begins
    while (!min_heap.empty()) {
        // Get data from the minimum node
        node current_node = min_heap.top();
        int current_position[2] = {current_node.second.first, current_node.second.second};
        int current_cost = current_node.first;
        min_heap.pop();

        // std::cout << current_node.second.first << " " << current_node.second.second << std::endl;

        // Inspect adjacent nodes and add to heap
        node adjacent_node;
        int adjacent_cost;
        int adjacent_position[2];
        // Left
        if (current_position[1] > 0) {
            adjacent_position[0] = current_position[0];
            adjacent_position[1] = current_position[1] - 1;
            adjacent_cost = distances[adjacent_position[0]][adjacent_position[1]];

            // If there is a shorter path to neighbor through current
            // And does not pass through obstacles
            // Update the neighbor weight
            if (adjacent_cost > current_cost + 1 && check_obstacle(adjacent_position)) {
                distances[adjacent_position[0]][adjacent_position[1]] = current_cost + 1;
                adjacent_node = std::make_pair(current_cost + 1, 
                                std::make_pair(adjacent_position[0], adjacent_position[1]));
                min_heap.push(adjacent_node);
                parent[adjacent_position[0]][adjacent_position[1]] = current_node;
            }
        }

        // Right
        if (current_position[1] < columns - 1) {
            adjacent_position[0] = current_position[0];
            adjacent_position[1] = current_position[1] + 1;
            adjacent_cost = distances[adjacent_position[0]][adjacent_position[1]];

            // If there is a shorter path to neighbor through current
            // Update the neighbor weight
            if (adjacent_cost > current_cost + 1 && !check_obstacle(adjacent_position)) {
                distances[adjacent_position[0]][adjacent_position[1]] = current_cost + 1;
                adjacent_node = std::make_pair(current_cost + 1, 
                                std::make_pair(adjacent_position[0], adjacent_position[1]));
                min_heap.push(adjacent_node);
                parent[adjacent_position[0]][adjacent_position[1]] = current_node;
            }
        }

        // Up
        if (current_position[0] > 0) {
            adjacent_position[0] = current_position[0] - 1;
            adjacent_position[1] = current_position[1];
            adjacent_cost = distances[adjacent_position[0]][adjacent_position[1]];

            // If there is a shorter path to neighbor through current
            // Update the neighbor weight
            if (adjacent_cost > current_cost + 1 && !check_obstacle(adjacent_position)) {
                distances[adjacent_position[0]][adjacent_position[1]] = current_cost + 1;
                adjacent_node = std::make_pair(current_cost + 1, 
                                std::make_pair(adjacent_position[0], adjacent_position[1]));
                min_heap.push(adjacent_node);
                parent[adjacent_position[0]][adjacent_position[1]] = current_node;
            }
        }

        // Down
        if (current_position[0] < rows - 1) {
            adjacent_position[0] = current_position[0] + 1;
            adjacent_position[1] = current_position[1];
            adjacent_cost = distances[adjacent_position[0]][adjacent_position[1]];

            // If there is a shorter path to neighbor through current
            // Update the neighbor weight
            if (adjacent_cost > current_cost + 1 && !check_obstacle(adjacent_position)) {
                distances[adjacent_position[0]][adjacent_position[1]] = current_cost + 1;
                adjacent_node = std::make_pair(current_cost + 1, 
                                std::make_pair(adjacent_position[0], adjacent_position[1]));
                min_heap.push(adjacent_node);
                parent[adjacent_position[0]][adjacent_position[1]] = current_node;
            }
        }
    }

    return restore_path(parent);
}

std::vector<pi> DijkstraAlgorithm::restore_path(node **parent) {
    std::vector<pi> path;
    pi current_point = std::make_pair(goal_position[0], goal_position[1]);

    node current_node;
    while (current_point.first != start_position[0] || current_point.second != start_position[1]) {
        path.push_back(current_point);
        current_node = parent[current_point.first][current_point.second];
        current_point.first = current_node.second.first;
        current_point.second = current_node.second.second;
    }

    reverse(path.begin(), path.end());
    return path;
}

bool DijkstraAlgorithm::check_obstacle(int position[2]) {
    for (int i = 0; i < obstacles.size(); i += 1) {
        if (obstacles[i].first == position[0] && obstacles[i].second == position[1])
            return true;
    }

    return false;
}