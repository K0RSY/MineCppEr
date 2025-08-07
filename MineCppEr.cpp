#include <iostream>
#include <algorithm>
#include <ctime>
#include <chrono>
using namespace std;

char get_inp() {
  	system("stty raw"); 
  	char result = getchar();
	system("stty cooked"); 
 	return result;
}

void print_void() {
	cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
}

void random_mines(int *cells, int height, int width, int mines_count) {
	for (int index {0}; index < mines_count; index++) {
		cells[index] = 1;
	}
	random_shuffle(&cells[0], &cells[width*height]);
}

void print_cells(int *visual_cells, int height, int width, int pointer) {
	cout << "+";
	for (int x {0}; x < width; x++) {
		cout << "---";
	}
	cout << "+\n";
	
	for (int y {0}; y < height; y++) {
		cout << "|";
		for (int x {0}; x < width; x++) {
			if (pointer == width*y+x) {
				cout << "\e[7m ";
			} else {
				cout << " ";
			}
			
			switch (visual_cells[width*y+x]) {
				case 0:
					cout << " ";
            		break;
				case 1:
					cout << "\e[96m1";
            		break;
				case 2:
					cout << "\e[92m2";
            		break;
				case 3:
					cout << "\e[91m3";
            		break;
				case 4:
					cout << "\e[94m4";
            		break;
				case 5:
					cout << "\e[31m5";
            		break;
				case 6:
					cout << "\e[36m6";
            		break;
				case 7:
					cout << "\e[93m7";
            		break;
				case 8:
					cout << "\e[33m8";
            		break;
				case 9:
					cout << "\e[31mP";
            		break;
				case 10:
					cout << "?";
            		break;
				case 11:
					cout << "*";
            		break;
				case 12:
					cout << "#";
            		break;
				case 13:
					cout << "\e[31m*";
            		break;
			}

			if (pointer == width*y+x) {
				cout << "\e[0m\e[7m \e[0m";
			} else {
				cout << "\e[0m ";
			}
		}
		cout << "|\n";
	}
	
	cout << "+";
	for (int x {0}; x < width; x++) {
		cout << "---";
	}
	cout << "+\n";
}

void move_pointer(int height, int width, int &pointer, int move_y, int move_x) {
	pointer += width*move_y+move_x;
	if (pointer < 0) {
		pointer = 0;
	} else if (pointer > width*height-1) {
		pointer = width*height-1;
	}
}

int count_mines(int *cells, int height, int width, int y, int x) {
	int mines = 0;
	
	for (int i {y-1}; i < y+2; i++) {
		for (int j {x-1}; j < x+2; j++) {
			if (i >= 0 && j >= 0 && i <= height-1 && j <= width-1) {
				if (cells[width*i+j]) {
					mines += 1;
				}
			}
		}
	}

	return mines;
}

void switch_to_blank_cell(int *cells, int height, int width, int pointer) {
	int cells_index = rand()%(width*height);
	while (cells[cells_index] == 1) {
		cells_index += 1;
		if (cells_index > width*height) {
			cells_index = 0;
		}
	}
	cells[pointer] = 0;
	cells[cells_index] = 1;
}

int count_blank_cells(int *visual_cells, int height, int width, int y, int x) {
	int blank_cells = 0;
	
	for (int i {y-1}; i < y+2; i++) {
		for (int j {x-1}; j < x+2; j++) {
			if (i >= 0 && j >= 0 && i <= height-1 && j <= width-1) {
				if ((visual_cells[width*i+j] == 9) or (visual_cells[width*i+j] == 0)) {
					blank_cells += 1;
				}
			}
		}
	}

	return blank_cells;
}

int count_flags(int *visual_cells, int height, int width, int y, int x) {
	int flags = 0;
	
	for (int i {y-1}; i < y+2; i++) {
		for (int j {x-1}; j < x+2; j++) {
			if (i >= 0 && j >= 0 && i <= height-1 && j <= width-1) {
				if (visual_cells[width*i+j] == 9) {
					flags += 1;
				}
			}
		}
	}

	return flags;
}

void set_flag(int *visual_cells, int pointer) {
	switch (visual_cells[pointer]) {
		case 0:
			visual_cells[pointer] = 9;
			break;
		case 9:
			visual_cells[pointer] = 10;
			break;
		case 10:
			visual_cells[pointer] = 0;
			break;
	}
}

