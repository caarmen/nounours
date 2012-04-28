me="Carmen Alvarez"
here=`dirname $0`
cd $here
here=`pwd`
top=$here/..
cd $top
version=`cat VERSION` 
dist_dir=dist/nounours-$version
mkdir -p $dist_dir
find . -type f |grep -v "dist\|.git\|debian\|tools\|repo" |xargs cp --parents -t $dist_dir
cd $dist_dir
echo -e "\n" | env LOGNAME="$me" DEBFULLNAME="$me" |dh_make -e c@rmen.ca -s --createorig
cd $top
mkdir -p $dist_dir/debian
cp -pr debian $dist_dir/.
cd $dist_dir
debuild
dpkg-scanpackages . > Packages
cd $top
mkdir -p repo/dists/stable/main/binary-i386
cp dist/*.deb repo/.
cp $dist_dir/Packages debian/Release repo/dists/stable/main/binary-i386/.
tar czvf repo.tar.gz repo
