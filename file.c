#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>

enum FileType { REG, LINK, DIR };

struct stat st1;

char *menu(char *fileName, enum FileType fileType){

  char *options = malloc(32);

  switch (fileType) {

    case REG:

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

    case DIR:

      printf("\n%s - It's a directory.\n", fileName);
      printf("-n: name\n");
      printf("-d: size of directory\n");
      printf("-a: access rights\n");
      printf("-c: total number of files with the .c extension\n");
      printf("\nGive option: ");

      scanf("%s", options);

    break;

    case LINK:
    
      printf("\n%s - It's a symbolic link.\n", fileName);
      printf("-n: name\n");
      printf("-l: delete symbolic link\n");
      printf("-d: size of symbolic link\n");
      printf("-t: size of target file\n");
      printf("-a: access rights\n");
      printf("-l: create symbolic link\n");
      printf("\nGive option: ");

      scanf("%s", options);

    break;

  }

  if (!valid(fileType, options)) {

    return menu(fileName, fileType);

  }

  return options;

}



// int countFilesWithC(char *dirName) {
//     int count = 0;
//     struct dirent *entry;

//     dir = opendir(dirName);
//     if (dir == NULL) {
//         perror("opendir");
//         return -1;
//     }

//     while ((entry = readdir(dir)) != NULL) {
//         if (entry->d_type == DT_REG && strstr(entry->d_name, ".c") != NULL) {
//             count++;
//         }
//     }

//     closedir(dir);
//     return count;
// }


void run(char *fileName, enum FileType fileType, char *options){

  struct stat st;
  stat(fileName, &st);

  for (int i = 0; options[i] != '\0'; i++) {

    switch (fileType) {

      case REG:
        switch (options[i]) {
        
          case 'n':
            printf("Name of file: %s\n", fileName);
            break;

          case 'd':
            size(st);
            break;

          case 'h':
            hardlinkCount(st);
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

      case DIR:
        switch (options[i]) {
        
          case 'n':
            printf("Name of directory: %s\n", fileName);
            break;

          case 'd':
            break;

          case 'a':
            accesRights(st);
            break;

          case 'c':
            //countFilesWithC(fileName);
            break;

    
          default:
            break;

      
        }
      break;
  

      case LINK:
        switch (options[i]) {
        
          case 'n':
            printf("Name of symbolic link: %s\n", fileName);
            break;

          case 'l':

            break;

          case 'd':

            break;

          case 't':

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

void parse(int argc, char *argv[]){

  for (int i = 1; i < argc; i++) {

    struct stat st;
    stat(argv[i], &st);

    if (S_ISREG(st.st_mode)) {

      char *options = menu(argv[i], REG);
      run(argv[i], REG, options);

    } 

    else if (S_ISLNK(st.st_mode)) {

      char *options = menu(argv[i], LINK);
      run(argv[i], LINK, options);

    } 
    
    else if (S_ISDIR(st.st_mode)) {

      char *options = menu(argv[i], DIR);
      run(argv[i], DIR, options);

    }
  }
}

int valid(enum FileType fileType, const char *options){

  switch (fileType) {

    case REG: {
      const char regValid[] = "-ndhmal";

      for (int i = 0; regValid[i] != '\0'; i++) {

        if (!strchr(regValid, options[i])) {

          printf("\nInvalid option!\n");

          printf("\nCHOOSE ANOTHER OPTION: \n\n", options[i]);
          return 0;

        }
      }

      break;

    }

    case DIR:{
      const char dirValid[] = "-ndac";

      for (int i = 0; dirValid[i] != '\0'; i++) {

        if (!strchr(dirValid, options[i])) {

          printf("\nInvalid option!\n");

          printf("\nCHOOSE ANOTHER OPTION: \n\n", options[i]);
          return 0;

        }
      }

    break;

    }

    case LINK:{
          const char symlinkValid[] = "-nldta";

      for (int i = 0; symlinkValid[i] != '\0'; i++) {

        if (!strchr(symlinkValid, options[i])) {

          printf("\nInvalid option!\n");

          printf("\nCHOOSE ANOTHER OPTION: \n\n", options[i]);
          return 0;

        }
      }

    break;

    }


  }

  return 1;

}

void hardlinkCount(struct stat st){

  printf("The number of hardlinks is: %lu\n", st.st_nlink);

}

void size(struct stat st) { 

  printf("The size is: %lu bytes\n", st.st_size); 

}

void lastModified(struct stat st){

  printf("WHAT");
  printf("The time of last modification is: ");

}

void accesRights(struct stat st){

  __mode_t mode = st.st_mode;

  printf("\nThe access rights are:\n");

  printf("  User:\n");

  printf("\tRead - %s\n", mode & S_IRUSR ? "yes" : "no");
  printf("\tWrite - %s\n", mode & S_IWUSR ? "yes" : "no");
  printf("\tExecute - %s\n", mode & S_IXUSR ? "yes" : "no");

  printf("  Group:\n");

  printf("\tRead - %s\n", mode & S_IRGRP ? "yes" : "no");
  printf("\tWrite - %s\n", mode & S_IWGRP ? "yes" : "no");
  printf("\tExecute - %s\n", mode & S_IXGRP ? "yes" : "no");

  printf("  Others:\n");

  printf("\tRead - %s\n", mode & S_IROTH ? "yes" : "no");
  printf("\tWrite - %s\n", mode & S_IWOTH ? "yes" : "no");
  printf("\tExecute - %s\n", mode & S_IXOTH ? "yes" : "no");

}