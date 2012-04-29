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
repo_dir=$top/repo/dists/stable/main
repo_bin_dir=$repo_dir/binary-i386
repo_src_dir=$repo_dir/source
cd $top
mkdir -p $repo_bin_dir
mkdir -p $repo_src_dir
cd $top/dist
dpkg-scanpackages . > $repo_bin_dir/Packages
dpkg-scansources . > $repo_src_dir/Sources
cd $top
cp dist/*.deb repo/.
cp debian/Release $repo_bin_dir/.
tar czvf repo.tar.gz repo
