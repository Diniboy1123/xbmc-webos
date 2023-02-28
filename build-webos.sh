#!/bin/sh

set -ex

mkdir -p /opt/xbmc-deps
# source /opt/ndk/environment-setup
export SDK_PATH=/opt/ndk
export PATH="$PATH:$SDK_PATH/bin:$SDK_PATH/sbin"


if [[ "$1" == "deps" ]]; then
    # Build dependencies
    cd tools/depends/
    git clean -xfd .
    ./bootstrap
    ./configure --with-tarballs=/opt/xbmc-deps/tarballs --host=arm-webos-linux-gnueabi --prefix=/opt/xbmc-deps --with-toolchain=$SDK_PATH --disable-debug
    make -j16
    cd ../../
    #./configure --with-toolchain=$SDK_PATH --prefix=/opt/xbmc-deps --host=arm-webos-linux-gnueabi --with-rendersystem=gles --enable-debug=no --with-target-cflags='-march=armv7-a -mfloat-abi=softfp -mfpu=neon' --with-platform=webos --with-target-cxxflags='-static-libstdc++'
    # make -j16 || make -j16
    # make -C native/flatbuffers
    # TODO: run make twice because fuck you xD
fi

make -C tools/depends/target/cmakebuildsys CMAKE_EXTRA_ARGUMENTS="-DSWIG_EXECUTABLE=/opt/xbmc-deps/x86_64-linux-gnu-native/bin/swig"
cd build
make -j 16
make ipk
# extra deps: m4 default-jre flex bison
# make -C native/flatbuffers
