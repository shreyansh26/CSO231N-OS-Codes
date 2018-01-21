#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void spawnNodes(int curLevel, int levelLimit, int childrenNumber, int nodeNumber, int offset) {
    if (curLevel == levelLimit)
        exit(0);

    curLevel++;
    printf("(%d, %d) Pid: %d with parent %d\n", curLevel, nodeNumber, getpid(), getppid());

    for (int i = 0; i < childrenNumber; i++) {
        pid_t childPid = fork();
        if (childPid == -1) {
            perror("Couldn't create process");
            exit(1);
        }

        if (childPid == 0) {
            spawnNodes(curLevel, levelLimit, childrenNumber, offset + i, 
                       offset + i);
        }
        else {
            wait(NULL);
        }
    }
}

int main() {
    int levelLimit, children;
    scanf("%d %d", &levelLimit, &children);

    spawnNodes(0, levelLimit, children, 0, 0);

    return 0;
}