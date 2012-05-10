SUBDIRS=data .
AM_CFLAGS = $(JPEG_CFLAGS) -Isrc/data/include -Isrc/io/include -Isrc/common/include -Isrc/ui/include -D__DATAROOT_DIR__=\"$(datarootdir)\"
#CFLAGS = -g -O0  
bin_PROGRAMS=nounours
nounours_LDADD = $(X11_LIBS) $(JPEG_LIBS) -lpthread -lm -lXpm
nounours_SOURCES=src/main.c src/common/src/nnnounours.c src/common/src/nnnounoursapp.c src/common/src/nncommon.c src/common/src/nnmath.c src/data/src/nnimage.c src/data/src/nnfeature.c src/data/src/nnanimation.c src/data/src/nntheme.c src/io/src/nnio.c src/io/src/nncsvreader.c src/io/src/nnpropertiesreader.c src/ui/src/nnuinounoursapp.c src/ui/src/nnclientmessage.c src/ui/src/nnwindow.c src/ui/src/nnuinounours.c src/ui/src/nnuiimage.c

