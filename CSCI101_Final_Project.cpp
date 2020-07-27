/*
    Program name: Sorry Game
    Author: Jose Leonardo Guerrero
    Date: 7/13/20
    Purpose: Play sorry with two dice, and one peg. Move your piece down the board first.
    But rolling certain numbers will change the pace of the game.
*/

#include <windows.h>
#include "sorry_game.h"


int main()
{
    std::unique_ptr<sorry_game> game(new sorry_game);

    /* https://stackoverflow.com/questions/7414983/how-to-use-the-ansi-escape-code-for-outputting-colored-text-on-console */
    DWORD l_mode;
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleMode(hStdout, &l_mode);
    SetConsoleMode(hStdout, l_mode |
        ENABLE_VIRTUAL_TERMINAL_PROCESSING |
        DISABLE_NEWLINE_AUTO_RETURN);

    game->init_game(); // start the game
}