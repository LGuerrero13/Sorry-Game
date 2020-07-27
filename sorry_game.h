#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <conio.h>
#include <algorithm>
#include <mmsystem.h> // must include the .lib file in the linker
#include "player.h"
#include "sgui.h"

#define MAX_BOARD_POSITIONS 50

class sorry_game {
private:
	std::vector<std::shared_ptr<player>> player_list; // vector that will hold the players
	std::vector<int> board_positions; // vector that will hold all the board positions
	std::unique_ptr<sgui> sgui_board;
	std::string has_game_been_won = "init"; // game response


	/* setup the board positions */
	void init_board_positions() {
		for (int i = 1; i <= MAX_BOARD_POSITIONS; i++) { // create 0-49 indexes that hold 1-50 instead of writing it out :)
			board_positions.push_back(i); // add the number to the vector and increase the size
		}
	}

	/* init and create smart pointer instances for our players */
	void init_players(int player_amount) {
		for (int i = 0; i < player_amount; i++) { // loop through amount of players they specified
			this->sgui_board = std::unique_ptr<sgui>(new sgui); // init sgui smart pointer
			std::shared_ptr<player> new_player(new player()); // create a new instance of the player. can't use unique_ptr cause we need to copy the ptr
			this->player_list.push_back(new_player); // add that player to the vector
		}
		#ifdef _DEBUG
			printf("Amount of players currently: %d\n", this->player_list.size()); // used for debugging purposes
		#endif
	}

	/* player dice roll */
	void wait_for_player_roll() {
		size_t i = 0;
		while (i <= player_list.size()) { // keep repeating so each player has a turn
			for (i = 0; i < this->player_list.size(); i++) { // loop through each player and have them hit enter
				this->check_for_player_collisions(i); // check for any collisions
				printf("\nPlayer %d's turn. Hit enter to roll both dices\n", i + 1);
				_getch(); // pause and hit any button to continue
				this->player_list[i]->roll_dice(); // roll the dice for the current player
				this->check_for_player_collisions(i); // check for any collisions
				this->has_player_rolled_double(i); // check if the player has rolled a double
				this->should_swap_player_with_leader(i); // check if we need to swap with leader
				this->should_swap_player_with_loser(i); // check if we need to swap with loser
				this->is_player_over_ending(i); // board wrapping check
				this->print_player_positions_info(); // print player pos after each roll
				this->sgui_board->sgui_update_player_board_pos(player_list);
				this->sgui_board->sgui_print_board();
				if (this->handle_game_won()) { // check if a player has won!
					this->new_game_response(); // do they want to play again?
					return; // break out of the game loop
				}
			}
		}
	}

	/*
		Check for colliding players
		NOTE: found it more appropriate for the game to check
		through all the player positions and check for collision instead
		of having that in the player class
	*/
	void check_for_player_collisions(int index_of_currently_rolled_player) {
		for (size_t i = 0; i < this->player_list.size(); i++) { // loop through all the players

			if (i == index_of_currently_rolled_player) continue; // that's us silly!

			// check to see if they're both at the beginning of the board
			if (this->player_list[index_of_currently_rolled_player]->player_position[0] == 0 && this->player_list[i]->player_position[0] == 0) return;

			// if the current rolling player landed on a player
			if (this->player_list[index_of_currently_rolled_player]->player_position[0] == this->player_list[i]->player_position[0]) {
				this->player_list[i]->player_position[0] = 0; // reset that players position!
				printf("\033[0;31m[INFO]: \033[0mPlayer %d landed on player %d! Player %d has been set back to the beginning.\n", index_of_currently_rolled_player + 1, i + 1, i + 1);
			}
		}
	}

