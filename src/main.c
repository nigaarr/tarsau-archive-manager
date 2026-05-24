#include "common.h"
#include "archive.h"

static void kullanimi_goster(const char *program_adi)
{
    fprintf(stderr, "Kullanim:\n");
    fprintf(stderr, "  %s -b <dosya1> [dosya2 ...] [-o <arsiv.sau>]\n", program_adi);
    fprintf(stderr, "  %s -a <arsiv.sau> [hedef_dizin]\n", program_adi);
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        kullanimi_goster(argv[0]);
        return 1;
    }

    // ARŞİVLEME MODU (-b)
    if (strcmp(argv[1], "-b") == 0) {
        return cmd_bundle(argc, argv);
    }

    // ARŞİVDEN ÇIKARMA MODU (-a)
    if (strcmp(argv[1], "-a") == 0) {
        if (argc != 3 && argc != 4) {
            printf("Fazladan parametre girdiniz\n");
            return 1;
        }
        return cmd_extract(argc, argv);
    }

    // HATALI PARAMETRE DURUMU
    fprintf(stderr, "Hata: Bilinmeyen parametre '%s'.\n", argv[1]);
    kullanimi_goster(argv[0]);
    return 1;
}