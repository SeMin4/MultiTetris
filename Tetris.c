#include <stdio.h>
#include <locale.h>
#include <ncursesw/curses.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <time.h>
#include<string.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/time.h>
#include<sys/select.h>
#include <pthread.h>

#define LEFTEDGE 20
#define RIGHTEDGE 18
#define ROW 20
#define SPACE 32
#define WHITESPACE 30
#define TAB 9
#define q 113
#define Q 81
#define BUF_SIZE 1024


void* server_read(void*);
void interface();
void singlemode(int,int);
void vsmode(int,int);
void keyinformation();
void help(int,int);
void bye(int,int);
void single_play();
void b();
void c();
int kbhit(void);
void current_block_delte();
void draw_Borad(int, int);
void block_extra();
int new_block();
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
void extra_block_print(int y, int x, int type_extra);
void extra_block_delete(int y, int x);
void score_print();
void ghost_block();
void competitor();


int mode = 0;
int keep_block_type = -1, score = 0;
int type[3];
int block_type, keep_count= 0;
int block_xpos, block_ypos, block_rotate, crush_flag = 0, before_inactive_check = 0; //ingame blcok x_pos, y_pos, block_rorate, crush_flag
int new_block_flag = 1;
int Other_Board[20][12];
int Other_Real_Board[20][12];
int Board[20][12] = {
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,1},
	{1,1,1,1,1,1,1,1,1,1,1,1}
};
char server_address[] = "155.230.88.6";
int portnum = 9999;
int sock;
char message[BUF_SIZE];
char messageout[BUF_SIZE];
int str_len,fd_max,fd_num;
struct sockaddr_in serv_adr;
fd_set reads,cpy_reads;
struct timeval timecheck;
pthread_t thread;