	/* check if the player rolled a 7 */
	void should_swap_player_with_leader(int current_player_index) {
		if (this->player_list[current_player_index]->get_roll_number() != 7) return;

		// where we store the value of leading player
		int leader_index = this->get_leader();

		if (leader_index == current_player_index) { // we're first, do nothing!
			printf("\033[0;31m[INFO]: \033[0mYou're in first, nothing happens.\n");
			return;
		}

		// get current pos of roller
		int cur_player_pos = this->player_list[current_player_index]->player_position[0];
		// get current pos of leader
		int leader_player_pos = this->player_list[leader_index]->player_position[0];

		#ifdef _DEBUG
			printf("BEFORE: cur_player_pos: %d | leader_player_pos: %d\n", cur_player_pos, leader_player_pos);
		#endif

		// swap both of our positions!
		this->player_list[leader_index]->player_position[0] = cur_player_pos;
		this->player_list[current_player_index]->player_position[0] = leader_player_pos;

		printf("\033[0;31m[INFO]: \033[0mYou've rolled a 7, you swap positions with the leader!\n");

		#ifdef _DEBUG
			printf("AFTER: cur_player_pos: %d | leader_player_pos: %d\n", this->player_list[current_player_index]->player_position[0], this->player_list[leader_index]->player_position[0]);
		#endif

		#ifdef _DEBUG
			printf("should_swap_player_with_leader() called!\n");
		#endif
	}

	/* check if the player rolled an 11 */
	void should_swap_player_with_loser(int current_player_index) {
		if (this->player_list[current_player_index]->get_roll_number() != 11) return;

		// get the index of the leader
		// fixme: doesn't get player with highest position count, currently only gets max value in player_list.
		int loser_index = this->get_loser();

		if (loser_index == current_player_index) { // we're last, do nothing!
			printf("\033[0;31m[INFO]: \033[0mYou're in last, nothing happens.\n");
			return;
		}

		// get current pos of roller
		int cur_player_pos = this->player_list[current_player_index]->player_position[0];
		// get current pos of leader
		int loser_player_pos = this->player_list[loser_index]->player_position[0];

		#ifdef _DEBUG
			printf("BEFORE: cur_player_pos: %d | loser_player_pos: %d\n", cur_player_pos, loser_player_pos);
		#endif

		// swap both of our positions!
		this->player_list[loser_index]->player_position[0] = cur_player_pos;
		this->player_list[current_player_index]->player_position[0] = loser_player_pos;

		printf("\033[0;31m[INFO]: \033[0mYou've rolled an 11, you swap positions with the loser!\n");

		#ifdef _DEBUG
			printf("AFTER: cur_player_pos: %d | loser_player_pos: %d\n", this->player_list[current_player_index]->player_position[0], this->player_list[loser_index]->player_position[0]);
		#endif

		#ifdef _DEBUG
			printf("should_swap_player_with_loser() called!\n");
		#endif
	}

	int get_leader() {
		std::vector<int> player_positions{}; // holder position for all players positions for easier reading

		for (size_t i = 0; i < this->player_list.size(); i++) { // loop through each player
			player_positions.push_back(this->player_list[i]->player_position[0]); // store their position in vector and add size
		}

		// return the index of the player with highest pos
		return std::distance(player_positions.begin(), std::max_element(player_positions.begin(), player_positions.end()));
	}

	int get_loser() {
		std::vector<int> player_positions{}; // holder position for all players positions for easier reading

		for (size_t i = 0; i < this->player_list.size(); i++) { // loop through each player
			player_positions.push_back(this->player_list[i]->player_position[0]); // store their position in vector and add size
		}

		// return the index of the player with highest pos
		return std::distance(player_positions.begin(), std::min_element(player_positions.begin(), player_positions.end()));
	}

