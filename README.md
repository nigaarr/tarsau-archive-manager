# TarSau Arşiv Yöneticisi (tarsau)

Bu proje, Sakarya Üniversitesi Bilgisayar Mühendisliği Bölümü **Sistem Programlama** dersi (2025-2026 Bahar Dönemi) kapsamında geliştirilmiştir. Projenin amacı; Linux/Unix ortamında C dili kullanarak, `tar` veya `zip` mantığıyla çalışan fakat sıkıştırma yapmadan birden fazla metin dosyasını tek bir arşiv dosyasında (`*.sau`) birleştiren ve geri ayıklayan modüler bir sistem programı oluşturmaktır.

---

## 🛠️ Proje Mimarisi

Kod tabanı, sistem programlama pratiklerine uygun olarak modüler bir yapıda tasarlanmıştır:

* **`tarsau.h`**: Projede kullanılan tüm sistem kütüphanelerini, `MAX_FILES` (32) ve `MAX_TOTAL_SIZE` (200MB) gibi kısıtlamaları ve fonksiyon prototiplerini barındıran başlık dosyası.
* **`main.c`**: Komut satırından gelen parametreleri (`-b`, `-a`, `-o`) ayrıştıran ve ilgili modülü tetikleyen ana giriş kapısı.
* **`archive.c`**: Arşivleme (`-b`) işlevini üstlenir. Giriş dosyalarının ASCII formatında olup olmadığını kontrol eder, boyut analizi yapar ve `.sau` formatına uygun organizasyon (header) yapısıyla dosyaları birleştirir.
* **`extract.c`**: Arşivden çıkarma (`-a`) işlevini üstlenir. `.sau` uzantılı dosyayı çözümler, metadata alanını ayrıştırarak dosyaları orijinal isimleri, içerikleri ve Linux erişim izinleriyle (`chmod`) hedef dizine çıkartır.
* **`Makefile`**: Projenin otomatik ve hatasız bir şekilde derlenmesini sağlayan derleme betiği.

---

## 🚀 Çalıştırma ve Kullanım Talimatları

### 1. Derleme (Kompile Etme)
Proje dizininde terminali açarak sadece `make` yazmanız yeterlidir:
```bash
make
