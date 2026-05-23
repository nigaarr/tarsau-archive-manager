#!/bin/bash
# ============================================================
#  test_tarsau.sh  -  tarsau programi icin otomatik test
#
#  Kullanim:
#    chmod +x test_tarsau.sh
#    ./test_tarsau.sh
#
#  Not: tarsau ile ayni dizinde olmali (ya da PATH'te).
# ============================================================
 
TARSAU=$(cd "$(dirname "$0")" && pwd)/tarsau
PASS=0
FAIL=0
TEST_DIR=__test_alan__
 
# Renkler (terminal destekliyorsa)
YESIL="\033[0;32m"
KIRMIZI="\033[0;31m"
SIFIRLA="\033[0m"
 
# ---- Yardimci fonksiyonlar ----------------------------------
 
baslik() {
    echo ""
    echo "==== $1 ===="
    echo ""
}
 
run_test() {
    local ad="$1"
    local beklenen="$2"
    local gercek="$3"
    if [ "$beklenen" = "$gercek" ]; then
        printf "  ${YESIL}[GECTI]${SIFIRLA} %s\n" "$ad"
        PASS=$((PASS + 1))
    else
        printf "  ${KIRMIZI}[KALDI]${SIFIRLA} %s  (beklenen='%s'  gercek='%s')\n" \
               "$ad" "$beklenen" "$gercek"
        FAIL=$((FAIL + 1))
    fi
}
 
# ---- Onkosullar ---------------------------------------------
 
if [ ! -f "$TARSAU" ]; then
    echo "HATA: '$TARSAU' bulunamadi. Once 'make' komutu ile derleyin."
    exit 1
fi
 
# Temiz test ortami
rm -rf "$TEST_DIR"
mkdir  "$TEST_DIR"
cd     "$TEST_DIR" || exit 1
 
# ---- Test dosyalarini olustur --------------------------------
 
# Normal metin dosyalari
echo "Birinci dosya icerigi."               > t1.txt
printf "Ikinci dosya\niki satirdan olusum\n" > t2.txt
printf "Ucuncu dosya\nnewline ile biter\n"   > t3.txt
chmod 644 t1.txt t2.txt
chmod 755 t3.txt
 
# Binary (ikili) dosya - gercek null byte iceriyor
python3 -c "open('bin.dat','wb').write(bytes([0,1,2,65,66,67]))"
 
# Bos metin dosyasi
touch bos.txt
 
# Buyuk metin dosyasi (~36 KB)
python3 -c "
with open('buyuk.txt', 'w') as f:
    for i in range(1000):
        f.write(f'Satir {i:04d}: merhaba dunya, bu bir test satiridir.\n')
"
 
# ============================================================
 
baslik "1) NORMAL ARSİVLEME VE ACMA"
 
# Arsiv olustur
"$TARSAU" -b t1.txt t2.txt t3.txt -o arsiv1.sau > /dev/null 2>&1
run_test "Arsiv olusturma basarili (cikis=0)" "0" "$?"
run_test "Arsiv dosyasi diskte var mi"         "1" "$(ls arsiv1.sau 2>/dev/null | wc -l | tr -d ' ')"
 
# Arsivi ac
mkdir -p ac1
"$TARSAU" -a arsiv1.sau ac1 > /dev/null 2>&1
run_test "Arsiv acma basarili (cikis=0)"       "0" "$?"
run_test "t1.txt cikartildi mi"                "1" "$(ls ac1/t1.txt 2>/dev/null | wc -l | tr -d ' ')"
run_test "t2.txt cikartildi mi"                "1" "$(ls ac1/t2.txt 2>/dev/null | wc -l | tr -d ' ')"
run_test "t3.txt cikartildi mi"                "1" "$(ls ac1/t3.txt 2>/dev/null | wc -l | tr -d ' ')"
 
# Icerik esitligi
diff t1.txt ac1/t1.txt > /dev/null 2>&1
run_test "t1.txt icerigi orijinal ile ayni mi" "0" "$?"
diff t2.txt ac1/t2.txt > /dev/null 2>&1
run_test "t2.txt icerigi orijinal ile ayni mi" "0" "$?"
diff t3.txt ac1/t3.txt > /dev/null 2>&1
run_test "t3.txt icerigi orijinal ile ayni mi" "0" "$?"
 
# Izin korunmasi
run_test "t1.txt izni korundu mu (644)" \
         "$(stat -c '%a' t1.txt)" \
         "$(stat -c '%a' ac1/t1.txt)"
run_test "t3.txt izni korundu mu (755)" \
         "$(stat -c '%a' t3.txt)" \
         "$(stat -c '%a' ac1/t3.txt)"
 
# ============================================================
 
baslik "2) VARSAYILAN DOSYA ADI (a.sau)"
 
"$TARSAU" -b t1.txt > /dev/null 2>&1
run_test "Varsayilan a.sau olusturuldu mu" "1" "$(ls a.sau 2>/dev/null | wc -l | tr -d ' ')"
 
# ============================================================
 
baslik "3) HATA DURUMLARI"
 
