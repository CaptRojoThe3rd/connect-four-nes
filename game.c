#include "LIB/neslib.h"
#include "LIB/nesdoug.h"
#include "LIB/nescapt.h"

#pragma bss-name(push, "ZEROPAGE")
unsigned char pad_1;
unsigned char pad_2;
unsigned char selected_option;
unsigned char gamemode;
unsigned char piece_pos;
unsigned char piece_pos_offset_plus;
unsigned char piece_pos_offset_minus;
unsigned char turn;

unsigned char i;
unsigned char j;
unsigned char k;
unsigned char l;
unsigned char m;
unsigned short a;

unsigned char win_check_data[4];
unsigned char win_positions[4];
unsigned char board[42];
unsigned char board_computer_move_check[42];
unsigned char board_player_move_check[42];

unsigned char test_computer_move;
unsigned char test_player_move;
unsigned char computer_move_scores[7];
unsigned char computer_move_found;
#pragma bss-name(pop)

const unsigned char title_pal[] = {
	0xf, 0, 0x10, 0x30,
	0xf, 0xa, 0x1a, 0x30,
	0xf, 0, 0, 0,
	0xf, 0, 0, 0
};
const unsigned char bg_pal_game[] = {
	0x1c, 0x1c, 0x1c, 0x30,
	0x1c, 0x2, 0x11, 0x30,
	0x1c, 0x6, 0x16, 0x30,
	0x1c, 0x1c, 0xf, 0x30
};
const unsigned char spr_pal_game[] = {
	0x1c, 0x2, 0x11, 0x12,
	0x1c, 0x6, 0x16, 0x5,
	0x1c, 0x17, 0x27, 0x37,
	0x1c, 0x1c, 0x1c, 0x30
};

/*
All text used in game

PLAYER VS. PLAYER
PLAYER VS. COMPUTER
PLAYER 1'S TURN
PLAYER 2'S TURN
COMPUTER'S TURN
PLAYER 1 WINS
PLAYER 2 WINS
COMPUTER WINS

Only need to have the characters used in the CHR-ROM - Characters used:
ACEILMNOPRSTUVWY
.'12

*/
const unsigned char player_vs_player_text[] = {
	0xa, 0x6, 0x1, 0x11, 0x3, 0xb, 0, 0xf, 0xc, 0x12, 0, 0xa, 0x6, 0x1, 0x11, 0x3, 0xb
};
const unsigned char player_vs_computer_text[] = {
	0xa, 0x6, 0x1, 0x11, 0x3, 0xb, 0, 0xf, 0xc, 0x12, 0, 0x2, 0x9, 0x7, 0xa, 0xe, 0xd, 0x3, 0xb
};
const unsigned char player_1s_turn_text[] = {
	0xa, 0x6, 0x1, 0x11, 0x3, 0xb, 0, 0x15, 0x13, 0xc
};
const unsigned char player_2s_turn_text[] = {
	0xa, 0x6, 0x1, 0x11, 0x3, 0xb, 0, 0x16, 0x13, 0xc
};
const unsigned char computers_turn_text[] = {
	0x2, 0x9, 0x7, 0xa, 0xe, 0xd, 0x3, 0xb, 0x13, 0xc
};
const unsigned char player_1_wins_text[] = {
	0xa, 0x6, 0x1, 0x11, 0x3, 0xb, 0, 0x15, 0, 0x10, 0x5, 0x8, 0xc, 0x14
};
const unsigned char player_2_wins_text[] = {
	0xa, 0x6, 0x1, 0x11, 0x3, 0xb, 0, 0x16, 0, 0x10, 0x5, 0x8, 0xc, 0x14
};
const unsigned char computer_wins_text[] = {
	0x2, 0x9, 0x7, 0xa, 0xe, 0xd, 0x3, 0xb, 0, 0x10, 0x5, 0x8, 0xc, 0x14
};

