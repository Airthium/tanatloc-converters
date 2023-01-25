#!/bin/bash

set -e

# Check SONAR_TOKEN
if [ -z "$SONAR_TOKEN" ]; then
  echo "SONAR_TOKEN environment variable not defined"
  exit 1
fi

# Check build-wrapper-linux-x86-64
type build-wrapper-linux-x86-64 >/dev/null 2>&1 || {
  echo "build-wrapper-linux-x86-64 not available"
  exit 2
}

# Check sonar-scanner
type sonar-scanner >/dev/null 2>&1 || {
  echo "sonar-scanner not available"
  exit 3
}

echo "Build"
mkdir -p build
cd build
cmake -DCOVERAGE=ON ..
build-wrapper-linux-x86-64 --out-dir ../bw-output make clean coverage
cd ..

echo "Gcov"
cd build
cppgcda=$(find . -name "*.cpp.gcda")
for file in $cppgcda; do
  path=$(dirname "$file")
  file=$(basename "$file")
  cd "$path"
  gcov -a "$file"
  cd -
done
cd ..

echo "Scan"
sonar-scanner \
  -Dsonar.projectVersion=1.0 \
  -Dsonar.scm.exclusions.disabled=true \
  -Dsonar.cfamily.threads="$(nproc)" \
  -Dsonar.cfamily.cache.enabled=true \
  -Dsonar.cfamily.cache.path=/tmp \
  -Dsonar.cfamily.gcov.reportsPath=/build \
  -Dsonar.organization=airthium \
  -Dsonar.projectKey=Airthium_tanatloc-converters \
  -Dsonar.sources=src \
  -Dsonar.test.inclusions=**.test.cpp \
  -Dsonar.exclusions=src/dxflib/*,src/logger/Logger.cpp \
  -Dsonar.host.url=https://sonarcloud.io \
  -Dsonar.cfamily.build-wrapper-output=bw-output \
  -Dsonar.cfamily.gcov.reportsPath=.
