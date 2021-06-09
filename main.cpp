#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <signal.h>
#include <conio.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <dirent.h>
#include <process.h>
#include <dos.h>
#define GetCurrentDir getcwd
#define GetCurrentDir getcwd
using namespace std;

char command_buf[500];

char key;
STARTUPINFO si[100];
PROCESS_INFORMATION  pi[100];
int n, background_mode = 1;

void kill_all(){
    for (int i = 1;i <= n; i++){
        TerminateProcess(pi[i].hProcess, 1);CloseHandle(pi[i].hProcess);CloseHandle(pi[i].hThread);
    }
    printf("All process killed ... \n");
    n = 0;
}
void kill(int id){
    for (int i = 1;i <= n; i++){
        if (id == pi[i].dwProcessId){
            TerminateProcess(pi[i].hProcess, 1);CloseHandle(pi[i].hProcess);CloseHandle(pi[i].hThread);
            for (int j = i; j < n; j++) pi[j] = pi[j + 1], si[j] = si[j + 1];
            n--;
            break;
        }
    }
}
void child(){
    ++n;
    ZeroMemory(&si[n], sizeof(si[n]));
    si[n].cb = sizeof(si[n]);
    CreateProcess("child.exe",NULL,NULL,NULL,FALSE,CREATE_NEW_CONSOLE,NULL,NULL,&si[n],&pi[n]);
    if (background_mode == 1) return;
    WaitForSingleObject(pi[n].hProcess,10000);
}
void list(){
    for (int i = 1;i <= n; i++)
        printf("%d %p %s\n", pi[i].dwProcessId, pi[i].hProcess, si[i].wShowWindow);
}
void notepad(){
    ++n;
    ZeroMemory(&si[n], sizeof(si[n]));
    si[n].cb = sizeof(si[n]);
    CreateProcess("C:\\Windows\\System32\\notepad.exe",NULL,NULL,NULL,FALSE,CREATE_NEW_CONSOLE,NULL,NULL,&si[n],&pi[n]);
    if (background_mode == 1) return;
    WaitForSingleObject(pi[n].hProcess,10000);
}
void exit_process(){
    printf("Sending signal to kill all child processes ...\n");
    kill_all();
}
void sigint(int a){   
    printf("Killing foreground process ... \n");
    for (int i = 1;i <= n; i++){
        TerminateProcess(pi[i].hProcess, 1);CloseHandle(pi[i].hProcess);CloseHandle(pi[i].hThread);
    }
    n = 0;
}

void help() {
    // Display all commands
    printf("KILL -1    	Kill or stop all running processes\n");
    printf("KILL 'ID'   	Kill or stop a running process\n");
    printf("DATE    	Displays or sets the date\n");
    printf("NOTEPAD    	Open system notepad\n");
    printf("CALC    	Open system calculator\n");
    printf("LIST    	Displays list of commands\n");
    printf("HELP    	Provides Help information for Windows commands\n");
    printf("CLS 		Clear tiny shell\n");
    printf("CD		Change current directory\n");
    printf("BAT		Running a batch file\n");
}

std::string get_current_dir() {
	char buff[FILENAME_MAX]; //create string buffer to hold path
	GetCurrentDir( buff, FILENAME_MAX);
	string current_working_dir(buff);
	return current_working_dir;
}

int main(){
    
	while(1) 
	{
        //if (!fgets(command_buf, 128, stdin)) break;
        cout << get_current_dir() << ">";
        scanf("%s", &command_buf);

	if(!strcmp(command_buf, "ifconfig")) { 
		system("ifconfig"); 
	}
	else if(!strcmp(command_buf, "date")) { 
		system("date"); 
	}
	else if(!strcmp(command_buf, "exit") || !strcmp(command_buf, "quit") || !strcmp(command_buf, "e") || !strcmp(command_buf, "q")) {
            exit_process();
        return 0;
        }
		else if(!strcmp(command_buf, "child")) {
            child();
        }
        else if(!strcmp(command_buf, "notepad")) {
			notepad();
        }
        else if (!strcmp(command_buf, "kill")){
            int id;
            scanf("%d", &id);
            if (id == -1) kill_all();
            else kill(id);
        }
        else if (!strcmp(command_buf, "background")){
            scanf("%s", command_buf);
            if (!strcmp(command_buf, "on")) background_mode = 1;
            if (!strcmp(command_buf, "off")) background_mode = 0;
        }
        else if (!strcmp(command_buf, "list")){
            list();
        }
        else if (!strcmp(command_buf, "sh")){
            scanf("%s", &command_buf);
            system(command_buf);
        }
        else if(!strcmp(command_buf, "help")) {
            help();
        }
        else if(!strcmp(command_buf, "dir")) {
            system("dir");
        }
        else if(!strcmp(command_buf, "cd..")) {
            chdir("..");
	}
        else if(!strcmp(command_buf, "cd")) {
            chdir(command_buf);
	    scanf("%s", &command_buf);
	    if(chdir(command_buf)) 
		    printf("No such file or directory\n");
        }
        else if(!strcmp(command_buf, "clear")) {
            system("cls");
        }
        else if(!strcmp(command_buf, "bat")) {
			scanf("%s", &command_buf);
			char command[] = "start ";
			strcat(command, command_buf);
			system(command);
			system("taskkill /F /IM cmd.exe >NUL 2> 1");
		}
	}
        signal(SIGINT, sigint);
}
