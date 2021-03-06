## Submission Rules

This is your submission repository for CS5400, puzzle assignment 2. Within this repository, you need to write a program that will solve two puzzle instances of Isotope Boat using Iterative-Deepening Depth-First Graph Search (ID-DFGS). The puzzle instances are located in puzzles/puzzle1.txt and puzzles/puzzle2.txt, and the solutions should be placed in solutions/solution1.txt and solutions/solution2.txt, respectively. See http://web.mst.edu/~tauritzd/courses/cs5400/sp2017/puzzle.html for information on the puzzle and solution formats. Once your submission is ready, you should commit and push your submission to the master branch on git-classes by the submission deadline.

Note that there is also an examplePuzzle.txt in the puzzles directory. You are not required to provide a solution for this one, but your program should still be able to solve it.

Your repository also includes a new file, named "readyToSubmit.txt". The first line of this file just contains the word "no". When you are ready to submit, change the "no" to a "yes", and submit your files as normal (including that file). This is how you will let us know whether your repository is ready to be graded on the due date. If you do need to turn in the assignment late, then this will also let us know when you are ready to submit. Hopefully this will be easier than needing to e-mail us to tell us about late submissions.

You may copy over and re-use code from puzzle assignment 1, but your final submission for puzzle assignment 2 should be an ID-DFGS algorithm.

In order for your submission to count, you **MUST** change the *run.sh* script to **compile and run** your submission. 
    * Note: This script, and your program, must run on the standard MST campus linux machines, without any additional command line arguments.


Feel free to:
1. Add any files or folders you require.
2. Commit, branch, and clone this repo. (We'll only look at *master* for grading)



## Compiling and Running
As mentioned above, we will be using the *run.sh* bash script to compile and run your submissions. This script must work on campus linux machines, without any additional command line arguments. Therefore, when testing and running your submission, I suggest always using the command:
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
