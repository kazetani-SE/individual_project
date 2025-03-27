#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MAP_SIZE 3
#define ROUND_LIMIT 5
#define NUM_OF_BLOCK 9

void map(char a[3][3]);
void setup(char a[3][3], short check[]);
void player(char a[3][3], short check[], short *x1, short *x2);
short ifCorner(char a[3][3],short x1, short y1);
short botThink(char a[3][3],short x1, short y1, short *times);
void bot(char a[3][3], short check[], short x1, short y1);
char win(char a[3][3], int round);

int main(){
	char a[3][3];
	char won;
	short x1, y1;
	short check[9];
	
	setup(a, check);
	
	srand(time(NULL));
	
	for(int turn = 0; turn < 5; turn++){
		map(a);
		
		player(a, check, &x1, &y1);
		won = win(a, turn);
		if(won == 'x'){
			map(a);
			printf("\nPlayer win!\n");
			return 0;
		}
		if(turn == 4){
			break;
		}
		bot(a, check, x1, y1);
		won = win(a, turn);
		if(won == 'o'){
			map(a);
			printf("\nBot win!\n");
			return 0;
		}else if(won == 'd'){
			map(a);
			printf("\nDrawn!\n");
			return 0;
		}
	}
	
	map(a);
	
	printf("\n\n");
	return 0;
}

void setup(char a[3][3], short check[]){
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			a[i][j] = ' ';
		}
	}
	for(int i = 0; i < 9; i++){
		check[i] = 0;
	}
}

void map(char a[3][3]){
	for(int i = 0; i < 3; i++){
		printf("%c | %c | %c", a[i][0], a[i][1], a[i][2]);
		if(i < 2) printf("\n---------\n");
	}
}

void player(char a[3][3], short check[], short *x1, short *y1){
	short choice;
	do{
		printf("\nwhat block do you choose: ");
		scanf("%hd", &choice);
		
		*x1 = (choice - 1) / 3;
    	*y1 = (choice - 1) % 3;
    	
    	if(check[choice-1] != 0 || choice < 1 || choice >9){
    		printf("Your choice breaks the rule, please choose again!");
		}
	}while(check[choice-1] != 0 || choice < 1 || choice >9);

	a[*x1][*y1] = 'x';
	check[choice-1] = 1;
}

short ifCorner(char a[3][3],short x1, short y1){
	short corner, subx, suby, move;
	move = -1;
	corner = x1*3 + y1;
	if(corner == 0 || corner == 2 || corner == 6 || corner == 8){
		for(int i = 0; i < 3; i += 2){
			for(int j = 0; j < 3; j += 2){
				if(corner != (i*3 + j)){
					if(a[i][j] == 'x'){
						subx = abs((x1 - i)/2);
						subx = (subx == 0 ?i : subx);
						suby = abs((y1 - j)/2);
						suby = (suby == 0 ?j : suby);
						if(a[subx][suby] == 'o'){
							continue;
						}
						move = subx*3 + suby;
					}
				}
			}
		}
	}
	printf("\nMove:<%hd>\n", move);
	return move;
}

short botThink(char a[3][3],short x1, short y1, short *times){
	short move;
	
	move = ifCorner(a, x1, y1);
	
	if(move != -1){
		return move;
	}
	short subx, suby, checkx, checky;
	for(int i = -1; i <= 1; i++){
		for(int j = -1; j <= 1; j++){
			subx = x1 + i;
			suby = y1 + j;
			if(subx == x1 && suby == y1){
				continue;
			}
			if(subx < 0 || subx > 2){
				break;
			}
			if(suby < 0 || suby > 2){
				continue;
			}
			if(a[subx][suby] == 'x'){
				checkx = x1 + (x1 - subx);
				checky = y1 + (y1 - suby);
				printf("Check:\n<%hd>\n<%hd>\n", checkx, checky);
				if(checkx < 0 || checkx >2 || checky < 0 || checky > 2){
                    if(*times == 2){
                    	*times = 0;
                        continue;
                    }
                    printf("\nTimes:\n%hd", *times);
                    (*times)++;
					move = botThink(a, subx, suby, times);
					if(move != -1){
						return move;
					}
					break;
				}
				if(a[checkx][checky] == 'o'){
					continue;
				}
				if(move == -1) return checkx*3 + checky;
			}
		}
	}
	if(*times <= 1){
		return move;
	}
	return -1;
}

void bot(char a[3][3], short check[],short x1, short y1){
		short x2, y2, botthink, times;
        times = 0;
		
		botthink = botThink(a, x1, y1, &times);
		printf("Bot think:\n<%hd>\n", botthink);
		do{
			if(botthink > -1){
				x2 = botthink / 3;
				y2 = botthink % 3;
				break;
			}
			
			x2 = rand() % 3;	
			y2 = rand() % 3;	
			
			printf("bot move:<%hd>\n<%hd>\n", x2, y2);
			if(check[4] == 0){
				x2 = 1;
				y2 = 1;
			}
		}while(check[x2*3 + y2] != 0);
		
		a[x2][y2] = 'o';
		check[x2*3 + y2] = 2;	
}

char win(char a[3][3], int round){
	for(int i = 0; i < 3; i++){
		if(a[i][0] == a[i][1] && a[i][0] == a[i][2]){
			return a[i][0];
		}
	}
	for(int j = 0; j < 3; j++){
		if(a[0][j] == a[1][j] && a[0][j] == a[2][j]){
			return a[0][j];
		}
	}
	if(a[0][0] == a[1][1] && a[0][0] == a[2][2]){
		return a[0][0];
	}
	if(a[2][0] == a[1][1] && a[1][1] == a[0][2]){
		return a[1][1];
	}
	if(round != 4){
		return 'c';
	}
	return 'd';
}