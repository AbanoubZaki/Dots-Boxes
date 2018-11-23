#ifndef FUNCTIONS&GLOBALS_H_INCLUDED
#define FUNCTIONS&GLOBALS_H_INCLUDED
void delay(unsigned int mseconds);
void welcome();
typedef struct
{
    char name[31];
    int score;
} player;
void sort(player players[10]);
void playedLines(int x,int y,int h[][y],int v [][x]);
typedef struct
{
    int pc;
    char t[2];
    int r1,c1;
} RU;
typedef struct
{
    int sec;
    int min;
    int hr;
} times;
times time_count();
void help();
void initialize2Darray(int x,int y,int a[x][y]);
void undos(int pc,char t[],int r1,int c1,int k);
void undoOnce(int k,int x,int y,int h[][y],int v[][x],int *pc);
void undof(int k,int x,int y,int h[][y],int v[][x],int *pc,char m[],int sq[][y]);
void redoOnce(int k,int x,int y,int h[][y],int v[][x],int *pc,char m[]);
void redof(int k,int x,int y,int h[][y],int v[][x],int *pc,char m[],int sq[][y]);
void saving(char n[],char m[],char name[][31],int x,int y,int h[x][y],int v[y][x],int pc);
void load(char n[],char m[],char name[][31],int* pc);
void gameRun(int a,int x,int y,int h[x][y],int v[y][x],int sq[y][y],int pc,int sc1[2],char m[],char name[][31],char n[],int p,int sc[],char t[]);
void compTurn(int x,int y,int h[x][y],int v[y][x],int pc,int k);
int checkInputBox(int y,int sq[y][y],int sc[2],int sc1[2]);
void checkBoxes(int pc,int x,int y,int sq[y][y],int h[x][y],int v[y][x]);
void printing(int a,int x,int y,int h[x][y],int v[y][x],int sq[y][y],int pc,int sc[2],int sc1[2]);
void BegOrExp(char n[],int *a,int *p,int *x,int *y);
void hVSh_hVScomp(char m[],char name[][31]);
void PrintScores_moves(int sc[],char name[2][31],char m[],int pc,int x,int y,int sq[y][y],int h[x][y],int v[y][x],int sc1[2]);
void printRemainLines(int x,int y,int h[x][y],int v[y][x],int p,int *k);
int leaderboard(char n[],char names[],int sc);
void printLeaderboard();
void winner(int sc[],char m[],char name[][31],char n[]);
int afterEnd();
void MainMenu();








#endif // FUNCTIONS&GLOBALS_H_INCLUDED
