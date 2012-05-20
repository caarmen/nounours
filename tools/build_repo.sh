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


# This script creates a tarball of a debian repository
# which will host the nounours debian package.
# This should be run after build_dist.sh

if [ $# -ne 1 ]
then
 echo "usage: $0 <path to repository>"
 echo "<path to repository> is the part of the URI after the"
 echo "domain name where the repo will be hosted."
 echo "For example, if the repo will be at"
 echo "http://www.mysite.com/mystuff/repo, then the path"
 echo "is mystuff/repo"
 exit 1
fi
# Go to the root directory of the project
here=`dirname $0`
cd $here
here=`pwd`
top=$here/..
cd $top

# Set the version of this package
version=`cat VERSION` 

# Escape slashes in the repo location
repo_uri_path=$1
repo_uri_path_esc=`echo $repo_uri_path | sed -e 's|/|\\\/|g'`
repo_dir=$top/repo/

# Recreate a blank repository
rm -rf $repo_dir
mkdir -p $repo_dir

# Copy the debian package and other debian files into the repo
cp $top/dist/nounours-$version/debian/Release $top/dist/nounours_*.deb $repo_dir/
for file in $repo_dir/nounours*.deb
do 
 bname=`basename $file .deb`
 cp debian/changelog $repo_dir/$bname.changelog
done

# Generate the Packages file, with the proper Filename
# for the deb file, based on the repository directory
cd $repo_dir
dpkg-scanpackages -m . $top/debian/override ${repo_uri_path}/ > Packages
gzip -c Packages > Packages.gz

# Generate the Sources file
dpkg-scansources . > Sources
gzip -c Sources > Sources.gz
rm -f Release.gpg

# Sign the Release file
cat Release | sed -e 's|Codename: .*$|Codename: '${repo_uri_path_esc}'|g' > Release.tmp
mv Release.tmp Release
apt-ftparchive release . >> Release
gpg -abs -o Release.gpg Release
tar czvf repo.tar.gz *
cd $top
