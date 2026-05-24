#ifndef UTILS_H
#define UTILS_H
 

 
#include "common.h"
 
// Dosyanin yalnizca ASCII metin dosyasi olup olmadigini kontrol eder.

int is_text_file(const char *path);
 
// mode_t degerini "rwxrwxrwx" bicimine donusturur.

void mode_to_str(mode_t mode, char *buf);
 
// "rwxrwxrwx" bicimli dizgeyi mode_t degerine donusturur. 
mode_t str_to_mode(const char *buf);
 
// Dizin yolunu ozyinelemeli olarak olusturur (mkdir -p davranisi).

int mkdir_p(const char *path);
 
#endif 