const unsigned char board_positions_x[] = {
	64, 80, 96, 112, 128, 144, 160,
	64, 80, 96, 112, 128, 144, 160,
	64, 80, 96, 112, 128, 144, 160,
	64, 80, 96, 112, 128, 144, 160,
	64, 80, 96, 112, 128, 144, 160,
	64, 80, 96, 112, 128, 144, 160
};
const unsigned char board_positions_y[] = {
	64, 64, 64, 64, 64, 64, 64,
	80, 80, 80, 80, 80, 80, 80,
	96, 96, 96, 96, 96, 96, 96,
	112, 112, 112, 112, 112, 112, 112,
	128, 128, 128, 128, 128, 128, 128,
	144, 144, 144, 144, 144, 144, 144
};
const unsigned char board_index_positions[] = {
	0, 0, 2, 2, 4, 4, 6,
	0, 0, 2, 2, 4, 4, 6,
	14, 14, 16, 16, 18, 18, 20,
	14, 14, 16, 16, 18, 18, 20,
	28, 28, 30, 30, 32, 32, 34,
	28, 28, 30, 30, 32, 32, 34
};

const unsigned char board_index_values[] = {0, 42, 84};
const unsigned char piece_animation_values[] = {48, 32, 16, 0, 16, 32, 48};

const unsigned char row_of_slots_upper[] = {0x17, 0x18, 0x17, 0x18, 0x17, 0x18, 0x17, 0x18, 0x17, 0x18, 0x17, 0x18, 0x17, 0x18};
const unsigned char row_of_slots_lower[] = {0x19, 0x1a, 0x19, 0x1a, 0x19, 0x1a, 0x19, 0x1a, 0x19, 0x1a, 0x19, 0x1a, 0x19, 0x1a};

const unsigned char game_title_1[] = {0x33, 0x37, 0x32,   0x33, 0x37, 0x32,   };
const unsigned char game_title_2[] = {0x34, 0x00, 0x00,   0x34, 0x00, 0x36,   };
const unsigned char game_title_3[] = {0x34, 0x00, 0x00,   0x34, 0x00, 0x36,   };
const unsigned char game_title_4[] = {0x34, 0x00, 0x00,   0x34, 0x00, 0x36,   };
const unsigned char game_title_5[] = {0x31, 0x35, 0x30,   0x31, 0x35, 0x30,   };

unsigned char ROL(unsigned char n, unsigned char e) { // thanks geeksforgeeks
	return (n >> e)|(n << (8 - e));
}

unsigned char find_board_pos(unsigned char board_id) {
	board_id = board_index_values[board_id];
	for (i = 36; i > 0; i -= 7) {
		if (board[piece_pos + board_id] != 0) {
			return 255;
		}
		if (board[piece_pos + i - 1 + board_id] == 0) {
			return piece_pos + i - 1;
		}
	}
}

unsigned char check_for_win(unsigned char board_id, unsigned char piece) {
	board_id = board_index_values[board_id];
	for (j = 0; j < 4; ++j) {
		for (k = 0; k < 6; ++k) {
			win_check_data[0] = board[j + k * 7 + board_id];
			win_check_data[1] = board[j + 1 + k * 7 + board_id];
			win_check_data[2] = board[j + 2 + k * 7 + board_id];
			win_check_data[3] = board[j + 3 + k * 7 + board_id];
			if (win_check_data[0] == piece && win_check_data[1] == piece && win_check_data[2] == piece && win_check_data[3] == piece) {
				win_positions[0] = j + k * 7;
				win_positions[1] = j + 1 + k * 7;
				win_positions[2] = j + 2 + k * 7;
				win_positions[3] = j + 3 + k * 7;
				return 1;
			}
		}
	}
	for (j = 0; j < 7; ++j) {
		for (k = 0; k < 3; ++k) {
			win_check_data[0] = board[j + k * 7 + board_id];
			win_check_data[1] = board[j + (k + 1) * 7 + board_id];
			win_check_data[2] = board[j + (k + 2) * 7 + board_id];
			win_check_data[3] = board[j + (k + 3) * 7 + board_id];
			if (win_check_data[0] == piece && win_check_data[1] == piece && win_check_data[2] == piece && win_check_data[3] == piece) {
				win_positions[0] = j + k * 7;
				win_positions[1] = j + (k + 1) * 7;
				win_positions[2] = j + (k + 2) * 7;
				win_positions[3] = j + (k + 3) * 7;
				return 1;
			}
		}
	}
	for (j = 0; j < 4; ++j) {
		for (k = 3; k < 6; ++k) {
			win_check_data[0] = board[j + k * 7 + board_id];
			win_check_data[1] = board[j + 1 + (k - 1) * 7 + board_id];
			win_check_data[2] = board[j + 2 + (k - 2) * 7 + board_id];
			win_check_data[3] = board[j + 3 + (k - 3) * 7 + board_id];
			if (win_check_data[0] == piece && win_check_data[1] == piece && win_check_data[2] == piece && win_check_data[3] == piece) {
				win_positions[0] = j + k * 7;
				win_positions[1] = j + 1 + (k - 1) * 7;
				win_positions[2] = j + 2 + (k - 2) * 7;
				win_positions[3] = j + 3 + (k - 3) * 7;
				return 1;
			}
		}
	}
	for (j = 0; j < 4; ++j) {
		for (k = 0; k < 3; ++k) {
			win_check_data[0] = board[j + k * 7 + board_id];
			win_check_data[1] = board[j + 1 + (k + 1) * 7 + board_id];
			win_check_data[2] = board[j + 2 + (k + 2) * 7 + board_id];
			win_check_data[3] = board[j + 3 + (k + 3) * 7 + board_id];
			if (win_check_data[0] == piece && win_check_data[1] == piece && win_check_data[2] == piece && win_check_data[3] == piece) {
				win_positions[0] = j + k * 7;
				win_positions[1] = j + 1 + (k + 1) * 7;
				win_positions[2] = j + 2 + (k + 2) * 7;
				win_positions[3] = j + 3 + (k + 3) * 7;
				return 1;
			}
		}
	}
	return 0;
}

