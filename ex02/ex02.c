/*
 * Problema 2
 * Imbunatatiti interpretorul de la problema 1 astfel incat sa recunoasca
 * < si > ca redirectari ale intrarii si iesirii, realizand efectiv
 * redirectarile.
 *   Exemple:  ECOU salut > a.txt
 *             sort < nume.txt
 *
 * Utilizare: ex02
 */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <process.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(void)
{
    
char linie[1024], comanda[1024];
char* arg[100], * p;
char* intrare, * iesire;
int n, i, f, fdin, fdout;
int stdin_vechi, stdout_vechi;

while (1)
{
    printf("Comanda: ");

    if (fgets(linie, sizeof(linie), stdin) == NULL)
        break;

    linie[strcspn(linie, "\n")] = '\0';
    strcpy(comanda, linie);

    n = 0;
    intrare = NULL;
    iesire = NULL;

    p = strtok(linie, " ");

    while (p != NULL && n < 99)
    {
        if (strcmp(p, "<") == 0)
        {
            p = strtok(NULL, " ");
            intrare = p;
        }
        else if (strcmp(p, ">") == 0)
        {
            p = strtok(NULL, " ");
            iesire = p;
        }
        else
        {
            arg[n] = p;
            n++;
        }

        p = strtok(NULL, " ");
    }

    if (n == 0)
        continue;

    arg[n] = NULL;

    if (strcmp(arg[0], "EXIT") == 0)
        break;

    stdin_vechi = _dup(0);
    stdout_vechi = _dup(1);

    if (intrare != NULL)
    {
        fdin = _open(intrare, _O_RDONLY);

        if (fdin == -1)
        {
            printf("Nu se poate deschide fisierul\n");
            _close(stdin_vechi);
            _close(stdout_vechi);
            continue;
        }

        _dup2(fdin, 0);
        _close(fdin);
    }

    if (iesire != NULL)
    {
        fdout = _open(iesire,
            _O_WRONLY | _O_CREAT | _O_TRUNC,
            _S_IREAD | _S_IWRITE);

        if (fdout == -1)
        {
            printf("Nu se poate crea fisierul\n");
            _dup2(stdin_vechi, 0);
            _close(stdin_vechi);
            _close(stdout_vechi);
            continue;
        }

        fflush(stdout);
        _dup2(fdout, 1);
        _close(fdout);
    }

    if (strcmp(arg[0], "DIR") == 0 ||
        strcmp(arg[0], "TIME") == 0 ||
        strcmp(arg[0], "DATE") == 0)
    {
        char cmd[1024] = "";

        for (i = 0; i < n; i++)
        {
            if (i > 0)
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
            (const char* const*)arg);

        if (f == -1)
            printf("Comanda incorecta\n");
    }

    fflush(stdout);

    _dup2(stdin_vechi, 0);
    _dup2(stdout_vechi, 1);

    _close(stdin_vechi);
    _close(stdout_vechi);
}


    return 0;
}
