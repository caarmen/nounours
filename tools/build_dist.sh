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
repo_dir=$top/repo/
cd $top
mkdir -p $repo_dir/conf
cp debian/Release $repo_dir/conf/distributions
cd $repo_dir
reprepro --ask-passphrase -Vb . includedeb stable $top/dist/nounours_1.0-1_i386.deb
cd $top
tar czvf repo.tar.gz repo
