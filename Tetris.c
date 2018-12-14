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

//서버와 client 사이 통신
void* server_read(void*);//서버로 부터 데이터 들어오는 부분을 읽는 함수

//화면 첫 ui 구성 함수
void interface();//인터페이스 구현
void singlemode(int,int);
void vsmode(int,int);
void keyinformation();
void help(int,int);
void bye(int,int);
void single_play();

//화면 출력 관련 함수
void draw_Borad(int, int);//게임판 그리기
void block_extra();//옆에 부가적인 부분 생성
void extra_block_print(int y, int x, int type_extra);//옆에 부가적인 부분 프린트
void extra_block_delete(int y, int x);//옆에 부가적인 부분 프린트시 원래 있던걸을 삭제
void yesorno1();//나가시겠습니까 version 1
void yesorno2();//나가시겠습니까 version 2
void score_print();//점수 프린트
void ghost_block();//그림자 생성수

//블록 생성 관련 함수
int new_block();// 새로운 블록 생성
void change(int *a , int *b);//다음 블록 생성시 사용하는 함수

//키 value를 처리하는 함수
int kbhit(void);//키 value가 있는지 없는지 계속하여 체크한뒤에 반환하는 함수
void key_left();//key 왼쪽
void key_right();//key 오른쪽수
void key_down();//key 아래쪽
void key_up();//key 위쪽 (회전)
void quit();//key q or Q 시 반응
void keep_change();//tab 버튼 누를시 keep 해주기
void current_block_delte();//keep 시 현재 블록 삭제성

//게임 내부 설정 관련 함수
int crush_check(int, int, int);//충돌 체크
void block_inactive();//블록 굳히기
void delete_block();//한줄 채울시 블록 삭제

//게임 종료 체크 함수
void gameover();//게임오버 체크수
void game_close();//게임 종료


