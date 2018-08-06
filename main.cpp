/* This is the console executable, that makes use of the BullCow class
This acts as the view in a MVC pattern, and is responsible for all
user interaction. For game logic see the FBUllCowGame class.
*/
#pragma once

#include <iostream>
#include <string>
#include "FBullCowGame.h"

//to make syntax Unreal friendly
using int32 = int;
using FText = std::string;

//function ptototypes as outside a class
void PrintIntro();
int32 AskWordLength();
void PlayGame();
FText GetValidGuess();
bool AskToPlayAgain();
void PrintGameSummary();

FBullCowGame BCGame; //instantiate a new game, which we re-use across plays

//the entry point for our application
int main()
{
	PrintIntro();
	do {
		PlayGame();
	} while (AskToPlayAgain());

	return 0; //exit the application
}

void PrintIntro() 
{
	std::cout << "Welcome to Bulls and Cows, a fun word game!\n";
	std::cout << "Can you guess the " << BCGame.GetHiddenWordLength();
	std::cout << " letter isogram I'm thinking of?\n";
	
	return;
}

//plays a single game to completion
void PlayGame()
{
	BCGame.Reset();
	
	BCGame.GetHiddenWord(AskWordLength());
	int32 MaxTries = BCGame.GetMaxTries();
	
	//loop asking for guesses while the game 
	//is NOT won and there are still tries remaining
	while (!BCGame.IsGameWon() && BCGame.GetCurrentTry() <= MaxTries) 
	{
		FText Guess = GetValidGuess(); 

		//submit valid guess to the game, and receive counts
		FBullCowCount BullCowCount = BCGame.SubmitValidGuess(Guess);

		//print numbers of bulls and cows
		std::cout << "\t\bBulls = " << BullCowCount.Bulls << std::endl;
		std::cout << "\t\bCows = " << BullCowCount.Cows << std::endl;
		std::cout << std::endl;
	}
	PrintGameSummary();
}

int32 AskWordLength()
{
	int32 WordLength = 0;
	do
	{
		std::cout << "Please choose a word length from 3 to 6 letters: ";
		std::cin >> WordLength;
		std::cin.clear();
		std::cin.ignore();
		std::cout << std::endl;
	} while (WordLength < 3 || WordLength > 6);
	return WordLength;
}

//loop continually until the user gives a valid guess
FText GetValidGuess()
{
	FText Guess;
	EGuessStatus Status = EGuessStatus::Invalid_Status;
	do
	{
	//get a guess from the player
	int32 CurrentTry = BCGame.GetCurrentTry();
	std::cout << "Try " << CurrentTry << " of " << BCGame.GetMaxTries();
	std::cout<< ". Enter your guess: ";
	std::getline(std::cin, Guess);

	//check status and give feedback
	Status = BCGame.CheckGuessValidity(Guess);
		switch (Status)
		{
		case EGuessStatus::Wrong_Length:
			std::cout << "Please enter a " << BCGame.GetHiddenWordLength() << " letter word.\n\n";
			break;
		case EGuessStatus::Not_Isogram:
			std::cout << "Please enter a word without repeating letters.\n\n";
			break;
		case EGuessStatus::Not_Lowercase:
			std::cout << "Please enter all lowercase letters.\n\n";
			break;
		default:
			break;
		}
	} while (Status != EGuessStatus::OK); //keep looping until we get no errors
	return Guess;
}

bool AskToPlayAgain()
{
	std::cout << "Do you want to play again with the same hidden word? (y/n)";
	FText Response = "";
	std::getline(std::cin, Response);
	return (Response[0] == 'y' || Response[0] == 'Y');
	
}

void PrintGameSummary()
{
	if (BCGame.IsGameWon())
	{
		std::cout << "WELL DONE - YOU WIN\n";
	}
	else
	{
		std::cout << "Better luck next time!\n";
	}
	return;
}
