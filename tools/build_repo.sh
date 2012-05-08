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
rm -f Release.gpg
cat Release | sed -e 's/Codename: .*$/Codename: '${REPO_PATH}'/g' > Release.tmp
mv Release.tmp Release
apt-ftparchive release . >> Release
gpg -abs -o Release.gpg Release
tar czvf repo.tar.gz *
cd $top
