#!/bin/bash

set -e

IMAGE=spielhuus/toolchain
TAG=`if [ -z "$1" ]; then echo "master"; else echo "$1" ; fi`
PID=$(sudo docker run -itd -v $(pwd):/repo -v $(pwd)/.build:/.build $IMAGE /bin/sh)

DOCKER_EXEC="sudo docker exec $PID /bin/sh -c"

$DOCKER_EXEC "cd /repo && conan export . conan-cpp/latest && conan install avcpp/0.1.0@conan-cpp/latest --build=avcpp && conan upload avcpp/0.1.0@conan-cpp/latest --all -r=conan-cpp"

sudo docker rm -f $PID
