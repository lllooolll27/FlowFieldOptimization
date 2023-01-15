#include <stdio.h>
#define M_PI 3.14159265358979323846
#define GRID_SIZE_X 101
#define GRID_SIZE_Y 41
int main() {
    int currentPos[GRID_SIZE_X][GRID_SIZE_Y];
    int grid[GRID_SIZE_X][GRID_SIZE_Y];
    int cost[GRID_SIZE_X][GRID_SIZE_Y];
    int visited[GRID_SIZE_X][GRID_SIZE_Y];
    int obstacle[GRID_SIZE_X][GRID_SIZE_Y];
    int adjacency[GRID_SIZE_X][GRID_SIZE_Y];
    int x, y;

    // Initialize the grid
    for (y = 0; y < GRID_SIZE_Y; y++) {
        for (x = 0; x < GRID_SIZE_X; x++) {
            grid[x][y] = 0;
        }
    }
    
    // Initialize the cost
    for (y = 0; y < GRID_SIZE_Y; y++) {
        for (x = 0; x < GRID_SIZE_X; x++) {
            cost[x][y] = 65535;
        }
    }

    // Initialize visited
    for (y = 0; y < GRID_SIZE_Y; y++) {
        for (x = 0; x < GRID_SIZE_X; x++) {
            visited[x][y] = 0;
        }
    }

    // Initialize the obstacles
    {
    for (y = 30; y < 39; y++) {
        for (x = 19; x < 21; x++) {
            obstacle[x][y] = 1;
        }
    }
    for (y = 22; y < 39; y++) {
        for (x = 69; x < 71; x++) {
            obstacle[x][y] = 1;
        }
    }
    for (y = 4; y < 34; y++) {
        for (x = 8; x < 10; x++) {
            obstacle[x][y] = 1;
        }
    }
    for (y = 3; y < 16; y++) {
        for (x = 60; x < 65; x++) {
            obstacle[x][y] = 1;
        }
    }
    for (y = 16; y < 19; y++) {
        for (x = 60; x < 99; x++) {
            obstacle[x][y] = 1;
        }
    }
    for (y = 32; y < 35; y++) {
        for (x = 20; x < 90; x++) {
            obstacle[x][y] = 1;
        }
    }
    for (y = 7; y < 27; y++) {
        for (x = 22; x < 32; x++) {
            obstacle[x][y] = 1;
        }
    }
    }

    // Set the goal position
    int goal_x = GRID_SIZE_X / 2;
    int goal_y = GRID_SIZE_Y / 2;

    // Generate the cost
    // Start at Goal
    // Set visited tiles to 1
    cost[goal_x][goal_y] = 0; 
    visited[goal_x][goal_y] = 1;
    // Generate the flow field
    

    // Display the grid
    for (int y = 0; y < GRID_SIZE_Y; y++) {
        for (int x = 0; x < GRID_SIZE_X; x++) {
            if(obstacle[x][y] != 0){
                printf("#");
            }else{
                printf(".");
            }
        }
        printf("\n");
    }

    return 0;
}
