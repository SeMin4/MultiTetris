#include <stdio.h>
#include <locale.h>
#include <ncursesw/curses.h>
#include <stdlib.h>
#include <unistd.h>

#define LEFTEDGE 20
#define RIGHTEDGE 18
#define ROW 20
#define SPACE 32

void interface();
void singlemode();
void vsmode();
void bye();
void a();
void b();
void c();
int kbhit(void);
void draw_Borad(int, int);
void new_block();
void key_left();
void key_right();
void key_down();
void key_up();
int crush_check(int, int, int);
void block_inactive();
void delete_block();

int mode = 0;
int block_xpos, block_ypos, block_rotate, crush_flag = 0, before_inactive_check = 0; //ingame blcok x_pos, y_pos, block_rorate, crush_flag
int new_block_flag = 1;
int Board[20][16] = {
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};
int Real_game_Board[20][16];
int Block[7][4][5][5] = {
	{
		{
			{0,0,0,0,0},
			{0,0,0,0,0},
			{0,2,0,0,0},
			{0,2,2,2,0},
			{0,0,0,0,0}
		},//"ㄴ블록"
		{
			{0,0,0,0,0},
			{0,0,0,2,0},
			{0,0,0,2,0},
			{0,0,2,2,0},
			{0,0,0,0,0}
		},//"회전"
		{
			{0,0,0,0,0},
			{0,2,2,2,0},
			{0,0,0,2,0},
			{0,0,0,0,0},
			{0,0,0,0,0}
		},//"회전"
		{
			{0,0,0,0,0},
			{0,2,2,0,0},
			{0,2,0,0,0},
			{0,2,0,0,0},
			{0,0,0,0,0}
		}//"회전"
	},//"ㄴ 블록"
	{
		{
			{0,0,0,0,0},
			{0,0,0,0,0},
			{0,0,0,2,0},
			{0,2,2,2,0},
			{0,0,0,0,0}
		},
		{
			{0,0,0,0,0},
			{0,0,2,2,0},
			{0,0,0,2,0},
			{0,0,0,2,0},
			{0,0,0,0,0}
		},
		{
			{0,0,0,0,0},
			{0,2,2,2,0},
			{0,2,0,0,0},
			{0,0,0,0,0},
			{0,0,0,0,0}
		},
		{
			{0,0,0,0,0},
			{0,2,0,0,0},
			{0,2,0,0,0},
			{0,2,2,0,0},
			{0,0,0,0,0}
		}
	},//"j 블록"
	{
		{
			{0,0,0,0,0},
			{0,0,0,0,0},
			{0,0,2,2,0},
			{0,0,2,2,0},
			{0,0,0,0,0}
		},
		{
			{0,0,0,0,0},
			{0,0,0,0,0},
			{0,0,2,2,0},
			{0,0,2,2,0},
			{0,0,0,0,0}
		},
		{
			{0,0,0,0,0},
			{0,0,0,0,0},
			{0,0,2,2,0},
			{0,0,2,2,0},
			{0,0,0,0,0}
		},
		{
			{0,0,0,0,0},
			{0,0,0,0,0},
			{0,0,2,2,0},
			{0,0,2,2,0},
			{0,0,0,0,0}
		}
	},//"ㅁ블록"
	{
		{
			{0,0,0,0,0},
			{0,0,0,0,0},
			{0,0,2,0,0},
			{0,2,2,2,0},
			{0,0,0,0,0}
		},
		{
			{0,0,0,0,0},
			{0,0,0,2,0},
			{0,0,2,2,0},
			{0,0,0,2,0},
			{0,0,0,0,0}
		},
		{
			{0,0,0,0,0},
			{0,2,2,2,0},
			{0,0,2,0,0},
			{0,0,0,0,0},
			{0,0,0,0,0}
		},
		{
			{0,0,0,0,0},
			{0,2,0,0,0},
			{0,2,2,0,0},
			{0,2,0,0,0},
			{0,0,0,0,0}
		}
	},//"t블록"
	{
		{
			{0,0,0,0,0},
			{0,2,2,0,0},
			{0,0,2,2,0},
			{0,0,0,0,0},
			{0,0,0,0,0}
		},
		{
			{0,0,0,0,0},
			{0,0,0,2,0},
			{0,0,2,2,0},
			{0,0,2,0,0},
			{0,0,0,0,0}
		},
		{
			{0,0,0,0,0},
			{0,2,2,0,0},
			{0,0,2,2,0},
			{0,0,0,0,0},
			{0,0,0,0,0}
		},
		{
			{0,0,0,0,0},
			{0,0,0,2,0},
			{0,0,2,2,0},
			{0,0,2,0,0},
			{0,0,0,0,0}
		}
	},//"ㄹ블록"
	{
		{
			{0,0,0,0,0},
			{0,0,2,2,0},
			{0,2,2,0,0},
			{0,0,0,0,0},
			{0,0,0,0,0}
		},
		{
			{0,0,0,0,0},
			{0,0,2,0,0},
			{0,0,2,2,0},
			{0,0,0,2,0},
			{0,0,0,0,0}
		},
		{
			{0,0,0,0,0},
			{0,0,2,2,0},
			{0,2,2,0,0},
			{0,0,0,0,0},
			{0,0,0,0,0}
		},
		{
			{0,0,0,0,0},
			{0,0,2,0,0},
			{0,0,2,2,0},
			{0,0,0,2,0},
			{0,0,0,0,0}
		}
	},//"reverse ㄹ블록"
	{
		{
			{0,0,0,0,0},
			{0,0,0,0,0},
			{2,2,2,2,0},
			{0,0,0,0,0},
			{0,0,0,0,0}
		},
		{
			{0,0,0,0,0},
			{0,0,2,0,0},
			{0,0,2,0,0},
			{0,0,2,0,0},
			{0,0,2,0,0}
		},
		{
			{0,0,0,0,0},
			{0,0,0,0,0},
			{2,2,2,2,0},
			{0,0,0,0,0},
			{0,0,0,0,0}
		},
		{
			{0,0,0,0,0},
			{0,0,2,0,0},
			{0,0,2,0,0},
			{0,0,2,0,0},
			{0,0,2,0,0}
		}
	}//"ㅡ 블록"
};

