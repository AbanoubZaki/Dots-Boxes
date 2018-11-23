#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <windows.h>
#include <ctype.h>
FILE *f;
void delay(unsigned int mseconds)
{
    clock_t goal = mseconds + clock();
    while (goal > clock());
}
void welcome()
{
    HANDLE hConsole;
    hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
    int i=0;
    char wel[40];
    strcpy(wel,"Welcome To Our Game");
    printf("\n\n\n\n\n\n\n\n");
    printf("                                       ");
    while(wel[i]!='\0')
    {
        SetConsoleTextAttribute(hConsole,11);
        printf("%c",wel[i]);
        delay(150);
        i++;
    }
    SetConsoleTextAttribute(hConsole,15);
    printf("\n");
}
typedef struct
{
    char name[31];
    int score;
} player;
player players1[10];//top 10 players for mode 1 2*2
player players2[10];//top 10 players for mode 2 5*5
char savedGames[45][66];
//initialize the total array by -1 or the last one only by -1 ************************************************
int count=0,count1=0,count2=0;
void sort(player players[10]) //sorts top 10 from higher to lower
{
    int t=1,V,i,j;
    int s=10;//size of the array of structures
    while (t>s/3)
        t=t*3+1;
    while(t>0)
    {
        for(j=t; j<s; j++)
        {
            V=players[j].score;
            i=j;
            while( (i>t-1) && (players[i-t].score>=V) )
            {
                players[i].score=players[i-t].score;
                i=i-t;

            }
            players[i].score=V ;
        }
        t=(t-1)/3;
    }
    for(i=0; i<(s/2); i++)
    {
        int temp;
        temp=players[i].score;
        players[i].score=players[s-1-i].score;
        players[s-1-i].score=temp;
    }
}

