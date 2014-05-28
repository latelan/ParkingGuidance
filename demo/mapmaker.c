#include <stdio.h>
#define ROW 33 
#define COR 40
#define LENGTH 8

void print_hen_line(int n)
{
	int i = 0;
	while(i<n)
	{
		printf("_");
		i++;
	}
}

void print_shu_line(int n)
{
	int i = 0;
	while(i<n)
	{
		printf("|");
		i++;
	}
}

void print_blackspace(int n)
{
	int i = 0;
	while(i<n)
	{
		printf(" ");
		i++;
	}
}

int main()
{
	int i,j,n;

	printf("\n");
	print_blackspace(13);
	printf("A1");
	print_blackspace(12);
	printf("A2");
	printf("\n");

	for(i=0;i<ROW;i++)
	{

		switch(i)
		{
			case 0: // first line 
			case 6:
			case 14:
			case 22:
			{
				print_blackspace(3);
				print_hen_line(LENGTH);
				print_blackspace(6);
				print_hen_line(LENGTH);
				print_blackspace(6);
				print_hen_line(LENGTH);
				printf("\n");
				break;
			}
			case 1: // second line
			case 7:
			case 9:
			case 15:
			case 17:
			case 23:
			{
				print_blackspace(2);
				n = 0;
				while(n<3)
				{
					print_shu_line(1);
					print_blackspace(LENGTH);
					print_shu_line(1);
					print_blackspace(4);
					n++;
				}
				printf("\n");
				break;

			}
			case 2: // third line
			case 8:
			case 10:
			case 16:
			case 18:
			case 24:
			{
				print_blackspace(2);
				n = 0;
				while(n<3)
				{
					print_shu_line(1);
					print_hen_line(LENGTH);
					print_shu_line(1);
					print_blackspace(4);
					n++;
				}
				printf("\n");
				break;
			}
			//case 3:
			case 5:
			{
				printf("B1");
				print_blackspace(11);
				printf("B2");
				print_blackspace(12);
				printf("B3");
				print_blackspace(12);
				printf("B4");
				print_blackspace(10);
				printf("\n");
				break;
			}
			
			//case 11:
			case 13:
			{
				printf("C1");
				print_blackspace(11);
				printf("C2");
				print_blackspace(12);
				printf("C3");
				print_blackspace(12);
				printf("C4");
				print_blackspace(10);
				printf("\n");
				break;
			}
			
			//case 19:
			case 21:
			{
				printf("D1");
				print_blackspace(11);
				printf("D2");
				print_blackspace(12);
				printf("D3");
				print_blackspace(12);
				printf("D4");
				print_blackspace(10);
				printf("\n");
				break;
			}
			case 4:
			case 12:
			case 20:
			{
				printf("\n");
				break;

			}
			default:break;
		}
	}
	
	printf("\n");
	print_blackspace(13);
	printf("E1");
	print_blackspace(12);
	printf("E2");
	printf("\n");
}
