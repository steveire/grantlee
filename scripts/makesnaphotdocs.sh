#! /bin/bash

if [ -n "$1" ]
then
  version=$1
else
  echo "This script takes one argument- a branch name."
  exit
fi

tempDir="/tmp/grantlee-snapshot"

if [ ! -d "$tempDir" ]
then
  mkdir $tempDir
fi

echo Creating $tempDir/grantlee-$version.tar.gz

git archive --format=tar --prefix=grantlee-$version/ $version | gzip > $tempDir/grantlee-$version.tar.gz

rm -rf $tempDir/grantlee-$version/
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

cp $oldDir/scripts/qttags.tag $tempDir/grantlee-$version/scripts
cp $oldDir/scripts/libstdc++.tag $tempDir/grantlee-$version/scripts
cp $oldDir/scripts/libstdc++-longnames.tag $tempDir/grantlee-$version/scripts

echo Building
cmake -DCMAKE_INSTALL_PREFIX=../prefix ..

qch_version_string=`echo $version | sed 's/\.//g'`

echo "Creating Docs"
make docs

mkdir doxtmp
mv apidox doxtmp/$version-snapshot
mv doxtmp apidox
tar -czf $oldDir/apidox.tar.gz apidox

cd $oldDir
