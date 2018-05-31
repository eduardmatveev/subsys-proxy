#!/bin/sh
set -e

CONTAINER="docker_test"

mkdir -p $PWD/usr

#if windows
if [ -d "/c/Windows" ]
then
    VOLUMES="-v C:\:/c/"
fi
#if macosx
if [ -d "/Users" ]
then
    VOLUMES="-v /Users/:/Users/"
fi
docker stop $CONTAINER || echo > /dev/null
docker rm $CONTAINER || echo > /dev/null
docker image build -t $CONTAINER -f ./Dockerfile ./
docker run -it --name $CONTAINER --rm $VOLUMES $CONTAINER sh -c "cp -uard /usr/include $PWD/usr && cp -uard /usr/lib $PWD/usr"
