 #include "archive.h"
#include "utils.h"
/* 

// Arşiv OLUSTURMA  (-b)                              */
int cmd_bundle(int argc, char *argv[])
{
    const char *output_file = DEFAULT_ARCHIVE;
    char       *input_files[MAX_FILES];
    int         file_count = 0;
 
    // Komut satiri argümanlarini ayristir 
    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "-o") == 0) {
            if (i + 1 >= argc) {
                fprintf(stderr,
                    "Hata: -o parametresinden sonra dosya adi belirtilmelidir.\n");
                return 1;
            }
            output_file = argv[++i];
        } else {
            if (file_count >= MAX_FILES) {
                fprintf(stderr,
                    "Hata: En fazla %d giris dosyasi desteklenir.\n", MAX_FILES);
                return 1;
            }
            input_files[file_count++] = argv[i];
        }
    }
 
    if (file_count == 0) {
        fprintf(stderr, "Hata: En az bir giris dosyasi belirtilmelidir.\n");
        return 1;
    }
 
    // Her giris dosyasini dogrula 
    struct stat    st[MAX_FILES];
    unsigned long  total_size = 0;
 
    for (int i = 0; i < file_count; i++) {
        // Varlik ve duzenli dosya kontrolu 
        if (stat(input_files[i], &st[i]) != 0 || !S_ISREG(st[i].st_mode)) {
            fprintf(stderr,
                "%s ->Boyle bir giris dosyasi mevcut degildir ve ya giris dosyasinin formati uyumsuzdur!\n", input_files[i]);
            return 1;
        }
        // ASCII metin dosyasi kontrolu 
        if (!is_text_file(input_files[i])) {
            fprintf(stderr,
                "%s giris dosyasinin formati uyumsuzdur ve ya dosya Turkce Karakter içeriyor\n", input_files[i]);
            return 1;
        }
        // Toplam boyut siniri 
        total_size += (unsigned long)st[i].st_size;
        if (total_size > MAX_TOTAL_SIZE) {
            fprintf(stderr,
                "Hata: Giris dosyalarinin toplam boyutu 200 MB'i gecemez.\n");
            return 1;
        }
    }
 
    // Organizasyon (icerik)bolumu
    char org_body[1024 * 1024]; //1 MB yeterli 
    int  org_pos = 0;
 
    for (int i = 0; i < file_count; i++) {
        char perm[PERM_STR_LEN + 1];
        mode_to_str(st[i].st_mode & 0777, perm);
 
        // basename() icin gecici kopya (fonksiyon tamponu degistirebilir) 
        char path_copy[MAX_PATH];
        strncpy(path_copy, input_files[i], MAX_PATH - 1);
        path_copy[MAX_PATH - 1] = '\0';
        const char *bname = basename(path_copy);
 
        org_pos += snprintf(org_body + org_pos,
                            sizeof(org_body) - org_pos,
                            "|%s,%s,%ld",
                            bname, perm, (long)st[i].st_size);
    }
    //Son kapanma karakteri 
    if (org_pos < (int)sizeof(org_body) - 1)
        org_body[org_pos++] = '|';
    org_body[org_pos] = '\0';
 
    //Organizasyon bolumunun toplam uzunlugu (10 baytlik alan dahil) 
    int  total_org_len = HEADER_LEN_FIELD + org_pos;
    char header[12];
    snprintf(header, sizeof(header), "%10d", total_org_len);
 
    // Arsiv dosyasini yaz 
    FILE *out = fopen(output_file, "wb");
    if (!out) {
        fprintf(stderr, "Hata: '%s' dosyasi olusturulamadi.\n", output_file);
        return 1;
    }
 
    // 10 baytlik boyut alani 
    fwrite(header, 1, HEADER_LEN_FIELD, out);
    //Organizasyon govdesi 
    fwrite(org_body, 1, org_pos, out);
    //Dosya icerikleri (birbirini takip eder, ayirici yok) 
    for (int i = 0; i < file_count; i++) {
        FILE *in = fopen(input_files[i], "rb");
        if (!in) {
            fprintf(stderr,
                "Hata: '%s' dosyasi acilamadi.\n", input_files[i]);
            fclose(out);
            remove(output_file);
            return 1;
        }
        char   buf[65536];
        size_t n;
        while ((n = fread(buf, 1, sizeof(buf), in)) > 0)
            fwrite(buf, 1, n, out);
        fclose(in);
    }
 
    fclose(out);
    printf("Arsiv basariyla olusturuldu: %s\n", output_file);
    return 0;
}
 
