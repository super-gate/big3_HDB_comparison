# 실행방법

```bash
cd ./big3_HDB_comparison
rm -r build
mkdir build
cd build
```
HElib이 library install 되어있다면
```bash
cmake ..
```
local install이라면
```bash
cmake -Dhelib_DIR=/PATH/helib_install/helib_pack/share/cmake/helib ..
```
이후
```bash
make
```
하면 comparison library install 완료.

main코드를 돌리고 싶으면
```bash
cd ./big3_HDB_comparison
cd test
rm -r build
mkdir build
cd build
```
위에와 똑같이 cmake를 돌려주면 된다. 이후 컴파일된 binary는 'Test_dir' 폴더안에 생성.