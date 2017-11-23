#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>
#include <stdlib.h> 

void number_print(int *value, int new_block, int update, int gameover);

void gotoxy(int x, int y){
	COORD pos = {x, y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void cursor_set(){
  HANDLE hConsole;
  CONSOLE_CURSOR_INFO ConsoleCursor;
  hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  ConsoleCursor.bVisible = 0;
	ConsoleCursor.dwSize = 2;
  SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}

void board_print() { // 판을 프린트하는 함수 
	int cnt = 0;
	
	printf("□□□□□□□□□□□□□□□□□\n"); 
	for (int i = 0; i < 4; i++){
		printf("□      □      □      □      □\n");
		printf("□      □      □      □      □\n");
		printf("□      □      □      □      □\n");
		printf("□□□□□□□□□□□□□□□□□\n");
	}
	printf("\n");
}

int gen(int *value){ // 수가 들어있지 않은 랜덤한 위치에 수를 생성하는 함수 
	int pos, num, nums[2] = {2, 4}, fulled = 1;
	
	for (int i = 0; i < 16; i++)
		if (value[i] == 0){
			fulled = 0;
			break;
		}
	
	while (fulled == 0){
		pos = rand() % 16;
		num = rand() % 2;
		if (value[pos] == 0){
			value[pos] = nums[num];
			return pos;
		}
	}
}

int check_end(int *value){ // 게임이 끝났는지를 체크하는 함수 
	int cnt = 0;             // 1을 반환하면 게임이 끝남 

	for (int i = 0; i < 16; i++){ // 0이 하나라도 있으면 게임 안 끝남 
		if (value[i] == 0){
			return 0;
		}
	}

	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 3; j++) // 가로 바로 옆에 같은 숫자가 있으면 게임 안 끝남 
			if (value[i * 4 + j] == value[i * 4 + j + 1])
				return 0;
		for (int j = 0; j < 3; j++) // 세로 바로 아래에 같은 숫자가 있으면 게임 안 끝남 
			if (value[i + j * 4] == value[i + (j + 1) * 4])
				return 0;
	}

	// 게임이 끝났을 때 
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
	gotoxy(0, 0);
	board_print();
	gotoxy(12, 18);
	printf("Game Over!\n");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	gotoxy(12, 23);
	return 1;
}

void debug(int *value, int new_block){ // 2048을 만드는 동안 디버깅하기 위하여 보드에 들어가는 숫자들만 출력하는 함수
	gotoxy(0, 17);
	for (int i = 0, cnt = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			if (cnt == new_block)
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
			printf("%4d ", value[cnt++]);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		}
		if (cnt % 4 == 0)
			printf("\n");
	}
}

int input_kb(){
	int key;
	key = getch();
	
	if (key == 224){
		key = getch();
		return key;
	}
}

void score_print(int sum){
	static int score = 0;

	if (sum < 0){
		for (int i = 0; i < 5; i++){
			gotoxy(2, 18 + i);
			if (i == 0 || i == 4)
				printf("■□□□□□□□□□□□□□■"); // 30자 
			else if (i == 2){
				printf("□        ");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
				printf("2048  GAME");
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				printf("        □");
			}
			else
				printf("□                          □");
		}
		gotoxy(5, 24);
		printf("Game Start (   Y /   N )"); // 24자 
		gotoxy(5, 25);
		printf("←→: 선택   ENTER: 확인");

		int key, select = 18, out = 0;
		gotoxy(18, 24);
		printf("▷");	
		while (out == 0){
			key = getch();
			if (key == 13){
				if (select == 18){
					gotoxy(5, 24);
					printf("                        ");
					gotoxy(5, 25);
					printf("                        ");
					out = 1;
				}
				if (select == 24){
					gotoxy(0, 26);
					exit(1);
				}
			}
			if (key == 224){
				key = getch();
				
				switch(key){
					case 75:
						select = 18;
						gotoxy(24, 24);
						printf("  ");
						gotoxy(18, 24);
						printf("▷");
						break;
					case 77:
						select = 24;
						gotoxy(18, 24);
						printf("  ");
						gotoxy(24, 24);
						printf("▷");
						break;
					default:
						break;
				}
			}
		}
		return;
	}

	score += sum;
	int score_copy = score, cnt = 0;

	for (int i = 0; i < 5; i++){
		gotoxy(2, 18 + i);

		if (i == 2){
			printf("□    Score : ");

			for (int j = 1; j <= score_copy; cnt++)
				j *= 10;
			if (score_copy == 0)
				cnt = 1;

			if (cnt % 2 == 1){
				for (int j = 0; j < 5 - (cnt / 2 + 1); j++)
					printf(" ");
				printf("%d", score);
				for (int j = 0; j < 5 - (cnt / 2); j++){
					printf(" ");
				}
			} else {
				for (int j = 0; j < 5 - (cnt / 2); j++){
					printf(" ");
				}
				printf("%d", score);
				for (int j = 0; j < 5 - (cnt / 2); j++){
					printf(" ");
				}
			}
			printf("    □\n");
			continue;
		}

		if (i == 0 || i == 4){
			printf("■□□□□□□□□□□□□□■\n");
			continue;
		}

		printf("□                          □\n");
	}
}

