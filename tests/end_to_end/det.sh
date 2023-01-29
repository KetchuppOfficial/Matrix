#!/bin/bash

# argv[1]: the number of matrices
# argv[2]: the size of matrices
# argv[3]: the determinant of matrices

green="\033[1;32m"
red="\033[1;31m"
default="\033[0m"

top_dir="../../"
build_dir="${top_dir}build/"    

test_generator="test_generator"
test_driver="test_driver"

function Mkdir
{
    rm -rf $1
    mkdir $1
}

function Build_Tests
{   
    cmake ${top_dir} -B ${build_dir}
    
    echo "Building test generator..."
    cmake --build ${build_dir} --target ${test_generator}
    echo -en "\n"

    echo "Building test driver..."
    cmake --build ${build_dir} --target ${test_driver}
    echo -en "\n"
}

function Run_Tests
{
    local test_generator="${build_dir}tests/end_to_end/${test_generator}"
    local test_driver="${build_dir}tests/end_to_end/${test_driver}"

    local test_dir="tests/"
    local ans_dir="answers/"
    local res_dir="results/"
    
    Mkdir ${test_dir}
    Mkdir ${ans_dir}
    
    echo "Generating tests and answers..."
    ${test_generator} ${test_dir} ${ans_dir} ${n_matrices} ${size} ${det}
    echo -en "\n"

    Mkdir ${res_dir}

    echo "Testing..."
    for ((i = 1; i <= ${n_matrices}; i++))
    do
        ${test_driver} < ${test_dir}test_${i} > ${res_dir}result_${i}

        echo -n "Test ${i}: "
        if diff -Z ${ans_dir}/answer_${i} ${res_dir}result_${i} > /dev/null
        then
            echo -e "${green}passed${default}"
        else
            echo -e "${red}failed${default}"
        fi
    done
}

if [ $# -ne 3 ]
then
    echo "Testing script requires exactly 3 arguments"
else
    if [ $1 -le 0 ]
    then
        echo "The number of matrices has to be a positive integer number"
    else
        if [ $2 -le 0 ]
        then
            echo "The size of matrices has to be a positive integer number"
        else
            n_matrices=$1
            size=$2
            det=$3

            Build_Tests
            Run_Tests
        fi
    fi
fi
