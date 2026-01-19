BUILD_DIR="build"
EXE_FILE="Chess"
COMPILE_CONFIG="$1"

set -e
echo "----------------"
echo "Start building"

if [ "$COMPILE_CONFIG" = "Release" ]; then 
    echo "Config for Release mode "
    cmake -S . -B $BUILD_DIR -DCMAKE_BUILD_TYPE=Release 
else
    echo "Default debug mode"
    cmake -S . -B $BUILD_DIR -DCMAKE_BUILD_TYPE=Debug
    COMPILE_CONFIG="Debug"
fi

echo "Compiling source" 
cmake --build $BUILD_DIR --config $COMPILE_CONFIG --parallel 4

echo "-----------------"
echo "Running Game"

if [ -f "$BUILD_DIR/$EXE_FILE" ]; then
    cd $BUILD_DIR
    ./$EXE_FILE 
else
    echo "didn't find executable file"
    exit 1
fi