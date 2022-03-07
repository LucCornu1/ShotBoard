// Code by Luc Cornu

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "SteamAPISubsystem.generated.h"

/**
 * 
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
	IOnlineSessionPtr SessionInterface;

	TSharedPtr<FOnlineSessionSearch> SessionSearch;

protected:
	virtual void OnCreateSessionComplete(FName ServerName, bool Successful);
	virtual void OnFindSessionsComplete(bool Successful);

	UFUNCTION(BlueprintCallable, Category = "SteamAPI|Server")
		void CreateServer();

	UFUNCTION(BlueprintCallable, Category = "SteamAPI|Server")
		void JoinServer();
};
