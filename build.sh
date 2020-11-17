#!/bin/bash
project_path=$(cd `dirname $0`; pwd);
project_name="${project_path##*/}";
echo $project_path;
echo $project_name;
cd `dirname $0`;
echo "mkdir -p build & cd build";
mkdir -p build;
cd build;
echo "cmake .. & make";
cmake ..;
make;
