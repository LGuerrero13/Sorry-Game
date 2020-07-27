#pragma once

/* sorry-game user interface WIP */
class sgui {
public:
	std::vector<std::vector<std::string>> board_layout
	{
		{"|   |", "|   |", "|   |", "|   |", "|   |", "|   |", "|   |", "|   |", "|   |", "|   |"},
		{"|   |", "|   |", "|   |", "|   |", "|   |", "|   |", "|   |", "|   |", "|   |", "|   |"},
		{"|   |", "|   |", "|   |", "|   |", "|   |", "|   |", "|   |", "|   |", "|   |", "|   |"},
		{"|   |", "|   |", "|   |", "|   |", "|   |", "|   |", "|   |", "|   |", "|   |", "|   |"},
		{"|   |", "|   |", "|   |", "|   |", "|   |", "|   |", "|   |", "|   |", "|   |", "|   |"}
	};

	void sgui_print_board() {
		for (size_t i = 0; i < board_layout.size(); i++) {
			std::cout << std::endl; // addline for new column
			for (size_t j = 0; j < board_layout[i].size(); j++) {
				std::cout << board_layout[i][j]; // print the values in the array into the console
			}
		}
		std::cout << std::endl; // addline for new column
	}

	/* reset the board to its original state */
	void reset_board() {
		for (size_t i = 0; i < 5; i++) {
			for (size_t j = 0; j < 10; j++) {
				board_layout[i][j] = "|   |";
			}
		}
	}

	void sgui_update_player_board_pos(std::vector<std::shared_ptr<player>> player_list) {

		reset_board(); // clear our board to stop duplication of positions

		for (size_t i = 0; i < player_list.size(); i++) { // loop through each player

			player_list[i]->row = player_list[i]->player_position[0]; // initial row will be the player pos
			player_list[i]->column = 0; // initial column will be 0

			if (player_list[i]->player_position[0] > 10 && player_list[i]->player_position[0] <= 19) {
				player_list[i]->column = 1; // we went to next column, add 1
				player_list[i]->row = player_list[i]->player_position[0] - 10; // remove 10 cause 1 column = 10 rows (in theory)
			}
			else if (player_list[i]->player_position[0] > 20 && player_list[i]->player_position[0] <= 29) {
				player_list[i]->column = 2;
				player_list[i]->row = player_list[i]->player_position[0] - 20;
			}
			else if (player_list[i]->player_position[0] > 30 && player_list[i]->player_position[0] <= 39) {
				player_list[i]->column = 3;
				player_list[i]->row = player_list[i]->player_position[0] - 30;
			}
			else if (player_list[i]->player_position[0] > 40 && player_list[i]->player_position[0] <= 49) {
				player_list[i]->column = 4;
				player_list[i]->row = player_list[i]->player_position[0] - 40;
			}
			else if (player_list[i]->player_position[0] == 20) {
				player_list[i]->column = 1; // 20th box is in [1][10]
				player_list[i]->row = 10;
			}
			else if (player_list[i]->player_position[0] == 30) {
				player_list[i]->column = 2; // 30th box is in [2][10]
				player_list[i]->row = 10;
			}
			else if (player_list[i]->player_position[0] == 40) {
				player_list[i]->column = 3; // 40th box is in [3][10]
				player_list[i]->row = 10;
			}
			else if (player_list[i]->player_position[0] == 50) {
				player_list[i]->column = 4; // 50th box is in [4][10]
				player_list[i]->row = 10;
			}

			char buffer[6]; // create an array of 6 characters
			snprintf(buffer, sizeof(buffer), "| %d |", i + 1); // copy the converted string into buffer

			if (player_list[i]->row == 0) // 0 check so we don't subtract 1 from row when index is 0 or we crash
				board_layout[player_list[i]->column][player_list[i]->row] = buffer; // add the text into that position
			else
				board_layout[player_list[i]->column][player_list[i]->row - 1] = buffer; // add the text into that position but - 1 to get a nice display

			player_list[i]->column = 0; // reset var for next use 
			player_list[i]->row = 0; // reset var for next use 
		}
	}
};