int main(){

	setlocale(LC_CTYPE, "ko_KR.utf-8");

	interface();
	return 0;
}
void interface(){
	int ch,count=1;
	
	initscr();
	curs_set(0);
	attron(A_BLINK);
	keypad(stdscr,TRUE);
	

	clear();
	
	mvprintw(3 ,10,"@@@@@@@@@@              @@@@@@@@@@              @@@@@@@@@@              @@@@@@@@@@              @@@@@@@@@@              @@@@@@@@@@");
	mvprintw(4 ,10,"@@@@@@@@@@              @@@@@@@@@@              @@@@@@@@@@              @@@@@@@@@@              @@@@@@@@@@              @@@@@@@@@@");
	mvprintw(5 ,10,"    @@                  @@                          @@                  @@      @@                  @@                  @@        ");
	mvprintw(6 ,10,"    @@                  @@                          @@                  @@      @@                  @@                  @@        ");
	mvprintw(7 ,10,"    @@                  @@@@@@@@@@                  @@                  @@@@@@@@@@                  @@                  @@@@@@@@@@");
	mvprintw(8 ,10,"    @@                  @@@@@@@@@@                  @@                  @@@@@@@@@@                  @@                  @@@@@@@@@@");
	mvprintw(9 ,10,"    @@                  @@                          @@                  @@  @@@                     @@                           @");
	mvprintw(10,10,"    @@                  @@                          @@                  @@   @@@                    @@                           @");
	mvprintw(11,10,"    @@                  @@@@@@@@@@                  @@                  @@    @@@               @@@@@@@@@@              @@@@@@@@@@");
	mvprintw(12,10,"    @@                  @@@@@@@@@@                  @@                  @@     @@@              @@@@@@@@@@              @@@@@@@@@@");
	refresh();
	
	singlemode();
	
	while(1)
	{

		ch = getch();
		if(ch == 10)
		{
			switch(count)
			{
				case 1:
					a();
					mode = 1;
					break;
				case 2:
					b();
					mode = 1;
					break;
				case 3:
					clear();
					refresh();
					endwin();
					exit(0);
					
			}
		}
		else if( ch == KEY_DOWN )
		{
			
			count++;
			if(count==4)
			{
				count=1;
				singlemode();
			}
			else if(count==2)
			{
				
				vsmode();
			}

			else if(count==3)
			{
				
				bye();
			}
				
			
		}
		else
			continue;
		if(mode!=0)
		{

			break;
		}
	}
	mvprintw(33,60,"test");
	refresh();	
	while(1);

	//attroff(A_BLINK);
	//endwin();	

}
void singlemode(){
	standend();
	mvprintw(24, 60,"*******************************");
	standout();
	mvprintw(25, 60,"            싱글 MODE          ");
	standend();
	mvprintw(26, 60,"            배틀 MODE          ");
	mvprintw(27, 60,"              나가기           ");
	mvprintw(28, 60,"*******************************");
	refresh();
}
void vsmode(){
	standend();
	mvprintw(24, 60,"*******************************");
	mvprintw(25, 60,"            싱글 MODE          ");
	standout();
	mvprintw(26, 60,"            배틀 MODE          ");
	standend();	
	mvprintw(27, 60,"              나가기           ");
	mvprintw(28, 60,"*******************************");
	refresh();
}

