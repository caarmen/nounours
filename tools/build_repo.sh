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

if [ $# -ne 1 ]
then
 echo "usage: $0 <path to repository>"
 exit 1
fi
VERSION=1.0-1_i386
REPO_PATH=$1
REPO_PATH=`echo $REPO_PATH | sed -e 's|/|\\\/|g'`
echo "repo: ${REPO_PATH}"
here=`dirname $0`
cd $here
here=`pwd`
top=$here/..
repo_dir=$top/repo/
cd $top
rm -rf $repo_dir
mkdir -p $repo_dir
cp debian/Release $top/dist/nounours_$VERSION.deb $repo_dir/
cp debian/changelog $repo_dir/nounours_$VERSION.changelog
cd $repo_dir
dpkg-scanpackages . > Packages
cat Packages |sed -e 's/Filename: \./Filename: '${REPO_PATH}'/g' > Packages.tmp
mv Packages.tmp Packages
gzip -c Packages > Packages.gz
dpkg-scansources . > Sources
gzip -c Sources > Sources.gz
rm -f Release.gpg
cat Release | sed -e 's/Codename: .*$/Codename: '${REPO_PATH}'/g' > Release.tmp
mv Release.tmp Release
apt-ftparchive release . >> Release
gpg -abs -o Release.gpg Release
tar czvf repo.tar.gz *
cd $top
