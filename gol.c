#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define SIZE_Y 25
#define SIZE_X 80

char board[SIZE_Y][SIZE_X];
char next_board[SIZE_Y][SIZE_X];

void init_board() {
    for (int i = 0; i < SIZE_Y; i++) {
        for (int j = 0; j < SIZE_X; j++) {
            char state;
            scanf(" %c", &state);
            if (state == '$' || state == '_') {
                board[i][j] = state;
            } else {
                printf("n/a");
                return;
            }
        }
    }
}

void print_board() {
    system("clear");
    for (int i = 0; i < SIZE_Y; i++) {
        for (int j = 0; j < SIZE_X; j++) {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void update_board() {
    usleep(500000);
    for (int i = 0; i < SIZE_Y; i++) {
        for (int j = 0; j < SIZE_X; j++) {
            int sosed = 0;

            for (int y = -1; y <= 1; y++) {
                for (int x = -1; x <= 1; x++) {
                    if (y == 0 && x == 0) continue;

                    int sosed_Y = i + y;
                    int sosed_X = j + x;

                    if (sosed_Y >= 0 && sosed_Y < SIZE_Y && sosed_X >= 0 && sosed_X < SIZE_X &&
                        board[sosed_Y][sosed_X] == '$')
                        sosed++;
                }
            }

            if (board[i][j] == '$' && (sosed < 2 || sosed > 3))
                next_board[i][j] = '_';
            else if (board[i][j] == '_' && sosed == 3)
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
