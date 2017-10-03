## Submission Rules

This repository is your submission for CS5400, puzzle assignment 1. Within this repository, you need to write a program that will solve the puzzle instance of Isotope Boat located in puzzles/puzzle1.txt, and output the solution in solutions/solution1.txt. See http://web.mst.edu/~tauritzd/courses/cs5400/sp2017/puzzle.html for information on the puzzle and solution formats. Once your submission is ready, you should commit and push your submission to the master branch on git-classes by the submission deadline.

Note that there is also an examplePuzzle.txt in the puzzles directory. You are not required to provide a solution for this one, but your program should still be able to solve it if necessary.

In order for your submission to count, you **MUST** change the *run.sh* script to **compile and run** your submission. 
    * Note: This script, and your program, must run on the standard MST campus linux machines.


Feel free to:
1. Add any files or folders you require.
2. Commit, branch, and clone this repo. (We'll only look at *master* for grading)



## Compiling and Running
As mentioned above, we will be using the *run.sh* bash script to compile and run your submissions. This script must work on campus linux machines. Therefore, when testing and running your submission, I suggest always using the command:
```
./run.sh
```

I've also provided you with an example of what this script might look like for a simple C++ compilation and execution, example.cpp. Feel free to reuse parts of this for your own 
project, though I suggest instead using a makefile for compilation.

## Coding Standards
Code formatting and style for C, C++, C# and Java should roughly follow [MST's C++ coding guidelines.](http://web.mst.edu/~cpp/cpp_coding_standard_v1_1.pdf)
For python, [PEP8](https://www.python.org/dev/peps/pep-0008/) is ideal.

Because this course is more about the algorithms, we won't strictly hold you to these standards as long as your code is readable.
Having said that, we want you to comment and document the core algorithms very well so that it's clear you understand them.
