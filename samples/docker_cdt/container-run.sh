#!/bin/sh
set -ex

CONTAINER="docker_test"

#if windows
if [ -d "/c/Windows" ]
then
    VOLUMES='-v C:\:/c/'
fi
#if macosx
if [ -d "/Users" ]
then
    VOLUMES='-v /Users/:/Users/ -v /Applications/:/Applications/'
fi

docker stop $CONTAINER || echo > /dev/null
docker run -itd --name $CONTAINER -p 7777:7777 -p 8081:8080 --rm --cap-add sys_ptrace --publish-all $VOLUMES $CONTAINER
docker exec -it $CONTAINER sh
