#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define SIZE_Y 50
#define SIZE_X 160

char board[SIZE_Y][SIZE_X];
char next_board[SIZE_Y][SIZE_X];

void init_board() {
    for (int i = 0; i < SIZE_Y; i++) {
        for (int j = 0; j < SIZE_X; j++) {
            char state;
            scanf(" %c", &state);
            if (state == '$' || state == '.') {
                board[i][j] = state;
            } else {
                printf("n/a");
                return;
            }
        }
    }
}

void print_board() {
    usleep(100000);
    system("clear");
    for (int i = 0; i < SIZE_Y; i++) {
        for (int j = 0; j < SIZE_X; j++) {
            printf("%c", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void update_board() {
    for (int i = 0; i < SIZE_Y; i++) {
        for (int j = 0; j < SIZE_X; j++) {
            int sosed = 0;

            for (int y = -1; y <= 1; y++) {
                for (int x = -1; x <= 1; x++) {
                    //if (y == 0 && x == 0) continue;

                    int sosed_Y = i + y;
                    int sosed_X = j + x;

                    if (sosed_Y == -1) sosed_Y = SIZE_Y-1;
                    if (sosed_X == -1) sosed_X = SIZE_X-1;
                    if (sosed_Y == SIZE_Y) sosed_Y = 0;
                    if (sosed_X == SIZE_X) sosed_X = 0;

                    if (board[sosed_Y][sosed_X] == '$')
                        sosed++;
                }
            }

            if (board[i][j] == '$' && (sosed < 2 || sosed > 4))
                next_board[i][j] = '.';
            else if (board[i][j] == '.' && sosed == 3)
                next_board[i][j] = '$';
            else
                next_board[i][j] = board[i][j];
        }
    }

    for (int i = 0; i < SIZE_Y; i++) {
        for (int j = 0; j < SIZE_X; j++) {
            board[i][j] = next_board[i][j];
        }
    }
}

int main() {
    init_board();
    while (1) {
        print_board();
        update_board();
    }
    return 0;
}
