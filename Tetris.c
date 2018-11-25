#include <stdio.h>
#include <locale.h>
#include <ncursesw/curses.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <time.h>

#define LEFTEDGE 20
#define RIGHTEDGE 18
#define ROW 20
#define SPACE 32
#define WHITESPACE 30

void interface();
void singlemode(int,int);
void vsmode(int,int);
void keyinformation();
void help(int,int);
void bye(int,int);
void a();
void b();
void c();
int kbhit(void);
void current_block_delte();
void draw_Borad(int, int);
<<<<<<< HEAD
int new_block();
=======
void block_extra();
void extra_block_print();
void new_block();
>>>>>>> bb890fe4c5f5175fb5713278be871d7a64492960
void key_left();
void key_right();
void key_down();
void key_up();
int crush_check(int, int, int);
void block_inactive();
void delete_block();
void yesorno1();
void yesorno2();
void quit();
void keep_change();
void change(int *a , int *b);
void gameover();
int mode = 0;
int keep_block_type = -1;
int type[3];
int block_type, keep_count= 0;
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
	srand( (unsigned)time(NULL) );
	stdscr = initscr();

	interface();
	return 0;
}
void interface(){
	int ch,count=1;
	int y,x;	
	getmaxyx(stdscr,y,x); 

	clear();
	noecho();
	curs_set(0);
	attron(A_BLINK);
	keypad(stdscr,TRUE);
	

	//clear();
	
	mvprintw(y/6 ,x/6,    "@@@@@@@@@@              @@@@@@@@@@              @@@@@@@@@@              @@@@@@@@@@              @@@@@@@@@@              @@@@@@@@@@");
	mvprintw((y/6)+1 ,x/6,"@@@@@@@@@@              @@@@@@@@@@              @@@@@@@@@@              @@@@@@@@@@              @@@@@@@@@@              @@@@@@@@@@");
	mvprintw((y/6)+2 ,x/6,"    @@                  @@                          @@                  @@      @@                  @@                  @@        ");
	mvprintw((y/6)+3 ,x/6,"    @@                  @@                          @@                  @@      @@                  @@                  @@        ");
	mvprintw((y/6)+4 ,x/6,"    @@                  @@@@@@@@@@                  @@                  @@@@@@@@@@                  @@                  @@@@@@@@@@");
	mvprintw((y/6)+5 ,x/6,"    @@                  @@@@@@@@@@                  @@                  @@@@@@@@@@                  @@                  @@@@@@@@@@");
	mvprintw((y/6)+6 ,x/6,"    @@                  @@                          @@                  @@  @@@                     @@                           @");
	mvprintw((y/6)+7,x/6, "    @@                  @@                          @@                  @@   @@@                    @@                           @");
	mvprintw((y/6)+8,x/6, "    @@                  @@@@@@@@@@                  @@                  @@    @@@               @@@@@@@@@@              @@@@@@@@@@");
	mvprintw((y/6)+9,x/6, "    @@                  @@@@@@@@@@                  @@                  @@     @@@              @@@@@@@@@@              @@@@@@@@@@");
	refresh();
	singlemode(y,x);
	
	while(1)
	{

		ch = getch();
		if(ch == 10)
		{
			break;
		}
		else if( ch == KEY_DOWN )
		{
			
			count++;
			if(count==5)
			{
				count=1;
				singlemode(y,x);
			}
			else if(count==2)
			{
				
				vsmode(y,x);
			}
			else if(count==3)
			{
				help(y,x);
			}
			else if(count==4)
			{
				
				bye(y,x);
			}
				
			
		}
		else
			continue;
		
	}
	switch(count)
	{
		case 1:
			a();
			break;
		case 2:
			b();
			break;
		case 3:
			keyinformation();
			break;
		case 4:
			clear();
			refresh();
			endwin();
			exit(0);
					
	}
	interface();

}
void singlemode(int y, int x)
{
	standend();
	mvprintw(y/2,(x/3)+17,     "*******************************");
	standout();
	mvprintw((y/2)+1, (x/3)+17,"            싱글 MODE          ");
	standend();
	mvprintw((y/2)+2, (x/3)+17,"            배틀 MODE          ");
	mvprintw((y/2)+3, (x/3)+17,"             키 설명           ");
	mvprintw((y/2)+4, (x/3)+17,"             나가기            ");
	mvprintw((y/2)+5, (x/3)+17,"*******************************");
	refresh();
}
void vsmode(int y, int x)
{
	standend();
	mvprintw(y/2, (x/3)+17,"*******************************");
	mvprintw((y/2)+1, (x/3)+17,"            싱글 MODE          ");
	standout();
	mvprintw((y/2)+2, (x/3)+17,"            배틀 MODE          ");
	standend();	
	mvprintw((y/2)+3, (x/3)+17,"             키 설명           ");
	mvprintw((y/2)+4, (x/3)+17,"             나가기            ");
	mvprintw((y/2)+5, (x/3)+17,"*******************************");
	refresh();
	refresh();
}

