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

    PrintLine(TEXT("The hidden word is: %s"), *HiddenWord); // TODO delete, debug line
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

FString UBullCowCartridge::PickRandomWord(TArray<FString> AllWords)
{
    TArray<FString> ValidWords;

    for (int i = 0; i < AllWords.Num(); i++)
    {
        if (AllWords[i].Len() >= 3 && AllWords[i].Len() <= 8 && IsIsogram(AllWords[i]))
        {
            ValidWords.Emplace(AllWords[i]);
        }
    }

    // TODO pick random word from ValidWords

    return TEXT("hat");
}

void UBullCowCartridge::SetupGame()
{
    HiddenWord = PickRandomWord(Words); // TODO make dynamic
    Lives = HiddenWord.Len();
    bGameOver = false;

    // Welcoming the player
    PrintLine(TEXT("Welcome to Bull Cows!"));
    PrintLine(TEXT("Guess the %i letter word."), HiddenWord.Len());
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