SUBDIRS=data/icons
AM_CFLAGS = $(JPEG_CFLAGS) -Isrc/data/include -Isrc/io/include -Isrc/common/include -Isrc/ui/include -D__DATAROOT_DIR__=\"$(datarootdir)\"
# CFLAGS = -g -O0  
bin_PROGRAMS=nounours
nounours_LDADD = $(X11_LIBS) $(JPEG_LIBS) -lpthread -lm -lxpm
nounours_SOURCES=src/main.c src/common/src/nnnounours.c src/common/src/nnnounoursapp.c src/common/src/nncommon.c src/common/src/nnmath.c src/data/src/nnimage.c src/data/src/nnfeature.c src/data/src/nnanimation.c src/data/src/nntheme.c src/io/src/nnio.c src/io/src/nncsvreader.c src/io/src/nnpropertiesreader.c src/ui/src/nnuinounoursapp.c src/ui/src/nnclientmessage.c src/ui/src/nnwindow.c src/ui/src/nnuinounours.c src/ui/src/nnuiimage.c
nounoursdir=$(datarootdir)/nounours
nobase_nounours_DATA = data/icons/nounours.xpm data/nounours.xml data/themes/default/*.* data/themes/default/images/*.* data/themes/rainbow/*.* data/themes/rainbow/images/*.* data/themes/robot/*.* data/themes/robot/images/*.* data/themes/college/*.* data/themes/college/images/*.*

desktopdir=$(datadir)/applications
desktop_DATA=nounours.desktop


install-exec-hook: 
	sh deployscreensaver.sh $(XSCREENSAVER_CONFIG_PATH)
