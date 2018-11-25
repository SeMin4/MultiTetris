#include <stdio.h>
#include <time.h>

void main()
{
	int type[3];
	int ch;
	int keep=0;
	int keepcount = 0;
	int temp;

	srand((unsigned)time(NULL));
	
	for (int i = 0; i < 3; i++)
		type[i] = rand() % 7;

	while (1)
	{
		for (int i = 0; i < 3; i++)
			printf("%d ", type[i]);

		printf("     %d", keep);
		printf("\n");

		ch = getch();
		if (ch == 48)
		{
			if (keepcount == 2)
			{
				type[0] = type[1];
				type[1] = type[2];
				type[2] = rand() % 7;
				keepcount = 1;
			}
			else
			{
				type[0] = type[1];
				type[1] = type[2];
				type[2] = rand() % 7;
			
			}
			
		}
		else if (ch == 9)
		{
			if (keepcount == 0)
			{
				keepcount = 1;
				keep = type[0];
				type[0] = type[1];
				type[1] = type[2];
				type[2] = rand() % 7;
			}
			else if (keepcount == 1)
			{
				keepcount = 2;
				temp = type[0];
				type[0] = keep;
				keep = temp;
			}
			else
				printf("error\n");
			
		}
		else 
			break;

	}

}

