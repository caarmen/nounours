aclocal -I /opt/local/share/aclocal &&
automake -a &&
autoconf &&
./configure --with-jpeg-include-path=/opt/local/include --with-jpeg-lib-path=/opt/local/lib --with-xscreensaver-config-path=/opt/local/share/xscreensaver/config &&
make &&
./nounours