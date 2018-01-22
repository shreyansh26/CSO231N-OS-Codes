#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>
#include <regex.h>
 
#define MAXCOM 1000
#define MAXLIST 100
#define clear() printf("\033[H\033[J")

char* history[1000];
int cnt = 0, i;
regex_t regex;
int reti;

void check_regex() {
    reti = regcomp(&regex, "![0-9]+", REG_EXTENDED);
    if (reti != 0) {
        fprintf(stderr, "Could not compile regex\n");
        exit(1);
    }
}

void init_shell() {
    clear();
    printf("\tShreyansh's Shell");
    char* username = getenv("USER");
    printf("\n\n\nWelcome: @%s", username);
    printf("\n");
    sleep(1);
    clear();
}

int takeInput(char* str) {
    char* buf;
 
    buf = readline("\n$ ");
    if (strlen(buf) != 0) {
        add_history(buf);
        strcpy(str, buf);
        history[cnt] = buf;
        cnt++;
        return 0;
    } 
    else {
        return 1;
    }
}

void printDir() {
    char cwd[1024];
    char *user;
    getcwd(cwd, sizeof(cwd));
    user = getenv("USER");
    printf("\n%s: %s", user, cwd);
}

void execArgs(char** parsed) {
    pid_t pid = fork(); 
 
    if (pid == -1) {
        printf("\nFailed forking child..");
        return;
    } 
    else if (pid == 0) {
        if (execvp(parsed[0], parsed) < 0) {
            printf("Could not execute command..\n");
        }
        exit(0);
    } 
    else {
        wait(NULL); 
        return;
    }
}

void execArgsPiped(char** parsed, char** parsedpipe) {
    int pipefd[2]; 
    pid_t p1, p2;
 
    if (pipe(pipefd) < 0) {
        printf("\nPipe could not be initialized");
        return;
    }
    p1 = fork();
    if (p1 < 0) {
        printf("\nCould not fork");
        return;
    }
 
    if (p1 == 0) {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
 
        if (execvp(parsed[0], parsed) < 0) {
            printf("\nCould not execute command 1..");
            exit(0);
        }
    } 
    else {
        p2 = fork();
 
        if (p2 < 0) {
            printf("\nCould not fork");
            return;
        }
 
        if (p2 == 0) {
            close(pipefd[1]);
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);
            if (execvp(parsedpipe[0], parsedpipe) < 0) {
                printf("\nCould not execute command 2..");
                exit(0);
            }
        } 
        else {
            wait(NULL);
            wait(NULL);
        }
    }
}
 
void openHelp() {
    puts("\n\tHELP"
        "\n>cd"
        "\n>ls"
        "\n>exit"
        "\n>history"
        "\n>clhistory"
        "\n>general UNIX commands"
        "\n>pipe handling");
 
    return;
}
 
int parsePipe(char* str, char** strpiped) {
    int i;
    for (i = 0; i < 2; i++) {
        strpiped[i] = strsep(&str, "|");
        if (strpiped[i] == NULL)
            break;
    }
 
    if (strpiped[1] == NULL)
        return 0; 
    else {
        return 1;
    }
}

void parseSpace(char* str, char** parsed) {
    int i;
 
    for (i = 0; i < MAXLIST; i++) {
        parsed[i] = strsep(&str, " ");
 
        if (parsed[i] == NULL)
            break;
        if (strlen(parsed[i]) == 0)
            i--;
    }
}

int processString(char* str, char** parsed, char** parsedpipe);

int ownCmdHandler(char** parsed) {
    int builtinCmds = 6, i, switchArg = 0, execFlag = 0;
    char* ListOfBuiltinCmds[builtinCmds];
    char* username;
    char* parsedArgsPped[MAXLIST];
    char *parsedAgs[MAXLIST];
 
    ListOfBuiltinCmds[0] = "exit";
    ListOfBuiltinCmds[1] = "cd";
    ListOfBuiltinCmds[2] = "help";
    ListOfBuiltinCmds[3] = "hello";
    ListOfBuiltinCmds[4] = "history";
    ListOfBuiltinCmds[5] = "clhistory";

    for (i = 0; i < builtinCmds; i++) {
        if (strcmp(parsed[0], ListOfBuiltinCmds[i]) == 0) {
            switchArg = i + 1;
            break;
        }
    }
    int index = 1;
    char *pEnd;
    if(switchArg == 0) {
        char newCmd[10];
        int l;
        for(l=0; l<strlen(parsed[0]); l++) {
            newCmd[l] = parsed[0][l];
        }
        char num[30];

        reti = regexec(&regex, parsed[0], 0, NULL, 0);
        if (!reti) {
            switchArg = 7;
            strncpy(num, newCmd+1, strlen(parsed[0])-1);
            num[strlen(parsed[0])-1] = '\0';
            index = (int)strtol(num, &pEnd, 10);
        }
        else if(strlen(parsed[0]) == 2 && parsed[0][1] == '!') {
            switchArg = 7;
            index = 1;
        }
    }
    char *user;
    char *comd;
    switch (switchArg) {
    case 1:
        user = getenv("USER");
        printf("Goodbye @%s\n", user);
        exit(0);
    case 2:
        chdir(parsed[1]);
        return 1;
    case 3:
        openHelp();
        return 1;
    case 4:
        username = getenv("USER");
        printf("\nHello %s.\nThis shell was made by Shreyansh\n",
            username);
        return 1;
    case 5:
        for(i=0; i<cnt-1; i++){
            printf("%d. %s\n", cnt-i-1, history[i]);
        }
        return 1;
    case 6:
        for(i=0; i<cnt; i++){
            history[i] = NULL;
        }
        cnt = 0;
        return 1;
    case 7:
        if(cnt-index-1 < 0) {
            printf("Invalid command\n");
            return 1;
        }
        else if(history[cnt-index-1][0] == '!') {
            printf("%s\n", history[cnt-index-1]);
            printf("Addressed command sholudn't iteslf be a '!' prefixed command.\n");
            return 1;
        }
        comd = history[cnt-index-1];
        printf("%s\n", comd);
        execFlag = processString(comd,
        parsedAgs, parsedArgsPped);

        if (execFlag == 1)
            execArgs(parsedAgs);
 
        if (execFlag == 2)
            execArgsPiped(parsedAgs, parsedArgsPped);
        return 1;
    default:
        break;
    }
 
    return 0;
}

int main() {
    check_regex();
    char inputString[MAXCOM], *parsedArgs[MAXLIST];
    char* parsedArgsPiped[MAXLIST];
    int execFlag = 0;
    init_shell();
 
    while (1) {
        printDir();

        if (takeInput(inputString))
            continue;

        execFlag = processString(inputString,
        parsedArgs, parsedArgsPiped);

        if (execFlag == 1)
            execArgs(parsedArgs);
 
        if (execFlag == 2)
            execArgsPiped(parsedArgs, parsedArgsPiped);
    }
    return 0;
}

int processString(char* str, char** parsed, char** parsedpipe) {
 
    char* strpiped[2];
    int piped = 0;
 
    piped = parsePipe(str, strpiped);
 
    if (piped) {
        parseSpace(strpiped[0], parsed);
        parseSpace(strpiped[1], parsedpipe);
    }
    else {
        parseSpace(str, parsed);
    }
 
    if (ownCmdHandler(parsed))
        return 0;
    else
        return 1 + piped;
}