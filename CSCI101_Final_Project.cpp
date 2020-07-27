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
    game->init_game(); // start the game
}