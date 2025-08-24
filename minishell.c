/*********************************************************************
Program  : miniShell                   Version    : 1.3
--------------------------------------------------------------------
skeleton code for linix/unix/minix command line interpreter
--------------------------------------------------------------------
File			: minishell.c
Compiler/System	: gcc/linux

********************************************************************/

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>


#define NV 20			/* max number of command tokens */
#define NL 100			/* input buffer size */
char            line[NL];	/* command input buffer */

#define MAX_BACKGROUND 20 // max background process
/*
    shell prompt
*/
typedef struct {
    int pid;
    char command[NL];
} backgroundProcess;

backgroundProcess bgProcesses[MAX_BACKGROUND]
int latest_background = 0;

void prompt(void)
{

    fflush(stdout);
}

/*
check if any backgrouund process finishes 
*/
void checkBackground() {
    int status;
    int pid;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        for (int i = 0; i < MAX_BACKGROUND; i++) {
            if (bgProcesses[i].pid == pid) {
                fprintf(stdout, "[%i]+ Done\t%s\n", i, bgProcesses[i].command);
                fflush(stdout);

                bgProcesses[i].pid = -1;

                if (i >= latest_background) {
                    while (bgProcesses[latest_background].pid == -1) {
                        latest_background--;
                    }
                    if (latest_background < 0) {
                        latest_background = 0;
                    }
                }
                break;
            }
        }
    }
}

/* argk - number of arguments */
/* argv - argument vector from command line */
/* envp - environment pointer */
int main(int argk, char *argv[], char *envp[])
{
    int             frkRtnVal;	    /* value returned by fork sys call */
    char           *v[NV];	        /* array of pointers to command line tokens */
    char           *sep = " \t\n";  /* command line token separators    */
    int             i;		          /* parse index */

    for (i = 0; i < MAX_BACKGROUND; i++) {
        bgProcesses[i].pid = -1;
    }
        /* prompt for and process one command line at a time  */

    while (1) {			/* do Forever */
        int is_background = 0; // is running in background
        prompt();
        fgets(line, NL, stdin);
        fflush(stdin);

        // This if() required for gradescope
        if (feof(stdin)) {		/* non-zero on EOF  */
        exit(0);
        }
        if (line[0] == '#' || line[0] == '\n' || line[0] == '\000'){
        continue;			/* to prompt */
        }

        // check the process that will run in background
        char command[strlen(line)];
        if (line[strlen(line) - 2] == '&') {
            is_background = 1;
            line[strlen(line) - 2] = '\0'; // remove '&' char
            strcpy(command, line);
        }

        v[0] = strtok(line, sep);
        for (i = 1; i < NV; i++) {
            v[i] = strtok(NULL, sep);
            if (v[i] == NULL){
                break;
            }
        }
        // Handle cd
    if (strcmp(v[0], "cd") == 0) {
      if (v[1] == NULL) {
        char *home = getenv("HOME");
        if (home == NULL) {
          fprintf(stdout, "HOME not set\n");
          fflush(stdout);
        }
        else if (chdir(home) != 0) perror("chdir");
      }
      else if (chdir(v[1]) != 0) perror("chdir");

    } else {
      /* fork a child process to exec the command in v[0] */
      switch (frkRtnVal = fork()) {
        case -1:			/* fork returns error to parent process */
        {
          perror("Fork");
          break;
        }
        case 0:			/* code executed only by child process */
        {
          if (execvp(v[0], v) == -1) {
            perror("Execvp");
          }
          exit(1);
        }
        default:			/* code executed only by parent process */
        {
          if (!isBackground) {
            int status;
            if (waitpid(frkRtnVal, &status, 0) == -1) perror("waitpid");
          } else {
            latestBackground++;
            bgProcesses[latestBackground].pid = frkRtnVal;
            strcpy(bgProcesses[latestBackground].command, command);

            fprintf(stdout, "[%i] %d\n", latestBackground, frkRtnVal);
            fflush(stdout);
          }
          break;
        }
      }				/* switch */
      checkBackground();
    }
  }				/* while */
}				/* main */