void help(int y, int x)
{
	standend();
	mvprintw(y/2, (x/3)+17,"*******************************");
	mvprintw((y/2)+1, (x/3)+17,"            싱글 MODE          ");
	mvprintw((y/2)+2, (x/3)+17,"            배틀 MODE          ");
	standout();	
	mvprintw((y/2)+3, (x/3)+17,"             키 설명           ");
	standend();
	mvprintw((y/2)+4, (x/3)+17,"             나가기            ");
	mvprintw((y/2)+5, (x/3)+170,"*******************************");
	refresh();
}

void bye(int y, int x)
{
	standend();
	mvprintw(y/2, (x/3)+17,"*******************************");
	mvprintw((y/2)+1, (x/3)+17,"            싱글 MODE          ");
	mvprintw((y/2)+2, (x/3)+17,"            배틀 MODE          ");	
	mvprintw((y/2)+3, (x/3)+17,"             키 설명           ");
	standout();
	mvprintw((y/2)+4, (x/3)+17,"             나가기            ");
	standend();
	mvprintw((y/2)+5, (x/3)+17,"*******************************");
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
	srand((unsigned)time(NULL));
	for(int i = 0; i<3; i++){
		type[i] = rand()% 16777216;
		type[i] %= 7;
	}
block_extra();
	while(1){
		if(new_block_flag == 1){
			block_type = type[0];
			if(new_block() == -1)
				return;
		}		
		draw_Borad(y_pos, x_pos);
		//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
		
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
				case 9:
					if(keep_block_type == -1){
						current_block_delte();
						keep_change();
						keep_count = 1;		
					}	
					else
					{
						if(keep_count == 2){

						}
						else if(keep_count == 1){
							current_block_delte();
							change(&keep_block_type,&type[0]);
							new_block_flag = 1;
							keep_count = 2;
						}
					} 
					break;
				case 81:
					quit();
					return;
				case 113:
					quit();
					return;
				default:
					break;
			}
			if(crush_check(block_xpos,block_ypos+1, block_rotate)==false&&(before_inactive_check == 3)){//check_inactive
				block_inactive();
				if(keep_count == 2){
					keep_count = 1;
				}
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
				if(keep_count == 2){
					keep_count = 1;
				}
				delete_block();				
			}
			else if(crush_check(block_xpos,block_ypos+1, block_rotate)==false){
				before_inactive_check++;
			}
			usleep(500000);
		}
		
	}
}
void draw_Borad(int y_pos, int x_pos){
	char wall[] = "@";	
	char blank[] = " ";
	char fill[] = "*";
	for(int i = 0; i<40;i++){
		for(int j = 0; j<32; j++){
			if(Board[i/2][j/2]!=Real_game_Board[i/2][j/2]){
<<<<<<< HEAD
				move(y_pos + i, x_pos+j);
				//if(Real_game_Board[i/2][j/2] == 0)
					//addstr(blank);
				if(Real_game_Board[i/2][j/2] == 1){
=======
				move(y_pos + i, WHITESPACE+x_pos+j);
				if(Real_game_Board[i/2][j/2] == 0)
					addstr(blank);
				else if(Real_game_Board[i/2][j/2] == 1){
>>>>>>> bb890fe4c5f5175fb5713278be871d7a64492960
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
void current_block_delte(){
	for(int i = 0; i< 5; i++){
		for(int j = 0; j< 5; j++){
			if(Block[block_type][block_rotate][i][j] == 2){
				Real_game_Board[block_ypos+i][block_xpos+j] = 0;
			}
		}
	}
}
void keep_change(){
	change(&keep_block_type,&type[0]);
	type[0] = type[1];
	type[1] = type[2];
	type[2] = rand()%16777216;
	type[2] = type[2]% 7;
	new_block_flag = 1;
}
void change(int *a, int *b){
	int temp;
	temp = *a;
	*a = *b;
	*b = temp;
}
int new_block(){
	block_xpos = 5;
	block_ypos = 0;	
	block_rotate = 0;
	for(int i = 0; i<5; i++){			
		for(int j = 0; j<5; j++){
				if(Block [block_type][block_rotate][i][j] == 2){
					if(Real_game_Board[block_ypos+i][block_xpos+j] == 3){
						gameover();
						return -1;
					}
					else{
						Real_game_Board[block_ypos+i][block_xpos+j] = 2;	
					}
						
				}
					
		}		
	}
	new_block_flag = 0;
<<<<<<< HEAD
	return 1;
=======

	block_extra();
>>>>>>> bb890fe4c5f5175fb5713278be871d7a64492960
	
}
void key_left(){
	for(int i = 0; i< 5; i++){
		for(int j = 0; j< 5; j++){
			if(Block[block_type][block_rotate][i][j] == 2){
				Real_game_Board[block_ypos+i][block_xpos+j] = 0;
			}
		}
	}
	block_xpos--;
	for(int i = 0; i< 5; i++){
		for(int j = 0; j< 5; j++){
			if(Block[block_type][block_rotate][i][j] == 2){
				Real_game_Board[block_ypos+i][block_xpos+j] = 2;
			}
		}
	}
}
void key_right(){
	for(int i = 0; i< 5; i++){
		for(int j = 0; j< 5; j++){
			if(Block[block_type][block_rotate][i][j] == 2){
				Real_game_Board[block_ypos+i][block_xpos+j] = 0;
			}
		}
	}
	block_xpos++;
	for(int i = 0; i< 5; i++){
		for(int j = 0; j< 5; j++){
			if(Block[block_type][block_rotate][i][j] == 2){
				Real_game_Board[block_ypos+i][block_xpos+j] = 2;
			}
		}
	}
}
void key_down(){
	for(int i = 0; i< 5; i++){
		for(int j = 0; j< 5; j++){
			if(Block[block_type][block_rotate][i][j] == 2){
				Real_game_Board[block_ypos+i][block_xpos+j] = 0;
			}
		}
	}
	block_ypos++;
	for(int i = 0; i< 5; i++){
		for(int j = 0; j< 5; j++){
			if(Block[block_type][block_rotate][i][j] == 2){
				Real_game_Board[block_ypos+i][block_xpos+j] = 2;
			}
		}
	}
}
void key_up(){
	for(int i = 0; i< 5; i++){
		for(int j = 0; j< 5; j++){
			if(Block[block_type][block_rotate][i][j] == 2){
				Real_game_Board[block_ypos+i][block_xpos+j] = 0;
			}
		}
	}
	block_rotate++;
	block_rotate %= 4;
	for(int i = 0; i< 5; i++){
		for(int j = 0; j< 5; j++){
			if(Block[block_type][block_rotate][i][j] == 2){
				Real_game_Board[block_ypos+i][block_xpos+j] = 2;
			}
		}
	}
}
int crush_check(int check_bx, int check_by, int check_rotate){
	for(int i = 0; i <5; i++){
		for(int j = 0; j<5; j++){
			if((Block[block_type][check_rotate][i][j]==2)&&((Real_game_Board[check_by+i][check_bx+j]==3)||(Real_game_Board[check_by+i][check_bx+j]==1)))
				return false;
		}
	}
	return true;
}
void block_inactive(){
	for(int i = 0; i< 5; i++){
		for(int j = 0; j< 5; j++){
			if(Block[block_type][block_rotate][i][j] == 2){
				Real_game_Board[block_ypos+i][block_xpos+j] = 3;
			}
		}
	}
	type[0] = type[1];
	type[1] = type[2];
	type[2] = rand()%16777216;
	type[2] = type[2]% 7;
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
void gameover(){
	int y, x, ch;
	clear();
	getmaxyx(stdscr,y,x);
	mvprintw(15,15,"GAME OVER!");
	refresh();
	sleep(5);

}

void b(){
	int ch;
	clear();
	mvprintw(30,60," 안녕하세요 황보승우입니다.");
	refresh();
	while(1)
	{
		
		noecho();
		ch = getch();
		if( (ch == 81) || (ch== 113) )
		{
			yesorno1();
			while(1)
			{
				ch = getch();
				if( ch == 10 )
				{
					break;
				}
				else if( ch == KEY_RIGHT )
				{
					yesorno2();
					ch = getch();
					if( ch == KEY_RIGHT)
					{
						yesorno1();
						continue;
					}
					else if( ch == 10 )
						continue;
						
				}
			}
		}
		else
			continue;
		
		break;
	}
//	mode=1;
}
void keyinformation()
{
	int ch;
	clear();
	
	mvprintw(5,65, "******************************키설명****************************");
	mvprintw(12, 84," ↑ : 회전 ");
	mvprintw(16,84," ↓ : 한칸 내리기 ");
	mvprintw(20,84," → :오른쪽으로 한칸 이동 ");
	mvprintw(24,84," ← : 왼쪽으로 한칸 이동 ");
	mvprintw(28,84," Space bar : 한번에 내리기 ");
	mvprintw(32,84," Shift : Keep기능 ");
	mvprintw(36,84," Backspace : 뒤로가기 ");	
	mvprintw(43,65, "**************************************************************");
	refresh();
	while(1)
	{
		
		noecho();
		ch = getch();
		if( (ch == 81) || (ch== 113) )
		{
			yesorno1();
			while(1)
			{
				ch = getch();
				if( ch == 10 )
				{
					break;
				}
				else if( ch == KEY_RIGHT )
				{
					yesorno2();
					ch = getch();
					if( ch == KEY_RIGHT)
					{
						yesorno1();
						continue;
					}
					else if( ch == 10 )
						continue;
						
				}
			}
		}
		else
			continue;
		
		break;
	}	
}
void quit()
{
	int ch;
	yesorno1();
	while(1)
	{
		ch = getch();
		if( ch == 10 )
		{
			break;
		}
		else if( ch == KEY_RIGHT )
		{
			yesorno2();
			ch = getch();
			if( ch == KEY_RIGHT)
			{
				yesorno1();
				continue;
			}
			else if( ch == 10 )
				continue;
				
		}
	}
		
	
}
void c()
{
	
	endwin();
//	mode=1;
}

void yesorno1()
{
	clear();
	mvprintw(21,80,"***************************");
	mvprintw(23,80,"   나 가 시 겠 습 니 까 ?    ");
	standout();
	mvprintw(24,87,"yes");
	standend();
	mvprintw(24,96,"no");
	mvprintw(26,80,"***************************");

}

void yesorno2()
{
	clear();
	mvprintw(21,80,"***************************");
	mvprintw(23,80,"   나 가 시 겠 습 니 까 ?    ");
	mvprintw(24,87,"yes");
	standout();
	mvprintw(24,96,"no");
	standend();
	mvprintw(26,80,"***************************");

}

void block_extra()
{
	mvprintw(8, WHITESPACE+40,"ㅡㅡㅡㅡ N E X T ㅡㅡㅡㅡ");
	mvprintw(9, WHITESPACE+40,"ㅣ                     ㅣ");
	mvprintw(10,WHITESPACE+40,"ㅣ                     ㅣ");	
	mvprintw(11,WHITESPACE+40,"ㅣ                     ㅣ");
	mvprintw(12,WHITESPACE+40,"ㅣ                     ㅣ");
	mvprintw(13,WHITESPACE+40,"ㅣ                     ㅣ");
	mvprintw(14,WHITESPACE+40,"ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ");

	mvprintw(17,WHITESPACE+40,"ㅡㅡㅡㅡ N E X T ㅡㅡㅡㅡ");
	mvprintw(18,WHITESPACE+40,"ㅣ                     ㅣ");
	mvprintw(18,WHITESPACE+40,"ㅣ                     ㅣ");	
	mvprintw(20,WHITESPACE+40,"ㅣ                     ㅣ");
	mvprintw(21,WHITESPACE+40,"ㅣ                     ㅣ");
	mvprintw(22,WHITESPACE+40,"ㅣ                     ㅣ");
	mvprintw(23,WHITESPACE+40,"ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ");

	mvprintw(12,2,"ㅡㅡㅡㅡ K E E P ㅡㅡㅡㅡ");
	mvprintw(13,2,"ㅣ                     ㅣ");
	mvprintw(14,2,"ㅣ                     ㅣ");	
	mvprintw(15,2,"ㅣ                     ㅣ");
	mvprintw(16,2,"ㅣ                     ㅣ");
	mvprintw(17,2,"ㅣ                     ㅣ");
	mvprintw(18,2,"ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ");
	refresh();
}

void extra_block_print()
{
	int i,j;

	for(i=1; i<5; i++)
	{
		for(j=0; j<5;j++)
		{
			if(Block[type[1][0][i][j]==2)
			{
				
			}
		}
	}
}
