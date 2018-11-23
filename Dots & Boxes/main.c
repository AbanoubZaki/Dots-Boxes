#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <windows.h>
#include <ctype.h>
#include "functions&globals.h"

int main()
{
    welcome();
    MainMenu();
    while(1)
    {
        afterEnd();
    }
    return 0;
}
