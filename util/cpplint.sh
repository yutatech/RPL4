SCRIPT_DIR=$(cd $(dirname $0); pwd)
RPL4_DIR=$SCRIPT_DIR/../
cd $RPL4_DIR

echo $RPL4_DIR

cpplint --repository include \
  include/*.hpp \
  include/*/*.hpp \
  include/*/*/*.hpp \
  include/*/*/*/*.hpp \
  src/*.cpp  \
  src/*/*.cpp 