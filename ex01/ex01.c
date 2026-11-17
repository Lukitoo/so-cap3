/*
 * Problema 1
 * Realizati un interpretor de comenzi propriu:
 *   - recunoaste comenzile DIR, TIME, DATE, COMANDA, ECOU;
 *   - DIR, TIME, DATE sunt transmise interpretorului sistemului (cmd);
 *   - COMANDA lanseaza interpretorul sistemului pana se tasteaza EXIT;
 *   - ECOU afiseaza toate argumentele primite;
 *   - orice alta comanda e lansata ca program (spawnvp: directorul
 *     curent, apoi PATH); daca nu reuseste se afiseaza "Comanda incorecta";
 *   - separatorul dintre cuvinte este doar spatiul.
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
    char linie[1024], copie[1024];
    char* arg[100], * p;
    int n, i;

    while (1)
    {
        printf("Comanda: ");

        if (fgets(linie, sizeof(linie), stdin) == NULL)
            break;

        linie[strcspn(linie, "\n")] = '\0';
        strcpy(copie, linie);

        n = 0;
        p = strtok(linie, " ");

        while (p != NULL && n < 99)
        {
            arg[n] = p;
            n++;
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
            _spawnlp(_P_WAIT, "cmd.exe",
                "cmd.exe", "/C", copie, NULL);
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
            if (_spawnvp(_P_WAIT, arg[0],
                (const char* const*)arg) == -1)
            {
                printf("Comanda incorecta\n");
            }
        }
    }
    return 0;
}
