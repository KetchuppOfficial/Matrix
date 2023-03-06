# HWMX - matrix

This project is a task on course "Uses and applications of C++ language" by [K.I.Vladimirov](https://github.com/tilir). The task was to implement a class representing matrix that provides a method to compute determinant.

## Requirements

The following application has to be installed:
- CMake of version 3.10 (or higher)

## How to install
```bash
git clone git@github.com:KetchuppOfficial/Matrix.git
cd Matrix
```

## How to build

### 0) Make sure you are in the root directory of the project (i.e. Matrix/)

### 1) Build the project

```bash
cmake -B build
cmake --build build [--target <tgt>]
```
**tgt** can be **unit_tests**, **test_generator**, **gauss** or **bareiss**. The name of the first target
is pretty much straightforward. The second target is a program that generates a matrix of a certain size
with a certain determinant. Last two targets are drivers which input is a number **N** (the size of a matrix)
and then **N * N** numbers (matrix itself) (on stdin). The output is the determinant (on stdout). 

Last three targets are recommended to be built simultaneously as they are a part of the end-to-end
testing system.

If --target option is omitted, all targets will be built.

### 3) Install executable files (optional)

You can install executable files in any directory you wish:

```bash
cmake --install build [--prefix <prefix>]
```

**prefix** can be any directory you want to install in.

If --prefix option is omitted, executable files will be installed in Matrix/bin/ directory.

## How to generate tests

If you want to run some tests on the determinant calculation algorithm, go to
[Matrix/tests/end_to_end](./tests/end_to_end) directory. There you will find a special script **det.sh** provided for 
such purpose.

Let **N** be the number of tests you want to run, **S** - the size of matrices, **mD** - maximal 
absolute value of the determinant, then command sequence:
```bash
./det.sh mode N S mD
```
generates **N** random matrices of size **S** with determinants in range [**-mD**; **mD**]. The
determinants are saved in files **answers_i** (**i** in range [1; **N**]). After that this script
runs determinant calculation algorithm on those tests and saves results in files **result_i**. 
Finally, both sets of values are compared. If values in any pair **answer_i**/**result_i** are 
different, then this test is considered "failed". It is considered "passed" otherwise.

**mode** argument has to be of one of 2 values only: **gauss** or **bareiss**. It specifies what algorithm will be tested.
