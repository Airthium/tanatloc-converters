#!/bin/sh

set -x
set -e

echo "Build"
mkdir -p build
cd build
cmake -DCOVERAGE=ON ..
build-wrapper-linux-x86-64 --out-dir bw-output make clean coverage

echo "Compute gcov reports"
gcov CMakeFiles/DXFToBRep.dir/src/DXFToBRep.cpp.gcda
gcov CMakeFiles/BRepToThreeJS.dir/src/BRepToThreeJS.cpp.gcda
gcov CMakeFiles/GmshToThreeJS.dir/src/GmshToThreeJS.cpp.gcda
gcov CMakeFiles/StepToThreeJS.dir/src/StepToThreeJS.cpp.gcda
gcov CMakeFiles/StepUnion.dir/src/StepUnion.cpp.gcda
gcov CMakeFiles/VTUToThreeJS.dir/src/VTUToThreeJS.cpp.gcda
gcovFiles=$(find CMakeFiles/tests.dir -name "*.gcda")
for file in $gcovFiles
do
  gcov "$file"
done

echo "Scanner"
sonar-scanner \
  -Dsonar.projectVersion=1.0 \
  -Dsonar.scm.exclusions.disabled=true \
  -Dsonar.cfamily.threads="$(nproc)" \
  -Dsonar.cfamily.cache.enabled=true \
  -Dsonar.cfamily.cache.path=/tmp \
  -Dsonar.organization=airthium \
  -Dsonar.projectKey=Airthium_tanatloc-dockers \
  -Dsonar.sources=src \
  -Dsonar.test.inclusions=**.test.cpp \
  -Dsonar.exclusions=src/dxflib/*,src/logger/Logger.cpp \
  -Dsonar.host.url=https://sonarcloud.io \
  -Dsonar.cfamily.build-wrapper-output=bw-output \
  -Dsonar.login=efc3fcc7ba5c34a270492718d9aa4d045854e774 \
  -Dsonar.cfamily.gcov.reportsPath=.
