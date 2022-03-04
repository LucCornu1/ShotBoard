// Code by Luc Cornu

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "SteamAPISubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCSOnCreateSessionComplete, bool, Successful);

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
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FDelegateHandle CreateSessionCompleteDelegateHandle;
	TSharedPtr<FOnlineSessionSettings> LastSessionSettings;

protected:
	void OnCreateSessionCompleted(FName SessionName, bool Successful);

public:
	void CreateSession(int32 NumPublicConnections, bool IsLANMatch);

	FCSOnCreateSessionComplete OnCreateSessionCompleteEvent;
};
