#include <time.h>
#include <iostream>
#include <windows.h>
#include <cstdio>
#include <conio.h>
#include <time.h> 
#include <fstream>
#include <string>
using namespace std;
int level = 1;// 레벨 4부터는 맵 생성 시간 줄이기
int score = 0;//네모 1개당 점수 10점
int bscore=0; 
int health = 0;//점수랑 동기화할것
int mob = 0;//화면 속 네모 개수
int late = 0;//놓친 사각형 수
int wrong = 0;//틀린 사각형 수
int board[10] = { 0,0,0,0,0,0,0,0,0,0 };
float playtime;
void dos_size() // 창 사이즈
{
	system("mode con: cols=80 lines=30");
}
void delay(clock_t n)
{
	clock_t start = clock();
	while (clock() - start < n);
}
void gotoxy(int x, int y)//좌표이동 y축 커서크기= 2* x축커서크기
{
	COORD pos = { x - 1 , y - 1 };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}
void RemoveCursor(void)
{
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0; // bVisible 멤버 변경
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo); // 변경값 적용
}
int map[4][4] = {      //실사용부분은 왼쪽 아래 3x3 만큼
	{ 0,0,0,0 },
	{ 0,0,0,0 },
	{ 0,0,0,0 },
	{ 0,0,0,0 }
};
void blank(int x,int y)// 좌표단위로 수정할것,네모는 가로축 커서 2개 차지.
{
	gotoxy(x, y);
	cout<<"□□□";
	gotoxy(x, y + 1);
	cout << "□□□";
	gotoxy(x, y + 2);
	cout << "□□□";
	RemoveCursor();
}
void text(int a)
{
	gotoxy(20,5);
	switch(a)
	{
		case 1:
			cout<<"■        ■   ■■■ ■■■                 "; 
				gotoxy(20,6);
			cout<<"■       ■■    ■   ■                      "; 
				gotoxy(20,7);
			cout<<"■      ■  ■   ■   ■■■                  "; 
				gotoxy(20,8);
			cout<<"■     ■■■■  ■   ■                      "; 
				gotoxy(20,9);
			cout<<"■■■■      ■ ■   ■■■                  " ;
			
			break;
		case 2:
			cout<<"    ■■■  ■■   ■■  ■■■              ";
			gotoxy(20,6);
			cout<<"  ■■     ■  ■ ■  ■ ■   ■             ";
			gotoxy(20,7);
			cout<<"  ■  ■■ ■  ■ ■  ■ ■   ■             ";
			gotoxy(20,8);
			cout<<"  ■■  ■ ■  ■ ■  ■ ■   ■              ";
			gotoxy(20,9);
			cout<<"    ■■■  ■■   ■■  ■■■              ";
			break;
		case 3:
			cout<<" ■        ■ ■■■   ■■  ■   ■   ■■";
			gotoxy(20,6);
			cout<<"  ■  ■  ■  ■   ■ ■  ■ ■■ ■  ■";
			gotoxy(20,7);
			cout<<"  ■ ■■ ■  ■■■  ■  ■ ■ ■■  ■  ■■";
			gotoxy(20,8);
			cout<<"   ■■■■   ■  ■  ■  ■ ■ ■■  ■    ■";
			gotoxy(20,9);
			cout<<"    ■  ■    ■   ■  ■■  ■   ■   ■■■"   ;
			
	}
}
void sqare(int x, int y)
{
	gotoxy(x, y);
	cout << "■■■";
	gotoxy(x, y + 1);
	cout << "■■■";
	gotoxy(x, y + 2);
	cout << "■■■";
	
	RemoveCursor();
}
void generatemap()
{
	A:
	srand(GetTickCount());//매번 다른 시드 적용
	int a[4][4] = {      //실사용부분은 왼쪽 아래 3x3 만큼
		{ 0,0,0,0 },
		{ 0,0,0,0 },
		{ 0,0,0,0 },
		{ 0,0,0,0 }
	};
	int i = 0, j = 0;
	i=rand() % 3 + 1;
	j=rand() % 3 + 1;
	a[i][j] = 1;
	if (a[i][j] == map[i][j])
	{
		goto A;
	}
	map[i][j] = a[i][j];
	mob++;
}

