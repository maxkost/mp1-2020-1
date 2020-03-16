#include "windows.h"
#include <stdio.h>
#include <memory.h>
#include <conio.h>

enum TColor {BLACK, BLUE, GREEN, CYAN, RED, MAGENTA, BROWN, LIGHTGRAY,
             DARKGRAY, LIGHTBLUE, LIGHTGREEN, LIGHTCYAN, LIGHTRED,
             LIGHTMAGENTA, YELLOW, WHITE};
             
enum KB
{
    KB_DEFAULT,
    KB_ENTER,
    KB_ESC,
    KB_ARROWUP,
    KB_ARROWDOWN,
    KB_ARROWLEFT,
    KB_ARROWRIGHT
};

//-----------------------------------------------------------------------------
int wherex(void)
{
  HANDLE Console = GetStdHandle(STD_OUTPUT_HANDLE);
  if (!Console)
    return -1;

  CONSOLE_SCREEN_BUFFER_INFO buf;

  GetConsoleScreenBufferInfo(Console, &buf);
  return buf.dwCursorPosition.X;
}

//-----------------------------------------------------------------------------
int wherey(void)
{
  HANDLE Console = GetStdHandle(STD_OUTPUT_HANDLE);
  if (!Console)
    return - 1;

  CONSOLE_SCREEN_BUFFER_INFO buf;

  GetConsoleScreenBufferInfo(Console, &buf);
  return buf.dwCursorPosition.Y;
}

//-----------------------------------------------------------------------------
COORD WhereXY(void)
{
  HANDLE Console = GetStdHandle(STD_OUTPUT_HANDLE);
  COORD pos = {-1, -1};
  if (!Console)
    return pos;

  CONSOLE_SCREEN_BUFFER_INFO buf;

  GetConsoleScreenBufferInfo(Console, &buf);
  return buf.dwCursorPosition;
}

//-----------------------------------------------------------------------------
void clrscr(void)
{
  HANDLE Console = GetStdHandle(STD_OUTPUT_HANDLE);
  if (!Console)
    return;

  CONSOLE_SCREEN_BUFFER_INFO buf;
  GetConsoleScreenBufferInfo(Console, &buf);

  WORD Attr;
  DWORD Count;
  COORD pos = buf.dwCursorPosition;
  ReadConsoleOutputAttribute(Console, &Attr, 1, pos, &Count);

  int col = Attr;
  int width = buf.dwSize.X;
  int height = buf.dwSize.Y;

  COORD zpos;
  zpos.X = 0;
  zpos.Y = 0;
  SetConsoleCursorPosition(Console, zpos);

  FillConsoleOutputAttribute(Console, col, width * height, zpos, &Count);
  FillConsoleOutputCharacter(Console, ' ', width * height, zpos, &Count);
}

//-----------------------------------------------------------------------------
void gotoxy(int x, int y)
{
  HANDLE Console = GetStdHandle(STD_OUTPUT_HANDLE);
  if (!Console)
    return;

  COORD pos;
  pos.X = x;
  pos.Y = y;

  SetConsoleCursorPosition(Console, pos);
}

//-----------------------------------------------------------------------------
void textcolor(int color)
{
  HANDLE Console = GetStdHandle(STD_OUTPUT_HANDLE);
  if (!Console)
    return;

  CONSOLE_SCREEN_BUFFER_INFO buf;
  GetConsoleScreenBufferInfo(Console, &buf);

  WORD Attr;
  DWORD Count;
  COORD pos = buf.dwCursorPosition;
  ReadConsoleOutputAttribute(Console, &Attr, 1, pos, &Count);

  int bk_col = Attr / 16;
  int col = color % 16;
  col = col + bk_col * 16;

  SetConsoleTextAttribute(Console, col);

  char ch;
  pos = WhereXY();
  ReadConsoleOutputCharacter(Console, &ch, 1, pos, &Count);
  printf("%c", ch);
  gotoxy(pos.X, pos.Y);
}

