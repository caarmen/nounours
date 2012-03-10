#!/bin/sh
if [ $# -ne 1 ]
then
	echo "usage: $0 </path/to/xscreensaver/config>"
	exit 1
fi
xscreensaver_config_path=$1
homedir=`echo ~`
xscreensaver_local_config_file="$homedir/.xscreensaver"

if [ -d "$xscreensaver_config_path" ] ; then
	echo "Copying xscreensaver config" 
	cp data/nounours.xml $xscreensaver_config_path/.
else 
	echo "xscreensaver is not installed. $xscreensaver_config_path does not exist. Install xscreensaver or rerun ./configure with the '--with-xscreensaver-config-path <path>' option"
	exit 1
fi
 
if [ -e "$xscreensaver_local_config_file" ] 
then 
	nounourspresent=`grep -c nounours $xscreensaver_local_config_file`
	if [ $nounourspresent -eq 0 ]
	then
		echo "updating $xscreensaver_local_config_file"
		cp $xscreensaver_local_config_file "$homedir/.xscreensaver.save.`date +'%s'`" 
		echo "backed up file"
		cat "$xscreensaver_local_config_file" |sed -e "s/\(programs:.*\$\)/\1 nounours -screensaver -stretch/g" > $homedir/.xscreensaver.tmp;
		mv $homedir/.xscreensaver.tmp $xscreensaver_local_config_file
	fi 
else 
	echo "no $xscreensaver_local_config_file file found.  Run xscreensaver-demo. Then either rerun 'make install' here or manually add the following line to your ~/.xscreensaver file, in the programs: section";
	echo 'nounours -screensaver -stretch \\n\\ '
	exit 1
fi