void animate_piece_falling(void) {
	for (i = board_positions_y[piece_pos] - 16; i < board_positions_y[j]; ++i) {
		ppu_wait_nmi();
		oam_clear();
		// Layer 1
		oam_spr(64 + (piece_pos * 16), 63, 0x24, 0b00000011);
		oam_spr(72 + (piece_pos * 16), 63, 0x24, 0b01000011);
		oam_spr(64 + (piece_pos * 16), 71, 0x24, 0b10000011);
		oam_spr(72 + (piece_pos * 16), 71, 0x24, 0b11000011);
		// Layer 2
		oam_spr(64 + (piece_pos * 16), 79, 0x24, 0b00000011);
		oam_spr(72 + (piece_pos * 16), 79, 0x24, 0b01000011);
		oam_spr(64 + (piece_pos * 16), 87, 0x24, 0b10000011);
		oam_spr(72 + (piece_pos * 16), 87, 0x24, 0b11000011);
		// Layer 3
		oam_spr(64 + (piece_pos * 16), 95, 0x24, 0b00000011);
		oam_spr(72 + (piece_pos * 16), 95, 0x24, 0b01000011);
		oam_spr(64 + (piece_pos * 16), 103, 0x24, 0b10000011);
		oam_spr(72 + (piece_pos * 16), 103, 0x24, 0b11000011);
		// Layer 4
		oam_spr(64 + (piece_pos * 16), 111, 0x24, 0b00000011);
		oam_spr(72 + (piece_pos * 16), 111, 0x24, 0b01000011);
		oam_spr(64 + (piece_pos * 16), 119, 0x24, 0b10000011);
		oam_spr(72 + (piece_pos * 16), 119, 0x24, 0b11000011);
		// Layer 5
		oam_spr(64 + (piece_pos * 16), 127, 0x24, 0b00000011);
		oam_spr(72 + (piece_pos * 16), 127, 0x24, 0b01000011);
		oam_spr(64 + (piece_pos * 16), 135, 0x24, 0b10000011);
		oam_spr(72 + (piece_pos * 16), 135, 0x24, 0b11000011);
		// Layer 6
		oam_spr(64 + (piece_pos * 16), 143, 0x24, 0b00000011);
		oam_spr(72 + (piece_pos * 16), 143, 0x24, 0b01000011);
		oam_spr(64 + (piece_pos * 16), 151, 0x24, 0b10000011);
		oam_spr(72 + (piece_pos * 16), 151, 0x24, 0b11000011);
		// Falling piece
		oam_spr(64 + (piece_pos * 16), i, 0x21, 0b00000000 + turn);
		oam_spr(72 + (piece_pos * 16), i, 0x22, 0b01000000 + turn);
		oam_spr(64 + (piece_pos * 16), i + 8, 0x22, 0b10000000 + turn);
		oam_spr(72 + (piece_pos * 16), i + 8, 0x22, 0b11000000 + turn);
		if (i & 0b10) {
			++i;
		}
	}
}