int output()
{
	for (int y = 1; y <= 3; y++) 
	{
		for (int x = 1; x <= 3; x++)
		{
			if (map[x][y] == 0)
			{
				blank(6*x-5,3*y-2);
			}
			else
			{
				sqare(6*x-5,3*y-2);
			}
		}
	}
	gotoxy(20, 1);
	cout << "level:" << level;
	gotoxy(20, 2);
	cout <<"score:"<< score;
	gotoxy(20, 3);
	cout << "health:" << health;
	gotoxy(1,13);
	puts("┌──┐┌──┐ ┌──┐┌───┐");
	puts("│Num ││ /  │ │ *  ││ ___  │");
	puts("│Lock││/   │ │    ││      │");
	puts("└──┘└──┘ └──┘└───┘");
	puts(" □□□  □□□   □□□ ┌───┐");
	puts(" □7 □  □8 □   □9 □ │  +   │");
	puts(" □□□  □□□   □□□ │      │");
	puts(" □□□  □□□   □□□ │      │");
	puts(" □4 □  □5 □   □6 □ │      │");
	puts(" □□□  □□□   □□□ └───┘");
    puts(" □□□  □□□   □□□ ┌───┐");
	puts(" □1 □  □2 □   □3 □ │Enter │");
	puts(" □□□  □□□   □□□ │      │");
	puts("┌──────┐ ┌──┐│      │");
	puts("│ 0          │ │ .  ││      │");
	puts("│            │ │Del ││      │");
	puts("└──────┘ └──┘└───┘");
	return(0);
}
void effect(int x,int y)//200ms
{
	score = score + 10;
	bscore = bscore + 10;
	health = health + 10;
	gotoxy(x, y);
	sqare(x, y);
	delay(50);
	gotoxy(x, y);
	cout << "  ■  ";
	gotoxy(x, y + 1);
	cout << "■■■";
	gotoxy(x, y + 2);
	cout << "  ■  ";
	delay(50);
	gotoxy(x, y);
	cout << "      ";
	gotoxy(x, y + 1);
	cout << "  ■  ";
	gotoxy(x, y + 2);
	cout << "      ";
	delay(50);
	gotoxy(x, y);
	cout << "      ";
	gotoxy(x, y + 1);
	cout << "      ";
	gotoxy(x, y + 2);
	cout << "      ";
	delay(50);
	blank(x,y);
	delay(50);
	mob--;
}
int help(void)
{
	system("cls");
	gotoxy(1,1);
	puts("Tutorial");
	puts("");
	puts("┌──┐┌──┐ ┌──┐┌───┐");
	puts("│Num ││ /  │ │ *  ││ ___  │");
	puts("│Lock││/   │ │    ││      │");
	puts("└──┘└──┘ └──┘└───┘");
	puts(" □□□  □□□   □□□ ┌───┐");
	puts(" □7 □  □8 □   □9 □ │  +   │");
	puts(" □□□  □□□   □□□ │      │");
	puts(" □□□  □□□   □□□ │      │");
	puts(" □4 □  □5 □   □6 □ │      │");
	puts(" □□□  □□□   □□□ └───┘");
    puts(" □□□  □□□   □□□ ┌───┐");
	puts(" □1 □  □2 □   □3 □ │Enter │");
	puts(" □□□  □□□   □□□ │      │");
	puts("┌──────┐ ┌──┐│      │");
	puts("│ 0          │ │ .  ││      │");
	puts("│            │ │Del ││      │");
	puts("└──────┘ └──┘└───┘");
	delay(1000); 
	puts("press enter to start...");
	TU:
		int key=_getch();
		if (key == 13)
		{
			return(0);
		}
		else
		{
			goto TU;
		}
}
void kbhit(int num)
{
	switch (num)
	{
	case 55://키보드 기준, 아스키 코드//7
		if (map[1][1] == 1)
		{
			effect(1, 1);
			map[1][1] = 0;
			text(2);
		}
		else
		{
			health = health - (level * 10);
			wrong++;
			text(3);
		}
		break;
	case 56://8
		if (map[2][1] == 1)
		{
			effect(7, 1);
			map[2][1] = 0;
			text(2);
		}
		else
		{
			health = health - (level * 10);
			wrong++;
			text(3);
		}
		break;
	case 57://9
		if (map[3][1] == 1)
		{
			effect(13, 1);
			map[3][1] = 0;
			text(2);
		}
		else
		{
			health = health - (level * 10);
			wrong++;
			text(3);
		}
		break;
	case 52://4
		if (map[1][2] == 1)
		{
			effect(1, 4);
			map[1][2] = 0;
			text(2);
		}
		else
		{
			health = health - (level * 10); 
			wrong++;
			text(3);
		}
		break;
	case 53://5
		if (map[2][2] == 1)
		{
			effect(7, 4);
			map[2][2] = 0;
			text(2);
		}
		else
		{
			health = health - (level * 10);
			wrong++;
			text(3);
		}
		break;
	case 54://6
		if (map[3][2] == 1)
		{
			effect(13, 4);
			map[3][2] = 0;
			text(2);
		}
		else
		{
			health = health - (level * 10);
			wrong++;
			text(3);
		}
		break;
	case 49://1
		if (map[1][3] == 1)
		{
			effect(1, 7);
			map[1][3] = 0;
			text(2);
		}
		else
		{
			health = health - (level * 10);
			wrong++;
			text(3);
		}
		break;
	case 50://2
		if (map[2][3] == 1)
		{
			effect(7, 7);
			map[2][3] = 0;
			text(2);
		}
		else
		{
			health = health - (level * 10);
			wrong++;
			text(3);
		}
		break;
	case 51://3
		if (map[3][3] == 1)
		{
			effect(13, 7);
			map[3][3] = 0;
			text(2);
		}
		else
		{
			health = health - (level * 10);
			wrong++;
			text(3);
		}
		break;
	}
}
int main();
void end(int mode)
{
	if (mode == 0)
	{
		for (int y = 1; y <= 30; y++)
		{
			for (int x = 1; x <= 80; x = x + 2)
			{
				gotoxy(x, y);
				cout << "▩";
				delay(1);
			}
		}
	}
	delay(1000);
	gotoxy(1, 1);
	system("cls");
	gotoxy(1, 10);
	puts("       ▩▩▩▩▩            ▩▩▩▩▩             ▩▩");
	puts("     ▩▩                  ▩▩                     ▩▩");
	puts("    ▩                     ▩                      ▩▩");
	puts("    ▩                     ▩                      ▩▩");
	puts("    ▩                     ▩                     ▩▩");
	puts("    ▩                     ▩                     ▩▩");
	puts("    ▩     ▩▩▩▩        ▩     ▩▩▩▩         ");
	puts("     ▩          ▩   ▩▩  ▩          ▩       ▩▩");
	puts("       ▩▩▩▩  ▩   ▩▩    ▩▩▩▩  ▩       ▩▩");
	Sleep(1000);
	if (mode == 0)
	{
		system("cls");
		SYSTEMTIME a;
		GetLocalTime(&a);
		gotoxy(1, 1);
		cout << "Your score  : " << score << endl;
		cout << "level       : " << level << endl;
		cout << "missed sqare: " << late << endl;
		cout << "wrong hit   : " << wrong << endl;
		cout << "current time: " << a.wHour << ":" << a.wMinute << ":" << a.wSecond << endl;
		cout << "play time   : " << playtime << "sec" << endl;
		delay(1000);
		cout << "press enter to quit";
		X:
		int key=_getch();
		if (key == 13)
		{
			main();
		}
		else
		{
			goto X;
		}
	}
}
int infinite(int mode)
{
	level = 0;
	RemoveCursor();
	dos_size();
	generatemap();
	output();
	clock_t pbegin, pend;
	pbegin = clock();
	if (mode <3)
	{
		generatemap();
		output();
	}
	if (mode < 2)
	{
        generatemap();
		output();
	}
	int kb;
	kb = _getch();//아스키 코드 반환
	kbhit(kb);
	level = 1;
	generatemap();
HERE:
	for (; health > 0;)
	{
		clock_t begin, end;
		 output();
		 begin = clock();
         int kb;
	     kb = _getch();//아스키 코드 반환
	     kbhit(kb);
		 end = clock();
		 float time=((float)end-begin)/CLOCKS_PER_SEC;
		 gotoxy(20, 4);
		 cout << "time:" << time;
		 if (level == 1 && time>1.2)
		 {
			 score = score - 10;
			 health=health-((level+1)*10);
			 late++;
			 text(1);
		 }
		 else if (level == 2 && time > 1)
		 {
			 score = score - 10;
			  health=health-((level+1)*10);
			 late++;
			 text(1);
		 }
		 else if (level == 3 && time > 0.8)
		 {
			 score = score - 10;
			  health=health-((level+1)*10);
			 late++;
			 text(1);
		 }
		 else if (level == 4 && time > 0.6)
		 {
			 score = score - 10;
			  health=health-((level+1)*10);
			 late++;
			 text(1);
		 }
		 else if (level == 5 && time > 0.5)
		 {
			 score = score - 10;
			  health=health-((level+1)*10);
			 late++;
			 text(1);
		 }
		 else if (level == 6 && time > 0.4)
		 {
			 score = score - 10;
			  health=health-((level+1)*10);
			 late++;
			 text(1);
		 }
		 else if (level == 7 && time > 0.3)
		 {
			 score = score - 10;
			  health=health-((level+1)*10);
			 late++;
			 text(1);
		 }
		 else if (level == 8 && time > 0.2)
		 {
			 score = score - 10;
			  health=health-((level+1)*10);
			 late++;
			 text(1);
		 }
		 else if (level >= 9 && time > 0.1)
		 {
			 score = score - 10;
			  health=health-((level+1)*10);
			 late++;
			 text(1);
		 }
		 if (mob < 3&&mode==1)
		 {
			 generatemap();
		 }
		 if (mob < 2 && mode == 2)
		 {
			 generatemap();
		 }
		 if (mob < 1 && mode == 3)
		 {
			 generatemap();
		 }
		 if (bscore % 200 == 0)
		 {
			 level++;
		 }
		 goto HERE;
	}
	pend = clock();
	playtime = ((float)pend - pbegin) / CLOCKS_PER_SEC;
	return(1);
}
int opening(void)
{
	cin.ignore();
	system("cls");
	for (int y = 1; y <= 30; y++)
	{
		for (int x = 1; x <= 80; x = x + 2)
		{
			gotoxy(x, y);
			cout << "□";
			RemoveCursor();
			delay(1);
		}
	}
	Sleep(500);
	int cursor = 0;
	int point = 0;
	system("cls");
	for (;;)
	{
		cursor = 0;
		gotoxy(1, 1);
		if (point <= 0)
		{
			cout << "                                Select Mode\n";
			cout << "               ■■■                                    □□□\n";
			cout << "               ■■■                                    □□□\n";
			cout << "               ■■■                                    □□□\n";
			cout << "           Infinite Mode                              Rhythm Mode\n";
		}
		else if (point >= 1)
		{
			cout << "                                Select Mode\n";
			cout << "               □□□                                    ■■■\n";
			cout << "               □□□                                    ■■■\n";
			cout << "               □□□                                    ■■■\n";
			cout << "           Infinite Mode                              Rhythm Mode\n";
		}
		RemoveCursor();
		cursor = _getch();
		if (cursor == 224)
		{
			cursor = _getch();
		}
		if (cursor == 75)
		{
			point--;
		}
		if (cursor == 77)
		{
			point++;
		}
		if (point < 0)
		{
			point = 0;
		}
		if (point > 1)
		{
			point = 1;
		}
		if (cursor == 13)
		{
			system("cls");
			return(point);
		}
		else
		{
			system("cls");
		}
	}
}
void rhythm()
{
	system("cls");
	gotoxy(1, 11);
	puts("                                       ■■■");
	puts("                                     ■      ■");
	puts("                                    ■■    ■■");
	puts("                                    ■■    ■■");
	puts("");
	puts("                        Use headphones for the best experience");
	delay(2000);
	dos_size();
	system("cls");
	gotoxy(27, 15);
	cout << "Ready";
	delay(1000);
	gotoxy(27, 15);
	cout << "Start";
	delay(1000);
	system("cls");
	system("mode con: cols=80 lines=30");
	gotoxy(1, 8);
	puts("           ■■■  ■  ■  ■■■  ■■■   ■■■      ■   ■■");
	puts("             ■    ■  ■  ■      ■   ■  ■          ■  ■   " );
	puts("             ■    ■■■  ■■■  ■■■   ■■■      ■   ■■");
	puts("             ■    ■  ■  ■      ■  ■   ■          ■      ■");
	puts("             ■    ■  ■  ■■■  ■   ■  ■■■      ■   ■■    ");
	puts("");
	puts("           ■   ■  ■■■       ■■        ■     ■      ■ ■■■       "); 
	puts("           ■■ ■ ■    ■    ■           ■■    ■■  ■■ ■           ");
	puts("           ■ ■■ ■    ■    ■   ■■   ■  ■   ■  ■  ■ ■■■      ");
	puts("           ■   ■ ■    ■    ■     ■  ■■■■  ■      ■ ■            " );
	puts("           ■   ■  ■■■       ■■ ■ ■      ■ ■      ■ ■■■          ");
	gotoxy(1,1);
	delay(2000);
}
void title()
{
	system("mode con: cols=80 lines=30");
	gotoxy(1, 8);
		puts("         ■    ■   ■  ■■■■■         ■■■■■   ■    ■   ■■■■");
		puts("         ■    ■   ■      ■                 ■       ■    ■   ■");
		puts("         ■    ■   ■      ■                 ■       ■    ■   ■");
		puts("         ■■■■   ■      ■                 ■       ■■■■   ■■■■");
		puts("         ■    ■   ■      ■                 ■       ■    ■   ■");
		puts("         ■    ■   ■      ■                 ■       ■    ■   ■");
		puts("         ■    ■   ■      ■                 ■       ■    ■   ■■■■");
		puts("");
		puts("");
		int key = 0;
		for (;;)
		{
			gotoxy(1,18);
			puts("                                  ■■■");
			puts("                                  ■■■");
			puts("                                  ■■■");
			puts("");
			puts("                               PRESS ENTER");
			gotoxy(1,18);
			delay(1500);
			if (_kbhit())
			{
				break;
			}
			puts("                                  □■■");
			puts("                                  ■■■");
			puts("                                  ■■■");
			puts("");
			puts("                               PRESS ENTER");
			gotoxy(1, 18);
			delay(150); 
			if (_kbhit())
			{
				break;
			}
			puts("                                  □□■");
			puts("                                  □□■");
			puts("                                  ■■■");
			puts("");
			puts("                               PRESS ENTER");
			gotoxy(1,18);
			delay(150);
			if (_kbhit())
			{
				break;
			}
			puts("                                  □□□");
			puts("                                  □□□");
			puts("                                  □□□");
			puts("");
			puts("                               PRESS ENTER");
			gotoxy(1,18);
			delay(1500);
			if (_kbhit())
			{
				break;
			}
			puts("                                  ■□□");
			puts("                                  □□□");
			puts("                                  □□□");
			puts("");
			puts("                               PRESS ENTER");
			gotoxy(1,18);
			delay(150);
			if (_kbhit())
			{
				break;
			}
			puts("                                  ■■□");
			puts("                                  ■■□");
			puts("                                  □□□"); 
			puts("");
			puts("                               PRESS ENTER");
			gotoxy(1,18);
			delay(150);
			if (_kbhit())
			{
				break;
			}
		}
}
int difficulty()
{
	int pointer=1;
	for (;;)
	{
		system("cls");
		RemoveCursor();
		int key=0;
		if (pointer == 1)
		{
			gotoxy(1, 1);
			cout << "                                Select difficulty\n";
			cout << "               ■■■               □□□               □□□\n";
			cout << "               ■■■               □□□               □□□\n";
			cout << "               ■■■               □□□               □□□\n";
			cout << "                Easy               Moderate               Hard  \n";
		}
		else if (pointer == 2)
		{
			gotoxy(1, 1);
			cout << "                                Select difficulty\n";
			cout << "               □□□               ■■■               □□□\n";
			cout << "               □□□               ■■■               □□□\n";
			cout << "               □□□               ■■■               □□□\n";
			cout << "                Easy               Moderate               Hard  \n"; 
		}
		else if (pointer == 3)
		{
			gotoxy(1, 1);
			cout << "                                Select difficulty\n";
			cout << "               □□□               □□□               ■■■\n";
			cout << "               □□□               □□□               ■■■\n";
			cout << "               □□□               □□□               ■■■\n";
			cout << "                Easy               Moderate               Hard  \n";
		}
		key = _getch();
		if (key == 224)
		{
			key = _getch();
		}
		if (key == 75)
		{
			pointer--;
		}
		if (key == 77)
		{
			pointer++;
		}
		if (pointer > 3)
		{
			pointer = 1;
		}
		if (pointer < 1)
		{
			pointer = 3;
		}
		if (key == 13)
		{
			system("cls");
			for (int y = 1; y <= 30; y++)
			{
				for (int x = 1; x <= 80; x = x + 2)
				{
					gotoxy(x, y);
					cout << "□";
					RemoveCursor();
					delay(1);
				}
			}
			Sleep(500);
			return(pointer);
		}
	}
}
int main()
{
START:
	late = 0;//놓친 사각형 수
	wrong = 0;//틀린 사각형 수
	score = 0;//reset game
	bscore=0;
	mob = 0;
	health = 0;
	for (int x = 1; x <= 3; x++)
	{
		for (int y = 1; y <= 3; y++)
		{
			map[x][y] =0 ;
		}
	 }
	RemoveCursor();
	int mode = 0;
	title();
	mode=opening();
	for (int y = 30; y >= 1; y--)
	{
		for (int x = 80; x >= 1; x = x - 2)
		{
			gotoxy(x, y);
			cout << "□";
			RemoveCursor();
			delay(1);
		}
	}
	Sleep(500);
	switch (mode)
	{
	case 0:
		gotoxy(1,1);
		help();
				infinite(difficulty());
		break;
	case 1:
		rhythm();
	}
	end(mode);
	system("cls");
	gotoxy(1, 1);
	goto START;
	return(0);
}
