#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>

	void manMenu();
	void playHangman();

	const char *words[500] = {"arise", "vixen", "wheat", "press", "prude", "guide", "defog", "genre", "lived", "scale", "false", "twirl", "catty", "nifty", "haven", "pesky", "karma", "hedge", "arise", "music", "jolly", "alone", "bleak", "speed", "rigor", "noah", "balls", "star", "why", "bye", "dad", "who", "gem", "jam", "old", "owl", "art", "booger", "poop", "cheese", "fly", "map", "baby" };

	void instructions() {
		char input[100];
		printf("\n");
		printf("Your goal is to find all the letter to figure\n");
		printf("out the word. You will get 6 tries to guess the word.\n");
		printf("\n");	
		printf("Press Enter to continue. ");

                while (fgets(input, sizeof(input), stdin)) {
                	if (input[0] == '\n') {
                        	break;
                	} else {
                		printf("Invalid input! Press Enter dummy! ");
                        }
                }
		
	}
	

	void options() {
		printf("\n");
		printf("----Options----\n");
                printf("1. Play Game\n");
                printf("2. Instructions\n");    
                printf("3. Exit Game\n");
                printf("---------------\n");
		printf("\n");
	}

	void exitGame() {
		printf("\n");
		printf("Thanks for playing! Bye.\n");
		printf("\n");
		exit(1);
	}

	void mainMenu() {
		int choice;
                char input[100];
                options();
                printf("Choose an option: ");	
                while (1) {
                        
                	while (fgets(input, sizeof(input), stdin)) {
                                if (sscanf(input, "%d", &choice) == 1 && (choice > 0 && choice < 4)) {
                                        break;
                                } else {
					printf("\n");
                                        printf("Invalid input! Please choose an option: ");
                                }
                }
                if (choice == 1) {
      			playHangman();
		}
		if (choice == 2) {
			instructions();
			mainMenu();
		}
		if (choice == 3) {
			exitGame();
		}
	}
	}

	void gameOver () {
		
	}
		
	void startEntry() {
                printf("\n");
                printf("=======================\n");
                printf("| Welcome to Hangman! |\n");
                printf("=======================\n");
                printf("\n");
		
		mainMenu();
	}
	
	void playHangman() {
		char i;
		char arr[100];
		int c = 0;
		char input[100];
		char lineHide[100];
		char wrongLetter[100];
		char letters[100];
		char guess;
		int found = 0;
		int guessIndex = 0;
		int maxTries = 5;
		printf("\n");
		printf("------------------------\n");
		printf(" Try to guess the word!\n");
		printf("------------------------\n");
		printf("\n");
		words[0];
		srand(time(NULL));	
		int picker = rand() % 30;
		const char *pickedWord = words[picker];
		size_t length = strlen(pickedWord);
		for (int i = 0; i < length; i++) {
			letters[i] = pickedWord[i];
		}
//		printf("%s\n", pickedWord); // This is a debug statement.
		for (size_t i = 0; i < length; i++) {
			lineHide[i] = '_';
		}
		
		for (size_t i = 0; i < length; i++) {
			wrongLetter[i];
		}

		lineHide[length] = '\0';
		printf("%s", lineHide); 
		while (guessIndex != length) {
			printf("\n\n");
			printf("What letter do you pick? ");
			while (fgets(input, sizeof(input), stdin)) {
				size_t len = strlen(input);
        			if (len > 0 && input[len - 1] == '\n') {
					len--;
        			}

				if (len != 1 || !isalpha((unsigned char)input[0])) {
					printf("Please choose one letter. ");
					continue;
				}

				arr[c] = input[0];
					break;
				}

			const char* result = strchr(pickedWord, arr[c]);

			if (result != NULL) {
				int already = 0;
				for (int k = 0; k < length; k++) {
					if (lineHide[k] == arr[c]) {
						already = 1;
						break;
					}
				}	
				 
				if (already) {
					printf("\nYou already used '%c'. Try again.", arr[c]);
						continue;
				}

				for (int i = 0; i < length; i++) {
					if (pickedWord[i] == arr[c]) {
						lineHide[i] = arr[c];
						guessIndex++;
					}	
				}

				printf("\n");
				printf("Nice job!");
				printf("\n\n");
				printf("%s", lineHide);

				
			}
			else {
				int alreadyWrong = 0;
				for (int k = 0; k < 6; k++) {
					if (wrongLetter[k] == arr[c]) {
						alreadyWrong = 1;
						break;
					}
				}

				if (maxTries == 0 ) {
					printf("\n\nSorry, you lost. The word was %s.\n\n", pickedWord);
					printf("    ------\n");
					printf("    |    |\n");
					printf("    |  . o .\n");
					printf("    |  '-|-'\n");
					printf("    |   | |\n");
					printf("    |\n");
					printf("---------\n");
					mainMenu();
				} 
				if (alreadyWrong) {
					printf("\n");
                    printf("You already used the letter '%c'. Try again.", arr[c]);
					continue;
				}
				wrongLetter[c] = arr[c];

				printf("\n");	
				printf("Sorry, but that's not right. You have %d tries left.\n", maxTries);

				if (maxTries == 5) {
					printf("\n");
					printf("    ------\n");
					printf("    |    |\n");
					printf("    |    o\n");
					printf("    |\n");
					printf("    |\n");
					printf("    |\n");
					printf("---------\n");
				}
				if (maxTries == 4) {
					printf("\n");
					printf("    ------\n");
					printf("    |    |\n");
					printf("    |    o\n");
					printf("    |    |\n");
					printf("    |\n");
					printf("    |\n");
					printf("---------\n");
				}
				if (maxTries == 3) {
					printf("\n");
					printf("    ------\n");
					printf("    |    |\n");
					printf("    |  . o\n");
					printf("    |  '-|\n");
					printf("    |    \n");
					printf("    |\n");
					printf("---------\n");
				}
				if (maxTries == 2) {
					printf("\n");
					printf("    ------\n");
					printf("    |    |\n");
					printf("    |  . o .\n");
					printf("    |  '-|-'\n");
					printf("    |\n");
					printf("    |\n");
					printf("---------\n");
				}
				if (maxTries == 1) {
					printf("\n");
					printf("    ------\n");
					printf("    |    |\n");
					printf("    |  . o .\n");
					printf("    |  '-|-'\n");
					printf("    |   |\n");
					printf("    |\n");
					printf("---------\n");
				}
				maxTries--;
				printf("\n\n%s", lineHide);
				}
			}
		if (guessIndex == length) {
			printf("\n\n");
			printf("*===========================*\n");
			printf("| Congratulations, YOU won! |\n");
			printf("*===========================*\n");
			printf("\n\n");
			mainMenu();
		}
		c++;
	}
	
	int main() {
		startEntry();

	return 0;
	}
