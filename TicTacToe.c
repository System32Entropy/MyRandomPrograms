#include <stdio.h>
#include <stdlib.h>
#define SIDE 3
#define Player1 1
#define Player2 2
#define PLAYER1MOVE 'x'
#define PLAYER2MOVE 'o'

void mainMenu();
int rowCrossed(char board[][SIDE]);
int columnCrossed(char board[][SIDE]);
int diagonalCrossed(char board[][SIDE]);
int gameOver(char board[][SIDE]);
void playTicTacToe(int whoseTurn);
void initialize(char board[][SIDE], int moves[]);

	void instructions() {
		int choice;
		char input[100];
		printf("\n");
		printf("Player 1 starts by placing a x\n");
		printf("followed be player 2 placing an o.\n");
		printf("To place on the board, use the coordinates as shown below:\n"); printf("\n");	
		printf("-------------\n");
		printf("| 1 | 2 | 3 |\n");
		printf("-------------\n");
		printf("| 4 | 5 | 6 |\n");
		printf("-------------\n");
		printf("| 7 | 8 | 9 |\n");
                printf("-------------\n\n");	
		printf("Press Enter to continue. ");

                while (fgets(input, sizeof(input), stdin)) {
                	if (input[0] == '\n') {
                        	break;
                	} else {
                		printf("Invalid input! Press Enter dummy! ");
                        }
                }
		
	}
	
	void declareWinner(int whoseTurn) {
   		 if (whoseTurn == Player1) {
        		printf("Player 1 has won!\n");
			mainMenu();
		}
		else {
        		printf("Player 2 has won!\n");
			mainMenu();
		}
	}

	void initialize(char board[][SIDE], int moves[]) {
		for (int i = 0; i < SIDE; i++)
			for (int j = 0; j < SIDE; j++)
				board[i][j] = ' ';

		for (int i = 0; i < SIDE * SIDE; i++)
       			moves[i] = 0;
	}

	void showBoard(char board[][SIDE], int whoseTurn) {
		if (whoseTurn == Player1) {
			printf("\n\n");
			printf("Player 1\n");
			printf("-------------\n");
			printf("| %c | %c | %c |\n", board[0][0],board[0][1],board[0][2]);
			printf("-------------\n");
			printf("| %c | %c | %c |\n", board[1][0],board[1][1],board[1][2]);
			printf("-------------\n");
			printf("| %c | %c | %c |\n", board[2][0],board[2][1],board[2][2]);
			printf("-------------\n");
			printf("\n");
		}
		else {
			printf("\n\n");
			printf("Player 2\n");
			printf("-------------\n");
			printf("| %c | %c | %c |\n", board[0][0],board[0][1],board[0][2]);
			printf("-------------\n");
			printf("| %c | %c | %c |\n", board[1][0],board[1][1],board[1][2]);
			printf("-------------\n");
			printf("| %c | %c | %c |\n", board[2][0],board[2][1],board[2][2]);
			printf("-------------\n");
			printf("\n");
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

			
	int gameOver(char board[][SIDE]) {
   		return (rowCrossed(board) || columnCrossed(board)
            		|| diagonalCrossed(board));
	}

	int rowCrossed(char board[][SIDE])
	{
	    for (int i = 0; i < SIDE; i++) {
		if (board[i][0] == board[i][1]
		    && board[i][1] == board[i][2]
		    && board[i][0] != ' ')
		    return 1;
	    }
	    return 0;
	}

	int columnCrossed(char board[][SIDE])
	{
	    for (int i = 0; i < SIDE; i++) {
		if (board[0][i] == board[1][i]
		    && board[1][i] == board[2][i]
		    && board[0][i] != ' ')
		    return 1;
	    }
	    return 0;
	}

	int diagonalCrossed(char board[][SIDE])
	{
	    if ((board[0][0] == board[1][1]
		 && board[1][1] == board[2][2]
		 && board[0][0] != ' ')
		|| (board[0][2] == board[1][1]
		    && board[1][1] == board[2][0]
		    && board[0][2] != ' '))
		return 1;

	    return 0;
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
                                        printf("Invalid input! Please choose an option: ");
                                }
                }
                if (choice == 1) {
      			playTicTacToe(Player1);
		
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
	void startEntry() {
                printf("\n");
                printf("===========================\n");
                printf("| Welcome to Tic Tac Toe! |\n");
                printf("===========================\n");
                printf("\n");
		
		mainMenu();
	}
	
	void playTicTacToe(int whoseTurn) {
		char input[100];
		char board[SIDE][SIDE];
   		int moves[SIDE * SIDE];
		int moveIndex = 0;
		int move;
		int x;
		int y;
		initialize(board, moves);
		while (!gameOver(board) && moveIndex != SIDE * SIDE) {
			
			if (whoseTurn == Player1) {
				showBoard(board, Player1);	
				printf("\nPlayer 1, enter your move: ");	
				while (fgets(input, sizeof(input), stdin)) {
					if (sscanf(input, "%d", &move) == 1 && (move >= 1 && move <= 9)) {
						break;
					} else {
						printf("Invalid input! ");
					}
				}
			
				x = (move - 1) / SIDE;
				y = (move - 1) % SIDE;
				if (board[x][y] == ' ') {
					board[x][y] = PLAYER1MOVE;
					showBoard(board, Player2);
					moveIndex++;
					if (gameOver(board)) {
						declareWinner(Player1);
						return;
					}
					whoseTurn = Player2;
				}
				else {
					printf("Cell %d is already occupied. Try "
						"again.\n",
						move);
					}	
				}

			else if(whoseTurn == Player2) {		
				printf("\nPlayer 2, enter your move: ");
				while (fgets(input, sizeof(input), stdin)) {
					if (sscanf(input, "%d", &move) == 1 && (move >= 1 && move <= 9)) {
                                        	break;
                                	} else {
                                        	printf("Invalid input! ");
                                        }
                                }
					
				x = (move - 1) / SIDE;
				y = (move - 1) % SIDE;
				if (board[x][y] == ' ') {
					board[x][y] = PLAYER2MOVE;	
					moveIndex++;
					if (gameOver(board)) {
						declareWinner(Player2);
						return;
					}
				whoseTurn = Player1;
				}
				else {
					printf("Cell %d is already occupied. Try "
						"again.\n",
						move);
					}	
				}
			if (!gameOver(board) && moveIndex == SIDE * SIDE) {
        			printf("It's a draw!\n");
				mainMenu();
				return;
			}
		}
	}
	int main() {
		startEntry();


	return 0;
}