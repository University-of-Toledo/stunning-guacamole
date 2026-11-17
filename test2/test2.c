#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int checkGuess(int guess, int secret) {
	if(guess <secret) {
		return -1;
	} else if (guess > secret) {
		return 1;
	} else {
		return 0;
	}
}
int main(void) {
	srand(time(NULL));
	int secret = (rand() % 250) +1;
	int guess;
	int attempts = 0;
	int maxAttempts = 10;
	int won = 0;

	printf("Welcome to the Number Guessing Game!\n");
	printf("I have chosen a number between 1 and 250. You have %d tries to guess it.\n\n", maxAttempts);

	for(attempts = 1; attempts <= maxAttempts; attempts++) {
		printf("Attempt %d - Enter your guess: ", attempts);
		scanf("%d", &guess);
		int result = checkGuess(guess, secret);
		if (result == -1) {
			printf("Too low.\n");
		} else if (result == 1) {
			printf("Too high.\n");
		} else {
			printf("Correct!\n");
			won = 1;
			break;
		}
	}
if (won) {
	printf("It took you %d attempts to win!\n", attempts);
	} else {
		printf("You lost! The secret number was %d.\n", secret);
	}
	return 0;
}

