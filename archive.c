#include "tarsau.h"

int is_ascii_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) return 0; 

    int ch;
    while ((ch = fgetc(file)) != EOF) {
        if (ch < 0 || ch > 127) { // Karakter başına 1 bayt ASCII [cite: 12]
            fclose(file);
            return 0; 
        }
    }
    fclose(file);
    return 1; 
}

long get_file_size(const char *filename) {
    struct stat st;
    if (stat(filename, &st) == 0) return st.st_size;
    return -1;
}

int get_file_permissions(const char *filename) {
    struct stat st;
    if (stat(filename, &st) == 0) return st.st_mode & 0777;
    return 0;
}

int create_archive(int file_count, char *input_files[], char *output_filename) {
    long total_size = 0;

    // Giriş dosyalarının kurallara uygunluk kontrolleri
    for (int i = 0; i < file_count; i++) {
        if (!is_ascii_file(input_files[i])) {
            printf("%s giriş dosyasının formatı uyumsuzdur!\n", input_files[i]); // [cite: 20]
            return 0; // Sorunsuz çıkış [cite: 21, 29]
        }

        long size = get_file_size(input_files[i]);
        if (size == -1) {
            printf("Hata: %s dosyası okunamadı!\n", input_files[i]);
            return 1;
        }
        total_size += size;
    }

    if (total_size > MAX_TOTAL_SIZE) { // [cite: 18]
        printf("Hata: Giriş dosyalarının toplam boyutu 200 MB'ı geçemez!\n"); // [cite: 18]
        return 1;
    }

    // Header boyut hesabı
    int header_size = 10; // [cite: 34]
    for (int i = 0; i < file_count; i++) {
        char temp[512];
        sprintf(temp, "|%s,%04o,%ld|", input_files[i], get_file_permissions(input_files[i]), get_file_size(input_files[i])); // [cite: 35, 36]
        header_size += strlen(temp);
    }

    FILE *archive = fopen(output_filename, "w");
    if (archive == NULL) {
        printf("Hata: Çıktı dosyası oluşturulamadı!\n");
        return 1;
    }

    // İlk 10 bayta boyutu yaz ve ardından metadataları dök [cite: 34, 35, 36]
    fprintf(archive, "%010d", header_size);
    for (int i = 0; i < file_count; i++) {
        fprintf(archive, "|%s,%04o,%ld|", input_files[i], get_file_permissions(input_files[i]), get_file_size(input_files[i]));
    }

    // Dosya içeriklerini ardışık ekle [cite: 37, 38, 39]
    for (int i = 0; i < file_count; i++) {
        FILE *in_file = fopen(input_files[i], "r");
        if (in_file != NULL) {
            int ch;
            while ((ch = fgetc(in_file)) != EOF) {
                fputc(ch, archive);
            }
            fclose(in_file);
        }
    }

    fclose(archive);
    printf("Dosyalar birleştirildi.\n"); // [cite: 5, 8]
    return 0;
}