#include <sys/stat.h>

#ifndef FILE_H
#define FILE_H

enum FileType { REG, LINK, DIR };

char *menu(char *fileName, enum FileType fileType);
int valid(enum FileType fileType, const char *options);
void size(struct stat st);
void hardlinkCount(struct stat st);
void run(char *fileName, enum FileType fileType, char *options);
void accesRights(struct stat st);
void parse(int argc, char *argv[]);

#endif // !FILE_H