int mode = 0;
int keep_block_type = -1, score = 0;
int type[3];
int block_type, keep_count= 0;
int block_xpos, block_ypos, block_rotate, crush_flag = 0, before_inactive_check = 0; //ingame blcok x_pos, y_pos, block_rorate, crush_flag
int new_block_flag = 1;
int Other_Board[20][12];
int Other_Real_Board[20][12];
int Board[20][12] = {//첫번째 판 초기화화
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



int Real_game_Board[20][12];//진짜 게임 보드 판 비교하기 위해서 한개 더있음
int Block[7][4][5][5] = {//블록 초기화 4차원 배열을 이용용
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
//타이틀 효과를 위한 배열열
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

	setlocale(LC_CTYPE, "ko_KR.utf-8");//한글 설정
	srand( (unsigned)time(NULL) );//랜덤 함수 설정
	stdscr = initscr();

	interface();//인터페이스 함수 호출출
	return 0;
}
//인터페이스 함수 호출
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
	//색설정
	init_pair(1,COLOR_BLACK,COLOR_BLACK);
	init_pair(2,COLOR_RED,COLOR_RED);
	init_pair(3,COLOR_GREEN,COLOR_GREEN);
	init_pair(4,COLOR_YELLOW,COLOR_YELLOW);
	init_pair(5,COLOR_BLUE,COLOR_BLUE);
	init_pair(6,COLOR_MAGENTA,COLOR_MAGENTA);
	init_pair(7,COLOR_CYAN,COLOR_CYAN);
	init_pair(8,COLOR_WHITE,COLOR_WHITE);
	init_pair(11,COLOR_WHITE,COLOR_BLACK);
	
	//타이틀 색깔 입히기
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
	//타이틀에서 밑에 부분 출력해주기 위해서 쓰는 부분 키를 밑으로 내릴때 마다 다른 함수를 호출 mode로 구분
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
	//count로 구분하여 모드를 구분 모드 함수 호출 멀티도 역시 싱글을 호출 안에서 구분분
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
			//game_close();
			exit(0);
					
	}
	interface();

}
//타이틀 밑에 출력하는 함수 밑에꺼 전부다다
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
//싱글 플레이 함수 호출
void single_play(){
	//판 초기화화
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
	//출력시 구분하기 위해서 board에 쓰레기 vlaue 집어 넣기기
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
	mvprintw(33,WHITESPACE+58,"곧ㄱ=");
	mvprintw(34,WHITESPACE+58,"2015116484황보승우");
	mvprintw(35,WHITESPACE+58,"2015110115오세민");
	mvprintw(36,WHITESPACE+58,"2015113955천지완");
	refresh();
	y_pos += 2;
	
	
	srand((unsigned)time(NULL));
	for(int i = 0; i<3; i++){
		type[i] = rand()% 16777216;
		type[i] %= 7;
	}
	if(mode == 1){//sever와 connection
		
		sock=socket(PF_INET,SOCK_STREAM,0);//server와 통신하기 위하여 socket을 생성
		if(sock==-1){
			mvprintw(60,70,"error_socket\n");
			refresh();
		}
		memset(&serv_adr,0,sizeof(serv_adr));//connection 하기전 초기화
		serv_adr.sin_family=AF_INET;
		serv_adr.sin_addr.s_addr=inet_addr(server_address);
		serv_adr.sin_port=htons(portnum);
		if(connect(sock, (struct sockaddr*)&serv_adr,sizeof(serv_adr))==-1){//connection 하는 함수
			mvprintw(60,70,"error_connect.\n");
			refresh();
		}
		else{
			mvprintw(60,70,"connected......\n");
			refresh();
		}
		pthread_create(&thread,NULL,server_read,NULL);//스레드를 사용하여 서버로 부터 정보를 받아오는 부분
	}
	
	block_extra();//엑스트라 블럭을 생성
	while(1){
		//flag 변수를 이용하여 새로운 블록이 만들어 져야 하는지 항상 체크
		if(new_block_flag == 1){
			extra_block_delete(9,WHITESPACE+58);
			extra_block_delete(18,WHITESPACE+58);
			extra_block_delete(13,2);
			extra_block_print(9,WHITESPACE+58,type[1]);
			extra_block_print(18,WHITESPACE+58,type[2]);
			extra_block_print(13,2,keep_block_type);
			score_print();
			block_type = type[0];
			if(new_block() == -1)//새로운 블록 생성에 들어왔지만 new block을 만들지 못할 경우 game over 처리		
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
		
				
		ghost_block();//그림자 출력
		draw_Borad(y_pos, x_pos);//메인으로 게임 안에 판을 출력 시키는 부분
		//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
		//kbhit 함수에서 키입력을 구분하여 키입력이 있는 경우 안에 함수를 호출		
		if(kbhit()){
			ch = getch();
			switch(ch){//getch() 모든 경우 키를 체크해주고 그 키에 맡는 함수를 호출 항상 키를 받았을때 충돌이 있는지 없는지 검사
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
					before_inactive_check = 3;//굳히기 변수로써 스페이스를 누를 경우 바로 블럭 굳히기
					break;
				case TAB://탭을 누른 경우 keep 하기
					//그림자 삭제
					for(int i=0; i<20; i++)
						for(int j=0; j<12; j++)
							if(Real_game_Board[i][j]==-1)
								Real_game_Board[i][j]=0;
					//킵에 들어가 있는 블록을 생성
					if(keep_block_type == -1){
						current_block_delte();
						keep_change();
						keep_count = 1;		
					}	
					else
					{
						//keep 카운트는 keep을 한 횟수를 체크 2번인 경우에는 킵을 하지 못함
						if(keep_count == 2){

						}
						else if(keep_count == 1){//킵이 가능능
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
			if(crush_check(block_xpos,block_ypos+1, block_rotate)==false&&(before_inactive_check == 3)){//check_inactive 블럭 굳히기 블럭이 굳혀지면 delte block 호출하여 지워져야 하는지도 검사사
				block_inactive();
				score += 10;
				if(keep_count == 2){
					keep_count = 1;
				}
				delete_block();				
			}
			//블록이 아직 굳히면 안되서 3번 카운트 하는 부분
			else if(crush_check(block_xpos,block_ypos+1, block_rotate)==false){
				before_inactive_check++;
			}
		}
		//아무런 키입력이 없는 경우 자동으로 키다운 함수를 호출하여 한칸씩 밑으로 내리기 충돌검사 굳히기 모두 동일 
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
	for(int i = 0; i<20;i++)//다른 상대방의 블록을 처리하기 위해 상대방 블록을 초기화
	{
		for(int j = 0; j<12; j++)
			Other_Board[i][j] = 100;
	}
	while(1)//서버로 부터 thread를 이용하여 무한루프를 통하여 정보를 읽는 함수
	{
		cpy_reads=reads;
		timecheck.tv_sec=5;
		timecheck.tv_usec=5000;
		
	
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
		
	}
}		


void draw_Borad(int y_pos, int x_pos){
	char wall[] = "■ ";	
	char blank[] = "  ";
	char fill[] = "□ ";
	char shadow[] = "▧ ";
	//색깔 초기화
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

	// 리얼게임보드에서 0=공백 1=벽 2=현재 보드에서 움직일수있는 블럭 3=굳힌 블럭 -1=그림자
	for(int i = 0; i<40;i++){
		for(int j = 0; j<24; j++){
			if(Board[i/2][j/2]!=Real_game_Board[i/2][j/2])
			{
				move(y_pos + i, WHITESPACE+x_pos+(2*j));
				if(Real_game_Board[i/2][j/2] == 0) // 공백채우기 ( 검은색 )
				{
					attron(COLOR_PAIR(1));
					addstr(blank);
				}
					
				else if(Real_game_Board[i/2][j/2] == 1) // 벽 ( 흰색 )
				{
					attron(COLOR_PAIR(11));
					addstr(wall);
				}
				else if(Real_game_Board[i/2][j/2] == 2) // 현재 이동할 수 있는 블럭 ( 블럭마다 색 다름 )
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
				else if(Real_game_Board[i/2][j/2] == 3) // 굳힌 블럭 
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
	for(int i = 0 ; i<20; i++) // 게임판 비교하기전에 그 전거 board에 저장
		for(int j = 0; j<12; j++)
			Board[i][j] = Real_game_Board[i][j]; 
	y_pos = 2;
	x_pos = 0;
	for(int i = 0; i<20; i++) // 이거는 멀티모드에서 상대방 판 띄어주는 부분
	{
		
		for(int j = 0; j<12; j++)
		{
			if(Other_Board[i][j]!=Other_Real_Board[i][j])
			{
				move(y_pos + i, WHITESPACE+100+x_pos+(2*j));
				switch(Other_Real_Board[i][j]){
					case 0: // 공백
						attron(COLOR_PAIR(1));
						addstr(blank);
						break;
					case 1: // 벽
						attron(COLOR_PAIR(11));
						addstr(wall);
						break;
					case 2: // 이동가능 블럭
						attron(COLOR_PAIR(7));
						addstr(fill);
						break;
					case 3: // 굳힌거
						attron(COLOR_PAIR(11));
						addstr(fill);
						break;
				}
			
			}
		}
	}
			
		// 상대방 게임판도 전꺼랑 비교하기 위해 이전내용 저장	
	for(int i = 0 ; i<20; i++)
		for(int j = 0; j<12; j++)
			Other_Board[i][j] = Other_Real_Board[i][j]; 
	
}
// 키입력 함수수
int kbhit(void){
	int ch = getch();
	if(ch != ERR){
		ungetch(ch);
		return 1;
	}else{
		return 0;
	}
}
// 현재 게임판에 떠있는 블럭 삭제제
void current_block_delte(){
	for(int i = 0; i< 5; i++){
		for(int j = 0; j< 5; j++){
			if(Block[block_type][block_rotate][i][j] == 2){
				Real_game_Board[block_ypos+i][block_xpos+j] = 0;
			}
		}
	}
}
// 처음 킵했을때 다음블럭들 한칸씩 땡겨오고 새로운 블럭 생성성
void keep_change(){
	change(&keep_block_type,&type[0]);
	type[0] = type[1];
	type[1] = type[2];
	type[2] = rand()%16777216;
	type[2] = type[2]% 7;
	new_block_flag = 1;
}

// 한칸씩 땡겨올때 쓰는 함수수
void change(int *a, int *b){
	int temp;
	temp = *a;
	*a = *b;
	*b = temp;
}

// 새로운 블럭 생성
int new_block(){
	block_xpos = 4;
	block_ypos = 0;	
	block_rotate = 0;
	

	for(int i = 0; i<5; i++){			
		for(int j = 0; j<5; j++){
				if(Block [block_type][block_rotate][i][j] == 2) 
				{
					if(Real_game_Board[block_ypos+i][block_xpos+j] == 3){ // 게임오버 체크
						gameover();
						return -1;
					}
					else{
						Real_game_Board[block_ypos+i][block_xpos+j] = 2; // 새로운 블럭 생성
					}
					
				}
				
					
		}		
	}
	new_block_flag = 0;
	return 1;

	//block_extra();
	
}
// 키 왼쪽쪽
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
// 키오른쪽쪽
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
//키 다운
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
//키 위로 ( 회전 )
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

// 충돌 검사 -> 모든 키입력에 대해 했을때 굳혀진블럭이 겹쳐지면 충돌 
int crush_check(int check_bx, int check_by, int check_rotate){
	for(int i = 0; i <5; i++){
		for(int j = 0; j<5; j++){
			if((Block[block_type][check_rotate][i][j]==2)&&((Real_game_Board[check_by+i][check_bx+j]==3)||(Real_game_Board[check_by+i][check_bx+j]==1)))
				return false;
		}
	}
	return true;
}
// 블럭 굳히기
void block_inactive(){
	for(int i = 0; i< 5; i++){
		for(int j = 0; j< 5; j++){
			if(Block[block_type][block_rotate][i][j] == 2){
				Real_game_Board[block_ypos+i][block_xpos+j] = 3;
			}
		}
	}
	// 굳히고 블럭 땡겨오기, 블럭 flag 1 주기 -> 블럭 생성
	type[0] = type[1];
	type[1] = type[2];
	type[2] = rand()%16777216;
	type[2] = type[2]% 7;
	new_block_flag = 1;
}

// 게임판 한줄 찼을때 한줄 지우기
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

//게임 오버
void gameover(){
	int y, x, ch;
	clear();
	getmaxyx(stdscr,y,x);
	mvprintw(15,15,"GAME OVER!");
	refresh();
	sleep(5);

}

// 키 설명 누르면 출력하는 함수
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

// q 누르면 출력하는 함수
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

// 게임 끝내기
void game_close()
{
	
	endwin();
//	mode=1;
}

// 나가시겠습니까?
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

// 나가시겠습니까? version 2
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

// 옆에 부분 출력
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

// 옆부분 뭐나올지 블럭 출력
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

// 옆부분 블럭 땡겨올때 원래꺼 지우고 새로 넣어야해서 만든 함수
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

// 점수 출력
void score_print(){
	init_pair(10,COLOR_WHITE,COLOR_BLACK);
	attroff(COLOR_PAIR(10));
	move(30,WHITESPACE+58);
	printw("                            ");
	move(30,WHITESPACE+58);
	printw("My score : %10d",score);
}


// 그림자 만드는거
void ghost_block()
{
	int ypos=block_ypos;
	int i,j;
	
	
	// 키다운을 존나해서 리얼게임보드에 3을 만나면 그 위치 기억해서 그 위치에 그림자 만드는거임
	// 하나씩 내리면서 2를 0으로 바꾸고 ypos ++해주고 제일 밑으로 왔을때 그 배열에 -1 넣는다
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

