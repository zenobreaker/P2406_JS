#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"
#include "CGameInstance.generated.h"

// Shared native behavior for the three room widget assets.
UCLASS()
class P2406_JS_API UCRoomMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Room")
	int32 MenuType = 0; // 0: settings, 1: list, 2: entry

	int32 ResultIndex = INDEX_NONE;
	void RefreshRooms();
	void SetStatus(const FString& Message, bool bBusy);

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void NativeConstruct() override;

private:
	UFUNCTION() void CreateRoom();
	UFUNCTION() void SearchRooms();
	UFUNCTION() void JoinRoom();
	UFUNCTION() void SelectRoom();
	UFUNCTION() void CloseMenu();
};

UCLASS()
class P2406_JS_API UCRoomPlayModeWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION() void OpenSettings();
	UFUNCTION() void OpenRoomList();
};

USTRUCT(BlueprintType)
struct FCRoomInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly) FString Name;
	UPROPERTY(BlueprintReadOnly) int32 Players = 0;
	UPROPERTY(BlueprintReadOnly) int32 Capacity = 0;
};

UCLASS(BlueprintType)
class P2406_JS_API UCGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UCGameInstance();

public:
	virtual void Init() override;
	virtual void Shutdown() override;

	UFUNCTION(BlueprintCallable, Category = "Room")
	void ShowRoomMenu(bool bRoomList);
	UFUNCTION(BlueprintCallable, Category = "Room")
	void CreateRoom(const FString& RoomName, int32 MaxPlayers, bool bPublic);
	UFUNCTION(BlueprintCallable, Category = "Room")
	void SearchRooms();
	UFUNCTION(BlueprintCallable, Category = "Room")
	void JoinRoom(int32 ResultIndex);

	void SelectRoom(int32 ResultIndex);
	void CloseRoomMenu();
	const TArray<FCRoomInfo>& GetRooms() const { return Rooms; }
	int32 GetSelectedRoom() const { return SelectedRoom; }
	bool IsRoomBusy() const { return bRoomBusy; }

public:
	UPROPERTY(EditAnywhere, Category = "SkillManager")
	TSubclassOf<class UCSkillManager> SkillManagerClass;

	UPROPERTY(EditAnywhere, Category = "BattleManager")
	TSubclassOf<class UCBattleManager> BattleManagerClass;

	UPROPERTY(EditAnywhere, Category = "GameManager")
	TSubclassOf<class UCPatternConditionManager> PatternConditionClass;

	UPROPERTY(EditAnywhere, Category = "GameManager")
	TSubclassOf<class UCGameManager> GameManagerClass;

	UPROPERTY(EditAnywhere, Category = "Buff")
	TSubclassOf<class UCBuffUIManager> BuffUIManagerClass;

public:
	UPROPERTY();
	class UCPatternConditionManager* PatternCondition;
	UPROPERTY();
	class UCSkillManager* SkillManager;
	UPROPERTY();
	class UCBattleManager* BattleManager;
	UPROPERTY();
	class UCGameManager* GameManager;
	UPROPERTY();
	class UCBuffUIManager* BuffUIManager; 

private:
	IOnlineSessionPtr GetRoomSessions() const;
	void FinishRoomOperation(const FString& Message);
	void OnRoomCreated(FName SessionName, bool bSuccess);
	void OnRoomsFound(bool bSuccess);
	void OnRoomJoined(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnRoomDestroyed(FName SessionName, bool bSuccess);
	void OnRoomNetworkFailure(UWorld* World, class UNetDriver* Driver, ENetworkFailure::Type Type, const FString& Error);
	void OnRoomTravelFailure(UWorld* World, ETravelFailure::Type Type, const FString& Error);
	void ClearRoomDelegates();
	void OnRoomMapLoaded(UWorld* World);

	UPROPERTY(Transient) UCRoomMenuWidget* RoomMenu = nullptr;
	TArray<FCRoomInfo> Rooms;
	TSharedPtr<class FOnlineSessionSearch> RoomSearch;
	TArray<class FOnlineSessionSearchResult> RoomResults;
	FDelegateHandle CreateRoomHandle, SearchRoomsHandle, JoinRoomHandle, DestroyRoomHandle;
	FDelegateHandle NetworkFailureHandle, TravelFailureHandle;
	FDelegateHandle MapLoadedHandle;
	int32 SelectedRoom = INDEX_NONE;
	bool bRoomBusy = false;
	int32 HostedRoomCapacity = 4;

};
