#! /bin/bash

if [ -n "$1" ]
then
  version=$1
else
  echo "This script takes one argument- a version string."
  exit
fi

tempDir="/tmp/grantlee-release"

if [ ! -d "$tempDir" ]
then
  mkdir $tempDir
fi

echo Creating $tempDir/grantlee-$version.tar.gz

git archive --format=tar --prefix=grantlee-$version/ v_$version | gzip > $tempDir/grantlee-$version.tar.gz

echo Unpacking to $tempDir/grantlee-$version
tar -C $tempDir -xvf $tempDir/grantlee-$version.tar.gz

if [ "$?" -ne "0" ]
then
  echo "Error creating tar archive."
  exit
fi

oldDir=$PWD

echo Creating build directory
mkdir -p $tempDir/grantlee-$version/build && cd $tempDir/grantlee-$version/build

echo Building.
cmake -DCMAKE_INSTALL_PREFIX=../prefix ..
make && make install

echo "Testing" 
cd tests && ctest


echo Copying archive to $oldDir/grantlee-$version.tar.gz
cp $tempDir/grantlee-$version.tar.gz $oldDir

