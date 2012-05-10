SUBDIRS=icons

desktopdir=$(datadir)/applications
desktop_DATA=nounours.desktop nounours-college.desktop nounours-rainbow.desktop nounours-robot.desktop nounours-xscreensaver.desktop

xscreensaverdir=$(XSCREENSAVER_CONFIG_PATH)
xscreensaver_DATA=nounours.xml

nounoursdatadir=$(datarootdir)/nounours/data
nobase_nounoursdata_DATA=deployscreensaverconfig.sh nounours.xscreensaver themes/default/*.* themes/default/images/*.* themes/rainbow/*.* themes/rainbow/images/*.* themes/robot/*.* themes/robot/images/*.* themes/college/*.* themes/college/images/*.*

nounours-xscreensaver.desktop deployscreensaverconfig.sh: 
	cat $@.in | sed -e 's|@datarootdir[@]|$(datarootdir)|' > $@	
