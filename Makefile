CC       = gcc
CFLAGS   = -Wall -Wextra -std=c11 -pedantic -g -Iinclude


SRCDIR   = src
INCDIR   = include
LIBDIR   = lib
BINDIR   = bin


EXE      = $(BINDIR)/tarsau
OBJS     = $(LIBDIR)/main.o $(LIBDIR)/archive.o $(LIBDIR)/utils.o



$(EXE) : $(LIBDIR) $(BINDIR) $(OBJS)
	$(CC) $(CFLAGS) -o $(EXE) $(OBJS)
	@echo "== Derleme işlemi başarı ile tamamlandı!.. [Çıktı: $(EXE)] =="


$(LIBDIR):
	mkdir -p $(LIBDIR)

$(BINDIR):
	mkdir -p $(BINDIR)




$(LIBDIR)/main.o : $(SRCDIR)/main.c $(INCDIR)/common.h $(INCDIR)/archive.h $(INCDIR)/utils.h
	$(CC) $(CFLAGS) -c $(SRCDIR)/main.c -o $(LIBDIR)/main.o


$(LIBDIR)/archive.o : $(SRCDIR)/archive.c $(INCDIR)/archive.h $(INCDIR)/utils.h $(INCDIR)/common.h
	$(CC) $(CFLAGS) -c $(SRCDIR)/archive.c -o $(LIBDIR)/archive.o


$(LIBDIR)/utils.o : $(SRCDIR)/utils.c $(INCDIR)/utils.h $(INCDIR)/common.h
	$(CC) $(CFLAGS) -c $(SRCDIR)/utils.c -o $(LIBDIR)/utils.o


clean :
	rm -f $(OBJS) $(EXE)
	@echo "== Temizlik tamamlandı. =="