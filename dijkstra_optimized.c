#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>

#define GET(arr,x,y) (arr)[(y) * GRID_SITZE_X + (x)]
#define GRID_SITZE_X 10048
#define GRID_SITZE_Y 10048
#define GOAL_X 1024
#define GOAL_Y 1024

clock_t start_time1, end_time1, start_time2, end_time2;

typedef struct tile {
    int x;
    int y;
}Tile;

// 4 adjacent tiles directly connected (sharing the same edge)
int xoffset[4] = { 0, 1, 0, -1 };
int yoffset[4] = { 1, 0, -1, 0 };
// 8 adjacent tiles
int xvec[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
int yvec[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };

bool* obstacle_ptr;
int* grid_ptr;
bool* visited_ptr;
char* vector_ptr;

void initialize_obstacles(int x, int y, int w, int h) {
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            if (x + i < 0 || y + j < 0 || x + i >= GRID_SITZE_X || y + j >= GRID_SITZE_Y)
                continue;
            GET(obstacle_ptr, x + i, y + j) = true;

        }
    }
}

void create_flow_field() {

    for (int x = 0; x < GRID_SITZE_X; x++) {
        for (int y = 0; y < GRID_SITZE_Y; y++) {
            int min_cost = INT_MAX;
            //go through 8 sourrounding tiles
            for (int n = 0; n < 8; n++) {
                //check for out of bounds & blocked tiles
                if (!(x + xvec[n] < 0 || y + yvec[n] < 0 || x + xvec[n] >= GRID_SITZE_X || y + yvec[n] >= GRID_SITZE_Y)) {
                    if (GET(obstacle_ptr, x, y) || GET(grid_ptr, x, y) == INT_MAX) { GET(vector_ptr, x, y) = ' '; continue; }
                    int cost = GET(grid_ptr, x + xvec[n], y + yvec[n]);
                    //place most efficient "vector" (direction)
                    if (cost < min_cost) {
                        min_cost = cost;
                        if (cost < GET(grid_ptr, x, y) && cost <= min_cost) {

                            if (n == 0) {
                                GET(vector_ptr, x, y) = 'D';
                            }
                            else if (n == 1) {
                                GET(vector_ptr, x, y) = 'N';
                            }
                            else if (n == 2) {
                                GET(vector_ptr, x, y) = 'R';
                            }
                            else if (n == 3) {
                                GET(vector_ptr, x, y) = 'W';
                            }
                            else if (n == 4) {
                                GET(vector_ptr, x, y) = 'U';
                            }
                            else if (n == 5) {
                                GET(vector_ptr, x, y) = 'S';
                            }
                            else if (n == 6) {
                                GET(vector_ptr, x, y) = 'L';
                            }
                            else if (n == 7) {
                                GET(vector_ptr, x, y) = 'E';
                            }
                        }
                        else if (GET(grid_ptr, x, y) == 0) {
                            GET(vector_ptr, x, y) = 'O';
                        }
                    }
                }
            }
        }
    }
}

void dijkstra() {

    Tile startTile = { GOAL_X, GOAL_Y };
    Tile currentTile;
    int neighborCount;

    GET(grid_ptr, startTile.x, startTile.y) = 0;


    // use a priority queue to keep track of tiles to visit next
    Tile* workload = malloc(sizeof(Tile) * GRID_SITZE_X * GRID_SITZE_Y);
    Tile* new_workload = malloc(sizeof(Tile) * GRID_SITZE_X * GRID_SITZE_Y);
    workload[0] = startTile;
    int cost = 0;
    int new_workload_count = 0, workload_count = 1;
    while (workload_count > 0) {
        cost++;
        new_workload_count = 0;
        for (int i = 0; i < workload_count; i++) {
            currentTile = workload[i];
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
                // add neighborTiles to new workload and update grid with new cost
                GET(grid_ptr, nx, ny) = cost;
                GET(visited_ptr, nx, ny) = true;
                new_workload[new_workload_count].x = nx;
                new_workload[new_workload_count].y = ny;
                new_workload_count++;
            }
        }
        Tile* temp = new_workload;
        new_workload = workload;
        workload = temp;
        workload_count = new_workload_count;
    }
}

int main() {
    //tile_ptr = malloc(sizeof(Tile)*GRID_SITZE_X*GRID_SITZE_Y*4);
    obstacle_ptr = malloc(sizeof(bool) * GRID_SITZE_X * GRID_SITZE_Y);
    grid_ptr = malloc(sizeof(int) * GRID_SITZE_X * GRID_SITZE_Y);
    visited_ptr = malloc(sizeof(bool) * GRID_SITZE_X * GRID_SITZE_Y);
    vector_ptr = malloc(sizeof(char) * GRID_SITZE_X * GRID_SITZE_Y);

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

    start_time1 = clock();
    dijkstra();
    end_time1 = clock();

    start_time2 = clock();
    create_flow_field();
    end_time2 = clock();

    /* output the grid with costs

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

    printf("Time taken by dijkstra algorithm: %f seconds\n", (double)(end_time1 - start_time1) / CLOCKS_PER_SEC);
    printf("Time taken by create_flow_field algorithm: %f seconds\n", (double)(end_time2 - start_time2) / CLOCKS_PER_SEC);
    return 0;
}
