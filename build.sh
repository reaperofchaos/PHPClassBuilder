rm -rf build
mkdir build
cp sampleClass.php build/sampleClass.php
cd build
cmake ../src
cmake --build .
./PhpClassBuilder -l sampleClass.php