int pls [2]= {0};
void playedLines(int x,int y,int h[][y],int v [][x])
{
    int i,j;
    pls [0]=0,pls [1]=0;
    for(i=0; i<x; i++)
    {
        for(j=0; j<y; j++)
        {
            if(h[i][j]==1)
            {
                pls[0]++;
            }
            else if(h[i][j]==2)
            {
                pls[1]++;
            }
        }
    }
    for(i=0; i<y; i++)
    {
        for(j=0; j<x; j++)
        {
            if(v[i][j]==1)
            {
                pls[0]++;
            }
            else if(v[i][j]==2)
            {
                pls[1]++;
            }
        }
    }
}
int pc1=0;//send it to redo function to know the last turn
int time1;
typedef struct
{
    int pc;
    char t[2];
    int r1,c1;
} RU;
RU undo[60],redo[60];
int redoc=0;
int co1=9,co2=12,cn=15,err1=14,err2=12;//colours
typedef struct
{
    int sec;
    int min;
    int hr;
} times;
times timesplit;//time variable
times time_count()
{
    int time2=clock()/1000-time1;
    timesplit.sec=time2%60;
    time2=time2/60;
    timesplit.min=time2%60;
    time2=time2/60;
    timesplit.hr=time2;
    return timesplit;
}
void help()
{
    system("cls");
    system("00.jpg");
    printf("Press Enter for the next instruction");
    getchar();
    system("01.jpg");
    system("cls");
    printf("Press Enter for the next instruction");
    getchar();
    system("02.jpg");
    system("cls");
    printf("Press Enter for the next instruction");
    getchar();
    system("03.jpg");
    system("cls");
}
void initialize2Darray(int x,int y,int a[x][y])
{
    int i=0,j=0;
    for(i=0; i<x; i++)
    {
        for(j=0; j<y; j++)
        {
            a[i][j]=0;
        }
    }
}
void undos(int pc,char t[],int r1,int c1,int k)
{
    undo[k].pc=pc;
    undo[k].t[0]=t[0];
    undo[k].r1=r1;
    undo[k].c1=c1;
}
void undoOnce(int k,int x,int y,int h[][y],int v[][x],int *pc)//undo for 2 player mode
{
    *pc=undo[k-1].pc;//giving the turn to the owner of the previous move
    redo[redoc].pc=undo[k-1].pc;//copy 1 move in redo array
    redo[redoc].t[0]=undo[k-1].t[0];
    redo[redoc].r1=undo[k-1].r1;
    redo[redoc].c1=undo[k-1].c1;
    if(undo[k-1].t[0]=='h')//give this move a zero
    {
        h[undo[k-1].r1-1][undo[k-1].c1-1]=0;
        redoc++;
    }
    else if(undo[k-1].t[0]=='v')
    {
        v[undo[k-1].r1-1][undo[k-1].c1-1]=0;
        redoc++;
    }
}
void undof(int k,int x,int y,int h[][y],int v[][x],int *pc,char m[],int sq[][y])//undo for 1 player mode
{
    if(k>=0)
    {
        if(undo[k-1].pc%2==0)
        {
            undoOnce(k,x,y,h,v,&pc);
        }
        else
        {
            undoOnce(k,x,y,h,v,&pc);
            checkBoxes(pc,x,y,sq,h,v);
            undof(k-1,x,y,h,v,&pc,m,sq);
        }
    }

}
void redoOnce(int k,int x,int y,int h[][y],int v[][x],int *pc,char m[])
{
    if(redoc!=0)
    {
        redoc--;

    }
    undo[k].pc=redo[redoc].pc;//copy 1 move in redo array
    undo[k].t[0]=redo[redoc].t[0];
    undo[k].r1=redo[redoc].r1;
    undo[k].c1=redo[redoc].c1;
    if(redo[redoc].t[0]=='h')//give this move a zero
    {
        h[redo[redoc].r1-1][redo[redoc].c1-1]=redo[redoc].pc%2+1;
    }
    else if(redo[redoc].t[0]=='v')
    {
        v[redo[redoc].r1-1][redo[redoc].c1-1]=redo[redoc].pc%2+1;
    }
    *pc=redo[redoc-1].pc;//giving the turn to the owner of the previous move
    if(redoc==0)
    {
        *pc=pc1;
    }
}
void redof(int k,int x,int y,int h[][y],int v[][x],int *pc,char m[],int sq[][y])//undo for 1 player mode
{
    redoOnce(k,x,y,h,v,&pc,m);
    k++;
    checkBoxes(pc,x,y,sq,h,v);
    if(redo[redoc-1].pc%2==0)
    {
        return;
    }
    else if(redo[redoc-1].pc%2==1)
    {
        redof(k,x,y,h,v,&pc,m,sq);
        checkBoxes(pc,x,y,sq,h,v);
    }
}
void saving(char n[],char m[],char name[][31],int x,int y,int h[x][y],int v[y][x],int pc)
{
    count=0;
    int i=0,j=0;
    int flag=0;
    char fileName[64];
    strcpy(fileName,name[0]);
    if(m[0]=='a')
    {
        strcat(fileName," VS computer.txt");
    }
    else if(m[0]=='b')
    {
        strcat(fileName," VS ");
        strcat(fileName,name[1]);
        strcat(fileName,".txt");
    }
    f=fopen("Loading Counter.txt", "r");
    if(f!=NULL)
    {
        fscanf(f,"%d",&count);
        fclose(f);
    }
    else
    {
        f=fopen("Loading Counter.txt", "w");
        fprintf(f,"%d",count);
        fclose(f);
        f=fopen("Loading Counter.txt", "r");
        fscanf(f,"%d",&count);
        fclose(f);
    }
    char savedGames[count][66];
    i=0;
    if(count==0)
    {
        count++;
    }
    f=fopen("saved games.txt", "r");
    if(f!=NULL)
    {
        while (!feof(f))
        {
            for(i=0; i<count; i++)
            {
                fscanf(f, "%[^,],\n",savedGames[i]);
                if(!strcmp(savedGames[i],fileName))
                {
                    flag++;
                    break;
                }
            }
        }
        fclose(f);
    }
    else
    {
        f=fopen("saved games.txt", "w");
        fclose(f);
        f=fopen("saved games.txt", "r");
        while (!feof(f))
        {
            for(i=0; i<count; i++)
            {
                fscanf(f, "%[^,],\n",savedGames[i]);
                if(!strcmp(savedGames[i],fileName))
                {
                    flag++;
                    break;
                }
            }
        }
        fclose(f);
    }
    if(flag==0)
    {
        f=fopen("Loading Counter.txt", "r");//if the game is repeated dont inc the counter
        fscanf(f,"%d",&count);
        count++;
        fclose(f);
        f=fopen("Loading Counter.txt", "w");
        fprintf(f,"%d",count);
        fclose(f);
        f=fopen("saved games.txt", "a");
        fprintf(f,"%s,\n",fileName);
        fclose(f);
        f=fopen(fileName, "w");
        if (f != NULL)
        {
            //write on file
            fprintf(f,"%c\n",n[0]);
            fprintf(f,"%c\n",m[0]);
            if(m[0]=='a')
            {
                fprintf(f,"%s,\n",name[0]);
            }
            else
            {
                fprintf(f,"%s,\n",name[0]);
                fprintf(f,"%s,\n",name[1]);
            }
            for(i=0; i<x; i++)
            {
                //printing h array on a file
                for(j=0; j<y; j++)
                {
                    fprintf(f,"%d ",h[i][j]);
                }
                fprintf(f,"\n");
            }
            for(i=0; i<y; i++)
            {
                //printing h array on a file
                for(j=0; j<x; j++)
                {
                    fprintf(f,"%d ",v[i][j]);
                }
                fprintf(f,"\n");
            }
            fprintf(f,"%d",pc);
            fclose(f);
        }
    }
    else
    {
        f=fopen(fileName, "w");
        if (f != NULL)
        {
            //write on file
            fprintf(f,"%c\n",n[0]);
            fprintf(f,"%c\n",m[0]);
            if(m[0]=='a')
            {
                fprintf(f,"%s,\n",name[0]);
            }
            else
            {
                fprintf(f,"%s,\n",name[0]);
                fprintf(f,"%s,\n",name[1]);
            }
            for(i=0; i<x; i++)
            {
                //printing h array on a file
                for(j=0; j<y; j++)
                {
                    fprintf(f,"%d ",h[i][j]);
                }
                fprintf(f,"\n");
            }
            for(i=0; i<y; i++)
            {
                //printing h array on a file
                for(j=0; j<x; j++)
                {
                    fprintf(f,"%d ",v[i][j]);
                }
                fprintf(f,"\n");
            }
            fprintf(f,"%d",pc);
            fclose(f);
        }
    }
}
void load(char n[],char m[],char name[][31],int* pc)
{
    HANDLE hConsole;
    hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
    char t[5000];//type of line
    t[0]='\0';
    int sc[2],sc1[2]; //scores
    sc[0]=sc[1]=sc1[0]=sc1[1]=0;
    char r[5000];
    int i=0,j=0,r1=0;
    int x=0,y=0,a=0,p=0;
    f=fopen("Loading Counter.txt", "r");
    fscanf(f,"%d",&count);
    fclose(f);
    char savedGames[count][66];
    f=fopen("saved games.txt", "r");
    system("cls");
    if (f != NULL&&count!=0)
    {
        while (!feof(f))
        {
            for(i=0; i<count; i++)
            {
                fscanf(f, "%[^,],\n",savedGames[i]);
                SetConsoleTextAttribute(hConsole,9);
                printf("(%d)%s\n",i+1,savedGames[i]);
                SetConsoleTextAttribute(hConsole,15);
            }
        }
        fclose(f);
        i=0;
        while(i==0)
        {
            //taking the number of the file to be loaded
            SetConsoleTextAttribute(hConsole,11);
            printf("\nTo go back to main menu enter (a)\nChoose your game : ");
            SetConsoleTextAttribute(hConsole,15);
            fflush(stdin);
            gets(r);
            if(r[1]=='\0')
            {
                if(r[0]=='0')
                {
                    i=0;
                }
                else if(r[0]=='a')
                {
                    system("cls");
                    MainMenu();
                }
                else
                {
                    r1=r[0]-'0';
                    if(r1<0||r1>count)
                    {
                        i=0;
                    }
                    else
                    {
                        i++;
                    }
                }
            }
        }
        f=fopen(savedGames[r1-1], "r");
        fscanf(f,"%s\n",&n[0]);
        switch(n[0])
        {
        //same mode of the game 2*2 or 5*5
        case 'a':
            a=3;
            p=12;
            x=3,y=2;
            break;
        case 'b':
            a=6;
            p=60;
            x=6,y=5;
            break;
        }
        fscanf(f,"%s\n",&m[0]);
        if(m[0]=='a')
        {
            fscanf(f,"%[^,],\n",name[0]);
        }
        else
        {
            fscanf(f,"%[^,],\n",name[0]);
            fscanf(f,"%[^,],\n",name[1]);
        }
        int h[x][y],v[y][x],sq[y][y];
        initialize2Darray(y,y,sq);// initialize by zeroes
        for(i=0; i<x; i++)
        {
            //printing h array on a file
            for(j=0; j<y; j++)
            {
                fscanf(f,"%d ",&h[i][j]);
            }
            fscanf(f,"\n");
        }
        for(i=0; i<y; i++)
        {
            //printing h array on a file
            for(j=0; j<x; j++)
            {
                fscanf(f,"%d ",&v[i][j]);
            }
            fscanf(f,"\n");
        }
        fscanf(f,"%d",&pc);
        gameRun(a,x,y,h,v,sq,pc,sc1,m,name,n,p,sc,t);
        fclose(f);
        if(t[0]=='s'||t[0]=='e')
        {
            //save game progress and close
            system("cls");
            afterEnd();
        }
        else
        {
            system ("cls");
            PrintScores_moves(sc,name,m,pc,x,y,sq,h,v,sc1);
            winner(sc,m,name,n);
            printing(a,x,y,h,v,sq,pc,sc,sc1);
            for(i=(r1-1); i<(count-r1+1); i++)
            {
                strcpy(savedGames[i],savedGames[i+1]);
            }
            strcpy(savedGames[i],"\0");
            f=fopen("Loading Counter.txt", "r");//if the game is repeated dont inc the counter
            fscanf(f,"%d",&count);
            count--;
            fclose(f);
            f=fopen("Loading Counter.txt", "w");
            fprintf(f,"%d",count);
            fclose(f);
            f=fopen("saved games.txt", "w");
            for(i=0; i<count; i++)
            {
                fprintf(f,"%s,\n",savedGames[i]);
            }
            fclose(f);
            afterEnd();
            return;
        }
    }
    else
    {
        system("cls");
        HANDLE hConsole;
        hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole,12);
        printf("There are no saved games :( \n\n");
        SetConsoleTextAttribute(hConsole,15);
    }

}
void gameRun(int a,int x,int y,int h[x][y],int v[y][x],int sq[y][y],int pc,int sc1[2],char m[],char name[][31],char n[],int p,int sc[],char t[])
{

    time1=clock()/1000;
    HANDLE hConsole;
    hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
    char ex;
    int k=0;//counter
    char r[5000],c[5000];//r for row & c for column
    int r1=0,c1=0;
    int i=0;
    int mf=0; //mistake flag takes 1 when a move is repeated
    if(m[0]=='a') //the running game of 1 player mode
    {
        while(k<p)
        {
            if((pc%2)+1==1) //Player 1 turn
            {
                system ("cls");
                PrintScores_moves(sc,name,m,pc,x,y,sq,h,v,sc1);
                SetConsoleTextAttribute(hConsole,co1);
                printf("Its %s's turn\n",name[(pc%2)]);
                printRemainLines(x,y,h,v,p,&k);
                SetConsoleTextAttribute(hConsole,15);
                printing(a,x,y,h,v,sq,pc,sc,sc1);
                if(mf==1)
                {
                    SetConsoleTextAttribute(hConsole,14);
                    printf("This move is not valid !!\n Try AGAIN :P \n\n");
                    SetConsoleTextAttribute(hConsole,15);
                }
                mf=0;
            }
            else //computer turn
            {
                system ("cls");
                PrintScores_moves(sc,name,m,pc,x,y,sq,h,v,sc1);
                SetConsoleTextAttribute(hConsole,co1);
                printf("Its %s's turn\n",name[(pc%2)]);
                printRemainLines(x,y,h,v,p,&k);
                SetConsoleTextAttribute(hConsole,15);
                printing(a,x,y,h,v,sq,pc,sc,sc1);
            }
            if(pc%2==0)
            {
                printf("\nEnter type of line Horizontal(h) or Vertical(v) : ");
                fflush(stdin);
                gets(t);
                if(islower(t[0]))
                {
                    if(t[1]=='\0')
                    {
                        switch (t[0])
                        {
                        case 'e':
                            system("cls");
                            printf("\n\n\n\n\n\n\n\n");
                            printf("                                       ");
                            SetConsoleTextAttribute(hConsole,12);
                            printf("Are you sure you want to exit?? :(\n");
                            SetConsoleTextAttribute(hConsole,9);
                            printf("\n                                       ");
                            printf("(y)YES :(\n");
                            printf("\n                                       ");
                            printf("(n)NO :)\n");
                            SetConsoleTextAttribute(hConsole,15);
                            ex=getche();
                            if(ex=='y')
                            {
                                return;
                            }
                            else if(ex=='n')
                            {
                                mf=2;
                            }
                            else
                            {
                                mf=1;
                            }
                            break;
                        case 's'://save game progress and close
                            /*saving in files*/
                            saving(n,m,name,x,y,h,v,pc);
                            break;
                        case 'r':
                            redof(k,x,y,h,v,&pc,m,sq);
                            break;
                        case 'u':
                            undof(k,x,y,h,v,&pc,m,sq);
                            break;
                        case 'h':
                            printf("\nEnter the index of the row: ");
                            fflush(stdin);
                            gets(r);
                            if(r[1]=='\0')
                            {
                                if(r[0]=='0')
                                {
                                    mf=1;
                                    break;
                                }
                                r1=r[0]-'0';
                                if(r1<0||r1>x)
                                {
                                    mf=1;
                                    break;
                                }
                                printf("\nEnter the index of the column: ");
                                fflush(stdin);
                                gets(c);
                                if(c[1]=='\0')
                                {
                                    if(c[0]=='0')
                                    {
                                        mf=1;
                                        break;
                                    }
                                    c1=c[0]-'0';
                                    if(c1<0||c1>y)
                                    {
                                        mf=1;
                                        break;
                                    }
                                    while(h[r1-1][c1-1]!=0) //move is repeated
                                    {
                                        mf=1;
                                        break;
                                    }
                                    if(h[r1-1][c1-1]==0) //move is valid
                                    {
                                        redoc=0;
                                        undos(pc,t,r1,c1,k);//saving in undo array of structures
                                        h[r1-1][c1-1]=(pc%2)+1;
                                        checkBoxes(pc,x,y,sq,h,v);
                                        pc++;
                                        if(checkInputBox(y,sq,sc,sc1))
                                        {
                                            pc--;
                                        }
                                    }
                                }
                                else
                                {
                                    mf=1;
                                    break;
                                }

                            }
                            else
                            {
                                mf=1;
                                break;
                            }

                            break;
                        case 'v':
                            printf("\nEnter the index of the row: ");
                            fflush(stdin);
                            gets(r);
                            if(r[1]=='\0')
                            {
                                if(r[0]=='0')
                                {
                                    mf=1;
                                    break;
                                }
                                r1=r[0]-'0';
                                if(r1<0||r1>y)
                                {
                                    mf=1;
                                    break;
                                }
                                printf("\nEnter the index of the column: ");
                                fflush(stdin);
                                gets(c);
                                if(c[1]=='\0')
                                {
                                    if(c[0]=='0')
                                    {
                                        mf=1;
                                        break;
                                    }
                                    c1=c[0]-'0';
                                    if(c1<0||c1>x)
                                    {
                                        mf=1;
                                        break;
                                    }
                                    while(v[r1-1][c1-1]!=0) //move is repeated
                                    {
                                        mf=1;
                                        break;
                                    }
                                    if(v[r1-1][c1-1]==0) //move is valid
                                    {
                                        redoc=0;
                                        undos(pc,t,r1,c1,k);//saving in undo array of structures
                                        v[r1-1][c1-1]=(pc%2)+1;
                                        checkBoxes(pc,x,y,sq,h,v);
                                        pc++;
                                        if(checkInputBox(y,sq,sc,sc1))
                                        {
                                            pc--;
                                        }
                                    }
                                }
                                else
                                {
                                    mf=1;
                                    break;
                                }

                            }
                            else
                            {
                                mf=1;
                                break;
                            }

                            break;
                        default:
                            mf=1;
                            break;
                        }
                    }
                    else
                    {
                        mf=1;
                    }

                }
                else
                {
                    if(t[0]!='\0')
                    {
                        mf=1;
                    }
                }
            }
            else
            {
                compTurn(x,y,h,v,pc,k);
                checkBoxes(pc,x,y,sq,h,v);
                pc++;
                if(checkInputBox(y,sq,sc,sc1))
                {
                    pc--;
                }
            }
            if(k==p-1&&mf==0&&(sc[0]+sc[1]==y*y))
            {
                k++;
            }
            if(t[0]=='s')
            {
                break;
            }
        }
    }
    else //the running game of 2 players mode
    {
        while(k<p)
        {
            if((pc%2)+1==1) //Player 1 turn
            {
                system ("cls");
                PrintScores_moves(sc,name,m,pc,x,y,sq,h,v,sc1);
                SetConsoleTextAttribute(hConsole,co1);
                printf("Its %s's turn\n",name[(pc%2)]);
                printRemainLines(x,y,h,v,p,&k);
                SetConsoleTextAttribute(hConsole,15);
                printing(a,x,y,h,v,sq,pc,sc,sc1);
                if(mf==1)
                {
                    SetConsoleTextAttribute(hConsole,14);
                    printf("This move is not valid !!\n Try AGAIN :P \n\n");
                    SetConsoleTextAttribute(hConsole,15);
                }
                mf=0;
            }
            else //player 2 turn
            {
                system ("cls");
                PrintScores_moves(sc,name,m,pc,x,y,sq,h,v,sc1);
                SetConsoleTextAttribute(hConsole,co2);
                printf("Its %s's turn\n",name[(pc%2)]);
                printRemainLines(x,y,h,v,p,&k);
                SetConsoleTextAttribute(hConsole,15);
                printing(a,x,y,h,v,sq,pc,sc,sc1);
                if(mf==1)
                {
                    SetConsoleTextAttribute(hConsole,14);
                    printf("This move is not valid !!\n Try AGAIN :P \n\n");
                    SetConsoleTextAttribute(hConsole,15);
                }
                mf=0;
            }
            printf("\nEnter type of line Horizontal(h) or Vertical(v) : ");
            fflush(stdin);
            gets(t);
            if(islower(t[0]))
            {
                if(t[1]=='\0')
                {
                    switch (t[0])
                    {
                    case 'e':
                        system("cls");
                        printf("\n\n\n\n\n\n\n\n");
                        printf("                                       ");
                        SetConsoleTextAttribute(hConsole,12);
                        printf("Are you sure you want to exit?? :(\n");
                        SetConsoleTextAttribute(hConsole,9);
                        printf("\n                                       ");
                        printf("(y)YES :(\n");
                        printf("\n                                       ");
                        printf("(n)NO :)\n");
                        SetConsoleTextAttribute(hConsole,15);
                        ex=getche();
                        if(ex=='y')
                        {
                            return;
                        }
                        else if(ex=='n')
                        {
                            mf=2;
                        }
                        else
                        {
                            mf=1;
                        }
                        break;
                    case 's'://save game progress and close
                        saving(n,m,name,x,y,h,v,pc);
                        break;
                    case 'r':
                        redoOnce(k,x,y,h,v,&pc,m);
                        break;
                    case 'u':
                        if(redoc==0)
                        {
                            pc1=pc;
                        }
                        undoOnce(k,x,y,h,v,&pc);
                        checkInputBox(y,sq,sc,sc1);
                        checkBoxes(pc,x,y,sq,h,v);
                        break;
                    case 'h':
                        printf("\nEnter the index of the row: ");
                        fflush(stdin);
                        gets(r);
                        if(r[1]=='\0')
                        {
                            if(r[0]=='0')
                            {
                                mf=1;
                                break;
                            }
                            r1=r[0]-'0';
                            if(r1<0||r1>x)
                            {
                                mf=1;
                                break;
                            }
                            printf("\nEnter the index of the column: ");
                            fflush(stdin);
                            gets(c);
                            if(c[1]=='\0')
                            {
                                if(c[0]=='0')
                                {
                                    mf=1;
                                    break;
                                }
                                c1=c[0]-'0';
                                if(c1<0||c1>y)
                                {
                                    mf=1;
                                    break;
                                }
                                while(h[r1-1][c1-1]!=0) //move is repeated
                                {
                                    mf=1;
                                    break;
                                }
                                if(h[r1-1][c1-1]==0) //move is valid
                                {
                                    redoc=0;
                                    undos(pc,t,r1,c1,k);//saving in undo array of structures
                                    h[r1-1][c1-1]=(pc%2)+1;
                                    checkBoxes(pc,x,y,sq,h,v);
                                    pc++;
                                    if(checkInputBox(y,sq,sc,sc1))
                                    {
                                        pc--;
                                    }
                                }
                            }
                            else
                            {
                                mf=1;
                                break;
                            }

                        }
                        else
                        {
                            mf=1;
                            break;
                        }

                        break;
                    case 'v':
                        printf("\nEnter the index of the row: ");
                        fflush(stdin);
                        gets(r);
                        if(r[1]=='\0')
                        {
                            if(r[0]=='0')
                            {
                                mf=1;
                                break;
                            }
                            r1=r[0]-'0';
                            if(r1<0||r1>y)
                            {
                                mf=1;
                                break;
                            }
                            printf("\nEnter the index of the column: ");
                            fflush(stdin);
                            gets(c);
                            if(c[1]=='\0')
                            {
                                if(c[0]=='0')
                                {
                                    mf=1;
                                    break;
                                }
                                c1=c[0]-'0';
                                if(c1<0||c1>x)
                                {
                                    mf=1;
                                    break;
                                }
                                while(v[r1-1][c1-1]!=0) //move is repeated
                                {
                                    mf=1;
                                    break;
                                }
                                if(v[r1-1][c1-1]==0) //move is valid
                                {
                                    redoc=0;
                                    undos(pc,t,r1,c1,k);//saving in undo array of structures
                                    v[r1-1][c1-1]=(pc%2)+1;
                                    checkBoxes(pc,x,y,sq,h,v);
                                    pc++;
                                    if(checkInputBox(y,sq,sc,sc1))
                                    {
                                        pc--;
                                    }
                                }
                            }
                            else
                            {
                                mf=1;
                                break;
                            }

                        }
                        else
                        {
                            mf=1;
                            break;
                        }

                        break;
                    default:
                        mf=1;
                        break;
                    }
                }
                else
                {
                    mf=1;
                }

            }
            else
            {
                if(t[0]!='\0')
                {
                    mf=1;
                }
            }
            if(k==p-1&&mf==0&&(sc[0]+sc[1]==y*y))
            {
                k++;
            }
            if(t[0]=='s')
            {
                break;
            }
        }
    }
}
void compTurn(int x,int y,int h[x][y],int v[y][x],int pc,int k) //gives the computer a random right move
{
    int i=0,j=0;//counters
    int f=0,f1=0,f2=0;//flags
    int a,b;//carries a random values for a random place
    int c=0;
    char t[1];
    for(i=0; i<y; i++) //checks if there is a sideless square and completes it(square with 3 sides only)
    {
        for(j=0; j<y; j++)
        {
            if(h[i][j]==0&&v[i][j]!=0&&h[i+1][j]!=0&&v[i][j+1]!=0)
            {
                h[i][j]=2;
                t[0]='h';
                undos(pc,t,i+1,j+1,k);
                f++;
                return;
            }
            else if(h[i][j]!=0&&v[i][j]==0&&h[i+1][j]!=0&&v[i][j+1]!=0)
            {
                v[i][j]=2;
                t[0]='v';
                undos(pc,t,i+1,j+1,k);
                f++;
                return;
            }
            else if(h[i][j]!=0&&v[i][j]!=0&&h[i+1][j]==0&&v[i][j+1]!=0)
            {
                h[i+1][j]=2;
                t[0]='h';
                undos(pc,t,i+2,j+1,k);
                f++;
                return;
            }
            else if(h[i][j]!=0&&v[i][j]!=0&&h[i+1][j]!=0&&v[i][j+1]==0)
            {
                v[i][j+1]=2;
                t[0]='v';
                undos(pc,t,i+1,j+2,k);
                f++;
                return;
            }
        }
    }
    if(f==0)
    {
        for(i=0; i<y; i++) //checks if there is a sideless square and completes it(square with 3 sides only)
        {
            for(j=0; j<y; j++)
            {
                /**when it reaches here theres no sideless moraba3 so play in a i side square **/
                if(h[i][j]==0&&v[i][j]==0&&h[i+1][j]==0&&v[i][j+1]!=0)
                {
                    h[i][j]=2;
                    t[0]='h';
                    undos(pc,t,i+1,j+1,k);
                    f++;
                    return;
                }
                else if(h[i][j]!=0&&v[i][j]==0&&h[i+1][j]==0&&v[i][j+1]==0)
                {
                    v[i][j]=2;
                    t[0]='v';
                    undos(pc,t,i+1,j+1,k);
                    f++;
                    return;
                }
                else if(h[i][j]==0&&v[i][j]!=0&&h[i+1][j]==0&&v[i][j+1]==0)
                {
                    h[i+1][j]=2;
                    t[0]='h';
                    undos(pc,t,i+2,j+1,k);
                    f++;
                    return;
                }
                else if(h[i][j]==0&&v[i][j]==0&&h[i+1][j]!=0&&v[i][j+1]==0)
                {
                    v[i][j+1]=2;
                    t[0]='v';
                    undos(pc,t,i+1,j+2,k);
                    f++;
                    return;
                }
            }
        }
    }
    if(f==0) //theres no sideless square
    {
        for(i=0; i<x; i++) //search if there is a empty horizontal line
        {
            for(j=0; j<y; j++)
            {
                if(h[i][j]==0)
                {
                    f1++;
                }
            }
        }
        if(f1!=0) //found a free horizontal line so it gives a random horizontal move
        {
            f1=0;
            while(f1==0) //checks if the random horizontal move is valid
            {
                a=rand() % x;
                b=rand() % y;
                if(h[a][b]==0)
                {
                    h[a][b]=2;
                    t[0]='h';
                    undos(pc,t,a+1,b+1,k);
                    f1++;
                }
            }
        }
        else //there is no valid horizontal move so it gives a random vertical move
        {
            while(f2==0) //checks if the random vertical move is valid
            {
                a=rand() % y;
                b=rand() % x;
                if(v[a][b]==0)
                {
                    v[a][b]=2;
                    t[0]='v';
                    undos(pc,t,a+1,b+1,k);
                    f2++;
                }
            }
        }
    }
}
int checkInputBox(int y,int sq[y][y],int sc[2],int sc1[2])
{
    int i,j,f=0;
    sc1[0]=sc[0],sc1[1]=sc[1];
    sc[0]=0,sc[1]=0;
    for(i=0; i<y; i++)
    {
        for(j=0; j<y; j++)
        {
            if(sq[i][j]!=0)
            {
                switch(sq[i][j])
                {
                case 1:
                    sc[0]++;
                    break;
                case 2:
                    sc[1]++;
                    break;
                default:
                    break;
                }
            }
        }
    }
    for(i=0; i<2; i++)
    {
        if(sc[i]!=sc1[i])
        {
            f++;
            break;
        }
    }
    return f;
}
void checkBoxes(int pc,int x,int y,int sq[y][y],int h[x][y],int v[y][x])
{
    int i=0,j=0;
    for(i=0; i<y; i++)
    {
        for(j=0; j<y; j++)
        {
            if(h[i][j]==0&&v[i][j]!=0&&h[i+1][j]!=0&&v[i][j+1]!=0)
            {
                sq[i][j]=0;
            }
            else if(h[i][j]!=0&&v[i][j]==0&&h[i+1][j]!=0&&v[i][j+1]!=0)
            {
                sq[i][j]=0;
            }
            else if(h[i][j]!=0&&v[i][j]!=0&&h[i+1][j]==0&&v[i][j+1]!=0)
            {
                sq[i][j]=0;
            }
            else if(h[i][j]!=0&&v[i][j]!=0&&h[i+1][j]!=0&&v[i][j+1]==0)
            {
                sq[i][j]=0;
            }
            if(sq[i][j]==0)
            {
                if(h[i][j]!=0&&v[i][j]!=0&&h[i+1][j]!=0&&v[i][j+1]!=0)
                {
                    sq[i][j]=(pc%2)+1;//changing the value of sq[][]
                }
            }
        }
    }
}
void printing(int a,int x,int y,int h[x][y],int v[y][x],int sq[y][y],int pc,int sc[2],int sc1[2])
{
    HANDLE hConsole;
    hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
    time_count();
    SetConsoleTextAttribute(hConsole,11);/*******change timer colour********/
    printf("                               hr:min:sec\n");
    printf("                               %d   %d   %d\n",timesplit.hr,timesplit.min,timesplit.sec);
    SetConsoleTextAttribute(hConsole,15);
    int i,j;
    //this loop is to check if there is a square completed or not.
    checkInputBox(y,sq,sc,sc1);
    checkBoxes(pc,x,y,sq,h,v);
    //changing sq[][] is completed.
    /******* specefied for 2*2 & 5*5 only ********/
    if(x==3)
    {
        printf("    v     v     v\n");
        printf("    1  h  2  h  3\n");
        printf("       1     2   \n\n");

    }
    else
    {
        printf("    v     v     v     v     v     v\n");
        printf("    1  h  2  h  3  h  4  h  5  h  6\n");
        printf("       1     2     3     4     5   \n\n");
    }
    for(i=0; i<a; i++)
    {
        //print horizontal & vertical
        for(j=0; j<a; j++)
        {
            if(j==0)
            {
                printf("h %d ",i+1);
            }
            printf("*");
            if(j==y)
            {
                break;
            }
            switch(h[i][j])
            {
            case 0:
                printf("     ");// printing space
                break;
            case 1:
                SetConsoleTextAttribute(hConsole,co1);
                printf("-----");//colour 1
                SetConsoleTextAttribute(hConsole,15);
                break;
            case 2:
                SetConsoleTextAttribute(hConsole,co2);
                printf("-----");//colour 2
                SetConsoleTextAttribute(hConsole,15);
                break;
            }

        }
        printf("\n");
        int k=0;
        while(k<3)
        {
            if(k==1)
            {
                if(i==y)
                {

                }
                else
                {
                    printf("v %d ",i+1);
                }
            }
            else
            {
                printf("    ");
            }
            for(j=0; j<a; j++)
            {
                if(i==y)
                {
                    break;
                }
                if(j==a-1)
                {
                    switch(v[i][j])
                    {
                    case 0:
                        printf(" ");// printing line
                        printf("    ");// printing space
                        break;
                    case 1:
                        SetConsoleTextAttribute(hConsole,co1);
                        printf("|");//colour 1
                        SetConsoleTextAttribute(hConsole,15);
                        printf("     ");//space
                        break;
                    case 2:
                        SetConsoleTextAttribute(hConsole,co2);
                        printf("|");//colour 2
                        SetConsoleTextAttribute(hConsole,15);
                        printf("     ");//space
                        break;
                    }
                }
                else
                {
                    switch(v[i][j])
                    {
                    case 0:
                        printf(" ");// printing line
                        printf("     ");// printing space
                        break;
                    case 1:
                        SetConsoleTextAttribute(hConsole,co1);
                        printf("|");//colour 1
                        SetConsoleTextAttribute(hConsole,15);
                        switch(sq[i][j]) //printing depends on the array sq
                        {
                        case 0:
                            printf("     ");
                            break;
                        case 1:
                            SetConsoleTextAttribute(hConsole,co1);
                            printf("%c%c%c%c%c",219,219,219,219,219);//put the code of the colour of player one.
                            SetConsoleTextAttribute(hConsole,15);
                            break;
                        case 2:
                            SetConsoleTextAttribute(hConsole,co2);
                            printf("%c%c%c%c%c",219,219,219,219,219);//put the code the colour of player two.
                            SetConsoleTextAttribute(hConsole,15);
                            break;
                        }
                        break;
                    case 2:
                        SetConsoleTextAttribute(hConsole,co2);
                        printf("|");//colour 2
                        SetConsoleTextAttribute(hConsole,15);
                        switch(sq[i][j]) //printing depends on the array sq
                        {
                        case 0:
                            printf("     ");
                            break;
                        case 1:
                            SetConsoleTextAttribute(hConsole,co1);
                            printf("%c%c%c%c%c",219,219,219,219,219);//put the code of the colour of player one.
                            SetConsoleTextAttribute(hConsole,15);
                            break;
                        case 2:
                            SetConsoleTextAttribute(hConsole,co2);
                            printf("%c%c%c%c%c",219,219,219,219,219);//put the code the colour of player two.
                            SetConsoleTextAttribute(hConsole,15);
                            break;
                        }
                        break;
                    }
                }
            }
            printf("\n");
            k++;
        }

    }
    if((sc[0]+sc[1])<(y*y))
    {
        SetConsoleTextAttribute(hConsole,11);
        printf("To save the game and close enter (s)....To close the game without saving press (e)\n");
        SetConsoleTextAttribute(hConsole,15);
        SetConsoleTextAttribute(hConsole,2);
        printf("for UNDO enter (u), for REDO enter (r)\n");
        SetConsoleTextAttribute(hConsole,15);
    }
}
void BegOrExp(char n[],int *a,int *p,int *x,int *y)
{
    HANDLE hConsole;
    hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
    int i=0;
    while (i==0)
    {
        SetConsoleTextAttribute(hConsole,14);
        printf("(a)BEGINNER mode\n");
        printf("(b)EXPERT   mode\n");
        SetConsoleTextAttribute(hConsole,15);
        fflush(stdin);
        gets(n);
        if(n[1] == '\0')
        {
            switch(n[0])
            {
            case 'a':
                *a=3;
                *p=12;
                *x=3,*y=2;
                break;
            case 'b':
                *a=6;
                *p=60;
                *x=6,*y=5;
                break;
            default:
                system ("cls");
                SetConsoleTextAttribute(hConsole,co2);
                printf("You have to enter a or b only :P \n");
                SetConsoleTextAttribute(hConsole,15);
                break;
            }
            if(n[0]=='a'||n[0]=='b')
            {
                i++;
            }
        }
        else
        {
            system ("cls");
            SetConsoleTextAttribute(hConsole,co2);
            printf("You have to enter a or b only :P \n");
            SetConsoleTextAttribute(hConsole,15);
        }
    }
}
void hVSh_hVScomp(char m[],char name[][31]) //takes mode either human/human or human/computer
{
    HANDLE hConsole;
    hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
    int i=0,j=0;
    system ("cls");
    while(i==0)
    {
        SetConsoleTextAttribute(hConsole,10);
        printf("(a)ONE player mode\n");
        printf("(b)TWO player mode\n");
        SetConsoleTextAttribute(hConsole,15);
        fflush(stdin);
        gets(m);
        if(m[1]=='\0')
        {
            switch(m[0])
            {
            case 'a':
                SetConsoleTextAttribute(hConsole,11);
                printf("Enter your name: ");
                SetConsoleTextAttribute(hConsole,15);
                fflush(stdin);
                fgets(name[0],20,stdin);
                for(j=0; j<strlen(name[0]); j++)
                {
                    if(name[0][j]=='\n')
                    {
                        name[0][j]='\0';
                    }
                }
                fflush(stdin);
                while(isspace(name[0][0]) || name[0][0]=='\0')
                {
                    SetConsoleTextAttribute(hConsole,14);
                    system ("cls");
                    printf("Name can't start with spaces !!\n");
                    SetConsoleTextAttribute(hConsole,15);
                    SetConsoleTextAttribute(hConsole,11);
                    printf("Enter your name: ");
                    SetConsoleTextAttribute(hConsole,15);
                    fflush(stdin);
                    fgets(name[0],20,stdin);
                    for(j=0; j<strlen(name[0]); j++)
                    {
                        if(name[0][j]=='\n')
                        {
                            name[0][j]='\0';
                        }
                    }
                    fflush(stdin);
                }
                break;
            case 'b':
                SetConsoleTextAttribute(hConsole,co1);
                printf("\nEnter the name of player 1: ");
                SetConsoleTextAttribute(hConsole,15);
                fflush(stdin);
                fgets(name[0],20,stdin);
                for(j=0; j<strlen(name[0]); j++)
                {
                    if(name[0][j]=='\n')
                    {
                        name[0][j]='\0';
                    }
                }
                fflush(stdin);
                while(isspace(name[0][0]) || name[0][0]=='\0')
                {
                    SetConsoleTextAttribute(hConsole,14);
                    system ("cls");
                    printf("Name can't start with spaces !!\n");
                    SetConsoleTextAttribute(hConsole,15);
                    SetConsoleTextAttribute(hConsole,co1);
                    printf("\nEnter the name of player 1: ");
                    SetConsoleTextAttribute(hConsole,15);
                    fflush(stdin);
                    fgets(name[0],20,stdin);
                    for(j=0; j<strlen(name[0]); j++)
                    {
                        if(name[0][j]=='\n')
                        {
                            name[0][j]='\0';
                        }
                    }
                    fflush(stdin);
                }
                SetConsoleTextAttribute(hConsole,co2);
                printf("\nEnter the name of player 2: ");
                SetConsoleTextAttribute(hConsole,15);
                fflush(stdin);
                fgets(name[1],20,stdin);
                for(j=0; j<strlen(name[1]); j++)
                {
                    if(name[1][j]=='\n')
                    {
                        name[1][j]='\0';
                    }
                }
                fflush(stdin);
                while(isspace(name[1][0]) || name[1][0]=='\0')
                {
                    SetConsoleTextAttribute(hConsole,14);
                    system ("cls");
                    printf("Name can't start with spaces !!\n");
                    SetConsoleTextAttribute(hConsole,15);
                    SetConsoleTextAttribute(hConsole,co2);
                    printf("\nEnter the name of player 2: ");
                    SetConsoleTextAttribute(hConsole,15);
                    fflush(stdin);
                    fgets(name[1],20,stdin);
                    for(j=0; j<strlen(name[1]); j++)
                    {
                        if(name[1][j]=='\n')
                        {
                            name[1][j]='\0';
                        }
                    }
                    fflush(stdin);
                }
                break;
            default:
                system ("cls");
                SetConsoleTextAttribute(hConsole,co2);
                printf("You have to enter a or b only :P \n");
                SetConsoleTextAttribute(hConsole,15);
                break;
            }
            if(m[0]=='a'||m[0]=='b')
            {
                i++;
            }
        }
        else
        {
            system ("cls");
            SetConsoleTextAttribute(hConsole,co2);
            printf("You have to enter a or b only :P \n");
            SetConsoleTextAttribute(hConsole,15);
        }
    }
}
void PrintScores_moves(int sc[],char name[2][31],char m[],int pc,int x,int y,int sq[y][y],int h[x][y],int v[y][x],int sc1[2])
{
    HANDLE hConsole;
    hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
    checkInputBox(y,sq,sc,sc1);
    checkBoxes(pc,x,y,sq,h,v);
    SetConsoleTextAttribute(hConsole,co1);
    printf("%s's score = %d        ",name[0],sc[0]);
    SetConsoleTextAttribute(hConsole,15);
    SetConsoleTextAttribute(hConsole,co2);
    if(m[0]=='a')
    {
        printf("  Computer's score = %d\n",sc[1]);
    }
    else
    {
        printf("  %s's score = %d\n",name[1],sc[1]);
    }
    SetConsoleTextAttribute(hConsole,15);


    playedLines(x,y,h,v);
    SetConsoleTextAttribute(hConsole,co1);
    printf("moves for %s = %d        ",name[0],pls[0]);
    SetConsoleTextAttribute(hConsole,15);
    SetConsoleTextAttribute(hConsole,co2);
    if(m[0]=='a')
    {
        printf("moves for Computer = %d\n",pls[1]);
    }
    else
    {
        printf("moves for %s = %d\n",name[1],pls[1]);
    }
    SetConsoleTextAttribute(hConsole,15);
}
void printRemainLines(int x,int y,int h[x][y],int v[y][x],int p,int *k)
{
    int i=0,j=0,re=0;
    HANDLE hConsole;
    hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
    for(i=0; i<x; i++)
    {
        for(j=0; j<y; j++)
        {
            if(h[i][j]==0)
            {
                re++;
            }
        }
    }
    for(i=0; i<y; i++)
    {
        for(j=0; j<x; j++)
        {
            if(v[i][j]==0)
            {
                re++;
            }
        }
    }
    *k=p-re;
    SetConsoleTextAttribute(hConsole,14);
    printf("No. of remaining lines = %d\n",re);
    SetConsoleTextAttribute(hConsole,15);

}
int leaderboard(char n[],char names[],int sc)
{
    int rank=0;
    count1=0,count2=0;
    int flag=0;
    int i=0;
    if(n[0]=='a')
    {
        //for 2*2 mode
        f=fopen("scores1.txt","r");
        if(f!=NULL)
        {
            while (!feof(f))
            {
                fscanf(f, "%[^,], %d",players1[count1].name,&players1[count1].score);
                fscanf(f, "\n");
                count1++;
            }
            if(count1==1&&players1[0].score==0)
            {
                //Handle if the leaderboard is empty
                count1--;
            }
            fclose(f);
        }
        else
        {
            f=fopen("scores1.txt","w");
            fclose(f);
            f=fopen("scores1.txt","r");
            while (!feof(f))
            {
                fscanf(f, "%[^,], %d",players1[count1].name,&players1[count1].score);
                fscanf(f, "\n");
                count1++;
            }
            if(count1==1&&players1[0].score==0)
            {
                //Handle if the leaderboard is empty
                count1--;
            }
            fclose(f);
        }
        for(i=0; i<count1; i++)
        {
            if(!strcmp(players1[i].name,names))
            {
                flag++;
                break;
            }
        }
        if(flag==1)
        {
            if(sc>=players1[i].score)
            {
                players1[i].score=sc;
                sort(players1);
                f=fopen("scores1.txt","w");
                for(i=0; i<count1; i++)
                {
                    fprintf(f, "%s, %d\n",players1[i].name,players1[i].score);
                    if(!strcmp(players1[i].name,names))
                    {
                        rank=i+1;
                    }
                }
                fclose(f);
            }
        }
        else if(count1<10)
        {
            strcpy(players1[count1].name,names);
            players1[count1].score=sc;
            sort(players1);
            f=fopen("scores1.txt","w");
            for(i=0; i<=count1; i++)
            {
                fprintf(f, "%s, %d\n",players1[i].name,players1[i].score);
                if(!strcmp(players1[i].name,names))
                {
                    rank=i+1;
                }
            }
            fclose(f);

        }
        else
        {
            if(sc>players1[9].score)
            {
                strcpy(players1[9].name,names);
                players1[9].score=sc;
                sort(players1);
                f=fopen("scores1.txt","w");
                for(i=0; i<=count1; i++)
                {
                    fprintf(f, "%s, %d\n",players1[i].name,players1[i].score);
                    if(!strcmp(players1[i].name,names))
                    {
                        rank=i+1;
                    }
                }
                fclose(f);
            }
        }
    }
    else
    {
        //for 5*5 mode
        f=fopen("scores2.txt","r");
        if(f!=NULL)
        {
            while (!feof(f))
            {
                fscanf(f, "%[^,], %d",players2[count2].name,&players2[count2].score);
                fscanf(f, "\n");
                count2++;
            }
            if(count2==1&&players2[0].score==0)
            {
                //Handle if the leaderboard is empty
                count2--;
            }
            fclose(f);
        }
        else
        {
            f=fopen("scores2.txt","w");
            fclose(f);
            f=fopen("scores2.txt","r");
            while (!feof(f))
            {
                fscanf(f, "%[^,], %d",players2[count2].name,&players2[count2].score);
                fscanf(f, "\n");
                count2++;
            }
            if(count2==1&&players2[0].score==0)
            {
                //Handle if the leaderboard is empty
                count2--;
            }
            fclose(f);
        }

        for(i=0; i<count2; i++)
        {
            if(!strcmp(players2[i].name,names))
            {
                flag++;
                break;
            }
        }
        if(flag==1)
        {
            if(sc>=players2[i].score)
            {
                players2[i].score=sc;
                sort(players2);
                f=fopen("scores2.txt","w");
                for(i=0; i<count2; i++)
                {
                    fprintf(f, "%s, %d\n",players2[i].name,players2[i].score);
                    if(!strcmp(players2[i].name,names))
                    {
                        rank=i+1;
                    }
                }
                fclose(f);
            }
        }
        else if(count2<10)
        {
            strcpy(players2[count2].name,names);
            players2[count2].score=sc;
            sort(players2);
            f=fopen("scores2.txt","w");
            for(i=0; i<=count2; i++)
            {
                fprintf(f, "%s, %d\n",players2[i].name,players2[i].score);
                if(!strcmp(players2[i].name,names))
                {
                    rank=i+1;
                }
            }
            fclose(f);

        }
        else
        {
            if(sc>players2[9].score)
            {
                strcpy(players2[9].name,names);
                players2[9].score=sc;
                sort(players2);
                f=fopen("scores2.txt","w");
                for(i=0; i<=count2; i++)
                {
                    fprintf(f, "%s, %d\n",players2[i].name,players2[i].score);
                    if(!strcmp(players2[i].name,names))
                    {
                        rank=i+1;
                    }
                }
                fclose(f);
            }
        }
    }
    return rank;
}
void printLeaderboard()
{
    HANDLE hConsole;
    hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
    int i=0;
    char n[5000];
    system("cls");
    while (i==0)
    {
        SetConsoleTextAttribute(hConsole,10);
        printf("(a)BEGINNER mode LeaderBoard\n");
        printf("(b)EXPERT   mode LeaderBoard\n");
        SetConsoleTextAttribute(hConsole,15);
        fflush(stdin);
        gets(n);
        if(n[1] == '\0')
        {
            system("cls");
            switch(n[0])
            {
            case 'a':
                count1=0;
                f=fopen("scores1.txt","r");
                if(f!=NULL)
                {
                    while (!feof(f))
                    {
                        fscanf(f, "%[^,], %d",players1[count1].name,&players1[count1].score);
                        fscanf(f, "\n");
                        count1++;
                    }
                    fclose(f);
                }
                else
                {
                    f=fopen("scores1.txt","w");
                    fclose(f);
                    f=fopen("scores1.txt","r");
                    while (!feof(f))
                    {
                        fscanf(f, "%[^,], %d",players1[count1].name,&players1[count1].score);
                        fscanf(f, "\n");
                        count1++;
                    }
                    fclose(f);
                }
                if(players1[0].score==0)
                {
                    system("cls");
                    SetConsoleTextAttribute(hConsole,12);
                    printf("The LeaderBoard is empty\n");
                    SetConsoleTextAttribute(hConsole,15);
                }
                else
                {
                    SetConsoleTextAttribute(hConsole,13);
                    printf("Rank                 Name(Score)\n");
                    SetConsoleTextAttribute(hConsole,15);
                    for(i=0; i<count1; i++)
                    {
                        SetConsoleTextAttribute(hConsole,12);
                        printf(" %d                    %s(%d)\n",i+1,players1[i].name,players1[i].score);
                        SetConsoleTextAttribute(hConsole,15);
                    }
                }
                break;
            case 'b':
                count2=0;
                f=fopen("scores2.txt","r");
                if(f!=NULL)
                {
                    while (!feof(f))
                    {
                        fscanf(f, "%[^,], %d",players2[count2].name,&players2[count2].score);
                        fscanf(f, "\n");
                        count2++;
                    }
                    fclose(f);
                }
                else
                {
                    f=fopen("scores2.txt","w");
                    fclose(f);
                    f=fopen("scores2.txt","r");
                    while (!feof(f))
                    {
                        fscanf(f, "%[^,], %d",players2[count2].name,&players2[count2].score);
                        fscanf(f, "\n");
                        count2++;
                    }
                    fclose(f);
                }
                if(players2[0].score==0)
                {
                    system("cls");
                    SetConsoleTextAttribute(hConsole,12);
                    printf("The LeaderBoard is empty\n");
                    SetConsoleTextAttribute(hConsole,15);
                }
                else
                {
                    SetConsoleTextAttribute(hConsole,13);
                    printf("Rank                 Name(Score)\n");
                    SetConsoleTextAttribute(hConsole,15);
                    for(i=0; i<count2; i++)
                    {

                        SetConsoleTextAttribute(hConsole,12);
                        printf(" %d                    %s(%d)\n",i+1,players2[i].name,players2[i].score);
                        SetConsoleTextAttribute(hConsole,15);
                    }
                }
                break;
            default:
                system ("cls");
                SetConsoleTextAttribute(hConsole,12);
                printf("You have to choose a or b only \n");
                SetConsoleTextAttribute(hConsole,15);
                break;
            }
            if(n[0]=='a'||n[0]=='b')
            {
                i++;
            }
        }
        else
        {
            system ("cls");
            SetConsoleTextAttribute(hConsole,12);
            printf("You have to choose a or b only \n");
            SetConsoleTextAttribute(hConsole,15);
        }
    }
}
void winner(int sc[],char m[],char name[][31],char n[])/*********************************************************/
{
    int rank=0;
    HANDLE hConsole;
    hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
    if(sc[0]>sc[1])
    {
        SetConsoleTextAttribute(hConsole,11);
        printf("\n\a       THE WINNER IS %s\n",name[0]);
        SetConsoleTextAttribute(hConsole,15);
        rank=leaderboard(n,name[0],sc[0]);
        if(rank>0)
        {
            SetConsoleTextAttribute(hConsole,10);
            printf("       Your Rank is %d\n",rank);
            SetConsoleTextAttribute(hConsole,15);
        }
        else
        {
            SetConsoleTextAttribute(hConsole,12);
            printf("You are out of Rank\n");
            SetConsoleTextAttribute(hConsole,15);
        }
    }
    else if(sc[0]<sc[1])
    {
        if(m[0]=='a')
        {
            SetConsoleTextAttribute(hConsole,11);
            printf("\n\a       THE WINNER IS THE COMPUTER");
            SetConsoleTextAttribute(hConsole,15);
            SetConsoleTextAttribute(hConsole,co2);
            printf("\n              GAME OVER :p\n");
            SetConsoleTextAttribute(hConsole,15);
        }
        else
        {
            SetConsoleTextAttribute(hConsole,11);
            printf("\n\a       THE WINNER IS %s\n",name[1]);
            SetConsoleTextAttribute(hConsole,15);
            rank=leaderboard(n,name[1],sc[1]);
            if(rank>0)
            {
                SetConsoleTextAttribute(hConsole,10);
                printf("       Your Rank is %d\n",rank);
                SetConsoleTextAttribute(hConsole,15);
            }
            else
            {
                SetConsoleTextAttribute(hConsole,12);
                printf("You are out of Rank\n");
                SetConsoleTextAttribute(hConsole,15);
            }
        }
    }
    else
    {
        SetConsoleTextAttribute(hConsole,11);
        printf("\n             DRAW :)\n");
        SetConsoleTextAttribute(hConsole,15);
    }
}
int afterEnd()
{
    HANDLE hConsole;
    hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
    int i=0;
    char c[5000];
    SetConsoleTextAttribute(hConsole,8);
    printf("(a)GO back to main menu\n");
    printf("(b)LeaderBoard\n");
    printf("(c)Exit\n");
    SetConsoleTextAttribute(hConsole,15);
    while(i==0)
    {
        fflush(stdin);
        gets(c);
        if(c[1]=='\0')
        {
            switch(c[0])
            {
            case 'a':
                system ("cls");
                MainMenu();
                i++;
                break;
            case 'b':
                i++;
                printLeaderboard();
                break;
            case 'c':
                exit(0);
                i++;
                break;
            default:
                system("cls");
                SetConsoleTextAttribute(hConsole,12);
                printf("You have to choose from the menu !!\n");
                SetConsoleTextAttribute(hConsole,15);
                SetConsoleTextAttribute(hConsole,8);
                printf("(a)GO back to main menu\n");
                printf("(b)LeaderBoard\n");
                printf("(c)Exit\n");
                SetConsoleTextAttribute(hConsole,15);
                break;
            }
        }
        else
        {
            system("cls");
            SetConsoleTextAttribute(hConsole,12);
            printf("You have to choose from the menu !!\n");
            SetConsoleTextAttribute(hConsole,15);
            SetConsoleTextAttribute(hConsole,8);
            printf("(a)GO back to main menu\n");
            printf("(b)LeaderBoard\n");
            printf("(c)Exit\n");
            SetConsoleTextAttribute(hConsole,15);
        }
    }
    return 0;
}
void MainMenu()
{
    char t[5000];//type of line
    t[0]='\0';
    char m[5000],n[5000];//main menu variables to choose modes
    int p;//Counters
    int pc=0;// player counter which gives turns
    int a; //size of game
    char name[2][31];
    int sc[2],sc1[2]; //scores
    sc[0]=sc[1]=sc1[0]=sc1[1]=0;
    int x,y;//size of h & v arrays + size of squares array
    HANDLE hConsole;
    hConsole=GetStdHandle(STD_OUTPUT_HANDLE);
    int u=0;
    char c[5000];
    SetConsoleTextAttribute(hConsole,2);
    printf("(a)Start a new game\n");
    printf("(b)Load a game\n");
    printf("(c)Help ?? \n");
    printf("(d)LeaderBoard\n");
    printf("(e)Exit\n");
    SetConsoleTextAttribute(hConsole,15);
    while(u==0)
    {
        fflush(stdin);
        gets(c);
        if(c[1]=='\0')
        {
            if(c[0]=='a')
            {
                u++;
                system ("cls");
                BegOrExp(n,&a,&p,&x,&y);
                hVSh_hVScomp(m,name);
                int h[x][y],v[y][x],sq[y][y];//2 arrays of vertical and horizontal lines + array for squares
                initialize2Darray(y,y,sq);// initialize by zeroes
                initialize2Darray(x,y,h);
                initialize2Darray(y,x,v);
                SetConsoleTextAttribute(hConsole,13);
                printf("\nPress any key to begin the game :D \n");
                SetConsoleTextAttribute(hConsole,15);
                fflush(stdin);
                getch();
                system ("cls");
                gameRun(a,x,y,h,v,sq,pc,sc1,m,name,n,p,sc,t);
                if(t[0]=='s'||t[0]=='e')
                {
                    //save game progress and close
                    system("cls");
                    afterEnd();
                }
                else
                {
                    system ("cls");
                    PrintScores_moves(sc,name,m,pc,x,y,sq,h,v,sc1);
                    winner(sc,m,name,n);
                    printing(a,x,y,h,v,sq,pc,sc,sc1);
                }
            }
            else if(c[0]=='b')
            {
                load(n,m,name,&pc);
                u++;
            }
            else if(c[0]=='c')
            {
                u++;
                help();
            }
            else if(c[0]=='d')
            {
                u++;
                printLeaderboard(n);
            }
            else if(c[0]=='e')
            {
                u++;
                exit(0);
                return;
            }
            else
            {
                system ("cls");
                SetConsoleTextAttribute(hConsole,2);
                printf("(a)Start a new game\n");
                printf("(b)Load a game\n");
                printf("(c)Help ?? \n");
                printf("(d)Top 20 scores xD\n");
                printf("(e)Exit\n");
                SetConsoleTextAttribute(hConsole,12);
                printf("You have to choose from the main menu !!\n");
                SetConsoleTextAttribute(hConsole,15);
            }
        }
        else
        {
            system ("cls");
            SetConsoleTextAttribute(hConsole,2);
            printf("(a)Start a new game\n");
            printf("(b)Load a game\n");
            printf("(c)Help ?? \n");
            printf("(d)Top 20 scores xD\n");
            printf("(e)Exit\n");
            SetConsoleTextAttribute(hConsole,12);
            printf("You have to choose from the main menu !!\n");
            SetConsoleTextAttribute(hConsole,15);
        }
    }
}
