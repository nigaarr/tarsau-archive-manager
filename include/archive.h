#ifndef ARCHIVE_H
#define ARCHIVE_H
 
/* ============================================================
 *  archive.h  –  Arsivleme ve acma fonksiyon bildirimleri
 * ============================================================ */
 
/* Arşiv OLUSTURMA  (-b parametresi)
 *   argc / argv: main'den gelen komut satiri argumanlari
 *   Doner: 0 = basarili, 1 = hata */
int cmd_bundle(int argc, char *argv[]);
 
/* Arşiv ACMA  (-a parametresi)
 *   argc / argv: main'den gelen komut satiri argumanlari
 *   Doner: 0 = basarili, 1 = hata */
int cmd_extract(int argc, char *argv[]);
 
#endif /* ARCHIVE_H */