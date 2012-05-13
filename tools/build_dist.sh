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


# This script builds the debian package.

me="Carmen Alvarez"
# Go to the root directory of the project
here=`dirname $0`
cd $here
here=`pwd`
top=$here/..
cd $top
version=`cat VERSION` 
arch=`dpkg-architecture -qDEB_BUILD_ARCH`

# Copy all the files we want to include in the debian package, to a dist directory
dist_dir=$top/dist/nounours-$version
mkdir -p $dist_dir
find . -type f |grep -v "dist\|.git\|debian\|tools\|repo" |xargs cp --parents -t $dist_dir
cd $dist_dir

# Convert our sources to the Debian format
echo -e "\n" | env LOGNAME="$me" DEBFULLNAME="$me" |dh_make -e c@rmen.ca -s --createorig
mkdir -p $dist_dir/debian

# Overwrite some of the debian files
cp -pr $top/debian $dist_dir/.

# Use our architecture
cat $dist_dir/debian/control |sed -e 's|@arch@|'${arch}'|g' > $dist_dir/debian/control.tmp
mv $dist_dir/debian/control.tmp $dist_dir/debian/control
cat $dist_dir/debian/Release |sed -e 's|@arch@|'${arch}'|g' > $dist_dir/debian/Release.tmp
mv $dist_dir/debian/Release.tmp $dist_dir/debian/Release

# Let debuild do all the magic to build the debian package
debuild
cd $top
