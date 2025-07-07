/**
 * Simple shell interface program.
 *
 * Operating System Concepts - Tenth Edition
 * Copyright John Wiley & Sons - 2018
 */

#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>  // for open
#include <unistd.h> // for close

#define MAX_LINE 80 /* 80 chars per line, per comm */

struct command
{
    char *args[MAX_LINE / 2 + 1];
    char *infile;
    char *outfile;
    bool concurrent;
    struct command *next;
};

void comm_init(struct command *comm)
{
    for (int i = 0; i < MAX_LINE / 2 + 1; i++)
        comm->args[i] = NULL;
    comm->infile = NULL;
    comm->outfile = NULL;
    comm->concurrent = false;
    comm->next = NULL;
}
void comm_clear(struct command *comm)
{
    for (int i = 0; i < MAX_LINE / 2 + 1; i++)
        if (comm->args[i] != NULL)
        {
            free(comm->args[i]);
            comm->args[i] = NULL;
        }
    if (comm->infile != NULL)
    {
        free(comm->infile);
        comm->infile = NULL;
    }
    if (comm->outfile != NULL)
    {
        free(comm->outfile);
        comm->outfile = NULL;
    }
    comm->concurrent = false;
    if (comm->next != NULL)
    {
        comm_clear(comm->next);
        free(comm->next);
        comm->next = NULL;
    }
}

void comm_print(struct command *comm)
{
    printf("------------------------------");
    printf("comm: %p\n", (void *)comm);
    printf("args: ");
    for (int i = 0; i < MAX_LINE / 2 + 1; i++)
        if (comm->args[i] != NULL)
            printf("%s ", comm->args[i]);
    printf("\n");
    printf("infile: %s\n", comm->infile);
    printf("outfile: %s\n", comm->outfile);
    printf("concurrent: %d\n", comm->concurrent);
    printf("next: %p\n", (void *)comm->next);
    printf("------------------------------\n");
}

void comm_copy(struct command *from_comm, struct command *to_comm)
{
    comm_clear(to_comm);

    for (int i = 0; i < MAX_LINE / 2 + 1; i++)
        to_comm->args[i] = from_comm->args[i];
    to_comm->infile = from_comm->infile;
    to_comm->outfile = from_comm->outfile;
    to_comm->concurrent = from_comm->concurrent;
    to_comm->next = from_comm->next;

    comm_init(from_comm);
}

void comm_prase(struct command *comm)
{
    comm_clear(comm);
    char c;
    int argcnt = 0;
    do
    {
        char *s = (char *)malloc(sizeof(char) * MAX_LINE);
        scanf("%s", s);
        c = getchar();
        if (strcmp(s, "&") == 0)
        {
            comm->concurrent = true;
            free(s);
            continue;
        }
        if (s[0] == '<')
        {
            strcpy(s, s + 1);
            if (s[0] == '\0')
            {
                scanf("%s", s);
                c = getchar();
            }
            comm->infile = s;
            continue;
        }
        if (s[0] == '>')
        {
            strcpy(s, s + 1);
            if (s[0] == '\0')
            {
                scanf("%s", s);
                c = getchar();
            }
            comm->outfile = s;
            continue;
        }
        if (strcmp(s, "|") == 0)
        {
            free(s);
            comm->next = (struct command *)malloc(sizeof(struct command));
            comm_init(comm->next);
            comm_prase(comm->next);
            break;
        }
        comm->args[argcnt++] = s;
    } while (c == ' ');
}

#define READ_END 0
#define WRITE_END 1
int comm_execute(struct command *comm, int in_fd,int final_fd)
{
    // comm_print(comm);
    int pipeconn_fd[2];
    int pipeconn=pipe(pipeconn_fd);
    int out_fd=final_fd;

    

    pid_t id = fork();
    if (id == 0) // child
    {
        close(pipeconn_fd[READ_END]);
        if(comm->infile != NULL)
        {
            in_fd = open(comm->infile, O_RDONLY);
            dup2(in_fd, STDIN_FILENO);
            close(in_fd);
        }
        else
        {
            dup2(in_fd, STDIN_FILENO);
            close(in_fd);
        }

        if(comm->outfile != NULL)
        {
            out_fd = open(comm->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            dup2(out_fd, STDOUT_FILENO);
            close(out_fd);
        }
        else if(comm->next != NULL)
            dup2(pipeconn_fd[WRITE_END], STDOUT_FILENO);
        close(pipeconn_fd[WRITE_END]);
        execvp(comm->args[0], comm->args);
        exit(1);
    }
    else // parent
    {
        close(pipeconn_fd[WRITE_END]);
        if (comm->concurrent == false)
            waitpid(id, NULL, 0);

        if (comm->next != NULL)
        {
            pid_t id2 = fork();
            if (id2 == 0) // child
            {
                close(pipeconn_fd[WRITE_END]);
                dup2(pipeconn_fd[READ_END], STDIN_FILENO);
                close(pipeconn_fd[READ_END]);
                comm_execute(comm->next, pipeconn_fd[READ_END], final_fd);
                exit(1);
            }
            else // parent
            {
                close(pipeconn_fd[READ_END]);
                waitpid(id2, NULL, 0);
            }
        }
    }
    
    
    


    
}

int main(void)
{

    int should_run = 1;
    struct command *history, *current;
    history = (struct command *)malloc(sizeof(struct command));
    current = (struct command *)malloc(sizeof(struct command));
    comm_init(history);
    comm_init(current);

    while (should_run)
    {
        printf("osh>");
        fflush(stdout);

        comm_copy(current, history);
        comm_prase(current);

        if (strcmp(current->args[0], "exit") == 0)
        {
            should_run = 0;
            continue;
        }
        if (strcmp(current->args[0], "!!") == 0)
        {
            if (history->args[0] == NULL)
            {
                printf("No commands in history.\n");
                comm_clear(current);
                continue;
            }
            comm_copy(history, current);
        }
        comm_execute(current, STDIN_FILENO,STDOUT_FILENO);
    }
    return 0;
}

