#ifndef COMMON_H
#define COMMON_H
 

 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <libgen.h>
 
//Sabitler 
#define MAX_FILES         32
#define MAX_TOTAL_SIZE    (200UL * 1024 * 1024)   // 200 MB 
#define HEADER_LEN_FIELD  10                       // ilk 10 bayt 
#define DEFAULT_ARCHIVE   "a.sau"
#define MAX_PATH          4096
#define PERM_STR_LEN      9                        // "rwxrwxrwx" 
 
//Arşivdeki her dosyaya ait meta-veri kaydı 
typedef struct {
    char name[256];            // dosyanin taban adi 
    char perm[PERM_STR_LEN+1]; // "rwxrwxrwx\0" 
    long size;                 // bayt cinsinden dosya boyutu
} FileEntry;
 
#endif 