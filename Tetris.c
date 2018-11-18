#include <stdio.h>
#include <locale.h>
#include <ncursesw/curses.h>
#include <stdlib.h>


void interface();
void singlemode();
void vsmode();
void bye();
void a();
void b();
void c();
int mode = 0;

int main(){

	setlocale(LC_CTYPE, "ko_KR.utf-8");

	interface();
	return 0;
}

void interface()
{
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
void singlemode()
{
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
void vsmode()
{
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

void bye()
{
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

void a()
{
	
	clear();
	mvprintw(30,60," 안녕하세요 오세민입니다.");
	//mode=1;
}

void b()
{
	
	clear();
	mvprintw(30,60," 안녕하세요 황보승우입니다.");
//	mode=1;
}

void c()
{
	
	endwin();
//	mode=1;
}