# 3a. Binary dosya reddedilmeli
"$TARSAU" -b bin.dat -o x.sau > /dev/null 2>&1
BIN_CIKIS=$?
run_test "Binary dosya reddedildi mi (cikis!=0)" \
         "1" "$([ $BIN_CIKIS -ne 0 ] && echo 1 || echo 0)"
 
MESAJ=$("$TARSAU" -b bin.dat -o x.sau 2>&1)
run_test "Binary dosya icin 'uyumsuzdur' mesaji" \
         "1" "$(echo "$MESAJ" | grep -c 'uyumsuzdur')"
 
# 3b. Olmayan dosya
"$TARSAU" -b olmayan_dosya.txt -o x.sau > /dev/null 2>&1
run_test "Olmayan dosya reddedildi mi" \
         "1" "$([ $? -ne 0 ] && echo 1 || echo 0)"
 
# 3c. Gecersiz arsiv uzantisi
"$TARSAU" -a t1.txt > /dev/null 2>&1
run_test "Gecersiz uzanti (.txt) reddedildi mi" \
         "1" "$([ $? -ne 0 ] && echo 1 || echo 0)"
 
MESAJ=$("$TARSAU" -a t1.txt 2>&1)
run_test "Gecersiz arsiv icin 'uygunsuz' mesaji" \
         "1" "$(echo "$MESAJ" | grep -c 'uygunsuz')"
 
# 3d. Parametresiz calisma
"$TARSAU" > /dev/null 2>&1
run_test "Parametresiz calisma reddedildi mi" \
         "1" "$([ $? -ne 0 ] && echo 1 || echo 0)"
 
# 3e. Bilinmeyen parametre
"$TARSAU" -z > /dev/null 2>&1
run_test "Bilinmeyen parametre (-z) reddedildi mi" \
         "1" "$([ $? -ne 0 ] && echo 1 || echo 0)"
 
# ============================================================
 
baslik "4) BÜYÜK DOSYA"
 
"$TARSAU" -b buyuk.txt -o buyuk.sau > /dev/null 2>&1
run_test "Buyuk dosya arsivlendi mi"       "0" "$?"
 
mkdir -p ac_buyuk
"$TARSAU" -a buyuk.sau ac_buyuk > /dev/null 2>&1
run_test "Buyuk dosya acildi mi"           "0" "$?"
 
diff buyuk.txt ac_buyuk/buyuk.txt > /dev/null 2>&1
run_test "Buyuk dosya icerigi korundu mu"  "0" "$?"
 
# ============================================================
 
baslik "5) OTOMATİK DİZİN OLUŞTURMA"
 
"$TARSAU" -a arsiv1.sau yeni/alt/dizin > /dev/null 2>&1
run_test "Ic ice dizin olusturuldu mu"     "0" "$?"
run_test "Dosyalar ic ice dizine kondu mu" "1" \
         "$(ls yeni/alt/dizin/t1.txt 2>/dev/null | wc -l | tr -d ' ')"
 
# ============================================================
 
baslik "6) ARŞİV FORMAT DOĞRULAMA"
 
# Ilk 10 bayt sayisal olmali
BOY=$(head -c 10 arsiv1.sau | tr -d ' ')
run_test "10 baytlik boyut alani sayisal mi" \
         "1" "$(echo "$BOY" | grep -cE '^[0-9]+$')"
 
# 11. bayt '|' olmali (organizasyon bolumu baslangiCI)
CHAR11=$(dd if=arsiv1.sau bs=1 skip=10 count=1 2>/dev/null)
run_test "11. bayt pipe '|' karakteri mi" "|" "$CHAR11"
 
# Boyut alani gercek boyutla uyusmalI
GERCEK_BOY=$(wc -c < arsiv1.sau | tr -d ' ')
ORG_BOY=$(head -c 10 arsiv1.sau | tr -d ' ')
run_test "Org. boyutu arsiv boyutunu asmamali" \
         "1" "$([ "$ORG_BOY" -le "$GERCEK_BOY" ] && echo 1 || echo 0)"
 
# ============================================================
 
baslik "7) MEVCUT DİZİNE ACMA (dizin parametresi yok)"
 
mkdir -p ac_burada && cd ac_burada
"$TARSAU" -a ../arsiv1.sau > /dev/null 2>&1
run_test "Mevcut dizine acma basarili mi"  "0" "$?"
run_test "t1.txt mevcut dizinde olustu mu" "1" "$(ls t1.txt 2>/dev/null | wc -l | tr -d ' ')"
cd ..
 
# ============================================================
 
# Temizlik
cd ..
rm -rf "$TEST_DIR"
 
echo ""
echo "============================================"
if [ $FAIL -eq 0 ]; then
    printf "  SONUC: ${YESIL}%d test gecti, %d kaldi${SIFIRLA}\n" "$PASS" "$FAIL"
else
    printf "  SONUC: ${YESIL}%d gecti${SIFIRLA} | ${KIRMIZI}%d kaldi${SIFIRLA}\n" "$PASS" "$FAIL"
fi
echo "============================================"
echo ""
 