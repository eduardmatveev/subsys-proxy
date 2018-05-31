#!/bin/sh
set -ex

CONTAINER="docker_test"

docker stop $CONTAINER || echo > /dev/null
docker rm $CONTAINER || echo > /dev/null
