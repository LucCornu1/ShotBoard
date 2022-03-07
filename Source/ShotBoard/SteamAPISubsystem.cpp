// Code by Luc Cornu


#include "Engine/World.h"
#include "OnlineSubsystemUtils.h"
#include "SteamAPISubsystem.h"

// Constructor
USteamAPISubsystem::USteamAPISubsystem() 
{
	// None

}

void USteamAPISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	if (IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get())
	{
		SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			// Bind Delegates Here
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &USteamAPISubsystem::OnCreateSessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &USteamAPISubsystem::OnFindSessionsComplete);
		}
	}

	CreateServer();
	JoinServer();

}

void USteamAPISubsystem::Deinitialize()
{
	// None

}


void USteamAPISubsystem::OnCreateSessionComplete(FName ServerName, bool Successful)
{
	UE_LOG(LogTemp, Warning, TEXT("OnCreateSessionComplete, Succeeded : %d"), Successful);

	if (Successful)
	{
		GetWorld()->ServerTravel("/Game/Maps/TestMap?listen");
	}

}

void USteamAPISubsystem::OnFindSessionsComplete(bool Successful)
{
	UE_LOG(LogTemp, Warning, TEXT("OnFindSessionComplete, Succeeded : %d"), Successful);

	if (Successful)
	{
		TArray<FOnlineSessionSearchResult> SearchResults = SessionSearch->SearchResults;
		UE_LOG(LogTemp, Warning, TEXT("SearchResults, Counts : %d"), SearchResults.Num());
	}

}


void USteamAPISubsystem::CreateServer()
{
	UE_LOG(LogTemp, Warning, TEXT("Creating Server"));

	FOnlineSessionSettings SessionSettings;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bIsDedicated = false;
	SessionSettings.bIsLANMatch = true;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.NumPublicConnections = 5;

	SessionInterface->CreateSession(0, FName("sehndbebvsv"), SessionSettings);

}

void USteamAPISubsystem::JoinServer()
{
	UE_LOG(LogTemp, Warning, TEXT("Join Server"));

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->bIsLanQuery = true;
	SessionSearch->MaxSearchResults = 10000;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());

}