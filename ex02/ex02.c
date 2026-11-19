
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
    char linie[1024], cmd[1024];
    char *arg[100], *p;
    char *intrare, *iesire;
    int n, i, f, vechi_in, vechi_out;

    while (1)
    {
        printf("Comanda: ");
        fflush(stdout);

        if (fgets(linie, 1024, stdin) == NULL)
            break;

        linie[strcspn(linie, "\n")] = 0;

        n = 0;
        intrare = NULL;
        iesire = NULL;
        p = strtok(linie, " ");

        while (p != NULL && n < 99)
        {
            if (strcmp(p, "<") == 0)
                intrare = strtok(NULL, " ");
            else if (strcmp(p, ">") == 0)
                iesire = strtok(NULL, " ");
            else
                arg[n++] = p;

            p = strtok(NULL, " ");
        }

        if (n == 0)
            continue;

        arg[n] = NULL;

        if (strcmp(arg[0], "EXIT") == 0)
            break;

        vechi_in = _dup(0);
        vechi_out = _dup(1);

        if (intrare != NULL)
        {
            f = _open(intrare, _O_RDONLY);

            if (f == -1)
            {
                printf("Fisier inexistent\n");
                _close(vechi_in);
                _close(vechi_out);
                continue;
            }

            _dup2(f, 0);
            _close(f);
        }

        if (iesire != NULL)
        {
            f = _open(iesire, _O_WRONLY | _O_CREAT | _O_TRUNC,
                      _S_IREAD | _S_IWRITE);

            if (f == -1)
            {
                printf("Eroare fisier\n");
                _dup2(vechi_in, 0);
                _close(vechi_in);
                _close(vechi_out);
                continue;
            }

            _dup2(f, 1);
            _close(f);
        }

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
                _write(1, arg[i], (unsigned)strlen(arg[i]));

                if (i < n - 1)
                    _write(1, " ", 1);
            }

            _write(1, "\n", 1);
        }
        else
        {
            f = _spawnvp(_P_WAIT, arg[0],
                         (const char * const *)arg);

            if (f == -1)
                printf("Comanda incorecta\n");
        }

        fflush(stdout);

        _dup2(vechi_in, 0);
        _dup2(vechi_out, 1);

        _close(vechi_in);
        _close(vechi_out);
    }

    return 0;
}
