/* ============================================================
 *  main.c  –  tarsau  :  Metin dosyasi arsivleyici
 *
 *  Kullanim:
 *    tarsau -b <dosya1> [dosya2 ...] [-o <arsiv.sau>]
 *    tarsau -a <arsiv.sau> [hedef_dizin]
 *
 *  -b   : Belirtilen metin dosyalarini tek bir .sau arsivine paketter.
 *  -a   : Bir .sau arsivini belirtilen dizine (yoksa mevcut dizine) acar.
 * ============================================================ */
 
#include "common.h"
#include "archive.h"
 
static void kullanimi_goster(const char *program_adi)
{
    fprintf(stderr, "Kullanim:\n");
    fprintf(stderr, "  %s -b <dosya1> [dosya2 ...] [-o <arsiv.sau>]\n",
            program_adi);
    fprintf(stderr, "  %s -a <arsiv.sau> [hedef_dizin]\n",
            program_adi);
}
 
int main(int argc, char *argv[])
{
    if (argc < 2) {
        kullanimi_goster(argv[0]);
        return 1;
    }
 
    if (strcmp(argv[1], "-b") == 0)
        return cmd_bundle(argc, argv);
 
    if (strcmp(argv[1], "-a") == 0)
        return cmd_extract(argc, argv);
 
    fprintf(stderr,
        "Hata: Bilinmeyen parametre '%s'.\n", argv[1]);
    kullanimi_goster(argv[0]);
    return 1;
    }