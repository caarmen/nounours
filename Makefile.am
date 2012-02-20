AM_CFLAGS = $(JPEG_CFLAGS) -Isrc/data/include -Isrc/io/include -Isrc/common/include -Isrc/ui/include
CFLAGS =  -g -O0
bin_PROGRAMS=nounours
nounours_LDADD = $(X11_LIBS) $(JPEG_LIBS)
nounours_SOURCES=src/main.c src/common/src/nnnounours.c src/common/src/nncommon.c src/data/src/nnimage.c src/data/src/nnfeature.c src/data/src/nnanimation.c src/data/src/nntheme.c src/io/src/nncsvreader.c src/io/src/nnpropertiesreader.c src/ui/src/nnuinounours.c src/ui/src/nnuiimage.c
