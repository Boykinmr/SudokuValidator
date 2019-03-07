------------------------------------------------------------------------
This is the project README file. Here, you should describe your project.
Tell the reader (someone who does not know anything about this project)
all he/she needs to know. The comments should usually include at least:
------------------------------------------------------------------------

PROJECT TITLE:              	Sudoku Puzzle Validator

PURPOSE OF PROJECT:         	This project uses multiple program threads to test a sudoku puzzle solution for validity.  Each thread takes a single row, column or 3x3 segment of the puzzle solution and checks that this section is correctly solved.  Any thread that detects a failure in it's segment will result in a solution failure.

VERSION or DATE:            	09/20/2018

HOW TO START THIS PROJECT:	This was programmed to run on a Unix (Bash for Windows) system.  You will need to download and install the pthread libraries before this will compile and run correctly.  You will also need to compile it with the console command:
gcc SudokuValidator.c -o <name of unix executable> -lpthread

AUTHORS:                    	Michael R. Boykin

USER INSTRUCTIONS:          	SudokuPuzzle.txt is the file that is to be tested.  It is correctly solved.  The text files named WRONG are test cases in which the test should fail.  Since it wasn't part of the assignment, I did not implement a prompt to choose the name of the file to import and check.  You will have to rename the file to be tested to SudokuValidator.txt in order to run it.  This will possibly be visited in a future update.