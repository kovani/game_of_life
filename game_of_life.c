#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

#define SIZE_Y 25
#define SIZE_X 80
#define UP_KEY 'k'
#define DOWN_KEY 'm'



struct termios saved_attributes;

void reset_input_mode (void)
{
    tcsetattr (STDIN_FILENO, TCSANOW, &saved_attributes);
}

void set_input_mode (void)
{
    struct termios tattr;
    char *name;

    /* Make sure stdin is a terminal. */
    if (!isatty (STDIN_FILENO))
    {
        fprintf (stderr, "Not a terminal.\n");
        exit (EXIT_FAILURE);
    }

    /* Save the terminal attributes so we can restore them later. */
    tcgetattr (STDIN_FILENO, &saved_attributes);
    atexit (reset_input_mode);

    /* Set the funny terminal modes. */
    tcgetattr (STDIN_FILENO, &tattr);
    tattr.c_lflag &= ~(ICANON|ECHO); /* Clear ICANON and ECHO. */
    tattr.c_cc[VMIN] = 2;
    tattr.c_cc[VTIME] = 0;
    tcsetattr (STDIN_FILENO, TCSAFLUSH, &tattr);
}



void init_screen(char **arr, char **next_arr, int speed);
void print_screen(char **arr, int speed);
void change_speed(int speed);
void update_screen(char **arr, char **next_arr);

char ctrl_key = 'p';

int main() {
  char **screen = NULL;
  char **next_screen = NULL;
  int speed = 1000000;

  screen = (char **)malloc(SIZE_Y * sizeof(char *));
  for (int i = 0; i < SIZE_Y; i++) {
    screen[i] = (char *)malloc(SIZE_X * sizeof(char));
  }
  next_screen = (char **)malloc(SIZE_Y * sizeof(char *));
  for (int i = 0; i < SIZE_Y; i++) {
    next_screen[i] = (char *)malloc(SIZE_X * sizeof(char));
  }

  init_screen(screen, next_screen, speed);

set_input_mode();

  while (1) {
    scanf("%c", &ctrl_key);
    change_speed(speed);
    usleep(speed);
    print_screen(screen, speed);
    update_screen(screen, next_screen);
  }
  return 0;
}

void init_screen(char **arr, char **next_arr, int speed) {
  for (int i = 0; i < SIZE_Y; i++) {
    for (int j = 0; j < SIZE_X; j++) {
      char state;
      scanf("%c ", &state);
      if (state == '$' || state == '_') {
        arr[i][j] = state;
      } else {
        printf("n/a");
        return;
      }
    }
  }
  for (int i = 0; i < SIZE_Y; i++) {
    for (int j = 0; j < SIZE_Y; j++) {
      next_arr[i][j] = '_';
    }
  }
}

void print_screen(char **arr, int speed) {
  system("clear");
  printf("### GAME OF LIFE ###\n");
  printf("Current delay: %d m/s. [1] or [2] key for change velocity.\n", speed);
  for (int i = 0; i < SIZE_X + 1; i++)
    printf("XX");
  printf("\n");
  for (int i = 0; i < SIZE_Y; i++) {
    printf("X");
    for (int j = 0; j < SIZE_X; j++) {
      printf("%c ", arr[i][j]);
    }
    printf("X");
    printf("\n");
  }
  for (int i = 0; i < SIZE_X + 1; i++)
    printf("XX");
  printf("\n");
}

void change_speed(int speed) {
  char change = getchar();
	switch(ctrl_key)
	{
	 case UP_KEY: speed -= 10000;
	 case DOWN_KEY: speed += 10000;
	}
}

void update_screen(char **arr, char **next_arr) {
  for (int i = 0; i < SIZE_Y; i++) {
    for (int j = 0; j < SIZE_X; j++) {
      int sosed = 0;

      for (int y = -1; y <= 1; y++) {
        for (int x = -1; x <= 1; x++) {
          if (y == 0 && x == 0)
            continue;

          int sosed_Y = i + y;
          int sosed_X = j + x;

          if (sosed_Y >= 0 && sosed_Y < SIZE_Y && sosed_X >= 0 &&
              sosed_X < SIZE_X && arr[sosed_Y][sosed_X] == '$')
            sosed++;
        }
      }
      if (arr[i][j] == '$' && (sosed < 2 || sosed > 3))
        next_arr[i][j] = '_';
      else if (arr[i][j] == '_' && sosed == 3)
        next_arr[i][j] = '$';
      else
        next_arr[i][j] = arr[i][j];
    }
  }
  for (int i = 0; i < SIZE_Y; i++) {
    for (int j = 0; j < SIZE_X; j++) {
      arr[i][j] = next_arr[i][j];
    }
  }
}