	/* function for letting the player roll again if they've rolled a double */
	void has_player_rolled_double(int current_player_index) {

		unsigned int rolled_in_a_row = 0; // init amount of times they've rolled a double in a row

		while (this->player_list[current_player_index]->get_dice_numbers().d1 == this->player_list[current_player_index]->get_dice_numbers().d2) { // keep looping if they rolled a double
		// return if dice = 0 cause that's what they are in beginning or when they're reset;
			if (this->player_list[current_player_index]->get_dice_numbers().d1 == 0) return;
			if (this->player_list[current_player_index]->get_dice_numbers().d2 == 0) return;

			printf("\033[0;31m[INFO]: \033[0mYou've rolled a double ({%d, %d}) so you roll again.\n", this->player_list[current_player_index]->get_dice_numbers().d1, this->player_list[current_player_index]->get_dice_numbers().d2);

			rolled_in_a_row += 1; // add 1 to current count of double rolls

			#ifdef _DEBUG
				printf("\033[0;31m[INFO]: \033[0mrolled_in_a_row: %d\n", rolled_in_a_row);
			#endif

			if (rolled_in_a_row >= 2) { // check if they've rolled a double twice or more
				printf("\033[0;31m[INFO]: \033[0mYou've rolled a double twice buddy. Let's send you back to the beginning.\n");
				this->player_list[current_player_index]->player_position[0] = 0; // set them back to the beginning
				return; // do not continue and just get out!
			}

			this->player_list[current_player_index]->get_dice_numbers().d1 = 0; // reset dice value, we get it by reference so we can set the actual value back
			this->player_list[current_player_index]->get_dice_numbers().d2 = 0;

			this->player_list[current_player_index]->roll_dice(); // let them roll again!
		}
	}

	/* function for board wrapping, if the player has gone over the end */
	void is_player_over_ending(int current_player_index) {
		if (this->player_list[current_player_index]->player_position[0] > 50) { // if they've gone above 50
			// subtract 50 from where they've gone to reset them back but also get correct position
			this->player_list[current_player_index]->player_position[0] -= 50;
		}
	}

	/* let us know of positions for each player */
	void print_player_positions_info() {
		std::cout << std::endl << std::endl; // for spacing
		for (size_t i = 0; i < this->player_list.size(); i++) { // loop through each player
			printf("Player %d pos: %d\n", i + 1, this->player_list[i]->player_position[0]);
		}
		std::cout << std::endl << std::endl;
	}

	/* a player has won, end the game! */
	bool handle_game_won() {
		for (size_t i = 0; i < this->player_list.size(); i++) { // loop through all players
			if (this->player_list[i]->player_position[0] == 50) { // are they on spot 50?
				printf("\033[1;33mPlayer %d has won!\033[0m\n", i + 1); // he won
				// play them a little chime, sound folder is relative to solution directory
				//PlaySound(TEXT("dependencies/sounds/win_sound.wav"), NULL, SND_FILENAME);
				return true;
			}
		}
		return false;
	}

	/* called after a player has won, ask if the want to play again or not */
	void new_game_response() {
		printf("Would you like to start a new game? Enter 'yes' or 'no'\n");
		std::cin >> this->has_game_been_won; // get response for new game or not

		if (this->has_game_been_won == "yes" || this->has_game_been_won == "Yes") {
			system("cls"); // clear the console
			this->player_list.clear(); // clear all previous players from our vector!
		}
		else if (this->has_game_been_won == "no" || this->has_game_been_won == "No") {
			std::exit(0);
		}
		else {
			printf("Invalid response, closing.\n");
		}
	}

public:
	/* 
		init our game instance.
		only public function in class cause all other functions shouldn't be touched by anyone else outside
	*/
	void init_game() {
		// keep looping if it's initial game or they said yes
		while (this->has_game_been_won == "init" || this->has_game_been_won == "yes" || this->has_game_been_won == "Yes") {
			std::cout << "Please enter the amount of players that will be playing. (2-4)" << std::endl;
			int amount_of_players = 0;

			do {
				std::cin >> amount_of_players; // get the amount of players the input
			} while (amount_of_players <= 1 || amount_of_players > 4); // keep asking them if the amount of players is not between 2-4

			this->init_board_positions(); // setup our board baby!
			this->init_players(amount_of_players); // create the amount of players stated
			this->wait_for_player_roll(); // roll loop for each player
		}
	}
};