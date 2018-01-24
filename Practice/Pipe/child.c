#include <stdio.h>
#include <windows.h>

#define BUFFER_SIZE 25

int main() {
    HANDLE ReadHandle;
    char buffer[BUFFER_SIZE];
    DWORD read;

    /* get the handle of the pipe */
    ReadHandle = GetStdHandle(STD_INPUT_HANDLE);
    if(ReadFile(ReadHandle, buffer, BUFFER_SIZE, &read, NULL))
        printf("Child read: %s\n", buffer);
    else
        fprintf(stderr, "Error reading from pipe");
    return 0;
}
