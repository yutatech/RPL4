SCRIPT_DIR=$(cd $(dirname $0); pwd)
RPL4_DIR=$SCRIPT_DIR/../
BUILD_DIR=build

cd $RPL4_DIR
mkdir -p $BUILD_DIR
cd $BUILD_DIR

cmake -DRPL4_BUILD_EXAMPLE=ON \
      -DRPL4_LOG_LEVEL=OFF \
      ..

make -j 8