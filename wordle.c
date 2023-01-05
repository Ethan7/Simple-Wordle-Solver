/* By Ethan Hughes */
/* Written on 1/3/2023 */

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

#define WORDCOUNT 12972
#define WORDLEN 5
#define GUESSES 6
#define EMPTY '_'

int predict(char (*words)[WORDLEN+2], char *known, char (*nearby)[WORDLEN+1], char *unused){
	int newlist[WORDCOUNT];
	int k = 0;
	char cont = 0;
	for(int i = 0; i < WORDCOUNT; i++){
		for(int j = 0; j < WORDLEN; j++){
			//If letter from current word is unused
			if(unused[words[i][j]-'a'] != EMPTY){
				cont = WORDLEN;
				break;
			}
			//If known letter with position is not in the current word
			if(known[j] != EMPTY && known[j] != words[i][j]){
				cont = WORDLEN;
				break;
			}
			for(int l = 0; l < GUESSES; l++){
				if(nearby[l][j] != EMPTY){
					//If current word and letter are the same letter in the same position as the current nearby info (if a letter is known to be nearby then it can't be in it's current position)
					if(nearby[l][j] == words[i][j]){
						cont = WORDLEN;
						break;
					}
					//If letter known to be nearby is not found in the current word
					for(int m = 0; m < WORDLEN; m++){
						if(nearby[l][j] != words[i][m]){
							cont++;
						}
					}
					if(cont == WORDLEN){
						break;
					}
					cont = 0;
				}
			}
			if(cont == WORDLEN){
				break;
			}
		}
		if(cont == WORDLEN){
			cont = 0;
			continue;
		}
		//Add word to remaining words list
		newlist[k] = i;
		k++;
	}
	printf("Remaining possibilities: %d\n", k);
	//Find the probabilities of letters
	int letterprob[26];
	for(int i = 0; i < 26; i++){
		letterprob[i] = 0;
	}
	for(int i = 0; i < k; i++){
		for(int j = 0; j < WORDLEN; j++){
			letterprob[words[newlist[i]][j]-'a']++;
		}
	}
	//Find the probabilities of words based on their letters
	int wordprob[k];
	char letterused[26];
	for(int i = 0; i < 26; i++){
		letterused[i] = 0;
	}
	for(int i = 0; i < k; i++){
		wordprob[i] = 0;
		for(int j = 0; j < WORDLEN; j++){
			if(!letterused[words[newlist[i]][j]-'a']){
				wordprob[i] += letterprob[words[newlist[i]][j]-'a'];
				letterused[words[newlist[i]][j]-'a'] = 1;
			}
		}
		for(int j = 0; j < WORDLEN; j++){
			letterused[words[newlist[i]][j]-'a'] = 0;
		}
	}
	//Sort the list
	for(int i = 0; i < k; i++){
		for(int j = 0; j < k; j++){
			if(wordprob[i] > wordprob[j]){
				int temp = newlist[i];
				newlist[i] = newlist[j];
				newlist[j] = temp;
				temp = wordprob[i];
				wordprob[i] = wordprob[j];
				wordprob[j] = temp;
			}
		}
	}
	//Find the remaining words with the least entropy
	for(int i = 0; i < 10; i++){
		if(i == k){
			break;
		}
		printf("Remaining word: %s, Score: %d\n", words[newlist[i]], wordprob[i]);
	}
	return newlist[0];
}

int main(int argc, char **argv){
	//Load Wordle dictionary
	char words[WORDCOUNT][WORDLEN+2];
	FILE *fp = fopen("valid-wordle-words.txt", "r");
	for(int i = 0; i < WORDCOUNT; i++){
		fgets(words[i], WORDLEN+2, fp);
		words[i][WORDLEN] = 0;
	}
	fclose(fp);

	//Check for program arguments
	int answer = -1;
	if(argc > 1){
		//Take answer word from user input
		for(int i = 0; i < WORDCOUNT; i++){
			if(strcmp(words[i], argv[1]) == 0){
				answer = i;
				break;
			}
		}
		if(answer == -1){
			printf("Chosen answer word is not valid.\n");
			return 0;
		}
	} else {
		//Randomly choose answer word
		srand(time(NULL));
		answer = rand() % WORDCOUNT;
	}

	//Instantiate variables
	char *input;
	int guess;
	int bestguess;
	size_t len;
	int win;

	//Clear out unused, known and nearby word arrays
	char known[WORDLEN+1];
	char nearby[GUESSES][WORDLEN+1];
	char unused[27];
	for(int i = 0; i < 26; i++){
		unused[i] = EMPTY;
	}
	for(int i = 0; i < WORDLEN; i++){
		known[i] = EMPTY;
		for(int j = 0; j < GUESSES; j++){
			nearby[j][i] = EMPTY;
		}
	}
	for(int i = 0; i < GUESSES; i++){
		nearby[i][WORDLEN] = 0;
	}
	known[WORDLEN] = 0;
	unused[26] = 0;

	for(int i = 0; i < GUESSES; i++){
		//Predict best guess and print it
		bestguess = predict(words, known, nearby, unused);
		printf("Best Guess %d: %s\n", i, words[bestguess]);

		//Let the player pick their own guess
		guess = -1;
		while(guess == -1){
			printf("Guess: ");
			len = getline(&input, &len, stdin);
			input[len-1] = 0;
			for(int j = 0; j < WORDCOUNT; j++){
				if(strcmp(input, words[j]) == 0){
					guess = j;
					break;
				}
			}
			if(guess == -1){
				printf("%s is not a recognized word. Try again.\n", input);
			}
		}
			

		//Update hints based on guess
		win = 0;
		for(int j = 0; j < WORDLEN; j++){
			if(words[guess][j] == words[answer][j]){
				//Indicate letter in word in place
				known[j] = words[guess][j];
				win++;
			} else {
				for(int k = 0; k < WORDLEN; k++){
					if(words[guess][j] == words[answer][k]){
						//Indicate letter in word but not here
						nearby[i][j] = words[guess][j];
						break;
					}
				}
				if(nearby[i][j] == EMPTY){
					//Indicate letter not in word
					unused[words[guess][j]-'a'] = words[guess][j];
				}
			}
		}

		//Test if you have won
		if(win == WORDLEN){
			printf("You guessed correctly! Guesses: %d\n", i+1);
			break;
		}

		//Print known and nearby letters
		printf("Known letters: %s\n", known);
		printf("Nearby letters: %s\n", nearby[i]);
		printf("Unused letters: %s\n", unused);
	}
	printf("The Word was: %s.\n", words[answer]);
	return 0;
}
