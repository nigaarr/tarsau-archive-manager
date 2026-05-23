# ---- Makro Tanimlamalari (Slayt Sayfa 14) ----
CC       = gcc
CFLAGS   = -Wall -Wextra -std=c11 -pedantic -g
OBJS     = main.o archive.o utils.o
EXE      = tarsau

# ---- Ana Derleme Hedefi (Slayt Sayfa 12-13) ----
${EXE} : ${OBJS}
	${CC} ${CFLAGS} -o ${EXE} ${OBJS}
	@echo == Derleme islemi basari ile tamamlandi!.. ==

# ---- Nesne Dosyalari ve Bagimliliklari (Slayt Sayfa 12) ----
main.o : main.c common.h archive.h utils.h
	${CC} ${CFLAGS} -c main.c

archive.o : archive.c archive.h utils.h common.h
	${CC} ${CFLAGS} -c archive.c

utils.o : utils.c utils.h common.h
	${CC} ${CFLAGS} -c utils.c

# ---- Temizlik Hedefi (Slayt Sayfa 12) ----
clean :
	rm -f ${OBJS} ${EXE}
	@echo == Temizlik tamamlandi. ==