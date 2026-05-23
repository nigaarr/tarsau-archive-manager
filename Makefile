# ============================================================
#  Makefile  –  tarsau projesi
# ============================================================
 
# Derleyici ve bayraklar
CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 -pedantic
 
# Hedef calistirilabilir dosya
TARGET  = tarsau
 
# Kaynak ve nesne dosyalari
SRCS    = main.c archive.c utils.c
OBJS    = $(SRCS:.c=.o)
 
# Baslik dosyalari (bagimliliklarda kullanilir)
HDRS    = common.h archive.h utils.h
 
# ---- Varsayilan hedef: derleme --------------------------------
all: $(TARGET)
 
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^
	@echo "Derleme tamamlandi: $(TARGET)"
 
# ---- Nesne dosyasi kurallari ----------------------------------
main.o:    main.c    $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@
 
archive.o: archive.c archive.h utils.h common.h
	$(CC) $(CFLAGS) -c $< -o $@
 
utils.o:   utils.c   utils.h common.h
	$(CC) $(CFLAGS) -c $< -o $@
 
# ---- Temizlik -------------------------------------------------
clean:
	rm -f $(OBJS) $(TARGET)
	@echo "Temizlik tamamlandi."
 
# ---- Yeniden derleme ------------------------------------------
rebuild: clean all
 
# ---- Basit kurulum (opsiyonel) --------------------------------
install: $(TARGET)
	cp $(TARGET) /usr/local/bin/$(TARGET)
	@echo "$(TARGET) /usr/local/bin/ dizinine kuruldu."
 
uninstall:
	rm -f /usr/local/bin/$(TARGET)
	@echo "$(TARGET) kaldirildi."