void open_cell(int *cells, int *visual_cells, int height, int width, int pointer, int &run, int &first_move, bool open_flags) {
	if (first_move) {
		first_move = 0;
		int mines;
		do {
			mines = 0;
			for (int i {pointer/width-1}; i < pointer/width+2; i++) {
				for (int j {pointer%width-1}; j < pointer%width+2; j++) {
					if (i >= 0 && j >= 0 && i <= height-1 && j <= width-1) {
						if (cells[width*i+j]) {
							switch_to_blank_cell(cells, height, width, width*i+j);
							mines += 1;
						}
					}
				}
			}
		} while (mines != 0);
		open_cell(cells, visual_cells, height, width, pointer, run, first_move, false);
	}

	else if (visual_cells[pointer] == 0) {
		if (cells[pointer] == 1) {
			run = 0;
		} else {
			visual_cells[pointer] = count_mines(cells, height, width, pointer/width, pointer%width);
			if (visual_cells[pointer] == 0) {
				visual_cells[pointer] = 12;
				for (int i {pointer/width-1}; i < pointer/width+2; i++) {
					for (int j {pointer%width-1}; j < pointer%width+2; j++) {
						if (i >= 0 && j >= 0 && i <= height-1 && j <= width-1) {
							open_cell(cells, visual_cells, height, width, width*i+j, run, first_move, 0);
						}
					}
				}
			}
		}
	} else if ((visual_cells[pointer] >= 1 and visual_cells[pointer] <= 8) and open_flags) {
		if (count_blank_cells(visual_cells, height, width, pointer/width, pointer%width) == visual_cells[pointer]) {
			for (int i {pointer/width-1}; i < pointer/width+2; i++) {
				for (int j {pointer%width-1}; j < pointer%width+2; j++) {
					if (i >= 0 && j >= 0 && i <= height-1 && j <= width-1) {
						if (visual_cells[width*i+j] == 0) {
							set_flag(visual_cells, width*i+j);
						}
					}
				}
			}
		} else if (count_flags(visual_cells, height, width, pointer/width, pointer%width) == visual_cells[pointer]) {
			for (int i {pointer/width-1}; i < pointer/width+2; i++) {
				for (int j {pointer%width-1}; j < pointer%width+2; j++) {
					if (i >= 0 && j >= 0 && i <= height-1 && j <= width-1) {
						if (visual_cells[width*i+j] == 0) {
							open_cell(cells, visual_cells, height, width, width*i+j, run, first_move, open_flags);
						}
					}
				}
			}
		}
	}
}

void check_inp(int inp, int &run, int *cells, int *visual_cells, int height, int width, int &pointer, int &first_move, int &win) {
	switch (inp) {
		case 'w':
			move_pointer(height, width, pointer, -1, 0);
			break;
		case 'a':
			move_pointer(height, width, pointer, 0, -1);
			break;
		case 's':
			move_pointer(height, width, pointer, 1, 0);
			break;
		case 'd':
			move_pointer(height, width, pointer, 0, 1);
			break;
		case 'q':
			set_flag(visual_cells, pointer);
			break;
		case 'e':
			open_cell(cells, visual_cells, height, width, pointer, run, first_move, true);
			break;
		case 'x':
			win = 2;
			run = 0;
			break;
	}
}

void open_mines(int *cells, int *visual_cells, int height, int width) {
	for (int y {0}; y < height; y++) {
		for (int x {0}; x < width; x++) {
			if (cells[width*y+x]) {
				visual_cells[width*y+x] = 11;
			}
			else if (visual_cells[width*y+x] == 9) {
				visual_cells[width*y+x] = 13;
			}
		}
	}
	print_cells(visual_cells, height, width, -1);
}

void check_win(int *cells, int *visual_cells, int height, int width, int mines_count, int &win, int &run) {
	int cells_count = 0;
	for (int y {0}; y < height; y++) {
		for (int x {0}; x < width; x++) {
			if (cells[width*y+x] == 0 and ((visual_cells[width*y+x] > 0 and visual_cells[width*y+x] < 9) or visual_cells[width*y+x] == 12)) {
				cells_count += 1;
			}
		}
	}
	if (cells_count == height*width-mines_count) {
		run = 0;
		win = 1;
	}
}

int main() {
	srand(chrono::system_clock::to_time_t(chrono::system_clock::now()));

	print_void();
	
	int height; 
	int width;
	int mines_count;
	
	cout << "Введите размеры поля (ширина высота): ";
	cin >> width >> height;
	print_void();
	
	int cells[height][width] {};
	int visual_cells[height][width] {};
	int pointer = 0;

	do {
		cout << "Введите количество мин: ";
		cin >> mines_count;
		print_void();
	} while (mines_count >= height*width-9);

	random_mines(*cells, height, width, mines_count);

	int run = 1;
	char inp;
	int first_move = 1;
	int win = 0;
	
	do {
		print_cells(*visual_cells, height, width, pointer);
		cout << "Нажимайте wasd - чтобы двигать курсор, e - чтобы отркыть клетку, q - чтобы поставить или убрать флажок, x - чтобы выйти: ";
		inp = get_inp();
		check_inp(inp, run, *cells, *visual_cells, height, width, pointer, first_move, win);
		check_win(*cells, *visual_cells, height, width, mines_count, win, run);
		
		print_void();
	} while (run);

	open_mines(*cells, *visual_cells, height, width);
	if (win == 1) {
		cout << "Вы победили!!!!" << endl;
	} else if (win == 2) {
		cout << "Вы вышли. Вот где были мины" << endl;
	} else {
		cout << "Вы проиграли((((" << endl;
	}
	
	return 0;
}
