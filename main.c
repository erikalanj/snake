#include <ncurses.h>

#include <stdlib.h>

#include <time.h>

#include <unistd.h>
 // Author: Blaise Burby & Erik Jensen
// Initialization of variables
void compile_game();
void create_border();
void update_snake();
void snake_input(int key);
void end_game();
void place_trophy();
void check_trophy_expiration();
void check_win_condition();
int snake_x[1024], snake_y[1024];
int snake_length = 3;
int snake_direction;
int max_x, max_y;
int trophy_x, trophy_y, trophy_value;
time_t trophy_time;
// Author: Erik Jensen
// Main method declaration with game start/end and snake speed
int main() {

  compile_game();

  int user_input;

  while ((user_input = getch()) != 'q') { // quit the program when user presses q

    snake_input(user_input);

    update_snake();

    check_trophy_expiration();

    check_win_condition();

    usleep(500000 / snake_length); // speed proportional to snake length (it gets really fast)

  }

  end_game();

  return 0;
}
// Author: Blaise Burby
// Compile function that sets up the terminal window, border, and initial snake body
void compile_game() {

  initscr();

  curs_set(FALSE); // no cursor but more specifically no current character box when in game

  keypad(stdscr, TRUE); // arrow key enabler

  nodelay(stdscr, TRUE); // allows the program to play out without input

  // terminal window size grabber

  getmaxyx(stdscr, max_y, max_x);

  srand(time(NULL)); // seed the random number generator once at the beginning

  int initial_directions[] = {
    KEY_UP,
    KEY_DOWN,
    KEY_LEFT,
    KEY_RIGHT
  };

  snake_direction = initial_directions[rand() % 4]; // randomly select the direction of the snake starting

  // insert the initial snake body at start position

  for (int i = 0; i < snake_length; i++) {

    snake_x[i] = max_x / 2 - i;

    snake_y[i] = max_y / 2;

  }

  create_border();

  place_trophy(); // places initial trophy
}
// Author: Blaise Burby
// Creates the border by duplicating | and attaching it to the terminal sides
void create_border() {

  for (int x = 0; x < max_x; x++) { // x wall creations

    mvaddch(0, x, '|'); // top

    mvaddch(max_y - 1, x, '|'); // bottom

  }

  for (int y = 0; y < max_y; y++) { // y creation

    mvaddch(y, 0, '|'); // left

    mvaddch(y, max_x - 1, '|'); // right

  }
}
// Author: Blaise Burby & Erik Jensen
// Main snake movement, collision, and body update method
void update_snake() {

  int move_x = snake_x[0];

  int move_y = snake_y[0];

  switch (snake_direction) {

  case KEY_UP:

    move_y--; // decreases y so snake goes up

    break;

  case KEY_DOWN:

    move_y++; // increases y so snake goes down

    break;

  case KEY_LEFT:

    move_x--; // decreases x so snake goes left

    break;

  case KEY_RIGHT:

    move_x++; // increases x so snake goes right

    break;

  }

  // border hit check

  if (move_x == 0 || move_x == max_x - 1 || move_y == 0 || move_y == max_y - 1) {

    end_game();

    exit(0);

  }

  // self hit check

  for (int i = 1; i < snake_length; i++) {

    if (snake_x[i] == move_x && snake_y[i] == move_y) {

      end_game();

      exit(0);

    }

  }

  // body segment following updater

  for (int i = snake_length - 1; i > 0; i--) {

    snake_x[i] = snake_x[i - 1];

    snake_y[i] = snake_y[i - 1];

  }

  snake_x[0] = move_x;

  snake_y[0] = move_y;

  if (snake_x[0] == trophy_x && snake_y[0] == trophy_y) { // trophy eat check

    snake_length += trophy_value; // increase the length of the snake by the value of the trophy

    place_trophy(); // replace trophy

  }

  clear();

  create_border();

  // loop that creates the snake depending on the starting length

  for (int i = 0; i < snake_length; i++) {

    mvaddch(snake_y[i], snake_x[i], 'O');

  }

  mvaddch(trophy_y, trophy_x, '0' + trophy_value); // trophy drawer

  refresh();
}
// Author: Erik Jensen
// Main user input registration method
void snake_input(int key) {

  switch (key) {

  case KEY_UP:

    if (snake_direction != KEY_DOWN) snake_direction = KEY_UP;

    break;

  case KEY_DOWN:

    if (snake_direction != KEY_UP) snake_direction = KEY_DOWN;

    break;

  case KEY_LEFT:

    if (snake_direction != KEY_RIGHT) snake_direction = KEY_LEFT;

    break;

  case KEY_RIGHT:

    if (snake_direction != KEY_LEFT) snake_direction = KEY_RIGHT;

    break;

  }

  // wrong direction check

  if ((key == KEY_UP && snake_direction == KEY_DOWN) ||

    (key == KEY_DOWN && snake_direction == KEY_UP) ||

    (key == KEY_LEFT && snake_direction == KEY_RIGHT) ||

    (key == KEY_RIGHT && snake_direction == KEY_LEFT)) {

    end_game();

    exit(0);

  }
}
// Author: Erik Jensen

// This method heavily uses the random class to randomly declare where it will place each trophy one after the other
void place_trophy() {

  trophy_x = rand() % (max_x - 2) + 1; // the x position of the trophy

  trophy_y = rand() % (max_y - 2) + 1; // the y position of the trophy

  trophy_value = rand() % 9 + 1; // value of the trophy

  trophy_time = time(NULL) + rand() % 9 + 1; // random expiration time between 1 to 9 seconds
}
// Author: Blaise Burby
// Check trophy expiration/refresh
void check_trophy_expiration() {

  if (time(NULL) >= trophy_time) {

    place_trophy();

  }
}
// Author: Blaise Burby
// Check win condition method (if the snake grows to be half of the perimeter of the terminal)
void check_win_condition() {

  int half_perimeter = (max_x + max_y - 2) / 2;

  if (snake_length >= half_perimeter) {

    endwin();

    printf("You won!\n"); // end program and print "you won"

    exit(0);

  }
}
// Author: Erik Jensen
// End game and close the terminal window
void end_game() {

    endwin();

    printf("You lost!\n"); // end program and print "you lost"
}
