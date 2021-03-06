// Code by Luc Cornu & Joffrey Ziegeltrum

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "SteamAPISubsystem.generated.h"

/**
The class derived from UGameInstanceSubsystem :
	- It initialize & Deinitialize when the Game Instance Singleton says so (When the game start & the game close)
	- It's used to create a session, or find & join a session depending on the player choice
	- Both functionnalities can be accessed through Blueprint thanks to "CreateServer" & "JoinServer" methods
 */
UCLASS()
class SHOTBOARD_API USteamAPISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	USteamAPISubsystem();

	// Begin USubsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// End USubsystem


private:
	// Server configuration variables
	UPROPERTY(EditAnywhere, Category = "Debug")
		FName SessionName = "RomainPablo";

	IOnlineSessionPtr SessionInterface;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;

protected:
	// Delegates
	virtual void OnCreateSessionComplete(FName ServerName, bool Successful);
	virtual void OnFindSessionsComplete(bool Successful);
	virtual void OnJoinSessionComplete(FName ServerName, EOnJoinSessionCompleteResult::Type Result);

	// Blueprint accessibility
	/** Creates & hosts a server */
	UFUNCTION(BlueprintCallable, Category = "SteamAPI|Server")
		void CreateServer();

	/** Find & join a server */
	UFUNCTION(BlueprintCallable, Category = "SteamAPI|Server")
		void JoinServer();
};
