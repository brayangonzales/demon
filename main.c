#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
void main(){
	            char * const CP_G[] = {"g++", "/home/brayan/laravel/demon/Main.cpp", "-o", "/home/brayan/laravel/demon/Min", "-fno-asm", "-Wall", "-lm", "--static", "-std=c++0x", "-DONLINE_JUDGE", NULL };
            execvp(CP_G[0],CP_G);

}