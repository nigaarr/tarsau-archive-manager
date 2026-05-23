#include "tarsau.h"

int extract_archive(char *archive_name, char *target_dir) {
    // Uzantı kontrolü
    char *dot = strrchr(archive_name, '.');
    if (!dot || strcmp(dot, ".sau") != 0) { // [cite: 24]
        printf("Arşiv dosyası uygunsuz veya bozuk!\n"); // [cite: 25]
        return 1;
    }

    FILE *archive = fopen(archive_name, "r");
    if (archive == NULL) {
        printf("Arşiv dosyası uygunsuz veya bozuk!\n"); // [cite: 25]
        return 1;
    }

    // İlk 10 bayttan header boyutunu oku [cite: 34]
    char header_size_str[11];
    if (fread(header_size_str, 1, 10, archive) != 10) {
        printf("Arşiv dosyası uygunsuz veya bozuk!\n");
        fclose(archive);
        return 1;
    }
    header_size_str[10] = '\0';
    int header_size = atoi(header_size_str);

    int metadata_len = header_size - 10;
    char *metadata = malloc(metadata_len + 1);
    if (fread(metadata, 1, metadata_len, archive) != (size_t)metadata_len) {
        printf("Arşiv dosyası uygunsuz veya bozuk!\n");
        free(metadata);
        fclose(archive);
        return 1;
    }
    metadata[metadata_len] = '\0';

    if (target_dir != NULL) {
        mkdir(target_dir, 0755); // Dizin yoksa oluşturulur [cite: 28]
    }

    long data_start_pos = ftell(archive);
    char *token = strtok(metadata, "|"); // [cite: 35]
    long current_data_offset = 0;

    while (token != NULL) {
        if (strlen(token) > 0) {
            char file_name[256];
            unsigned int perms;
            long f_size;

            if (sscanf(token, "%[^,],%o,%ld", file_name, &perms, &f_size) == 3) { // [cite: 36]
                char full_path[512];
                if (target_dir != NULL) {
                    sprintf(full_path, "%s/%s", target_dir, file_name);
                } else {
                    strcpy(full_path, file_name);
                }

                long current_pos = ftell(archive);
                fseek(archive, data_start_pos + current_data_offset, SEEK_SET);

                FILE *out_file = fopen(full_path, "w");
                if (out_file != NULL) {
                    for (long i = 0; i < f_size; i++) {
                        int ch = fgetc(archive);
                        if (ch != EOF) fputc(ch, out_file);
                    }
                    fclose(out_file);
                    chmod(full_path, perms); // Aynı izinleri geri yükle [cite: 30]
                }

                current_data_offset += f_size;
                fseek(archive, current_pos, SEEK_SET);
            }
        }
        token = strtok(NULL, "|");
    }

    free(metadata);
    fclose(archive);

    if (target_dir != NULL) {
        printf("%s dizininde dosyalar açıldı.\n", target_dir); // [cite: 7, 10]
    } else {
        printf("Dosyalar geçerli dizinde açıldı.\n"); // [cite: 26]
    }
    return 0;
}