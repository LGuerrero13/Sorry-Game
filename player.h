#pragma once
#include <random>
#include <chrono>

struct dice {
	int d1, d2;
};

class player {
private:
	/*
		NOTE: For non commercial purposes it's probably fine to let the player
		store this kind of information. For commercial purposes you probably want to store this info
		in the server so hackers can't touch it!
	*/
	dice dices{ 0, 0 }; // init their 2 dices

public:
	std::vector<int> player_position{ 0 }; // they start at 0
	unsigned int column = 0;
	unsigned int row = 0;

	void roll_dice() {
		std::default_random_engine generator; // create instance of random engine
		// set a unique seed using chrono every call to get a more random #
		generator.seed(std::chrono::system_clock::now().time_since_epoch().count());
		std::uniform_int_distribution<int> distribution(1, 6); // 1-6
		dices.d1 = distribution(generator); // random number between 1-6
		dices.d2 = distribution(generator); // random number between 1-6
		printf("\033[0;32m[DICE]: \033[0mYou rolled %d!\n", dices.d1 + dices.d2);
		this->update_position(dices.d1 + dices.d2); // update the position once they've rolled

	}

	int get_roll_number() {
		return dices.d1 + dices.d2;
	}

	dice& get_dice_numbers() {
		return dices;
	}

	void update_position(int moves) {
		switch (moves) {
			case 4:
				if (player_position[0] == 0) { // do nothing if they're already at the beginning
					printf("\033[0;31m[INFO]: \033[0mYou're already at the beginning. Nothing happens.\n");
					break;
				}
				player_position[0] -= 1; // subtract 1 from the current position
				printf("\033[0;31m[INFO]: \033[0mYou've been sent back one space!\n");
				break;
			case 7:
				// these are special cases! let the game handle this
				break;
			case 11:
				// these are special cases! let the game handle this
				break;
			case 12:
				if (player_position[0] == 0) { // they're at the beginning
					printf("\033[0;31m[INFO]: \033[0mYou're already at the beginning. Nothing happens.\n");
					break;
				}
				else { // they're not at the beginning
					player_position[0] = 0; // reset to start!
					printf("\033[0;31m[INFO]: \033[0mYou've been sent back to the beginning!\n");
					break;
				}
			default:
				player_position[0] += moves; // add to their current position
				break;
		}
		
	}
};