//  cmd_extract  :  Arşiv ACMA  (-a)                                 
int cmd_extract(int argc, char *argv[])
{
    if (argc < 3) {
        fprintf(stderr, "Kullanim: tarsau -a <arsiv.sau> [dizin]\n");
        return 1;
    }
 
    const char *archive_path = argv[2];
    const char *dest_dir     = (argc >= 4) ? argv[3] : ".";
 
    //.sau uzantisi kontrolu 
    const char *ext = strrchr(archive_path, '.');
    if (!ext || strcmp(ext, ".sau") != 0) {
        fprintf(stderr, "Arsiv dosyasi mevcut degil ve ya uygunsuzdur!\n");
        return 1;
    }
 
    FILE *arc = fopen(archive_path, "rb");
    if (!arc) {
        fprintf(stderr, "Arsiv dosyasi mevcut degil ve ya uygunsuzdur!\n");
        return 1;
    }
 
    //Ilk 10 bayt: organizasyon bolumunun toplam boyutu
    char len_buf[HEADER_LEN_FIELD + 1];
    if (fread(len_buf, 1, HEADER_LEN_FIELD, arc) != (size_t)HEADER_LEN_FIELD) {
        fprintf(stderr, "Arsiv dosyasi uygunsuz veya bozuk!\n");
        fclose(arc);
        return 1;
    }
    len_buf[HEADER_LEN_FIELD] = '\0';
 
    int total_org_len = atoi(len_buf);
    if (total_org_len <= HEADER_LEN_FIELD) {
        fprintf(stderr, "Arsiv dosyasi uygunsuz veya bozuk!\n");
        fclose(arc);
        return 1;
    }
 
    //Organizasyon govdesini oku
    int   org_body_len = total_org_len - HEADER_LEN_FIELD;
    char *org_body     = (char *)malloc(org_body_len + 1);
    if (!org_body) {
        fprintf(stderr, "Hata: Bellek yetersiz.\n");
        fclose(arc);
        return 1;
    }
 
    if ((int)fread(org_body, 1, org_body_len, arc) != org_body_len) {
        fprintf(stderr, "Arsiv dosyasi uygunsuz veya bozuk!\n");
        free(org_body);
        fclose(arc);
        return 1;
    }
    org_body[org_body_len] = '\0';
 
    //Organizasyon govdesini ayristir
    //Format: |ad,rwxrwxrwx,boyut|ad,rwxrwxrwx,boyut|         
    FileEntry entries[MAX_FILES];
    int       entry_count = 0;
    char     *p = org_body;
 
    while (*p && entry_count < MAX_FILES) {
        //Bir sonraki '|' ayiracini bul 
        if (*p != '|') { p++; continue; }
        p++; // '|' atla 
 
        // Son kapanma '|' karakteri ya da string sonu 
        if (*p == '\0' || *p == '|') break;
 
        // Dosya adi: bir sonraki virgule kadar 
        char *comma1 = strchr(p, ',');
        if (!comma1) break;
        int name_len = comma1 - p;
        if (name_len <= 0 || name_len >= 256) break;
        strncpy(entries[entry_count].name, p, name_len);
        entries[entry_count].name[name_len] = '\0';
        p = comma1 + 1;
 
        //Izinler: tam 9 karakter, ardindan ',' olmali 
        if ((int)strlen(p) < PERM_STR_LEN + 1 || p[PERM_STR_LEN] != ',')
            break;
        strncpy(entries[entry_count].perm, p, PERM_STR_LEN);
        entries[entry_count].perm[PERM_STR_LEN] = '\0';
        p += PERM_STR_LEN + 1; //9 karakter 
 
        // Boyut: sayisal deger, '|' ile biter 
        char *end;
        entries[entry_count].size = strtol(p, &end, 10);
        if (end == p) break;
        p = end;
        //p su an kapanma '|' uzerinde; sonraki dongu basi onu isleyecek 
 
        entry_count++;
    }
 
    // Hedef dizini olustur (gerekiyorsa) 
    if (strcmp(dest_dir, ".") != 0) {
        if (mkdir_p(dest_dir) != 0) {
            fprintf(stderr,
                "Hata: '%s' dizini olusturulamadi.\n", dest_dir);
            free(org_body);
            fclose(arc);
            return 1;
        }
    }
 
    //Dosyalari ayikla ve izinlerini geri yukle
    for (int i = 0; i < entry_count; i++) {
        char out_path[MAX_PATH];
        snprintf(out_path, sizeof(out_path),
                 "%s/%s", dest_dir, entries[i].name);
 
        FILE *out = fopen(out_path, "wb");
        if (!out) {
            fprintf(stderr,
                "Hata: '%s' dosyasi olusturulamadi.\n", out_path);
            free(org_body);
            fclose(arc);
            return 1;
        }
 
        long   remaining = entries[i].size;
        char   buf[65536];
        while (remaining > 0) {
            size_t to_read = ((long)sizeof(buf) < remaining)
                             ? sizeof(buf)
                             : (size_t)remaining;
            size_t n = fread(buf, 1, to_read, arc);
            if (n == 0) break;
            fwrite(buf, 1, n, out);
            remaining -= (long)n;
        }
        fclose(out);
 
        // Orijinal izinleri geri yukle
        chmod(out_path, str_to_mode(entries[i].perm));
        printf("Cikartildi: %s\n", out_path);
    }
 
    free(org_body);
    fclose(arc);
    printf("Arsiv basariyla acildi.\n");
    return 0;
}