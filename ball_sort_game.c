#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

#define MAX_TUBES 10
#define MAX_BALLS_PER_TUBE 4

int tubes[MAX_TUBES][MAX_BALLS_PER_TUBE];
int tubeSizes[MAX_TUBES];
int totalTubes;
int totalColors;

char colors[] = {'K', 'M', 'T', 'P', 'S', 'Y', 'G', 'L'}; // K: Red, M: Blue, etc.

// Function to display the current state of the tubes and balls
void displayTubes() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    printf("Game Area:\n");
    for (int i = 0; i < totalTubes; i++) {
        printf("Tube %d: ", i + 1);
        for (int j = 0; j < tubeSizes[i]; j++) {
            printf("%c ", colors[tubes[i][j] - 1]);
        }
        printf("\n");
    }
}

// Function to check if the game is completed
bool isGameFinished() {
    int separatedColors = 0;

    for (int color = 1; color <= totalColors; color++) {
        bool colorSeparated = false;

        for (int i = 0; i < totalTubes; i++) {
            if (tubeSizes[i] == MAX_BALLS_PER_TUBE) {
                bool sameColor = true;
                for (int j = 0; j < MAX_BALLS_PER_TUBE; j++) {
                    if (tubes[i][j] != color) {
                        sameColor = false;
                        break;
                    }
                }
                if (sameColor) {
                    colorSeparated = true;
                    break;
                }
            }
        }
        if (colorSeparated) {
            separatedColors++;
        }
    }

    return separatedColors == totalColors;
}

// Function to validate a move from one tube to another
bool isValidMove(int from, int to) {
    if (tubeSizes[from] == 0) return false;  // Can't move from an empty tube
    if (tubeSizes[to] >= MAX_BALLS_PER_TUBE) return false;  // Target tube is full
    if (tubeSizes[to] > 0 && tubes[to][tubeSizes[to] - 1] != tubes[from][tubeSizes[from] - 1]) return false;  // Different colors can't stack
    return true;
}

// Function to move a ball from one tube to another
void moveBall(int from, int to) {
    tubes[to][tubeSizes[to]] = tubes[from][tubeSizes[from] - 1];
    tubeSizes[to]++;
    tubeSizes[from]--;
}

int main() {
    srand(time(NULL));  // Seed random number generator

    // Get the number of tubes from user
    printf("Enter the number of tubes (5-10): ");
    scanf("%d", &totalTubes);
    if (totalTubes < 5 || totalTubes > MAX_TUBES) {
        printf("Invalid number of tubes. Please choose between 5 and 10.\n");
        return 1;
    }

    totalColors = totalTubes - 2;

    // Initialize and shuffle the balls
    int balls[MAX_TUBES * MAX_BALLS_PER_TUBE];
    for (int i = 0; i < totalColors; i++) {
        for (int j = 0; j < MAX_BALLS_PER_TUBE; j++) {
            balls[i * MAX_BALLS_PER_TUBE + j] = i + 1;
        }
    }

    for (int i = 0; i < totalColors * MAX_BALLS_PER_TUBE; i++) {
        int randIndex = rand() % (totalColors * MAX_BALLS_PER_TUBE);
        int temp = balls[i];
        balls[i] = balls[randIndex];
        balls[randIndex] = temp;
    }

    // Fill tubes with balls
    int ballIndex = 0;
    for (int i = 0; i < totalTubes; i++) {
        tubeSizes[i] = (i < totalTubes - 2) ? MAX_BALLS_PER_TUBE : 0;
        for (int j = 0; j < tubeSizes[i]; j++) {
            tubes[i][j] = balls[ballIndex++];
        }
    }

    int moves = 0;

    // Main game loop
    while (true) {
        displayTubes();
        printf("Moves made: %d\n", moves);

        int from, to;
        printf("Move from tube (1-%d): ", totalTubes);
        scanf("%d", &from);
        printf("Move to tube (1-%d): ", totalTubes);
        scanf("%d", &to);

        if (from < 1 || from > totalTubes || to < 1 || to > totalTubes) {
            printf("Invalid tube numbers entered. Try again.\n");
            sleep(1);
            continue;
        }

        from--;
        to--;

        if (isValidMove(from, to)) {
            moveBall(from, to);
            moves++;

            if (isGameFinished()) {
                displayTubes();
                printf("Congratulations! You've completed the game in %d moves.\n", moves);
                break;
            }
        } else {
            printf("Invalid move. Try again.\n");
            sleep(1);
        }
    }

    return 0;
}
