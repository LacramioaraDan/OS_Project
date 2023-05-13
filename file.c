#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include<dirent.h>

#define PATH_MAX 4096

enum FileType { REGULAR, SYMBOLICLINK, DIRECTORY };

struct stat st;

int countCFiles(char *dirName) {
  
  int count = 0;
  struct dirent *entry;
  DIR *dir;

  dir = opendir(dirName);
  if (dir == NULL) {
    perror("opendir");
    return -1;
  }

  while ((entry = readdir(dir)) != NULL) {
    char filePath[PATH_MAX];
    snprintf(filePath, sizeof(filePath), "%s/%s", dirName, entry->d_name);

    struct stat fileStat;
    if (stat(filePath, &fileStat) == 0 && S_ISREG(fileStat.st_mode) &&
        strstr(entry->d_name, ".c") != NULL) {
      count++;
    }
  }

  closedir(dir);
  return count;
}

void accesRights(struct stat st){

  __mode_t mode = st.st_mode;

  printf("\nThe access rights are:\n\n");

  printf("\n  User access rights :\n\n");

  printf("\tRead right : %s\n", mode & S_IRUSR ? "yes" : "no");
  printf("\tWrite right : %s\n", mode & S_IWUSR ? "yes" : "no");
  printf("\tExecute right : %s\n", mode & S_IXUSR ? "yes" : "no");

  printf("\n  Group access rights:\n\n");

  printf("\tRead right : %s\n", mode & S_IRGRP ? "yes" : "no");
  printf("\tWrite right : %s\n", mode & S_IWGRP ? "yes" : "no");
  printf("\tExecute right : %s\n", mode & S_IXGRP ? "yes" : "no");

  printf("\n  Others access rights:\n\n");

  printf("\tRead right : %s\n", mode & S_IROTH ? "yes" : "no");
  printf("\tWrite right : %s\n", mode & S_IWOTH ? "yes" : "no");
  printf("\tExecute right : %s\n", mode & S_IXOTH ? "yes" : "no");

}


void run(char *fileName, enum FileType fileType, char *options){

  struct stat st;
  stat(fileName, &st);

  for (int i = 0; options[i] != '\0'; i++) {

    switch (fileType) {

      case REGULAR:
        switch (options[i]) {
        
          case 'n':
            printf("Name of file: %s\n", fileName);
          break;

          case 'd':
            printf("The size is: %lu bytes\n", st.st_size); 
          break;

          case 'h':
            printf("The number of hardlinks is: %lu\n", st.st_nlink);
          break;

          case 'm':
            char *timeStr = ctime(&st.st_mtime);
            printf("Time of last modification is: %s", timeStr);
        
          break;

          case 'a':
            accesRights(st);
          break;

          case 'l': {

            char linkName[256];

            printf("Link name: ");
            scanf("%s", linkName);
            symlink(fileName,linkName);
            printf("'%s' created.\n", linkName);
            break;

          }

          default:
            break;

        }
        break;

      case DIRECTORY:
        switch (options[i]) {
        
          case 'n':
            printf("Name of directory: %s\n", fileName);
          break;

          case 'd':
            printf("The size is: %lu bytes\n", st.st_size); 
          break;

          case 'a':
            accesRights(st);
          break;

          case 'c':
            int fileCount = countCFiles(fileName);
            if (fileCount == -1) {
              printf("Error counting files.\n");
            } else {
              printf("Number of '.c' files in the directory: %d\n", fileCount);
            }
          break;

    
          default:
            break;

      
        }
      break;
  

      case SYMBOLICLINK:
        switch (options[i]) {
        
          case 'n':
            printf("Name of symbolic link: %s\n", fileName);
          break;

          case 'l':
            int result = unlink(fileName);
            if (result == 0) {
              printf("Symbolic link deleted successfully.\n");
            } else {
              perror("Error!");
            }
          break;

          case 'd':
            printf("The size is: %lu bytes\n", st.st_size); 
          break;

          case 't':
            struct stat st;
            char targetPath[256];
            ssize_t targetSize = readlink(fileName, targetPath, sizeof(targetPath) - 1);
            if (targetSize == -1) {
              perror("Error!");
            }
            targetPath[targetSize] = '\0';

            if (stat(targetPath, &st) == -1) {
              perror("Error!");
            }
            off_t fileSize = st.st_size;

            printf("Size of target file: %ld bytes\n", fileSize);

          break;

          case 'a':
            accesRights(st);
          break;

          default:
            break;
        }
      break;
    }
  }
}

int valid(enum FileType fileType, const char *options){

  switch (fileType) {

    case REGULAR: {
      const char regValid[] = "-ndhmal";

      for (int i = 0; regValid[i] != '\0'; i++) {

        if (!strchr(regValid, options[i])) {

          perror("\nInvalid option!\n");

          printf("\nCHOOSE ANOTHER OPTION: %c\n\n", options[i]);
          return 0;

        }
      }

      break;

    }

    case DIRECTORY:{
      const char dirValid[] = "-ndac";

      for (int i = 0; dirValid[i] != '\0'; i++) {

        if (!strchr(dirValid, options[i])) {

          printf("\nInvalid option!\n");

          printf("\nCHOOSE ANOTHER OPTION: %c\n\n", options[i]);
          return 0;

        }
      }

    break;

    }

    case SYMBOLICLINK:{
          const char symlinkValid[] = "-nldta";

      for (int i = 0; symlinkValid[i] != '\0'; i++) {

        if (!strchr(symlinkValid, options[i])) {

          printf("\nInvalid option!\n");

          printf("\nCHOOSE ANOTHER OPTION: %c\n\n", options[i]);
          return 0;

        }
      }

    break;

    }


  }

  return 1;

}


char *menu(char *fileName, enum FileType fileType){

  char *options = malloc(32);

  switch (fileType) {

    case REGULAR:

      printf("\n%s - It's a regular file.\n", fileName);
      printf("-n: name\n");
      printf("-d: size\n");
      printf("-h: hard link count\n");
      printf("-m: time of last modification\n");
      printf("-a: access rights\n");
      printf("-l: create symbolic link\n");
      printf("\nGive option: ");

      scanf("%s", options);

    break; 

    case DIRECTORY:

      printf("\n%s - It's a directory.\n", fileName);
      printf("-n: name\n");
      printf("-d: size of directory\n");
      printf("-a: access rights\n");
      printf("-c: total number of files with the .c extension\n");
      printf("\nGive option: ");

      scanf("%s", options);

    break;

    case SYMBOLICLINK:
    
      printf("\n%s - It's a symbolic link.\n", fileName);
      printf("-n: name\n");
      printf("-l: delete symbolic link\n");
      printf("-d: size of symbolic link\n");
      printf("-t: size of target file\n");
      printf("-a: access rights\n");
      printf("\nGive option: ");

      scanf("%s", options);

    break;

  }

  if (!valid(fileType, options)) {

    return menu(fileName, fileType);

  }

  return options;

}