int Real_game_Board[20][12];
int Shadow_game_Board[20][12];
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
			{0,2,2,0,0},
			{0,2,0,0,0},
			{0,2,0,0,0},
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
			{0,0,0,2,0},
			{0,0,0,2,0},
			{0,0,2,2,0},
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
			{0,2,0,0,0},
			{0,2,0,0,0},
			{0,2,2,0,0},
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
			{0,0,2,2,0},
			{0,0,0,2,0},
			{0,0,0,2,0},
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
			{0,2,0,0,0},
			{0,2,2,0,0},
			{0,2,0,0,0},
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
			{0,0,0,2,0},
			{0,0,2,2,0},
			{0,0,0,2,0},
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
			{0,0,0,0,0},
			{0,2,2,0,0},
			{0,0,2,2,0},
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
			{0,2,0,0,0},
			{0,2,2,0,0},
			{0,0,2,0,0},
			{0,0,0,0,0}
		},
		{
			{0,0,0,0,0},
			{0,0,0,0,0},
			{0,0,2,2,0},
			{0,2,2,0,0},
			{0,0,0,0,0}
		},
		{
			{0,0,0,0,0},
			{0,2,0,0,0},
			{0,2,2,0,0},
			{0,0,2,0,0},
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

int title[10][60]=
	{
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0},
		{0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0},
		{0,0,0,0,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1},
		{0,0,0,0,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1},
		{0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
		{0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1},
		{0,0,0,0,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,1,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{0,0,0,0,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
	};

int main(){

	setlocale(LC_CTYPE, "ko_KR.utf-8");
	srand( (unsigned)time(NULL) );
	stdscr = initscr();

	interface();
	return 0;
}
void interface(){
	

	int ch,color=0;
	int y,x,i,j;	
	int count=1;
	getmaxyx(stdscr,y,x); 
	keypad(stdscr,TRUE);
	noecho();
	curs_set(0);

	if(has_colors() == FALSE)
	{
		endwin();
		printf("Your terminal does not support color\n");
		exit(1);
	}
	clear();
	start_color();

	init_pair(1,COLOR_BLACK,COLOR_BLACK);
	init_pair(2,COLOR_RED,COLOR_RED);
	init_pair(3,COLOR_GREEN,COLOR_GREEN);
	init_pair(4,COLOR_YELLOW,COLOR_YELLOW);
	init_pair(5,COLOR_BLUE,COLOR_BLUE);
	init_pair(6,COLOR_MAGENTA,COLOR_MAGENTA);
	init_pair(7,COLOR_CYAN,COLOR_CYAN);
	init_pair(8,COLOR_WHITE,COLOR_WHITE);
	init_pair(11,COLOR_WHITE,COLOR_BLACK);

	for(i=0; i<10; i++)
	{
		move((y/6)+i,(x/6)+5);
		for(j=0; j<60; j++)
		{
			if(title[i][j]==1)
			{
				switch(color)
				{
					case 0:
						attron(COLOR_PAIR(2));
						break;
					case 1:
						attron(COLOR_PAIR(4));
						break;
					case 2:
						attron(COLOR_PAIR(7));
						break;
					case 3:
						attron(COLOR_PAIR(3));
						break;
					case 4:
						attron(COLOR_PAIR(5));
						break;
					case 5:
						attron(COLOR_PAIR(6));
						break;
				}
			}
			else
				attron(COLOR_PAIR(1));
			
			if(j%10==9)
			{
				color++;
				if(color==6)
					color=0;
			}
			addch(title[i][j]);
			
		}
	}
	
	//refresh();
	
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
			if(count==5 || count==1)
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
			mode=0;
			single_play();
			break;
		case 2:
			mode=1;
			single_play();
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

void single_play(){
	for(int i=0; i<20; i++)
	{
		for(int j=0; j<12; j++)
		{
			if(i==19 || j==0 || j==11)
				Real_game_Board[i][j]=1;
			else
				Real_game_Board[i][j]=0;
		}
	}
	for(int i=0; i<20; i++)
		for(int j=0; j<12; j++)
			Board[i][j]=100;
	score=0;
	keep_block_type=-1;
	new_block_flag=1;
	char tetris[]  = "TETIRS";
	int i, j, ch;
	int y_pos = 0,x_pos=0;
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
	
	
	srand((unsigned)time(NULL));
	for(int i = 0; i<3; i++){
		type[i] = rand()% 16777216;
		type[i] %= 7;
	}
	if(mode == 1){//sever와 connection
		
		sock=socket(PF_INET,SOCK_STREAM,0);
		if(sock==-1){
			mvprintw(60,70,"error_socket\n");
			refresh();
		}
		memset(&serv_adr,0,sizeof(serv_adr));
		serv_adr.sin_family=AF_INET;
		serv_adr.sin_addr.s_addr=inet_addr(server_address);
		serv_adr.sin_port=htons(portnum);
		if(connect(sock, (struct sockaddr*)&serv_adr,sizeof(serv_adr))==-1){
			mvprintw(60,70,"error_connect.\n");
			refresh();
		}
		else{
			mvprintw(60,70,"connected......\n");
			refresh();
		}
		pthread_create(&thread,NULL,server_read,NULL);
	}
	
	block_extra();
	while(1){
		
		if(new_block_flag == 1){
			extra_block_delete(9,WHITESPACE+58);
			extra_block_delete(18,WHITESPACE+58);
			extra_block_delete(13,2);
			extra_block_print(9,WHITESPACE+58,type[1]);
			extra_block_print(18,WHITESPACE+58,type[2]);
			extra_block_print(13,2,keep_block_type);
			score_print();
			block_type = type[0];
			if(new_block() == -1)		
				return;
			if(mode == 1){
				char BUF[10];
				strcpy(messageout,"");
				for(int i = 0; i<20; i++)
				{
					for(int j = 0; j<12;j++)
					{
						sprintf(BUF,"%d",Real_game_Board[i][j]);
						strcat(messageout,BUF);
					}
					if(i !=19)
						strcat(messageout,",");
					else if(i == 19)
						messageout[strlen(messageout)]=0;
				}
				write(sock,messageout,strlen(messageout));
				//여기서 서버로 보내기
			}
			
		}
		if(mode == 1)//sever로 부터 받은 정보로 그림그리기
		{
			FD_ZERO(&reads);
			FD_SET(sock,&reads);
			//FD_SET(0,&reads);
			fd_max=sock;
		}
		
				
		ghost_block();
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
					while(crush_check(block_xpos, block_ypos+1,block_rotate) == true)
					{
						key_down();
					}					
					before_inactive_check = 3;
					break;
				case TAB:
					for(int i=0; i<20; i++)
						for(int j=0; j<12; j++)
							if(Real_game_Board[i][j]==-1)
								Real_game_Board[i][j]=0;

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
				case q:
					quit();
					return;
				case Q:
					quit();
					return;
					
				default:
					break;
			}
			if(crush_check(block_xpos,block_ypos+1, block_rotate)==false&&(before_inactive_check == 3)){//check_inactive
				block_inactive();
				score += 10;
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
				score+=10;
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
void* server_read(void *args){
	int count= 10;
	for(int i = 0; i<20;i++)
	{
		for(int j = 0; j<12; j++)
			Other_Board[i][j] = 100;
	}
	while(1)
	{
		cpy_reads=reads;
		timecheck.tv_sec=5;
		timecheck.tv_usec=5000;
		
		/*if((fd_num=select(fd_max+1,&cpy_reads,0,0,NULL))==-1)
		{	
			printf("select error()");
			break;
		}		

		if(fd_num==0)
		{	
			printf("Time out!!\n");
			continue;
		}
		if(FD_ISSET(sock,&cpy_reads))
		{*/
			str_len=read(sock, message, BUF_SIZE-1);
			message[str_len]=0;
			
			char *ptr = strtok(message, ",");
			int i=0;
			char sss[BUFSIZ];
			while(ptr != NULL)
			{
				//printf("%s\n",ptr);
				strcpy(sss,ptr);
				for(int j=0; j<12; j++)
				{
					Other_Real_Board[i][j]=sss[j]-48;
				}
				ptr=strtok(NULL,",");
				i++;
				
			}
			/*for(int i = 0; i<20; i++)
			{
				for(int j = ; j<20; j++)
				{

				}
			}*/
			//competitor();
		//}//other_print();
	}
}		

void competitor()
{
    char wall[] = "■ ";   
    char blank[] = "  ";
    char fill[] = "□ ";
	int x_pos2=0, y_pos2=2;
    attron(COLOR_PAIR(11));
    mvprintw(0,WHITESPACE+108,"COMPETITOR");
    init_pair(1,COLOR_BLACK,COLOR_BLACK);
    init_pair(2,COLOR_RED,COLOR_RED);
    init_pair(3,COLOR_GREEN,COLOR_GREEN);
    init_pair(4,COLOR_YELLOW,COLOR_YELLOW);
    init_pair(5,COLOR_BLUE,COLOR_BLUE);
    init_pair(6,COLOR_MAGENTA,COLOR_MAGENTA);
    init_pair(7,COLOR_CYAN,COLOR_CYAN);
    init_pair(8,COLOR_WHITE,COLOR_WHITE);
    init_color(77,372,843,372);
    init_pair(11,COLOR_WHITE,COLOR_BLACK);   
   
    for(int i = 0; i<20;i++){
        for(int j = 0; j<12; j++){
            if(Other_Board[i][j]!=Other_Real_Board[i][j])
            {
                move(y_pos2 + i, WHITESPACE+100+x_pos2+(2*j));
                if(Other_Real_Board[i][j] == 0)
                {
                    attron(COLOR_PAIR(1));
                    addstr(blank);
                }
                   
                else if(Other_Real_Board[i][j] == 1)
                {
                    attron(COLOR_PAIR(11));
                    addstr(wall);
                }
                else if(Other_Real_Board[i][j] == 2)
                {
                    switch(block_type)
                    {
                        case 0:
                            attron(COLOR_PAIR(2));
                            break;
                        case 1:
                            attron(COLOR_PAIR(3));
                            break;                           
                        case 2:
                            attron(COLOR_PAIR(4));
                            break;
                        case 3:
                            attron(COLOR_PAIR(5));
                            break;
                        case 4:
                            attron(COLOR_PAIR(6));
                            break;
                        case 5:
                            attron(COLOR_PAIR(7));
                            break;
                        case 6:
                            attron(COLOR_PAIR(8));
                            break;
                    }
                    addstr(fill);
                }
                else if(Other_Real_Board[i][j] == 3)
                {
                    attroff(COLOR_PAIR(8));
                    addstr(fill);
                }
                refresh();
               
            }       
        }
    }
   
    for(int i = 0 ; i<20; i++)
        for(int j = 0; j<12; j++)
            Other_Board[i][j] = Other_Real_Board[i][j];
   
} 

void draw_Borad(int y_pos, int x_pos){
	char wall[] = "■ ";	
	char blank[] = "  ";
	char fill[] = "□ ";
	char shadow[] = "▧ ";

	init_pair(1,COLOR_BLACK,COLOR_BLACK);
	init_pair(2,COLOR_RED,COLOR_RED);
	init_pair(3,COLOR_GREEN,COLOR_GREEN);
	init_pair(4,COLOR_YELLOW,COLOR_YELLOW);
	init_pair(5,COLOR_BLUE,COLOR_BLUE);
	init_pair(6,COLOR_MAGENTA,COLOR_MAGENTA);
	init_pair(7,COLOR_CYAN,COLOR_CYAN);
	init_pair(8,COLOR_WHITE,COLOR_WHITE);
	init_color(77,372,843,372);
	init_pair(11,COLOR_WHITE,COLOR_BLACK);

	
	for(int i = 0; i<40;i++){
		for(int j = 0; j<24; j++){
			if(Board[i/2][j/2]!=Real_game_Board[i/2][j/2])
			{
				move(y_pos + i, WHITESPACE+x_pos+(2*j));
				if(Real_game_Board[i/2][j/2] == 0)
				{
					attron(COLOR_PAIR(1));
					addstr(blank);
				}
					
				else if(Real_game_Board[i/2][j/2] == 1)
				{
					attron(COLOR_PAIR(11));
					addstr(wall);
				}
				else if(Real_game_Board[i/2][j/2] == 2)
				{
					switch(block_type)
					{
						case 0:
							attron(COLOR_PAIR(2));
							break;
						case 1:
							attron(COLOR_PAIR(3));
							break;							
						case 2:
							attron(COLOR_PAIR(4));
							break;
						case 3:
							attron(COLOR_PAIR(5));
							break;
						case 4:
							attron(COLOR_PAIR(6));
							break;
						case 5:
							attron(COLOR_PAIR(7));
							break;
						case 6:
							attron(COLOR_PAIR(8));
							break;
					}
					addstr(fill);
				}
				else if(Real_game_Board[i/2][j/2] == 3)
				{
					attroff(COLOR_PAIR(8));
					addstr(fill);
				}
				else if(Real_game_Board[i/2][j/2] == -1 )
				{
					
					attron(COLOR_PAIR(11));
					addstr(shadow);
				}
				refresh();
			}		
		}
	}
	for(int i = 0 ; i<20; i++)
		for(int j = 0; j<12; j++)
			Board[i][j] = Real_game_Board[i][j]; 
	y_pos = 2;
	x_pos = 0;
	for(int i = 0; i<20; i++)
	{
		
		for(int j = 0; j<12; j++)
		{
			if(Other_Board[i][j]!=Other_Real_Board[i][j])
			{
				move(y_pos + i, WHITESPACE+100+x_pos+(2*j));
				switch(Other_Real_Board[i][j]){
					case 0:
						attron(COLOR_PAIR(1));
						addstr(blank);
						break;
					case 1:
						attron(COLOR_PAIR(11));
						addstr(wall);
						break;
					case 2:
						attron(COLOR_PAIR(7));
						addstr(fill);
						break;
					case 3:
						attron(COLOR_PAIR(11));
						addstr(fill);
						break;
				}
			
			}
		}
	}
			
			
	for(int i = 0 ; i<20; i++)
		for(int j = 0; j<12; j++)
			Other_Board[i][j] = Other_Real_Board[i][j]; 
	
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
	block_xpos = 4;
	block_ypos = 0;	
	block_rotate = 0;
	

	for(int i = 0; i<5; i++){			
		for(int j = 0; j<5; j++){
				if(Block [block_type][block_rotate][i][j] == 2)
				{
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
	return 1;

	//block_extra();
	
}
void key_left(){
	for(int i=0; i<20; i++)
	{
		for(int j=0; j<12; j++)
		{
			if(Real_game_Board[i][j]==-1)
			{
				Real_game_Board[i][j]=0;
			}
		}
	}
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
	for(int i=0; i<20; i++)
	{
		for(int j=0; j<12; j++)
		{
			if(Real_game_Board[i][j]==-1)
			{
				Real_game_Board[i][j]=0;
			}
		}
	}
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
	for(int i=0; i<20; i++)
	{
		for(int j=0; j<12; j++)
		{
			if(Real_game_Board[i][j]==-1)
			{
				Real_game_Board[i][j]=0;
			}
		}
	}
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
	for(int i=0; i<20; i++)
	{
		for(int j=0; j<12; j++)
		{
			if(Real_game_Board[i][j]==-1)
			{
				Real_game_Board[i][j]=0;
			}
		}
	}
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
		for(int j = 0; j<12; j++){
			sum += Real_game_Board[i][j];
		}
		if(sum ==32)
		{
			score += 130;
			for(int k = i; k>0;k--)
			{
				for(int s = 0; s< 12; s++)
				{
					Real_game_Board[k][s] = Real_game_Board[k-1][s];
				}
			}
			i++;
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
	int a=1;
	yesorno1();
	
	while(1)
	{
		
		ch = getch();
		if( ch == 10 )
		{
			if( a==1 )
			{
				
				break;
				
			}
			
		}
		else if( ch == KEY_RIGHT )
		{	
			if(a==1)
			{
				a=2;
				yesorno2();
			}
			else if(a==2)
			{
				a=1;
				yesorno1();
			}
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
	init_pair(10,COLOR_WHITE,COLOR_BLACK);
	attron(COLOR_PAIR(10));
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
	init_pair(10,COLOR_WHITE,COLOR_BLACK);
	attron(COLOR_PAIR(10));
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
	
	mvprintw(8, WHITESPACE+58,"ㅡㅡㅡㅡ N E X T ㅡㅡㅡㅡ");
	mvprintw(9, WHITESPACE+58,"ㅣ                     ㅣ");
	mvprintw(10,WHITESPACE+58,"ㅣ                     ㅣ");	
	mvprintw(11,WHITESPACE+58,"ㅣ                     ㅣ");
	mvprintw(12,WHITESPACE+58,"ㅣ                     ㅣ");
	mvprintw(13,WHITESPACE+58,"ㅣ                     ㅣ");
	mvprintw(14,WHITESPACE+58,"ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ");
	//mvprintw(15,WHITESPACE+40,"■ ■ ■ ■ ■ ■ ■ ■ ■ ■ ");

	mvprintw(17,WHITESPACE+58,"ㅡㅡㅡㅡ N E X T ㅡㅡㅡㅡ");
	mvprintw(18,WHITESPACE+58,"ㅣ                     ㅣ");
	mvprintw(19,WHITESPACE+58,"ㅣ                     ㅣ");	
	mvprintw(20,WHITESPACE+58,"ㅣ                     ㅣ");
	mvprintw(21,WHITESPACE+58,"ㅣ                     ㅣ");
	mvprintw(22,WHITESPACE+58,"ㅣ                     ㅣ");
	mvprintw(23,WHITESPACE+58,"ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ");

	mvprintw(12,2,"ㅡㅡㅡㅡ K E E P ㅡㅡㅡㅡ");
	mvprintw(13,2,"ㅣ                     ㅣ");
	mvprintw(14,2,"ㅣ                     ㅣ");	
	mvprintw(15,2,"ㅣ                     ㅣ");
	mvprintw(16,2,"ㅣ                     ㅣ");
	mvprintw(17,2,"ㅣ                     ㅣ");
	mvprintw(18,2,"ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ");
	refresh();
}

void extra_block_print(int y, int x, int type_extra)
{
	init_pair(1,COLOR_BLACK,COLOR_BLACK);
	init_pair(8,COLOR_WHITE,COLOR_WHITE);
	int i,j;
	char fill[] = "■ ";
	if(type_extra == -1)
		return;
	else{
		for(i=0; i<5; i++)
		{
			for(j=0; j<5;j++)
			{ 
				move(y+i,x+7+(2*j));
				if(Block[type_extra][0][i][j]==2)
				{
					attron(COLOR_PAIR(8));
					addstr(fill);
				}
				//else
				//	attron(COLOR_PAIR(1));
			}
		}
		refresh();
	}
	
}
void extra_block_delete(int y, int x)
{
	init_pair(1,COLOR_BLACK,COLOR_BLACK);

	
	int i,j;
	char blank[] = "  ";
	for(i=0; i<5; i++)
	{
		for(j=0; j<10;j++)
		{ 
			move(y+i,x+2+(2*j));
			attron(COLOR_PAIR(1));
			addstr(blank);			
		}
	}
	refresh();
}
void score_print(){
	init_pair(10,COLOR_WHITE,COLOR_BLACK);
	attroff(COLOR_PAIR(10));
	move(30,WHITESPACE+58);
	printw("                            ");
	move(30,WHITESPACE+58);
	printw("My score : %10d",score);
}

void ghost_block()
{
	int ypos=block_ypos;
	int i,j;
	
	

	while(crush_check(block_xpos,ypos+1,block_rotate) == true)
	{
		for(int i = 0; i< 5; i++)
		{
			for(int j = 0; j< 5; j++)
			{
				if(Block[block_type][block_rotate][i][j] == 2)
				{
					Real_game_Board[block_ypos+i][block_xpos+j] = 0;
				}
			}
		}
		ypos++;
		for(int i = 0; i< 5; i++)
		{
			for(int j = 0; j< 5; j++)
			{
				if(Block[block_type][block_rotate][i][j] == 2)
				{
					Real_game_Board[block_ypos+i][block_xpos+j] = 2;
				}
			}
		}
		
	}

	for(int i = 0; i< 5; i++)
	{
		for(int j = 0; j< 5; j++)
		{
			if(Block[block_type][block_rotate][i][j] == 2)
			{
				if(Real_game_Board[ypos+i][block_xpos+j]==2)
					continue;
				else
					Real_game_Board[ypos+i][block_xpos+j] = -1;
			}
		}
	}
	

	
}

