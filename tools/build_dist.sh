me="Carmen Alvarez"
here=`dirname $0`
cd $here
here=`pwd`
top=$here/..
cd $top
version=`cat VERSION` 
dist_dir=dist/nounours-$version
mkdir -p $dist_dir
find . -type f |grep -v "dist\|.git\|debian\|tools" |xargs cp --parents -t $dist_dir
cd $dist_dir
env LOGNAME="$me" DEBFULLNAME="$me" dh_make -e c@rmen.ca -s --createorig
cd $top
mkdir -p $dist_dir/debian
cp -pr debian $dist_dir/.
cd $dist_dir
debuild
cd $top
