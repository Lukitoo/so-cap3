
/*
 * Problema 1
 * Realizati un interpretor de comenzi care recunoaste
 * DIR, TIME, DATE, COMANDA si ECOU.
 * Celelalte comenzi sunt executate cu spawnvp.
 *
 * Utilizare: ex01
 */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <process.h>

int main(void)
{
    char linie[1024], cmd[1024];
    char *arg[100], *p;
    int n, i, f;

    while (1)
    {
        printf("Comanda: ");

        if (fgets(linie, sizeof(linie), stdin) == NULL)
            break;

        linie[strcspn(linie, "\n")] = 0;
        n = 0;

        p = strtok(linie, " ");

        while (p != NULL && n < 99)
        {
            arg[n++] = p;
            p = strtok(NULL, " ");
        }

        if (n == 0)
            continue;

        arg[n] = NULL;

        if (strcmp(arg[0], "EXIT") == 0)
            break;

        if (strcmp(arg[0], "DIR") == 0 ||
            strcmp(arg[0], "TIME") == 0 ||
            strcmp(arg[0], "DATE") == 0)
        {
            strcpy(cmd, arg[0]);

            for (i = 1; i < n; i++)
            {
                strcat(cmd, " ");
                strcat(cmd, arg[i]);
            }

            _spawnlp(_P_WAIT, "cmd.exe",
                "cmd.exe", "/C", cmd, NULL);
        }
        else if (strcmp(arg[0], "COMANDA") == 0)
        {
            _spawnlp(_P_WAIT, "cmd.exe",
                "cmd.exe", NULL);
        }
        else if (strcmp(arg[0], "ECOU") == 0)
        {
            for (i = 1; i < n; i++)
            {
                printf("%s", arg[i]);

                if (i < n - 1)
                    printf(" ");
            }

            printf("\n");
        }
        else
        {
            f = _spawnvp(_P_WAIT, arg[0],
                (const char * const *)arg);

            if (f == -1)
                printf("Comanda incorecta\n");
        }
    }

    return 0;
}
