#include <core.h>

void setup() {
	// put your setup code here, to run once:
	
	int time;
	int i,step = 0;
	if(argc != 2)
	{
		time = 3000;
	}
	else
	{
		time = atoi(argv[1]);
	}

	for(int i=10;i<14;i++)
	{
		pinMode(i,OUTPUT);
	}
	for(int j=0;j<130;j++)
	{
		for(i=10;i<14;i++)
		{
			digitalWrite(i,1);
			delay(5);
			digitalWrite(i,0);
		}
	}
	delay(time);
	for(int j=0;j<130;j++)
	{
		for(i=13;i>9;i--)
		{
			digitalWrite(i,1);
			delay(5);
			digitalWrite(i,0);
		}
	}
	exit(0);
}
void loop()
{
	return;
}
