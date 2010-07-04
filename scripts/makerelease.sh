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

echo Building.
cmake -DCMAKE_INSTALL_PREFIX=../prefix ..
make && make install

echo "Testing" 
cd tests && ctest


echo Copying archive to $oldDir/grantlee-$name_fragment.tar.gz
cp $tempDir/grantlee-$name_fragment.tar.gz $oldDir

cd $oldDir
md5sum grantlee-$name_fragment.tar.gz > grantlee-$name_fragment-md5.txt

