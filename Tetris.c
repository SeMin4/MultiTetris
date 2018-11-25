#include <stdio.h>
#include <locale.h>
#include <ncursesw/curses.h>
#include <stdlib.h>
#include <termios.h>
#include <time.h>

#define LEFTEDGE 20
#define RIGHTEDGE 18
#define ROW 20


void interface();
void singlemode(y,x);
void vsmode(y,x);
void keyinformation();
void help(y,x);
void bye(y,x);
void a();
void b();
void c();
void yesorno1();
void yesorno2();


int mode = 0;


int Board[20][13] = {
{1,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,1},
{1,0,0,0,0,0,0,0,0,0,0,0,1},
{1,1,1,1,1,1,1,1,1,1,1,1,1},
};
int Block[7][4][5][5] = {
	{
		{
			{0,0,0,0,0},
			{0,0,0,0,0},
			{0,1,0,0,0},
			{0,1,1,1,0},
			{0,0,0,0,0}
		},//"ㄴ블록"
		{
			{0,0,0,0,0},
			{0,0,0,1,0},
			{0,0,0,1,0},
			{0,0,1,1,0},
			{0,0,0,0,0}
		},//"회전"
		{
			{0,0,0,0,0},
			{0,1,1,1,0},
			{0,0,0,1,0},
			{0,0,0,0,0},
			{0,0,0,0,0}
		},//"회전"
		{
			{0,0,0,0,0},
			{0,1,1,0,0},
			{0,1,0,0,0},
			{0,1,0,0,0},
			{0,0,0,0,0}
		}//"회전"
	},//"ㄴ 블록"
	{
		{
			{0,0,0,0,0},
			{0,0,0,0,0},
			{0,0,0,1,0},
			{0,1,1,1,0},
			{0,0,0,0,0}
		},
		{
			{0,0,0,0,0},
			{0,0,1,1,0},
			{0,0,0,1,0},
			{0,0,0,1,0},
			{0,0,0,0,0}
		},
		{
			{0,0,0,0,0},
			{0,1,1,1,0},
			{0,1,0,0,0},
			{0,0,0,0,0},
			{0,0,0,0,0}
		},
		{
			{0,0,0,0,0},
			{0,1,0,0,0},
			{0,1,0,0,0},
			{0,1,1,0,0},
			{0,0,0,0,0}
		}
	},//"j 블록"
	{
		{
			{0,0,0,0,0},
			{0,0,0,0,0},
			{0,0,1,1,0},
			{0,0,1,1,0},
			{0,0,0,0,0}
		},
		{
			{0,0,0,0,0},
			{0,0,0,0,0},
			{0,0,1,1,0},
			{0,0,1,1,0},
			{0,0,0,0,0}
		},
		{
			{0,0,0,0,0},
			{0,0,0,0,0},
			{0,0,1,1,0},
			{0,0,1,1,0},
			{0,0,0,0,0}
		},
		{
			{0,0,0,0,0},
			{0,0,0,0,0},
			{0,0,1,1,0},
			{0,0,1,1,0},
			{0,0,0,0,0}
		}
	},//"ㅁ블록"
	{
		{
			{0,0,0,0,0},
			{0,0,0,0,0},
			{0,0,1,0,0},
			{0,1,1,1,0},
			{0,0,0,0,0}
		},
		{
			{0,0,0,0,0},
			{0,0,0,1,0},
			{0,0,1,1,0},
			{0,0,0,1,0},
			{0,0,0,0,0}
		},
		{
			{0,0,0,0,0},
			{0,1,1,1,0},
			{0,0,1,0,0},
			{0,0,0,0,0},
			{0,0,0,0,0}
		},
		{
			{0,0,0,0,0},
			{0,1,0,0,0},
			{0,1,1,0,0},
			{0,1,0,0,0},
			{0,0,0,0,0}
		}
	},//"t블록"
	{
		{
			{0,0,0,0,0},
			{0,1,1,0,0},
			{0,0,1,1,0},
			{0,0,0,0,0},
			{0,0,0,0,0}
		},
		{
			{0,0,0,0,0},
			{0,0,0,1,0},
			{0,0,1,1,0},
			{0,0,1,0,0},
			{0,0,0,0,0}
		},
		{
			{0,0,0,0,0},
			{0,1,1,0,0},
			{0,0,1,1,0},
			{0,0,0,0,0},
			{0,0,0,0,0}
		},
		{
			{0,0,0,0,0},
			{0,0,0,1,0},
			{0,0,1,1,0},
			{0,0,1,0,0},
			{0,0,0,0,0}
		}
	},//"ㄹ블록"
	{
		{
			{0,0,0,0,0},
			{0,0,1,1,0},
			{0,1,1,0,0},
			{0,0,0,0,0},
			{0,0,0,0,0}
		},
		{
			{0,0,0,0,0},
			{0,0,1,0,0},
			{0,0,1,1,0},
			{0,0,0,1,0},
			{0,0,0,0,0}
		},
		{
			{0,0,0,0,0},
			{0,0,1,1,0},
			{0,1,1,0,0},
			{0,0,0,0,0},
			{0,0,0,0,0}
		},
		{
			{0,0,0,0,0},
			{0,0,1,0,0},
			{0,0,1,1,0},
			{0,0,0,1,0},
			{0,0,0,0,0}
		}
	},//"reverse ㄹ블록"
	{
		{
			{0,0,0,0,0},
			{0,0,0,0,0},
			{1,1,1,1,0},
			{0,0,0,0,0},
			{0,0,0,0,0}
		},
		{
			{0,0,0,0,0},
			{0,0,1,0,0},
			{0,0,1,0,0},
			{0,0,1,0,0},
			{0,0,1,0,0}
		},
		{
			{0,0,0,0,0},
			{0,0,0,0,0},
			{1,1,1,1,0},
			{0,0,0,0,0},
			{0,0,0,0,0}
		},
		{
			{0,0,0,0,0},
			{0,0,1,0,0},
			{0,0,1,0,0},
			{0,0,1,0,0},
			{0,0,1,0,0}
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

void interface()
{
	int ch,count=1;
	int y,x;	
	getmaxyx(stdscr,y,x); 

	clear();
	noecho();
	curs_set(0);
	attron(A_BLINK);
	keypad(stdscr,TRUE);
	

	//clear();
	
	mvprintw(y/6 ,x/6,"@@@@@@@@@@              @@@@@@@@@@              @@@@@@@@@@              @@@@@@@@@@              @@@@@@@@@@              @@@@@@@@@@");
	mvprintw((y/6)+1 ,x/6,"@@@@@@@@@@              @@@@@@@@@@              @@@@@@@@@@              @@@@@@@@@@              @@@@@@@@@@              @@@@@@@@@@");
	mvprintw((y/6)+2 ,x/6,"    @@                  @@                          @@                  @@      @@                  @@                  @@        ");
        mvprintw((y/6)+3 ,x/6,"    @@                  @@                          @@                  @@      @@                  @@                  @@        ");
	mvprintw((y/6)+4 ,x/6,"    @@                  @@@@@@@@@@                  @@                  @@@@@@@@@@                  @@                  @@@@@@@@@@");
	mvprintw((y/6)+5 ,x/6,"    @@                  @@@@@@@@@@                  @@                  @@@@@@@@@@                  @@                  @@@@@@@@@@");
	mvprintw((y/6)+6 ,x/6,"    @@                  @@                          @@                  @@  @@@                     @@                           @");
	mvprintw((y/6)+7,x/6,"    @@                  @@                          @@                  @@   @@@                    @@                           @");
	mvprintw((y/6)+8,x/6,"    @@                  @@@@@@@@@@                  @@                  @@    @@@               @@@@@@@@@@              @@@@@@@@@@");
	mvprintw((y/6)+9,x/6,"    @@                  @@@@@@@@@@                  @@                  @@     @@@              @@@@@@@@@@              @@@@@@@@@@");
	refresh();
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
void singlemode(y,x)
{
	standend();
	mvprintw(y/2,(x/3)+17,"*******************************");
	standout();
	mvprintw((y/2)+1, (x/3)+17,"            싱글 MODE          ");
	standend();
	mvprintw((y/2)+2, (x/3)+17,"            배틀 MODE          ");
	mvprintw((y/2)+3, (x/3)+17,"             키 설명           ");
	mvprintw((y/2)+4, (x/3)+17,"             나가기            ");
	mvprintw((y/2)+5, (x/3)+17,"*******************************");
	refresh();
}
void vsmode(y,x)
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

void help(y,x)
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

void bye(y,x)
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

void a()
{
	
	int ch;
	char wall[] = "@";	char blank[]   = "   ";
	char tetris[]  = "TETIRS";
	char fill[] = "*";
	int i, j;
	int pos = 0;
	int dir = 1;
	
	int type[3];	
		
	clear();
	move(pos,8);
	addstr(tetris);
	pos += dir;
	pos += dir;
	refresh();
	sleep(1);

	

	for(i = 0; i<40;i++){
		for(j = 0; j<26; j++){
			move(pos + i, LEFTEDGE+j);
			if(Board[i/2][j/2] == 0)
				addstr(blank);
			else if(Board[i/2][j/2] == 1){
				//standout();
				addstr(wall);
			}
			refresh();
			//sleep(1);
			
		}
	}
	refresh();
	for( i = 0; i< 5; i++)
	{
		for(j = 0; j< 5; j++)
		{
			if(Block[0][0][i][j]==1)
			{
				mvprintw(pos+i,LEFTEDGE+j+10,"@");
			}
			else
			{
				mvprintw(pos+i,LEFTEDGE+j+10,"  ");
			}			
		}
	}
	
	refresh();
	
	//sleep(200);

	while(1)		// q 누르면 나가기화면 출력 yes or no 
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
		else if( ch == 9 ) // TAB 키
		{
			
		}

		else			
			continue;
		
		break;
	}
	
		
}

void b()
{
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


