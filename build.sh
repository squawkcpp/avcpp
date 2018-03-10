#!/bin/bash

set -e

IMAGE=spielhuus/toolchain
TAG=`if [ -z "$1" ]; then echo "master"; else echo "$1" ; fi`
PID=$(sudo docker run -itd -v $(pwd):/repo -v $(pwd)/.build:/.build $IMAGE /bin/sh)

DOCKER_EXEC="sudo docker exec $PID /bin/sh -c"

$DOCKER_EXEC "cd .build && conan install /repo --build=missing"
$DOCKER_EXEC "cmake -H/repo -B/.build -G Ninja -DCMAKE_MAKE_PROGRAM=/usr/bin/ninja -DAVCPP_TAG_VERSION=$TAG"
$DOCKER_EXEC "cmake --build /.build"
#$DOCKER_EXEC "cmake --build /.build --target package"
$DOCKER_EXEC "conan user -p $BINTRAY_KEY -r conan-cpp squawkcpp"
$DOCKER_EXEC "cd /repo && conan export . conan-cpp/latest && conan install avcpp/0.0.1@conan-cpp/latest --build=avcpp && conan upload avcpp/0.0.1@conan-cpp/latest --all -r=conan-cpp"

#sudo docker build -f docker/Dockerfile --build-arg AVCPP_TAG_VERSION=$TAG -t avcpp .

sudo docker rm -f $PID
