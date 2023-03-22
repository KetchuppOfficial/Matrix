#!/bin/bash

# argv[1]: the algorithm of determinant calculation ("gauss" or "bareiss")
# argv[2]: the number of matrices
# argv[3]: the size of matrices
# argv[4]: maximal absolute value of the determinant of matrices

green="\033[1;32m"
red="\033[1;31m"
default="\033[0m"

top_dir="../../"
build_dir="${top_dir}build/"    

test_generator="test_generator"

function Mkdir
{
    rm -rf $1
    mkdir $1
}

function Build_Tests
{   
    local det_alg=$1

    cmake ${top_dir} -B ${build_dir}
    
    echo "Building test generator..."
    cmake --build ${build_dir} --target ${test_generator}
    echo -en "\n"

    echo "Building test driver for ${det_alg} algorithm..."
    cmake --build ${build_dir} --target ${det_alg}
    echo -en "\n"
}

function Run_Tests
{
    local det_alg=$1
    local test_generator="${build_dir}tests/end_to_end/${test_generator}"
    local test_driver="${build_dir}tests/end_to_end/${det_alg}"

    local test_dir="tests_${det_alg}/"
    local ans_dir="answers_${det_alg}/"
    local res_dir="results_${det_alg}/"
    
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

if [ $# -ne 4 ]
then
    echo "Testing script requires exactly 3 arguments"
else
    det_alg=$1

    if [ $det_alg = "gauss" ] || [ $det_alg = "bareiss" ]
    then        
        if [ $2 -le 0 ]
        then
            echo "The number of matrices has to be a positive integer number"
        else
            if [ $3 -le 0 ]
            then
                echo "The size of matrices has to be a positive integer number"
            else
                n_matrices=$2
                size=$3
                det=$4

                Build_Tests $det_alg
                Run_Tests $det_alg
            fi
        fi
    else
        echo "There is no testing mode with name $1"
    fi
fi