int check_move(int *value, int key){
	int cnt = 0;
	switch(key){
		case 72:
			for (int i = 0; i < 4; i++)
				for (int j = 2; j >= 0; j--){
					if (value[i + (j + 1) * 4] == 0)
						continue;
					if (value[i + j * 4] == 0)
						cnt++;
					if (value[i + j * 4] == value[i + (j + 1) * 4])
						cnt++;
			}
			break;
		case 75:
			for (int i = 0; i < 4; i++)
				for (int j = 2; j >= 0; j--){
					if (value[i * 4 + j + 1] == 0)
						continue;
					if (value[i * 4 + j] == 0)
						cnt++;
					if (value[i * 4 + j] == value[i * 4 + j + 1])
						cnt++;
				}
			break;
		case 77:
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 3; j++){
					if (value[i * 4 + j] == 0)
						continue;
					if (value[i * 4 + j + 1] == 0)
						cnt++;
					if (value[i * 4 + j] == value[i * 4 + j + 1])
						cnt++;
				}
			break;
		case 80:
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 3; j++){
					if (value[i + j * 4] == 0)
						continue;
					if (value[i + (j + 1) * 4] == 0)
						cnt++;
					if (value[i + j * 4] == value[i + (j + 1) * 4])
						cnt++;
				}
	}
	return cnt > 0 ? 1 : 0;
}

void action_kb(int *value, int key, int *new_limit){
	if (check_move(value, key) == 0)
		return;

	switch (key){
		case 72: // 72 = ↑ 
			for (int i = 0; i < 4; i++){
				for (int k = 0; k < 3; k++)
					for (int j = 2; j >= 0; j--)
						if (value[i + j * 4] == 0){
							value[i + j * 4] = value[i + (j + 1) * 4];
							value[i + (j + 1) * 4] = 0;
						}
				for (int j = 0; j < 3; j++)
					if (value[i + j * 4] == value[i + (j + 1) * 4]){
						value[i + j * 4] += value[i + (j + 1) * 4];
						score_print(value[i + j * 4]);
						value[i + (j + 1) * 4] = 0;
					}
				for (int k = 0; k < 3; k++)
					for (int j = 2; j >= 0; j--)
						if (value[i + j * 4] == 0){
							value[i + j * 4] = value[i + (j + 1) * 4];
							value[i + (j + 1) * 4] = 0;
						}
			}
			break;
		case 75: // 75 = ← 
			for (int i = 0; i < 4; i++){
				for (int k = 0; k < 3; k++)
					for (int j = 2; j >= 0; j--)
						if (value[i * 4 + j] == 0){
							value[i * 4 + j] = value[i * 4 + j + 1];
							value[i * 4 + j + 1] = 0;
						}
				for (int j = 0; j < 3; j++)
					if (value[i * 4 + j] == value[i * 4 + j + 1]){
						value[i * 4 + j] += value[i * 4 + j + 1];
						score_print(value[i * 4 + j]);
						value[i * 4 + j + 1] = 0;
					}
				for (int k = 0; k < 3; k++)
					for (int j = 2; j >= 0; j--)
						if (value[i * 4 + j] == 0){
							value[i * 4 + j] = value[i * 4 + j + 1];
							value[i * 4 + j + 1] = 0;
						}
			}
			break;
		case 77: // 77 = → 
			for (int i = 0; i < 4; i++){
				for (int k = 0; k < 3; k++)
					for (int j = 0; j < 3; j++)
						if (value[i * 4 + j + 1] == 0){
							value[i * 4 + j + 1] = value[i * 4 + j];
							value[i * 4 + j] = 0;
						}
				for (int j = 2; j >= 0; j--)
					if (value[i * 4 + j] == value[i * 4 + j + 1]){
						value[i * 4 + j + 1] += value[i * 4 + j];
						score_print(value[i * 4 + j + 1]);
						value[i * 4 + j] = 0;
					}
				for (int k = 0; k < 3; k++)
					for (int j = 0; j < 3; j++)
						if (value[i * 4 + j + 1] == 0){
							value[i * 4 + j + 1] = value[i * 4 + j];
							value[i * 4 + j] = 0;
						}
			}
			break;
		case 80: // 80 = ↓ 
			for (int i = 0; i < 4; i++){
				for (int k = 0; k < 3; k++)
					for (int j = 0; j < 3; j++)
						if (value[i + (j + 1) * 4] == 0){
							value[i + (j + 1) * 4] = value[i + j * 4];
							value[i + j * 4] = 0;
						}
				for (int j = 2; j >= 0; j--)
					if (value[i + j * 4] == value[i + (j + 1) * 4]){
						value[i + (j + 1) * 4] += value[i + j * 4];
						score_print(value[i + (j + 1) * 4]);
						value[i + j * 4] = 0;
					}
				for (int k = 0; k < 3; k++)
					for (int j = 0; j < 3; j++)
						if (value[i + (j + 1) * 4] == 0){
							value[i + (j + 1) * 4] = value[i + j * 4];
							value[i + j * 4] = 0;
						}
			}
			break;
		default:
			break;
	}
}

