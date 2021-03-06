// Code by Luc Cornu & Joffrey Ziegeltrum


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
// Goal : Initialize the subsystem with the Online Subsystem
{
	if (IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get()) // If the Online Subsystem is installed
	{
		SessionInterface = Subsystem->GetSessionInterface(); // Get the interface for accessing the session management services
		if (SessionInterface.IsValid())
		{
			// Binding Delegates
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &USteamAPISubsystem::OnCreateSessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &USteamAPISubsystem::OnFindSessionsComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &USteamAPISubsystem::OnJoinSessionComplete);
		}
	}

	// CreateServer();
	// JoinServer();

}

void USteamAPISubsystem::Deinitialize()
// Goal : Deinitialize the subsystem (nothing is done here)
{
	// None

}


// The Delegates bound with the "SessionInterface->CreateSession(0, SessionName, SessionSettings);" method call
void USteamAPISubsystem::OnCreateSessionComplete(FName ServerName, bool Successful)
// Goal : Verify if the session was successfuly created, and if so, launch the map on a listen server
{
	UE_LOG(LogTemp, Warning, TEXT("OnCreateSessionComplete, Succeeded : %d"), Successful);

	if (Successful) // If the session was successfuly created
	{
		GetWorld()->ServerTravel("/Game/Maps/TestMap?listen"); // Travel to map


		/*CSteamID ServerID = SteamGameServer()->GetSteamID();
		// SteamFriends()->ActivateGameOverlay("Friends");
		UE_LOG(LogTemp, Warning, TEXT("SteamGameServer : %d"), ServerID.ConvertToUint64());*/
	}

}

// The Delegates bound with the "SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());" method call
void USteamAPISubsystem::OnFindSessionsComplete(bool Successful)
// Goal : Verify if the research was successful, and that the amount of server found is superior to 0.
//			If so, join the first session found (in our case it will be the right session because we are in LAN)
{
	UE_LOG(LogTemp, Warning, TEXT("OnFindSessionComplete, Succeeded : %d"), Successful);

	if (Successful)
	{
		TArray<FOnlineSessionSearchResult> SearchResults = SessionSearch->SearchResults; // Retrieve the list of server found
		UE_LOG(LogTemp, Warning, TEXT("SearchResults, Counts : %d"), SearchResults.Num());

		if (SearchResults.Num()) // If the amount of server found is superior to 0
		{
			UE_LOG(LogTemp, Warning, TEXT("Joining Server"));
			SessionInterface->JoinSession(0, SessionName, SearchResults[0]); // Join the server
		}
	}

}

// The Delegates bound with the "SessionInterface->JoinSession(0, SessionName, SearchResults[0]);" method call
void USteamAPISubsystem::OnJoinSessionComplete(FName ServerName, EOnJoinSessionCompleteResult::Type Result)
// Goal : If a player controller is found, retrieve the server address, and connect to the server on the right map
{
	UE_LOG(LogTemp, Warning, TEXT("OnJoinSessionComplete"));

	if (APlayerController* PController = UGameplayStatics::GetPlayerController(GetWorld(), 0)) // Retrieve the player controller, if not do nothing
	{
		FString JoinAddress;
		SessionInterface->GetResolvedConnectString(SessionName, JoinAddress); // Retrieve the server address
		if (JoinAddress != "") // If the server address is not null
		{
			PController->ClientTravel(JoinAddress, ETravelType::TRAVEL_Absolute); // Travel on the server and travel to the right map
		}
	}

}


void USteamAPISubsystem::CreateServer()
// Goal : Set the session settings, and call the "CreateSession" method
{
	UE_LOG(LogTemp, Warning, TEXT("Creating Server"));

	// Set the session settings
	FOnlineSessionSettings SessionSettings;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bIsDedicated = false;
	SessionSettings.bIsLANMatch = true;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.NumPublicConnections = 5;
	// SessionSettings.bUseLobbiesIfAvailable = true;

	// SessionSettings.Set(SETTING_MAPNAME, FString(TEXT("")), EOnlineDataAdvertisementType::ViaOnlineService);

	SessionInterface->CreateSession(0, SessionName, SessionSettings); // Create the session and apply the settings defined before

}

void USteamAPISubsystem::JoinServer()
// Goal : Set the search settings, and call the "FindSession" method
{
	UE_LOG(LogTemp, Warning, TEXT("Join Server"));

	// Set the research settings
	SessionSearch = MakeShareable(new FOnlineSessionSearch());	
	SessionSearch->bIsLanQuery = true;
	SessionSearch->MaxSearchResults = 500;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef()); // Begin the research and apply the settings defined before

}