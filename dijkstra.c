#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>

#define GET(arr,x,y) (arr)[(y) * GRID_SITZE_X + (x)]
#define GRID_SITZE_X 2048
#define GRID_SITZE_Y 2048
#define GOAL_X 1024
#define GOAL_Y 1024

clock_t start_time, end_time;

typedef struct tile {
    int x;
    int y;
    int cost;
}Tile;

int xoffset[4] = { 0, 1, 0, -1 };
int yoffset[4] = { 1, 0, -1, 0 };
bool* obstacle_ptr;
int* grid_ptr;
bool* visited_ptr;

int compare(const void* a, const void* b) {
    return ((Tile*)a)->cost - ((Tile*)b)->cost;
}

void initialize_obstacles(int x, int y, int w, int h) {
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            if (x + i < 0 || y + j < 0 || x + i >= GRID_SITZE_X || y + j >= GRID_SITZE_Y)
                continue;
            GET(obstacle_ptr, x + i, y + j) = true;

        }
    }
}

void dijkstra() {

    Tile startTile = { GOAL_X, GOAL_Y, 0 };
    Tile currentTile;
    Tile neighborTiles[4];
    int neighborCount;

    GET(grid_ptr, startTile.x, startTile.y) = startTile.cost;


    // use a priority queue to keep track of tiles to visit next
    Tile* queue = malloc(sizeof(Tile) * GRID_SITZE_X * GRID_SITZE_Y);
    int front = 0, back = 0;
    queue[back++] = startTile;

    while (front != back) {
        currentTile = queue[front++];
        GET(visited_ptr, currentTile.x, currentTile.y) = true;
        // check left, right, up, and down tiles
        neighborCount = 0;
        for (int n = 0; n < 4; n++) {
            int nx = currentTile.x + xoffset[n];
            int ny = currentTile.y + yoffset[n];
            if (nx < 0 || ny < 0 || nx >= GRID_SITZE_X || ny >= GRID_SITZE_Y)
                continue;
            if (GET(visited_ptr, nx, ny))
                continue;
            if (GET(obstacle_ptr, nx, ny))
                continue;
            int cost = currentTile.cost + 1;
            // add neighborTiles to queue and update grid with new cost
            if (GET(grid_ptr, nx, ny) > cost) {
                GET(grid_ptr, nx, ny) = cost;
                queue[back].x = nx;
                queue[back].y = ny;
                queue[back].cost = cost;
                back++;
            }
        }
    }
}

int main() {
    //tile_ptr = malloc(sizeof(Tile)*GRID_SITZE_X*GRID_SITZE_Y*4);
    obstacle_ptr = malloc(sizeof(bool) * GRID_SITZE_X * GRID_SITZE_Y);
    grid_ptr = malloc(sizeof(int) * GRID_SITZE_X * GRID_SITZE_Y);
    visited_ptr = malloc(sizeof(bool) * GRID_SITZE_X * GRID_SITZE_Y);

    // Initialize the obstacles
    for (int i = 0; i < GRID_SITZE_X * GRID_SITZE_Y; i++) {
        obstacle_ptr[i] = false;
    }
    {
        initialize_obstacles(7, 7, 2, 14);
        initialize_obstacles(7, 7, 14, 2);
        initialize_obstacles(19, 7, 2, 14);
        initialize_obstacles(7, 19, 14, 2);
        initialize_obstacles(3, 40, 40, 2);
        initialize_obstacles(3, 40, 2, 9);
        initialize_obstacles(41, 33, 2, 7);
        initialize_obstacles(35, 0, 2, 17);
        initialize_obstacles(30, 10, 2, 27);
    }

    // Initialize the grid & visited
    for (int i = 0; i < GRID_SITZE_X * GRID_SITZE_Y; i++) {
        grid_ptr[i] = INT_MAX;
        visited_ptr[i] = false;
    }

    start_time = clock();
    dijkstra();
    end_time = clock();


    // output the grid with costs
/*
    for (int j = 0; j < GRID_SITZE_Y; j++) {
        for (int i = 0; i < GRID_SITZE_X; i++) {
            if (GET(obstacle_ptr, i, j)) {
                printf("####");
            }
            else if (GET(grid_ptr, i, j) == INT_MAX) {
                printf("   ");
            }
            else {
                printf("%3d ", GET(grid_ptr, i, j));
            }
        }
        printf("\n");
    }

    // Display obstacles
    for (int y = 0; y < GRID_SITZE_Y; y++) {
        for (int x = 0; x < GRID_SITZE_X; x++) {
            if (!GET(obstacle_ptr, x, y)) {
                printf("#");
            }
            else {
                printf(".");
            }
        }
        printf("\n");
    }

    */

    printf("Time taken by the simple dijkstra: %f seconds\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);

    return 0;
}
