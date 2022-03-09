// Code by Luc Cornu


#include "SteamAPISubsystem.h"
#include "Engine/World.h"
#include "OnlineSubsystemUtils.h"
// #include "ThirdParty/Steamworks/Steamv151/sdk/public/steam/steam_api.h"
// #include "ThirdParty/Steamworks/Steamv151/sdk/public/steam/isteamgameserver.h"
#include "Kismet/GameplayStatics.h"

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
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &USteamAPISubsystem::OnJoinSessionComplete);
		}
	}

	// CreateServer();
	// JoinServer();

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

		/*CSteamID ServerID = SteamGameServer()->GetSteamID();
		// SteamFriends()->ActivateGameOverlay("Friends");
		UE_LOG(LogTemp, Warning, TEXT("SteamGameServer : %d"), ServerID.ConvertToUint64());*/
	}

}

void USteamAPISubsystem::OnFindSessionsComplete(bool Successful)
{
	UE_LOG(LogTemp, Warning, TEXT("OnFindSessionComplete, Succeeded : %d"), Successful);

	if (Successful)
	{
		TArray<FOnlineSessionSearchResult> SearchResults = SessionSearch->SearchResults;
		UE_LOG(LogTemp, Warning, TEXT("SearchResults, Counts : %d"), SearchResults.Num());

		if (SearchResults.Num())
		{
			UE_LOG(LogTemp, Warning, TEXT("Joining Server"));
			SessionInterface->JoinSession(0, SessionName, SearchResults[0]);
		}
	}

}

void USteamAPISubsystem::OnJoinSessionComplete(FName ServerName, EOnJoinSessionCompleteResult::Type Result)
{
	UE_LOG(LogTemp, Warning, TEXT("OnJoinSessionComplete"));

	if (APlayerController* PController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		FString JoinAddress;
		SessionInterface->GetResolvedConnectString(SessionName, JoinAddress);
		if (JoinAddress != "")
		{
			PController->ClientTravel(JoinAddress, ETravelType::TRAVEL_Absolute);
		}
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
	// SessionSettings.bUseLobbiesIfAvailable = true;

	SessionSettings.Set(SETTING_MAPNAME, FString(TEXT("")), EOnlineDataAdvertisementType::ViaOnlineService);

	SessionInterface->CreateSession(0, SessionName, SessionSettings);

}

void USteamAPISubsystem::JoinServer()
{
	UE_LOG(LogTemp, Warning, TEXT("Join Server"));

	SessionSearch = MakeShareable(new FOnlineSessionSearch());	
	SessionSearch->bIsLanQuery = false;
	SessionSearch->MaxSearchResults = 500;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());

}