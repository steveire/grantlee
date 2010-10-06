#! /bin/bash

if [ -n "$1" ]
then
  version=$1
else
  echo "This script takes one required argument- a version string, and an optional argument for the tarball name fragment."
  exit
fi
if [ -n "$2" ]
then
  name_fragment=$2
else
  name_fragment=$1
fi

tempDir="/tmp/grantlee-release"

if [ ! -d "$tempDir" ]
then
  mkdir $tempDir
fi

echo Creating $tempDir/grantlee-$name_fragment.tar.gz

git archive --format=tar --prefix=grantlee-$name_fragment/ $version | gzip > $tempDir/grantlee-$name_fragment.tar.gz

rm -rf $tempDir/grantlee-$name_fragment/
echo Unpacking to $tempDir/grantlee-$name_fragment
tar -C $tempDir -xvf $tempDir/grantlee-$name_fragment.tar.gz

if [ "$?" -ne "0" ]
then
  echo "Error creating tar archive."
  exit
fi

oldDir=$PWD

echo Creating build directory
mkdir -p $tempDir/grantlee-$name_fragment/build && cd $tempDir/grantlee-$name_fragment/build

cp $oldDir/scripts/qttags.tag $tempDir/grantlee-$name_fragment/scripts

echo Building.
cmake -DCMAKE_INSTALL_PREFIX=../prefix ..
make && make install

echo "Testing"
cd tests && ctest


echo Copying archive to $oldDir/grantlee-$name_fragment.tar.gz
cp $tempDir/grantlee-$name_fragment.tar.gz $oldDir

qch_version_string=`echo $name_fragment | sed 's/\.//g'`

echo "Creating Docs"
make docs
sed -i 's/at your option, any later version.<\/p>/at your option, any later version.<\/p>\
<p>A version of this documentation suitable for viewing in Qt Assistant is available <a href\=\"http:\/\/www.grantlee.org\/grantlee-'${qch_version_string}'.qch\">here<\/a>.<\/p>/' apidox/index.html

tar -czf $oldDir/apidox.tar.gz apidox

cp $tempDir/grantlee-$name_fragment/build/grantlee-$qch_version_string.qch $oldDir

cd $oldDir
gpg --detach-sign --armor grantlee-$name_fragment.tar.gz
