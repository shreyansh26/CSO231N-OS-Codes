#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>
 
#define MAXCOM 1000
#define MAXLIST 100
#define clear() printf("\033[H\033[J")

char* history[1000];
int cnt = 0, i;

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
            printf("\nCould not execute command..");
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
 
int ownCmdHandler(char** parsed) {
    int builtinCmds = 6, i, switchArg = 0;
    char* ListOfBuiltinCmds[builtinCmds];
    char* username;
 
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
    char *user;
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
        for(i=0; i<cnt; i++){
            printf("%d. %s\n", i+1, history[i]);
        }
        return 1;
    case 6:
        for(i=0; i<cnt; i++){
            history[i] = NULL;
        }
        cnt = 0;
        return 1;
    default:
        break;
    }
 
    return 0;
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
 
int main() {
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