void draw_board_tile(unsigned char pos) {
	i = board_index_positions[pos];
	l = 0;
	l += board[i];
	l = ROL(l, 2);
	l += board[i + 1];
	l = ROL(l, 2);
	l += board[i + 7];
	l = ROL(l, 2);
	l += board[i + 8];
	l = ROL(l, 2);
	one_vram_buffer(l, get_at_addr(0, board_positions_x[i], board_positions_y[i]));
}

void wait_for_input(void) {
	j = 10;
	k = 0;
	l = 4;
	m = 0;
	while (1) {
		ppu_wait_nmi();
		oam_clear();

		pad_poll(0);
		pad_1 = get_pad_new(0);

		for (i = 0; i < 4; ++i) {
			oam_spr(board_positions_x[win_positions[i]], board_positions_y[win_positions[i]] - 1, 0x23 + l, 0b00000010);
			oam_spr(board_positions_x[win_positions[i]] + 8, board_positions_y[win_positions[i]] - 1, 0x23 + l, 0b01000010);
			oam_spr(board_positions_x[win_positions[i]], board_positions_y[win_positions[i]] + 7, 0x23 + l, 0b10000010);
			oam_spr(board_positions_x[win_positions[i]] + 8, board_positions_y[win_positions[i]] + 7, 0x23 + l, 0b11000010);
		}

		++k;
		if (k == 6) {
			if (m) {
				--l;
				if (!l) { --m; }
			} else {
				++l;
				if (l == 5) { ++m; }
			}
			k = 0;
		}

		if (pad_1) {
			reset_game();
		}
	}
}

unsigned char get_computer_move(void) {
	memfill(computer_move_scores, 0, 7);
	for (test_computer_move = 0; test_computer_move < 7; ++test_computer_move) {
		memcpy(board_computer_move_check, board, 42);
		piece_pos = test_computer_move;
		i = find_board_pos(1);
		if (i != 255) {
			board_computer_move_check[i] = 2;
			if (check_for_win(1, 2)) {
				return test_computer_move;
			}
		} else {
			continue;
		}
		for (test_player_move = 0; test_player_move < 7; ++test_player_move) {
			memcpy(board_player_move_check, board_computer_move_check, 42);
			piece_pos = test_player_move;
			i = find_board_pos(2);
			if (i != 255) {
				board_player_move_check[i] = 1;
				if (check_for_win(2, 1)) {
					computer_move_scores[test_computer_move] = 1;
					break;
				} else {
					computer_move_scores[test_computer_move] = 2;
				}
			}
		}
	}
	j = computer_move_scores[0];
	for (i = 1; i < 7; ++i) {
		if (j != computer_move_scores[i]) {
			break;
		}
	} 
	if (i == 7) {
		while (1) {
			piece_pos = rand8() & 0b111;
			if (piece_pos == 7) {
				--piece_pos;
			}
			if (find_board_pos(0) != 255) {
				j = piece_pos;
				break;
			}
		}
	} else {
		for (i = 0; i < 7; ++i) {
			if (computer_move_scores[j] < computer_move_scores[i]) {
				j = i;
			}
		}
	}
	return j;
}

