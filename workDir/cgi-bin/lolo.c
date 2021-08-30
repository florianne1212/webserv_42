#include <unistd.h>
#include <stdio.h>

int main(int argc, char **argv, char**envp) {
        printf("Content-Type: text/html\r\n");
        printf("\r\n");

        for (int i = 0; i < argc; i++)
                printf("argv[%d] = \"%s\" <br />\n", i, argv[i]);

        printf("<br />\n");

        for (int i = 0; envp[i]; i++)
                printf("envp[%d] = \"%s\" <br />\n", i, envp[i]);
}