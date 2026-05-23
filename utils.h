#ifndef UTILS_H
#define UTILS_H
 
/* ============================================================
 *  utils.h  –  Yardimci fonksiyon bildirimleri
 * ============================================================ */
 
#include "common.h"
 
/* Dosyanin yalnizca ASCII metin dosyasi olup olmadigini kontrol eder.
 * Doner: 1 = metin dosyasi,  0 = ikili (binary) veya okunamaz. */
int is_text_file(const char *path);
 
/* mode_t degerini "rwxrwxrwx" bicimine donusturur.
 * buf en az 10 bayt olmalidir (9 karakter + '\0'). */
void mode_to_str(mode_t mode, char *buf);
 
/* "rwxrwxrwx" bicimli dizgeyi mode_t degerine donusturur. */
mode_t str_to_mode(const char *buf);
 
/* Dizin yolunu ozyinelemeli olarak olusturur (mkdir -p davranisi).
 * Doner: 0 = basarili,  -1 = hata. */
int mkdir_p(const char *path);
 
#endif /* UTILS_H */