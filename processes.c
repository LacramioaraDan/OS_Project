#include <stdio.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include "file.c"

#define PATH_MAX 4096

void score(const char* fileName, int errors, int warnings) {

    int score;

    if (errors == 0 && warnings == 0) {
        score = 10;
    } else if (errors >= 1) {
        score = 1;
    } else if ((errors == 0) && (warnings > 10)) {
        score = 2;
    } else if ((errors == 0) && (warnings <= 10)){
        score = 2 + 8 * (10 - warnings) / 10;
    }

    FILE* file = fopen("grades.txt", "a");
    if (file != NULL) {
        fprintf(file, "%s: %d\n", fileName, score);
        fclose(file);
    } else {
        perror("Error!");
    }
}

void processes(int argc, char* argv[]) {
    pid_t pid;

    for (int i = 1; i < argc; i++) {
        pid = fork();

        if (pid < 0) {
            perror("Error!");
            exit(1);
        } else if (pid == 0) {

            struct stat st;
            lstat(argv[i], &st);

            if (S_ISREG(st.st_mode)) {
                char* options = menu(argv[i], REGULAR);
                run(argv[i], REGULAR, options);
            } else if (S_ISLNK(st.st_mode)) {
                char* options = menu(argv[i], SYMBOLICLINK);
                run(argv[i], SYMBOLICLINK, options);
            } else if (S_ISDIR(st.st_mode)) {
                char* options = menu(argv[i], DIRECTORY);
                run(argv[i], DIRECTORY, options);
            }

            exit(0);
        }
    }

    for (int i = 1; i < argc; i++) {
        int status;
        pid_t childPid = wait(&status);
        printf("\nProcess ended with pid: %d\n", childPid);

        struct stat st;
        lstat(argv[i], &st);

        if (S_ISREG(st.st_mode) && strstr(argv[i], ".c") != NULL) {
            pid = fork();

            if (pid < 0) {
                perror("Error!");
                exit(1);
            } else if (pid == 0) {

                if (execlp("./script.sh", argv[i], NULL) == -1) {
                    perror("Error!");
                    exit(1);
                }
            } else {

                int scriptStatus;
                waitpid(pid, &scriptStatus, 0);

                if (WIFEXITED(scriptStatus)) {
                    int exitCode = WEXITSTATUS(scriptStatus);
                    printf("\nProcess ended with pid: %d\n", pid);

                    if (exitCode == 0) {

                        FILE* output = fopen("output.txt", "r");
                        if (output == NULL) {
                            perror("Error!");
                            exit(1);
                        }

                        int errors = 0;
                        int warnings = 0;
                        char line[256];

                        while (fgets(line, sizeof(line), output) != NULL) {
                            if (strstr(line, "error") != NULL) {
                                errors++;
                            } else if (strstr(line, "warning") != NULL) {
                                warnings++;
                            }
                        }

                        fclose(output);

                        score(argv[i], errors, warnings);
                    } else {
                        perror("Script execution failed!");
                    }
                } else {
                    perror("Error!");
                }
            }

        } else if (S_ISREG(st.st_mode) && strstr(argv[i], ".c") == NULL) {

            FILE* f = fopen(argv[i], "r");
            if (f == NULL) {
                perror("Error!");
                exit(1);
            }

            int count = 0;
            char ch;
            while ((ch = fgetc(f)) != EOF) {
                if (ch == '\n') {
                    count++;
                }
            }

            fclose(f);
            printf("The file %s has %d lines\n", argv[i], count);
        } else if (S_ISDIR(st.st_mode)) {

            char fileName[PATH_MAX];
            snprintf(fileName, sizeof(fileName), "%s_file.txt", argv[i]);

            FILE* file = fopen(fileName, "w");
            if (file == NULL) {
                perror("Error!");
                exit(1);
            }

            fclose(file);
            printf("Created file %s\n", fileName);
        } else if (S_ISLNK(st.st_mode)) {

            if (chmod(argv[i], S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP) == -1) {
                perror("Error!");
                exit(1);
            }
            
            if (chmod(argv[i], S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | 0) == -1) {
                perror("Error!");
                exit(1);
            }else{

                printf("The permissions of the symbolic link %s have been changed!\n", argv[i]);

            }
        }
    }
}
