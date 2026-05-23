#ifndef TARSAU_H
#define TARSAU_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_FILES 32
#define MAX_TOTAL_SIZE (200 * 1024 * 1024) // 200 MB [cite: 18, 19]

// Ortak Kullanılacak Yardımcı Fonksiyonlar
int is_ascii_file(const char *filename);
long get_file_size(const char *filename);
int get_file_permissions(const char *filename);

// Modül Fonksiyonları
int create_archive(int file_count, char *input_files[], char *output_filename);
int extract_archive(char *archive_name, char *target_dir);

#endif