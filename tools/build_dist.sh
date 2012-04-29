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
dpkg-scansources . > Sources
cd $top
repo_dir=repo/dists/stable/main
repo_bin_dir=$repo_dir/binary-i386
repo_src_dir=$repo_dir/source
mkdir -p $repo_bin_dir
mkdir -p $repo_src_dir
cp dist/*.deb repo/.
cp $dist_dir/Packages debian/Release $repo_bin_dir/.
cp $dist_dir/Sources $repo_src_dir/.
tar czvf repo.tar.gz repo
