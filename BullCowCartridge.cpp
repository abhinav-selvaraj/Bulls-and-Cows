// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"


;void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    Isograms = GetValidWords(Words);
    InitGame();
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    if(bGameOver)
    {
        ClearScreen();
        InitGame();
    }
    else //checks the player guess
    {
        ProcessGuess(Input);
    }
}

void UBullCowCartridge::InitGame()
{
    //Weclomes the player
    PrintLine(TEXT("Weclome to Bulls Cows!"));
   
    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num()-1)];
    Lives = HiddenWord.Len() * 2;
    bGameOver = false;

    PrintLine(TEXT("Guess the %i Letter word"), HiddenWord.Len());
    PrintLine(TEXT("You have %i lives"), Lives);
    PrintLine(TEXT("Type in you guess and \nPress enter to Continue"));

   
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    
    PrintLine(TEXT("\nPress enter to play again"));
}

void UBullCowCartridge::ProcessGuess(const FString& Guess)
{
    if(Guess == HiddenWord)
    {
        PrintLine(TEXT("You have won!"));
        EndGame();
        return;
    }
    

    if(Guess.Len() != HiddenWord.Len())
    {
        PrintLine(TEXT("Sorry, try guessing again! \nYou have %i lives remaining"), Lives);
        PrintLine(TEXT("The hidden word is %i long"), HiddenWord.Len());

        return;
    }
    //checks if it is an isogram
    if(!IsIsogram(Guess))
    {
        PrintLine(TEXT("There are no repeating letters, guess again"));
        return;
    }
    PrintLine(TEXT("Lost a life!"));
      
    --Lives;

    if(Lives <= 0)
    {
        ClearScreen();
        PrintLine(TEXT("You have no lives left"));
        PrintLine(TEXT("The hidden word was %s"), *HiddenWord);
        EndGame();
        return;
    }

    //Show the player the Bulls and Cows
    FBullCowCount Score = GetBullCows(Guess);

    PrintLine(TEXT("You have %i Bulls and %i Cows left"), Score.Bulls, Score.Cows);
    PrintLine(TEXT("You have %i lives left"), Lives);
            
        
        
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
   for (int32 Index = 0; Index < Word.Len(); Index++)
   {
       for (int32 Comparison = Index+1; Comparison < Word.Len(); Comparison++)
       {
           if(Word[Index] == Word[Comparison])
           {
               return false;
           }
       }
       
   }
   
   return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString>& WordList) const
{
    TArray<FString> ValidWords;
   
    for(FString Word : WordList)
    {
        if(Word.Len() >= 4 && Word.Len() <= 8)
        {
            if(IsIsogram(Word))
            {
                ValidWords.Emplace(Word);
            }
        }
        
    }

    return ValidWords;

    
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& Guess) const
{
    FBullCowCount Count;
    //if the index Guess is same as the hidden index, bullcount++
    //If not a bull, was it a cow? if yes CowCount++

    for(int32 GuessIndex = 0; GuessIndex < Guess.Len(); GuessIndex++)
    {
        if(Guess[GuessIndex]== HiddenWord[GuessIndex])
        {
            Count.Bulls++;
            continue;
        }

        for(int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++)
        {
            if(Guess[GuessIndex] == HiddenWord[HiddenIndex])
            {
                Count.Cows++;
                break;
            }
        }
    }
    return Count;
}