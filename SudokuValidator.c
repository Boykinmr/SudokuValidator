/*
* Name:  Michael R. Boykin
* Class: CS3600-001
* Date:  09/20/2018
*/

#include <stdio.h>
#include <pthread.h>

typedef int bool;
#define TRUE 1
#define FALSE 0
#define NUM_THREADS 27

int workerThreads[NUM_THREADS];
int sudokuPuzzle[9][9]; //Will contain the values of the puzzle to check

//An array of columns, rows and grids respectively to be checked for errors
bool tid_columns[9];
bool tid_rows[9];
bool tid_subgrid[9];

char fileName[] = "SudokuPuzzle.txt";

void *worker(void *param); // the worker thread
void fillAndShowPuzzleArray();

typedef struct {
	int topRow;
	int bottomRow;
	int leftColumn;
	int rightColumn;
} worker_checkRange;

void main(int argc, char *argv[])
{
	//main Method
	int counter;
	pthread_t tid[NUM_THREADS];
	worker_checkRange workerID[NUM_THREADS];

	fillAndShowPuzzleArray();

	//Populate the column workers
	for (counter = 0; counter < 9; counter++) {
		workerID[counter].topRow = 0;
		workerID[counter].bottomRow = 8;
		workerID[counter].leftColumn = counter;
		workerID[counter].rightColumn = counter;
	}

	//Populate the row workers
	for (counter = 9; counter < 18; counter++) {
		workerID[counter].topRow = counter - 9;
		workerID[counter].bottomRow = counter - 9;
		workerID[counter].leftColumn = 0;
		workerID[counter].rightColumn = 8;
	}

	int sgRow, sgCol;
	counter = 18;

	//populate the subgrid workers
	for (sgRow = 0; sgRow < 3; sgRow++) {
		for (sgCol = 0; sgCol < 3; sgCol++) {
			workerID[counter].topRow = (sgRow * 3);
			workerID[counter].bottomRow = (sgRow * 3) + 2;
			workerID[counter].leftColumn = (sgCol * 3);
			workerID[counter].rightColumn = (sgCol * 3) + 2;
			counter++;
		}
	}

	for (counter = 0; counter < NUM_THREADS; counter++) {
		pthread_create(&(tid[counter]), NULL, worker, &(workerID[counter]));
	}

	for (counter = 0; counter < NUM_THREADS; counter++) {
		pthread_join(tid[counter], NULL);
	}

	bool puzzleResult = TRUE;
	for (counter = 0; counter < 9; counter++) {
		if (tid_columns[counter] == FALSE) { puzzleResult = FALSE; }
		if (tid_rows[counter]    == FALSE) { puzzleResult = FALSE; }
		if (tid_subgrid[counter] == FALSE) { puzzleResult = FALSE; }
	}
	char* result = (puzzleResult == TRUE) ? "valid!" : "invalid!";
	printf("Sudoku Puzzle: %s, %s\n", fileName, result);
}

void fillAndShowPuzzleArray() {
	char fileBuffer[82];
	const int ROW_LENGTH = 9, COL_SUBGRID_LENGTH = 3, SUBGRID_ROW_QTY = 27
		, ZERO_INT_VALUE = 48;

	FILE *puzzleFile = fopen(fileName, "r");
	int i = 0;

	while (fscanf(puzzleFile, "%s", &fileBuffer[i]) == 1) {
		sudokuPuzzle[i / ROW_LENGTH][i % ROW_LENGTH] =
			fileBuffer[i] - ZERO_INT_VALUE;
		i++;

	}

	for (i = 0; i < 81; i++) {
		if (i % ROW_LENGTH == 0) { printf("\n"); }
		if (i % SUBGRID_ROW_QTY == 0) { printf("\n"); }
		if (i % COL_SUBGRID_LENGTH == 0) { printf(" "); }
		printf("%d ", sudokuPuzzle[i / ROW_LENGTH][i % ROW_LENGTH]);
	}
	printf("\n\n");
}

void *worker(void *param)
{
	//Generate worker threads
	worker_checkRange *setupStruct;
	pthread_t self;
	
	setupStruct = (worker_checkRange *)param;

	self = pthread_self();
	int selfCheck[] = { 1,2,3,4,5,6,7,8,9 };

	int i, i2, counter, found;
	int topCheck, botCheck, leftCheck, rightCheck;

	topCheck   = setupStruct->topRow;
	botCheck   = setupStruct->bottomRow;
	leftCheck  = setupStruct->leftColumn;
	rightCheck = setupStruct->rightColumn;

	bool validSudoku = FALSE;
	int numFound = 0;
	for (i = topCheck; i <= botCheck; i++) {
		for (i2 = leftCheck; i2 <= rightCheck; i2++) {
			for (counter = 0; counter < 9; counter++) {
				if (sudokuPuzzle[i][i2] == selfCheck[counter]) {
					numFound++;
					selfCheck[counter] = 0;
				}
			}
		}
	}
	
	int gridNum = 0;

	if (numFound == 9) {
		validSudoku = TRUE;
	}
	char* result = (validSudoku == TRUE) ? "valid!" : "invalid!";
	char* testType[] = { "Row:    " , "Column: ", "Subgrid:" };
	
	int  testResult;

	if (topCheck == botCheck) {
		tid_rows[topCheck] = validSudoku;
		testResult = 0;
	} else if (leftCheck == rightCheck) {
		tid_columns[leftCheck] = validSudoku;
		testResult= 1;
	} else {
		gridNum += ((topCheck / 3) * 3);
		gridNum += (leftCheck / 3);
		tid_subgrid[gridNum] = validSudoku;
		testResult = 2;
	}
	printf("%s %lX TRow: %d Brow: %d LCol: %d RCol %d %s\n"
		, testType[testResult], (unsigned long)self, topCheck, botCheck, leftCheck, rightCheck, result);

	pthread_exit(0);
}