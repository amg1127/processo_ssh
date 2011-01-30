#define BUFFLEN 1024

#include <stdio.h>
#include <stdlib.h>
#include <pty.h>
#include <unistd.h>
#include <string.h>

int main () {
    FILE* fd;
    char buffer[BUFFLEN];
    pid_t proc;
    size_t leu;
    int master;
    printf ("Content-Type: text/plain; charset=utf-8\r\n\r\n");
    printf ("Abrindo janela de terminal...\n");
    proc = forkpty (&master, NULL, NULL, NULL);
    if (proc == -1) {
        perror ("forkpty");
        return (1);
    } else if (proc == 0) {
        // Processo filho
        putenv("TERM=xterm");
        execlp ("ssh", "ssh", "-q", "-q", "-q", "usuario@www.servidor.com.br", NULL);
        // Nao deveria retornar...
        perror ("execlp");
        return (1);
    } else {
        // Processo pai.
        fd = fdopen (master, "a+");
        if (fd == NULL) {
            perror ("fdopen");
            return (1);
        }
        sleep (10);
        strcpy (buffer, "<SENHA DO USUARIO NO SERVIDOR>\n");
        printf ("Digitou a senha...\n");
        leu = fwrite ((void*) buffer, sizeof (char), strlen (buffer), fd);
        if (leu == 0) {
            perror ("fwrite");
            return (1);
        }
        fflush (fd);
        sleep (3);
        strcpy (buffer, "iptables -v -I INPUT 1 -s www.cliente.net.br -p tcp --dport 22 -j ACCEPT\necho '**** HAHAHA! Deu certo! ****'\nexit 0\n");
        leu = fwrite ((void*) buffer, sizeof (char), strlen (buffer), fd);
        if (leu == 0) {
            perror ("fwrite");
            return (1);
        }
        fflush (fd);
        while ((master = fgetc (fd)) != EOF) {
            printf ("%c", master);
        }
        return (0);
    }
}
