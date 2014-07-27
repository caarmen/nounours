# Copyright 2012 Carmen Alvarez
#
# This file is part of Nounours.
#
# Nounours is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# (at your option) any later version.
#
# Nounours is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Nounours.  If not, see <http://www.gnu.org/licenses/>.

configure_flags= 
aclocal_flags=
if [ `uname` = "Darwin" ]
then
  configure_flags="--with-jpeg-include-path=/opt/local/include --with-jpeg-lib-path=/opt/local/lib --with-xscreensaver-config-path=/opt/local/share/xscreensaver/config"
# http://xquartz.macosforge.org/trac/wiki/DeveloperInfo:
  export PKG_CONFIG="/usr/local/bin/pkg-config"
  export PKG_CONFIG_PATH="/opt/X11/share/pkgconfig:/opt/X11/lib/pkgconfig:/usr/share/pkgconfig:/usr/lib/pkgconfig:${PKG_CONFIG_PATH}"
  export ACLOCAL="aclocal -I /opt/X11/share/aclocal -I /usr/local/share/aclocal -I/opt/local/share/aclocal"
fi

aclocal $aclocal_flags &&
automake -a &&
autoconf &&
./configure $configure_flags &&
make &&
./nounours -theme data/themes/default