//-----------------------------------------------------------------------------
void textbackground(int color)
{
  HANDLE Console = GetStdHandle(STD_OUTPUT_HANDLE);
  if (!Console)
    return;

  CONSOLE_SCREEN_BUFFER_INFO buf;
  GetConsoleScreenBufferInfo(Console, &buf);

  WORD Attr;
  DWORD Count;
  COORD pos = buf.dwCursorPosition;
  ReadConsoleOutputAttribute(Console, &Attr, 1, pos, &Count);

  int text_col = Attr % 16;
  int col = color % 16;
  col = color * 16 + text_col;

  SetConsoleTextAttribute(Console, col);

  char ch;
  pos = WhereXY();
  ReadConsoleOutputCharacter(Console, &ch, 1, pos, &Count);
  printf("%c", ch);
  gotoxy(pos.X, pos.Y);
}

//-----------------------------------------------------------------------------
void textattr(int color)
{
  HANDLE Console = GetStdHandle(STD_OUTPUT_HANDLE);
  if (!Console)
    return;

  SetConsoleTextAttribute(Console, color);

  DWORD Count;
  char ch;
  COORD pos = WhereXY();
  ReadConsoleOutputCharacter(Console, &ch, 1, pos, &Count);
  printf("%c", ch);
  gotoxy(pos.X, pos.Y);
}

//-----------------------------------------------------------------------------
void setwindow(int width, int height)
{
  HANDLE Console = GetStdHandle(STD_OUTPUT_HANDLE);
  if (!Console)
    return;

  SMALL_RECT rect;
  rect.Left = 0;
  rect.Right = width - 1;
  rect.Top= 0;
  rect.Bottom = height - 1;
  COORD size;
  size.X = width;
  size.Y = height;
  SetConsoleWindowInfo(Console, true, &rect);
//  printf("%d %d %d %d", rect.Left, rect.Top, rect.Right, rect.Bottom);
  SetConsoleScreenBufferSize(Console, size);
//  printf("%d %d", size.X, size.Y);
}

//-----------------------------------------------------------------------------
void SetWindow(SMALL_RECT rect, COORD size)
{
  HANDLE Console = GetStdHandle(STD_OUTPUT_HANDLE);
  if (!Console)
    return;

  SetConsoleScreenBufferSize(Console, size);
  SetConsoleWindowInfo(Console, true, &rect);
}

//-----------------------------------------------------------------------------
void GetWindow(SMALL_RECT *rect, COORD *size)
{
  HANDLE Console = GetStdHandle(STD_OUTPUT_HANDLE);
  if (!Console)
    return;

  CONSOLE_SCREEN_BUFFER_INFO buf;
  GetConsoleScreenBufferInfo(Console, &buf);

  *rect = buf.srWindow;
  size->X = buf.dwSize.X;
  size->Y = buf.dwSize.Y;
}

//-----------------------------------------------------------------------------
void HideCursor(void)
{
  HANDLE Console = GetStdHandle(STD_OUTPUT_HANDLE);
  if (!Console)
    return;

  CONSOLE_CURSOR_INFO buf;
  GetConsoleCursorInfo(Console, &buf);
  buf.bVisible = false;
  SetConsoleCursorInfo(Console, &buf);
}

//-----------------------------------------------------------------------------
void ShowCursor(void)
{
  HANDLE Console = GetStdHandle(STD_OUTPUT_HANDLE);
  if (!Console)
    return;

  CONSOLE_CURSOR_INFO buf;
  GetConsoleCursorInfo(Console, &buf);
  buf.bVisible = true;
  SetConsoleCursorInfo(Console, &buf);
}

//-----------------------------------------------------------------------------
char getKey()
{
    char ch;
    ch = _getch();
    if (ch == 13) return KB_ENTER;
    if (ch != 224) return KB_DEFAULT;
    ch = _getch();
    
    switch (ch)
    {
        case 72:
            return KB_ARROWUP;
        case 80:
            return KB_ARROWDOWN;
        case 74:
            return KB_ARROWRIGHT;
        case 75: 
            return KB_ARROWLEFT;
        default:
            return KB_DEFAULT;
    }   
}
