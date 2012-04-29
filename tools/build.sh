configure_flags= 
aclocal_flags=
if [ `uname` = "Darwin" ]
then
  configure_flags="--with-jpeg-include-path=/opt/local/include --with-jpeg-lib-path=/opt/local/lib --with-xscreensaver-config-path=/opt/local/share/xscreensaver/config"
  aclocal_flags="-I /opt/local/share/aclocal"
fi

aclocal $aclocal_flags &&
automake -a &&
autoconf &&
./configure $configure_flags &&
make &&
./nounours -theme data/themes/default
