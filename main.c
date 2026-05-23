#include "tarsau.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Hata: Lütfen parametreleri giriniz (-b veya -a)!\n");
        return 1;
    }

    // ARŞİVLEME MODU (-b) [cite: 11]
    if (strcmp(argv[1], "-b") == 0) {
        char *output_filename = "a.sau"; // Varsayılan çıktı adı [cite: 17]
        char *input_files[MAX_FILES];
        int input_file_count = 0;

        for (int i = 2; i < argc; i++) {
            if (strcmp(argv[i], "-o") == 0) { // [cite: 16]
                if (i + 1 < argc) {
                    output_filename = argv[i + 1];
                    i++; 
                } else {
                    printf("Hata: -o parametresinden sonra çıktı dosya adı belirtilmedi!\n");
                    return 1;
                }
            } else {
                if (input_file_count < MAX_FILES) { // [cite: 19]
                    input_files[input_file_count] = argv[i];
                    input_file_count++;
                } else {
                    printf("Hata: Giriş dosyası sayısı en fazla %d olabilir!\n", MAX_FILES); // [cite: 19]
                    return 1;
                }
            }
        }

        if (input_file_count == 0) {
            printf("Hata: Arşivlenecek giriş dosyası belirtilmedi!\n");
            return 1;
        }

        return create_archive(input_file_count, input_files, output_filename);
    } 
    // ARŞİV DEN ÇIKARMA MODU (-a) [cite: 22]
    else if (strcmp(argv[1], "-a") == 0) {
        if (argc < 3 || argc > 4) { // En fazla 2 parametre almalı (-a dahil toplam 3 veya 4 argüman) [cite: 23]
            printf("Arşiv dosyası uygunsuz veya bozuk!\n"); // [cite: 25]
            return 1;
        }
        char *archive_name = argv[2]; // [cite: 24]
        char *target_dir = (argc == 4) ? argv[3] : NULL; // [cite: 26]

        return extract_archive(archive_name, target_dir);
    } 
    else {
        printf("Hata: Bilinmeyen parametre '%s'. Sadece -b veya -a kullanılabilir.\n", argv[1]);
        return 1;
    }
}