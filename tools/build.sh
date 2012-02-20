aclocal -I /opt/local/share/aclocal &&
automake -a &&
autoconf &&
./configure --with-jpeg-include-path=/opt/local/include --with-jpeg-lib-path=/opt/local/lib &&
make &&
./nounours