void number_color(int value){
  switch(value){
  	case 0:
    		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
    		break;
    case 2:
    		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
    		break;
    case 4:
    		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 128);
    		break;
    case 8:
    		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 223);
    		break;
    case 16:
    		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 95);
    		break;
    case 32:
    		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 79);
    		break;
    case 64:
    		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 207);
    		break;
    case 128:
    		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 224);
    		break;
    case 256:
    		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 111);
    		break;
    case 512:
    		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 159);
    		break;
    case 1024:
    		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 160);
    		break;
    case 2048:
    		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 176);
  			break;
  	default:
    		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
    		break;
	}
}

void animation(int *value, int new_block, int gameover){
	for (int a = 0; a < 2; a++){
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
		for (int i = 1; i <= 3; i++){
			gotoxy(2 + (new_block % 4) * 8, i + (new_block / 4) * 4);
			if (i == 2)
				printf(" %4d ", value[new_block]);
			else
				printf("      ");
		}
		if (gameover == 0)
			Sleep(125);
		else
			Sleep(50);

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 240);
		if (gameover == 1)
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 207);
		for (int i = 1; i <= 3; i++){
			gotoxy(2 + (new_block % 4) * 8, i + (new_block / 4) * 4);
			if (i == 2)
				printf(" %4d ", value[new_block]);
			else
				printf("      ");
		}
		if (gameover == 0)
			Sleep(125);
		else
			Sleep(50);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0);
	}
	number_print(value, new_block, 1, 0);
}

void number_print(int *value, int new_block, int update, int gameover){
	if (update == 1){
		number_color(value[new_block]);
		for (int i = 1; i <= 3; i++){
			gotoxy(2 + (new_block % 4) * 8, i + (new_block / 4) * 4);
			if (i == 2)
				printf(" %4d ", value[new_block]);
			else
				printf("      ");
		}
		return;
	}

	int cnt = 0;
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			if (cnt == new_block){
				cnt++;
				continue;
			}
			number_color(value[cnt]); // 숫자에 따른 색깔 부여 

			for (int k = 1; k <= 3; k++){ // 3줄에 나눠서 6 * 3의 한 칸을 채움 
				gotoxy(2 + j * 8, k + i * 4);
				if (k == 2){ // 가운뎃 줄엔 숫자를 넣음 
					printf(" %4d ", value[cnt++]);
				} else { // 가운뎃 줄을 제외한 1, 3번째 줄엔 공백 6칸을 넣음 
					printf("      ");
				}
			}
		}
	}
	if (gameover == 0)
		animation(value, new_block, gameover);
	else
		for (int i = 0; i < 16; i++){
			if (value[i] == 0)
				continue;
			animation(value, i, gameover);
		}

	gotoxy(4, 23);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

main() {
	int value[16] = {0}, first = true, new_block, kb, new_limit = 1;
	srand(time(NULL));
	cursor_set();
	board_print();
	score_print(-1);

	while (1){
		if (new_limit == 1)
			new_block = gen(value);

		number_print(value, new_block, 0, 0);
		if (check_end(value) == 1){
			number_print(value, new_block, 0, 1);
			return 0;
		}
		kb = input_kb();
		new_limit = check_move(value, kb);
		action_kb(value, kb, &new_limit);
	}
}