void game(void) {
	ppu_off();

	pal_bg(bg_pal_game);
	pal_spr(spr_pal_game);

	vram_adr(NTADR_A(0, 0));
	for (a = 0; a < 960; ++a) {
		vram_put(0x0);
	}

	for (i = 8; i < 20; i += 2) {
		vram_adr(NTADR_A(8, i));
		vram_write(row_of_slots_upper, 14);
		vram_adr(NTADR_A(8, i + 1));
		vram_write(row_of_slots_lower, 14);
	}

	vram_adr(NTADR_A(13, 22));
	vram_put(0xd);
	vram_put(0xe);
	vram_put(0xb);
	vram_put(0x8);

	piece_pos = 3;
	ppu_on_all();
	set_vram_buffer();

	while (1) {
		ppu_wait_nmi();
		oam_clear();

		set_scroll_x(0);
		set_scroll_y(0);

		pad_poll(0);
		pad_1 = get_pad_new(0);
		pad_poll(1);
		pad_2 = get_pad_new(1);

		if (piece_pos_offset_minus) {
			--piece_pos_offset_minus;
		}
		if (piece_pos_offset_plus) {
			--piece_pos_offset_plus;
		}

		if (!piece_pos_offset_minus && !piece_pos_offset_plus && computer_move_found) {
			j = find_board_pos(0);
			animate_piece_falling();
			board[j] = 2;
			draw_board_tile(j);
			if (check_for_win(0, 2)) {
				multi_vram_buffer_horz(computer_wins_text, 14, NTADR_A(8, 4));
				wait_for_input();
			}
			--turn;
			piece_pos = 3;
			--computer_move_found;
		}

		if (!turn) {
			if (pad_1 & PAD_LEFT && piece_pos) {
				--piece_pos;
				piece_pos_offset_plus += 16;
			}
			if (pad_1 & PAD_RIGHT && piece_pos < 6) {
				++piece_pos;
				piece_pos_offset_minus += 16;
			}
			if (pad_1 & PAD_DOWN && !piece_pos_offset_minus && !piece_pos_offset_plus) {
				j = find_board_pos(0);
				if (j != 255) {
					animate_piece_falling();
					board[j] = 1;
					draw_board_tile(j);
					if (check_for_win(0, 1)) {
						multi_vram_buffer_horz(player_1_wins_text, 14, NTADR_A(8, 4));
						wait_for_input();
					}
					++turn;
					piece_pos = 3;
				}
			}
		} else {
			if (gamemode) {
				if (!computer_move_found) {
					piece_pos = get_computer_move();
					if (piece_pos > 3) {
						piece_pos_offset_minus = piece_animation_values[piece_pos];
					} else {
						piece_pos_offset_plus = piece_animation_values[piece_pos];
					}
					++computer_move_found;
				}
			} else {
				if (pad_2 & PAD_LEFT && piece_pos) {
					--piece_pos;
					piece_pos_offset_plus += 16;
				}
				if (pad_2 & PAD_RIGHT && piece_pos < 6) {
					++piece_pos;
					piece_pos_offset_minus += 16;
				}
				if (pad_2 & PAD_DOWN && !piece_pos_offset_minus && !piece_pos_offset_plus) {
					j = find_board_pos(0);
					if (j != 255) {
						animate_piece_falling();
						board[j] = 2;
						draw_board_tile(j);
						if (check_for_win(0, 2)) {
							multi_vram_buffer_horz(player_2_wins_text, 14, NTADR_A(8, 4));
							wait_for_input();
						}
						--turn;
						piece_pos = 3;
					}
				}
			}
		}

		if (turn) {
			if (gamemode) {
				multi_vram_buffer_horz(computers_turn_text, 10, NTADR_A(10, 21));
			} else {
				multi_vram_buffer_horz(player_2s_turn_text, 10, NTADR_A(10, 21));
			}
		} else {
			multi_vram_buffer_horz(player_1s_turn_text, 10, NTADR_A(10, 21));
		}

		oam_spr(board_positions_x[piece_pos] + piece_pos_offset_plus - piece_pos_offset_minus, 44, 0x21, 0b00000000 + turn);
		oam_spr(board_positions_x[piece_pos] + 8 + piece_pos_offset_plus - piece_pos_offset_minus, 44, 0x22, 0b01000000 + turn);
		oam_spr(board_positions_x[piece_pos] + piece_pos_offset_plus - piece_pos_offset_minus, 52, 0x22, 0b10000000 + turn);
		oam_spr(board_positions_x[piece_pos] + 8 + piece_pos_offset_plus - piece_pos_offset_minus, 52, 0x22, 0b11000000 + turn);
	}
}

void main(void) {
	ppu_off();

	pal_bg(title_pal);
	pal_spr(title_pal);

	vram_adr(NTADR_A(7, 18));
	vram_write(player_vs_player_text, 17);
	vram_adr(NTADR_A(6, 20));
	vram_write(player_vs_computer_text, 19);

	selected_option = 143;
	ppu_on_all();

	while (1) {
		ppu_wait_nmi();
		oam_clear();

		oam_spr(32, selected_option, 0x20, 0b00000001);
		oam_spr(207, selected_option, 0x20, 0b01000001);

		set_scroll_x(0);
		set_scroll_y(0);

		pad_poll(0);
		pad_1 = get_pad_new(0);

		if (pad_1 & PAD_SELECT) {
			if (selected_option == 143) {
				selected_option = 159;
			} else {
				selected_option = 143;
			}
		}

		if (pad_1 & PAD_START) {
			seed_rng();
			if (selected_option == 159) {
				++gamemode;
			}
			game();
		}
	}
}