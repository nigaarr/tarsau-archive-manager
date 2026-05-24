
  ### ==Önemli Bilgilendirme==
  ### 'chmod' ile işlem yapabilmeniz için uygulama NTFS dosya sistemi içerisinde çalıştırılMAMAlıdır.
---
### Bu ödev WSL de çalıştırılmak üzere tasarlanmıştır.


 1. Linux ortamına geçin:
```
   cd ~
```

2. Repo yu kopyalayın:
```
git clone https://github.com/nigaarr/tarsau-archive-manager.git
```

3. Proje klasörüne gelin:
````
cd tarsau-archive-manager
````

4. Klasörü yetkilendirin: (make ile oluşmuş dosyayı çağırabilmek için)
```
chmod -R 755 .
```

5. Test Alanına geçin
```
cd test-alani
```

```
--Test alanında bulunan hazır .txt ler ile denemeler yapabilir, arşivleme ve arşivden çıkarma işlemlerinizi yapabilirsiniz
```

6. Uygulamayı test etmek için hazır komutlar:
```
../bin/tarsau -b dosya1.txt dosya2.txt dosya3.txt
```

```
../bin/tarsau -a a.sau ayiklananlar
```