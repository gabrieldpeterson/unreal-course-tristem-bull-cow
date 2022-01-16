// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWordList.txt");
    FFileHelper::LoadFileToStringArray(Words, *WordListPath);

    SetupGame();
}

void UBullCowCartridge::OnInput(const FString& Input) // When the player hits enter
{
    if (!bGameOver)
    {
        // Was it correct, else process
        if (Input == HiddenWord)
        {
            PrintLine(TEXT("You are correct"));
            EndGame();
        }
        else
        {
            ProcessWrongGuess(Input);
        }
    }
    else
    {
        ClearScreen();
        SetupGame();
    }
}

FString UBullCowCartridge::PickRandomWord(const TArray<FString>& AllWords) const
{
    TArray<FString> ValidWords;

    for (FString WordToCheck : AllWords)
    {
        if (WordToCheck.Len() >= 3 && WordToCheck.Len() <= 8 && IsIsogram(WordToCheck))
        {
            ValidWords.Emplace(WordToCheck);
        }
    }

    int32 RandomWordIndex = FMath::RandRange(0, ValidWords.Num() - 1);

    return TEXT("%s"), *ValidWords[RandomWordIndex];
}

void UBullCowCartridge::SetupGame()
{
    HiddenWord = PickRandomWord(Words); 
    Lives = HiddenWord.Len();
    bGameOver = false;

    // Welcoming the player
    PrintLine(TEXT("Welcome to Bull Cows!"));
    PrintLine(TEXT("Guess the %i letter word."), HiddenWord.Len());
    PrintLine(TEXT("Debug: %s"), *HiddenWord);  // TODO: delete when finished, for debugging
    PrintLine(TEXT("Type in your guess.\nPress enter to continue..."));
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("Press enter to continue..."));
}

void UBullCowCartridge::ProcessWrongGuess(const FString& Guess)
{
    PrintLine(TEXT("Wrong"));

    if (Guess.Len() != HiddenWord.Len())
    {
        PrintLine(TEXT("The hidden word has %i characters"), HiddenWord.Len());
        return;
    }

    if (!IsIsogram(Guess))
    {
        PrintLine(TEXT("%s is not an isogram"), *Guess);
        return;
    }

    PrintLine(TEXT("You have lost a life."));
    PrintLine(TEXT("You have %i lives left"), --Lives);
    // Show the bulls and cows
    int32 Bulls, Cows;
    GetBullCows(Guess, Bulls, Cows);
    PrintLine(TEXT("You have %i Bulls and %i Cows"), Bulls, Cows);

    if (Lives == 0)
    {
        PrintLine(TEXT("You lose"));
        PrintLine(TEXT("The hidden word was %s"), *HiddenWord);
        EndGame();
    }
}

bool UBullCowCartridge::IsIsogram(const FString& Word) const
{
    for (int32 i = 0; i < Word.Len(); ++i)
    {
        for (int32 j = i + 1; j < Word.Len(); ++j)
        {
            if (Word[i] == Word[j])
            {
                return false;
            }
        }
    }
    return true;
}

void UBullCowCartridge::GetBullCows(const FString& Guess, int32& BullCount, int32& CowCount) const
{
    BullCount = 0;
    CowCount = 0;

    for (int32 i = 0; i < Guess.Len(); i++)
    {
        if (Guess[i] == HiddenWord[i])
        {
            BullCount++;
        }
        else
        {
            for (int32 j = 0; j < Guess.Len(); j++)
            {
                if (Guess[i] == HiddenWord[j])
                {
                    CowCount++;
                }
            }
        }
    }
}