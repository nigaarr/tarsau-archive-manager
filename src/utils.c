#include "utils.h"
 

// Dosyayi ikili modda okuyarak her baytı ASCII metin kurallarina gore denetler.                                                      */

int is_text_file(const char *path)
{
    FILE *f = fopen(path, "rb");
    if (!f) return 0;
 
    int c;
    while ((c = fgetc(f)) != EOF) {
        unsigned char uc = (unsigned char)c;
 
        if (uc == 0x09 || uc == 0x0A || uc == 0x0D) continue;
        if (uc >= 0x20 && uc <= 0x7E)                continue;
 
        
        fclose(f);
        return 0;
    }
 
    fclose(f);
    return 1;
}
 


void mode_to_str(mode_t mode, char *buf)
{
    buf[0] = (mode & S_IRUSR) ? 'r' : '-';
    buf[1] = (mode & S_IWUSR) ? 'w' : '-';
    buf[2] = (mode & S_IXUSR) ? 'x' : '-';
    buf[3] = (mode & S_IRGRP) ? 'r' : '-';
    buf[4] = (mode & S_IWGRP) ? 'w' : '-';
    buf[5] = (mode & S_IXGRP) ? 'x' : '-';
    buf[6] = (mode & S_IROTH) ? 'r' : '-';
    buf[7] = (mode & S_IWOTH) ? 'w' : '-';
    buf[8] = (mode & S_IXOTH) ? 'x' : '-';
    buf[9] = '\0';
}
 

mode_t str_to_mode(const char *buf)
{
    mode_t m = 0;
 
    if (buf[0] == 'r') m |= S_IRUSR;
    if (buf[1] == 'w') m |= S_IWUSR;
    if (buf[2] == 'x') m |= S_IXUSR;
 
    if (buf[3] == 'r') m |= S_IRGRP;
    if (buf[4] == 'w') m |= S_IWGRP;
    if (buf[5] == 'x') m |= S_IXGRP;
 
    if (buf[6] == 'r') m |= S_IROTH;
    if (buf[7] == 'w') m |= S_IWOTH;
    if (buf[8] == 'x') m |= S_IXOTH;
 
    return m;
}
 

//  Verilen yoldaki tum eksik dizinleri olusturur.  

int mkdir_p(const char *path)
{
    char tmp[MAX_PATH];
    snprintf(tmp, sizeof(tmp), "%s", path);
 
    size_t len = strlen(tmp);
    if (len && tmp[len - 1] == '/')
        tmp[len - 1] = '\0';
 
    for (char *p = tmp + 1; *p; p++) {
        if (*p == '/') {
            *p = '\0';
            if (mkdir(tmp, 0755) != 0 && errno != EEXIST)
                return -1;
            *p = '/';
        }
    }
 
    if (mkdir(tmp, 0755) != 0 && errno != EEXIST)
        return -1;
 
    return 0;
}