void bye(){
	standend();
	mvprintw(24, 60,"*******************************");
	mvprintw(25, 60,"            싱글 MODE          ");
	mvprintw(26, 60,"            배틀 MODE          ");
	standout();	
	mvprintw(27, 60,"              나가기           ");
	standend();
	mvprintw(28, 60,"*******************************");
	refresh();
}

void a(){
	char tetris[]  = "TETIRS";
	int i, j, ch;
	int y_pos = 0;
	int x_pos = 0;
	int dir = 1;
	clear();
	cbreak();
	noecho();
	nodelay(stdscr,TRUE);

	scrollok(stdscr, TRUE);

	move(y_pos,x_pos + 8);//move(y, x); 
	addstr(tetris);
	refresh();
	y_pos += 2;
	for(int i = 0; i<20; i++)
		for(int j = 0; j<16; j++)
			Real_game_Board[i][j] = Board[i][j];
	for(int i = 0 ; i< 20; i++)
		for(int j = 0; j<16;j++)
			Board[i][j] = 100;
	//EMPTY BOARD draw
	//draw_Borad(y_pos, x_pos);	
	//refresh();
	/*clear();
	refresh();
	endwin();
	for(int i = 0; i<20; i++){
		for(int j = 0; j<13; j++){
			printf("%d " ,Real_game_Board[i][j]);
		}
		printf("\n");
	}
	break;*/
	while(1){
		if(new_block_flag == 1){
			new_block();
			
		}		
		draw_Borad(y_pos, x_pos);
		if(kbhit()){
			ch = getch();
			switch(ch){//getch() 
				case KEY_DOWN :
					if(crush_check(block_xpos, block_ypos+1,block_rotate) == true){
						before_inactive_check = 0;
						key_down();
					}									
					break;
				case KEY_UP :
					if(crush_check(block_xpos, block_ypos,(block_rotate+1)%4) == true){
						key_up();	
					}				
					break;
				case KEY_RIGHT :
					if(crush_check(block_xpos+1, block_ypos,block_rotate) == true){
						key_right();
					}
					break;
				case KEY_LEFT :
					if(crush_check(block_xpos-1, block_ypos,block_rotate) == true){
						key_left();
					}
					//draw_Borad(y_pos, x_pos);
					break;
				case SPACE :
					while(crush_check(block_xpos, block_ypos+1,block_rotate) == true){
						key_down();
					}
					before_inactive_check = 3;
					break;
				default:
					break;
			}
			if(crush_check(block_xpos,block_ypos+1, block_rotate)==false&&(before_inactive_check == 3)){//check_inactive
			block_inactive();
			delete_block();				
			}
			else if(crush_check(block_xpos,block_ypos+1, block_rotate)==false){
				before_inactive_check++;
			}
		}
		else{
			if(crush_check(block_xpos, block_ypos+1,block_rotate) == true){
						before_inactive_check = 0;
						key_down();
			}
			if(crush_check(block_xpos,block_ypos+1, block_rotate)==false&&(before_inactive_check == 3)){//check_inactive
				block_inactive();
				delete_block();				
			}
			else if(crush_check(block_xpos,block_ypos+1, block_rotate)==false){
				before_inactive_check++;
			}

			usleep(500000);
		}
		
		//refresh(); 
	}
}
void draw_Borad(int y_pos, int x_pos){
	char wall[] = "@";	
	char blank[] = "   ";
	char fill[] = "*";
	for(int i = 0; i<40;i++){
		for(int j = 0; j<32; j++){
			if(Board[i/2][j/2]!=Real_game_Board[i/2][j/2]){
				move(y_pos + i, x_pos+j);
				if(Real_game_Board[i/2][j/2] == 0)
					addstr(blank);
				else if(Real_game_Board[i/2][j/2] == 1){
					addstr(wall);
				}
				else if(Real_game_Board[i/2][j/2] == 2){
					addstr(fill);
				}
				else if(Real_game_Board[i/2][j/2] == 3){
					addstr(fill);
				}
				refresh();
			}		
		}
	}
	for(int i = 0 ; i<20; i++)
		for(int j = 0; j<16; j++)
			Board[i][j] = Real_game_Board[i][j]; 
}
int kbhit(void){
	int ch = getch();
	if(ch != ERR){
		ungetch(ch);
		return 1;
	}else{
		return 0;
	}
}
void new_block(){
	block_xpos = 5;
	block_ypos = 0;	
	block_rotate = 0;
	for(int i = 0; i<5; i++){			
		for(int j = 0; j<5; j++){
				if(Block [0][block_rotate][i][j] == 2)
					Real_game_Board[block_ypos+i][block_xpos+j] = 2;
		}		
	}
	new_block_flag = 0;
	
}
void key_left(){
	for(int i = 0; i< 5; i++){
		for(int j = 0; j< 5; j++){
			if(Block[0][block_rotate][i][j] == 2){
				Real_game_Board[block_ypos+i][block_xpos+j] = 0;
			}
		}
	}
	block_xpos--;
	for(int i = 0; i< 5; i++){
		for(int j = 0; j< 5; j++){
			if(Block[0][block_rotate][i][j] == 2){
				Real_game_Board[block_ypos+i][block_xpos+j] = 2;
			}
		}
	}
}
void key_right(){
	for(int i = 0; i< 5; i++){
		for(int j = 0; j< 5; j++){
			if(Block[0][block_rotate][i][j] == 2){
				Real_game_Board[block_ypos+i][block_xpos+j] = 0;
			}
		}
	}
	block_xpos++;
	for(int i = 0; i< 5; i++){
		for(int j = 0; j< 5; j++){
			if(Block[0][block_rotate][i][j] == 2){
				Real_game_Board[block_ypos+i][block_xpos+j] = 2;
			}
		}
	}
}
void key_down(){
	for(int i = 0; i< 5; i++){
		for(int j = 0; j< 5; j++){
			if(Block[0][block_rotate][i][j] == 2){
				Real_game_Board[block_ypos+i][block_xpos+j] = 0;
			}
		}
	}
	block_ypos++;
	for(int i = 0; i< 5; i++){
		for(int j = 0; j< 5; j++){
			if(Block[0][block_rotate][i][j] == 2){
				Real_game_Board[block_ypos+i][block_xpos+j] = 2;
			}
		}
	}
}
void key_up(){
	for(int i = 0; i< 5; i++){
		for(int j = 0; j< 5; j++){
			if(Block[0][block_rotate][i][j] == 2){
				Real_game_Board[block_ypos+i][block_xpos+j] = 0;
			}
		}
	}
	block_rotate++;
	block_rotate %= 4;
	for(int i = 0; i< 5; i++){
		for(int j = 0; j< 5; j++){
			if(Block[0][block_rotate][i][j] == 2){
				Real_game_Board[block_ypos+i][block_xpos+j] = 2;
			}
		}
	}
}
int crush_check(int check_bx, int check_by, int check_rotate){
	for(int i = 0; i <5; i++){
		for(int j = 0; j<5; j++){
			if((Block[0][check_rotate][i][j]==2)&&((Real_game_Board[check_by+i][check_bx+j]==3)||(Real_game_Board[check_by+i][check_bx+j]==1)))
				return false;
		}
	}
	return true;
}
void block_inactive(){
	for(int i = 0; i< 5; i++){
		for(int j = 0; j< 5; j++){
			if(Block[0][block_rotate][i][j] == 2){
				Real_game_Board[block_ypos+i][block_xpos+j] = 3;
			}
		}
	}
	new_block_flag = 1;
}

void delete_block(){
	int sum = 0;
	for(int i = 19; i>=0; i--){
		sum = 0;
		for(int j = 0; j<16; j++){
			sum += Real_game_Board[i][j];
		}
		if(sum == 44)
		{
			for(int k = i; k>0;k--)
			{
				for(int s = 0; s< 16; s++)
				{
					Real_game_Board[k][s] = Real_game_Board[k-1][s];
				}
			}
		}
	}
}


void b(){
	clear();
	mvprintw(30,60," 안녕하세요 황보승우입니다.");
//	mode=1;
}

void c(){
	endwin();
//	mode=1;
}

