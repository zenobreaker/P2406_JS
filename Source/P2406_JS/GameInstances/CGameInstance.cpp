#include "GameInstances/CGameInstance.h"
#include "Global.h"

#include "GameInstances/CSkillManager.h"
#include "GameInstances/CBattleManager.h"
#include "GameInstances/CPatternConditionManager.h"
#include "GameInstances/CGameManager.h"
#include "GameInstances/CBuffUIManager.h"
#include "Widgets/CUserWidget_BuffSelectHUD.h"
#include "OnlineSubsystemUtils.h"
#include "OnlineSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/SpinBox.h"
#include "Components/CheckBox.h"
#include "Components/ScrollBox.h"
#include "Components/SizeBox.h"

UCGameInstance::UCGameInstance()
{
	SkillManagerClass = UCSkillManager::StaticClass();
	BattleManagerClass = UCBattleManager::StaticClass();
	PatternConditionClass = UCPatternConditionManager::StaticClass();
}

void UCGameInstance::Init()
{
	Super::Init();
	MapLoadedHandle = FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UCGameInstance::OnRoomMapLoaded);
	if (GEngine)
	{
		NetworkFailureHandle = GEngine->OnNetworkFailure().AddUObject(this, &UCGameInstance::OnRoomNetworkFailure);
		TravelFailureHandle = GEngine->OnTravelFailure().AddUObject(this, &UCGameInstance::OnRoomTravelFailure);
	}

	if (PatternConditionClass != nullptr)
	{
		PatternCondition = NewObject<UCPatternConditionManager>(this, PatternConditionClass);
		if (PatternCondition != nullptr)
			PatternCondition->InitConditionData();
	}


	if (SkillManagerClass != nullptr)
	{
		SkillManager = NewObject<UCSkillManager>(this, SkillManagerClass);
	}

	if (BattleManagerClass != nullptr)
	{
		BattleManager = NewObject<UCBattleManager>(this, BattleManagerClass);
	}


	if (BuffUIManagerClass != nullptr)
	{
		BuffUIManager = NewObject<UCBuffUIManager>(this, BuffUIManagerClass);
		if (!!BuffUIManager)
		{
			UCBuffUIManager* buffUIManager = BuffUIManagerClass->GetDefaultObject<UCBuffUIManager>();
			CheckNull(buffUIManager);

			BuffUIManager->SetBuffHUDClass(buffUIManager->GetBuffHUDClass().Get());
			BuffUIManager->BeginPlay(GetWorld());
		}
	}

	if (GameManagerClass == nullptr)
	{
		FHelpers::GetAssetAsync<UCGameManager>(&GameManager, "/Script/Engine.Blueprint'/Game/Managers/BP_CGameManager.BP_CGameManager'",
			[this]()
			{
				if (!!GameManager)
					GameManager->BeginPlay(GetWorld());
			});
	}
	else
	{
		GameManager = NewObject<UCGameManager>(this, GameManagerClass);
		if (GameManager != nullptr)
			GameManager->BeginPlay(GetWorld());
	}

	if (BuffUIManager != nullptr && GameManager != nullptr)
	{
		REGISTER_EVENT_WITH_REPLACE(BuffUIManager, OnShowedUIBuffList, GameManager, UCGameManager::OnShowBuffList);
		REGISTER_EVENT_WITH_REPLACE(BuffUIManager, OnHidedUIBuffList, GameManager, UCGameManager::OnHideBuffList);
	}
}


namespace
{
	const FName RoomNameKey(TEXT("P2406_ROOM_NAME"));
	const FName RoomProjectKey(TEXT("P2406_PROJECT"));
	const FString RoomProjectValue(TEXT("P2406_JS_LAN_1"));
}

IOnlineSessionPtr UCGameInstance::GetRoomSessions() const
{
	IOnlineSubsystem* online = Online::GetSubsystem(GetWorld());
	return online ? online->GetSessionInterface() : nullptr;
}

void UCGameInstance::ClearRoomDelegates()
{
	IOnlineSessionPtr sessions = GetRoomSessions();
	if (sessions.IsValid())
	{
		sessions->ClearOnCreateSessionCompleteDelegate_Handle(CreateRoomHandle);
		sessions->ClearOnFindSessionsCompleteDelegate_Handle(SearchRoomsHandle);
		sessions->ClearOnJoinSessionCompleteDelegate_Handle(JoinRoomHandle);
		sessions->ClearOnDestroySessionCompleteDelegate_Handle(DestroyRoomHandle);
	}
	CreateRoomHandle.Reset();
	SearchRoomsHandle.Reset();
	JoinRoomHandle.Reset();
	DestroyRoomHandle.Reset();
}

void UCGameInstance::Shutdown()
{
	ClearRoomDelegates();
	FCoreUObjectDelegates::PostLoadMapWithWorld.Remove(MapLoadedHandle);
	if (GEngine)
	{
		GEngine->OnNetworkFailure().Remove(NetworkFailureHandle);
		GEngine->OnTravelFailure().Remove(TravelFailureHandle);
	}
	RoomSearch.Reset();
	RoomResults.Empty();
	Super::Shutdown();
}

void UCGameInstance::FinishRoomOperation(const FString& Message)
{
	bRoomBusy = false;
	if (IsValid(RoomMenu))
		RoomMenu->SetStatus(Message, false);
	UE_LOG(LogTemp, Display, TEXT("Room: %s"), *Message);
}

void UCGameInstance::ShowRoomMenu(bool bRoomList)
{
	if (bRoomBusy)
		return;
	CloseRoomMenu();
	const TCHAR* path = bRoomList
		? TEXT("/Game/Widgets/WB_RoomList.WB_RoomList_C")
		: TEXT("/Game/Widgets/WB_RoomSettings.WB_RoomSettings_C");
	UClass* menuClass = LoadClass<UCRoomMenuWidget>(nullptr, path);
	if (!menuClass)
		return;
	RoomMenu = CreateWidget<UCRoomMenuWidget>(this, menuClass);
	if (!RoomMenu)
		return;
	RoomMenu->AddToViewport(200);
	if (APlayerController* controller = GetFirstLocalPlayerController())
	{
		controller->bShowMouseCursor = true;
		FInputModeUIOnly input;
		controller->SetInputMode(input);
	}
	if (bRoomList)
		SearchRooms();
}

void UCGameInstance::CloseRoomMenu()
{
	if (bRoomBusy)
		return;
	if (IsValid(RoomMenu))
		RoomMenu->RemoveFromParent();
	RoomMenu = nullptr;
}

void UCGameInstance::CreateRoom(const FString& RoomName, int32 MaxPlayers, bool bPublic)
{
	if (bRoomBusy)
		return;
	FString name = RoomName.TrimStartAndEnd();
	if (name.IsEmpty() || name.Len() > 32 || MaxPlayers < 2 || MaxPlayers > 16)
	{
		FinishRoomOperation(TEXT("방 이름은 1~32자, 인원은 2~16명으로 입력하세요."));
		return;
	}
	IOnlineSessionPtr sessions = GetRoomSessions();
	if (!sessions.IsValid())
	{
		FinishRoomOperation(TEXT("LAN 세션 기능을 사용할 수 없습니다."));
		return;
	}
	if (sessions->GetNamedSession(NAME_GameSession))
	{
		FinishRoomOperation(TEXT("이미 참가한 세션이 있습니다. 타이틀에서 새로 실행해 주세요."));
		return;
	}
	FOnlineSessionSettings settings;
	HostedRoomCapacity = MaxPlayers;
	settings.bIsLANMatch = true;
	settings.bIsDedicated = false;
	settings.NumPublicConnections = MaxPlayers;
	settings.bShouldAdvertise = bPublic;
	settings.bAllowJoinInProgress = true;
	settings.bUsesPresence = false;
	settings.Set(RoomNameKey, name, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	settings.Set(RoomProjectKey, RoomProjectValue, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	bRoomBusy = true;
	if (RoomMenu)
		RoomMenu->SetStatus(TEXT("방을 생성하고 있습니다..."), true);
	CreateRoomHandle = sessions->AddOnCreateSessionCompleteDelegate_Handle(
		FOnCreateSessionCompleteDelegate::CreateUObject(this, &UCGameInstance::OnRoomCreated));
	if (!sessions->CreateSession(0, NAME_GameSession, settings))
	{
		ClearRoomDelegates();
		FinishRoomOperation(TEXT("방 생성 요청에 실패했습니다. 다시 시도하세요."));
	}
}

void UCGameInstance::OnRoomCreated(FName SessionName, bool bSuccess)
{
	ClearRoomDelegates();
	if (!bSuccess)
	{
		FinishRoomOperation(TEXT("방을 생성하지 못했습니다. 다시 시도하세요."));
		return;
	}
	if (RoomMenu)
		RoomMenu->SetStatus(TEXT("방 생성 완료. 게임으로 이동합니다..."), true);
	if (APlayerController* controller = GetFirstLocalPlayerController())
	{
		controller->bShowMouseCursor = false;
		controller->SetInputMode(FInputModeGameOnly());
	}
	UGameplayStatics::OpenLevel(this, FName(TEXT("Level_2")), true,
		FString::Printf(TEXT("listen?MaxPlayers=%d"), HostedRoomCapacity));
}

void UCGameInstance::SearchRooms()
{
	if (bRoomBusy)
		return;
	IOnlineSessionPtr sessions = GetRoomSessions();
	if (!sessions.IsValid())
	{
		FinishRoomOperation(TEXT("LAN 세션 기능을 사용할 수 없습니다."));
		return;
	}
	Rooms.Empty();
	RoomResults.Empty();
	SelectedRoom = INDEX_NONE;
	RoomSearch = MakeShared<FOnlineSessionSearch>();
	RoomSearch->bIsLanQuery = true;
	RoomSearch->MaxSearchResults = 100;
	RoomSearch->TimeoutInSeconds = 8.0f;
	bRoomBusy = true;
	if (RoomMenu)
	{
		RoomMenu->RefreshRooms();
		RoomMenu->SetStatus(TEXT("같은 네트워크의 방을 검색하고 있습니다..."), true);
	}
	SearchRoomsHandle = sessions->AddOnFindSessionsCompleteDelegate_Handle(
		FOnFindSessionsCompleteDelegate::CreateUObject(this, &UCGameInstance::OnRoomsFound));
	if (!sessions->FindSessions(0, RoomSearch.ToSharedRef()))
	{
		ClearRoomDelegates();
		FinishRoomOperation(TEXT("검색 요청에 실패했습니다. 새로고침으로 다시 시도하세요."));
	}
}

void UCGameInstance::OnRoomsFound(bool bSuccess)
{
	ClearRoomDelegates();
	if (!bSuccess || !RoomSearch.IsValid())
	{
		FinishRoomOperation(TEXT("방 검색에 실패했습니다. 새로고침으로 다시 시도하세요."));
		return;
	}
	for (const FOnlineSessionSearchResult& result : RoomSearch->SearchResults)
	{
		FString project, name;
		if (!result.IsValid() || !result.Session.SessionSettings.Get(RoomProjectKey, project)
			|| project != RoomProjectValue || !result.Session.SessionSettings.Get(RoomNameKey, name))
			continue;
		FCRoomInfo info;
		info.Name = name;
		info.Capacity = result.Session.SessionSettings.NumPublicConnections;
		info.Players = FMath::Clamp(info.Capacity - result.Session.NumOpenPublicConnections, 0, info.Capacity);
		Rooms.Add(info);
		RoomResults.Add(result);
	}
	FinishRoomOperation(Rooms.Num() > 0 ? TEXT("방을 선택한 뒤 참가 버튼을 누르세요.")
		: TEXT("검색된 방이 없습니다. 같은 네트워크인지 확인한 뒤 새로고침하세요."));
	if (RoomMenu)
		RoomMenu->RefreshRooms();
}

void UCGameInstance::SelectRoom(int32 ResultIndex)
{
	if (bRoomBusy || !Rooms.IsValidIndex(ResultIndex))
		return;
	SelectedRoom = ResultIndex;
	if (RoomMenu)
	{
		RoomMenu->RefreshRooms();
		RoomMenu->SetStatus(FString::Printf(TEXT("선택한 방: %s"), *Rooms[ResultIndex].Name), false);
	}
}

void UCGameInstance::JoinRoom(int32 ResultIndex)
{
	if (bRoomBusy)
		return;
	IOnlineSessionPtr sessions = GetRoomSessions();
	if (!sessions.IsValid() || !RoomResults.IsValidIndex(ResultIndex))
	{
		FinishRoomOperation(TEXT("참가할 방을 먼저 선택하세요."));
		return;
	}
	if (Rooms[ResultIndex].Players >= Rooms[ResultIndex].Capacity)
	{
		FinishRoomOperation(TEXT("인원이 가득 찬 방입니다. 새로고침해 주세요."));
		return;
	}
	if (sessions->GetNamedSession(NAME_GameSession))
	{
		FinishRoomOperation(TEXT("이미 참가한 세션이 있습니다. 타이틀에서 새로 실행해 주세요."));
		return;
	}
	bRoomBusy = true;
	if (RoomMenu)
		RoomMenu->SetStatus(TEXT("선택한 방에 참가하고 있습니다..."), true);
	JoinRoomHandle = sessions->AddOnJoinSessionCompleteDelegate_Handle(
		FOnJoinSessionCompleteDelegate::CreateUObject(this, &UCGameInstance::OnRoomJoined));
	if (!sessions->JoinSession(0, NAME_GameSession, RoomResults[ResultIndex]))
	{
		ClearRoomDelegates();
		FinishRoomOperation(TEXT("참가 요청에 실패했습니다. 다시 검색해 주세요."));
	}
}

void UCGameInstance::OnRoomJoined(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	ClearRoomDelegates();
	IOnlineSessionPtr sessions = GetRoomSessions();
	if (Result != EOnJoinSessionCompleteResult::Success)
	{
		FinishRoomOperation(Result == EOnJoinSessionCompleteResult::SessionIsFull
			? TEXT("방 인원이 가득 찼습니다. 다시 검색해 주세요.")
			: TEXT("방 참가에 실패했습니다. 다시 검색해 주세요."));
		return;
	}
	FString address;
	APlayerController* controller = GetFirstLocalPlayerController();
	if (!sessions.IsValid() || !sessions->GetResolvedConnectString(SessionName, address) || !controller)
	{
		OnRoomTravelFailure(GetWorld(), ETravelFailure::ClientTravelFailure, TEXT("서버 주소 확인 실패"));
		return;
	}
	controller->bShowMouseCursor = false;
	controller->SetInputMode(FInputModeGameOnly());
	controller->ClientTravel(address, TRAVEL_Absolute);
}

void UCGameInstance::OnRoomMapLoaded(UWorld* World)
{
	if (World && World->GetGameInstance() == this && bRoomBusy
		&& UGameplayStatics::GetCurrentLevelName(World, true) == TEXT("Level_2"))
	{
		bRoomBusy = false;
		if (IsValid(RoomMenu))
			RoomMenu->RemoveFromParent();
		RoomMenu = nullptr;
		UE_LOG(LogTemp, Display, TEXT("Room: Level_2 travel completed, NetMode=%d"), int32(World->GetNetMode()));
	}
}

void UCGameInstance::OnRoomNetworkFailure(UWorld* World, UNetDriver* Driver, ENetworkFailure::Type Type, const FString& Error)
{
	if (World == GetWorld() && bRoomBusy)
		OnRoomTravelFailure(World, ETravelFailure::ClientTravelFailure, Error);
}

void UCGameInstance::OnRoomTravelFailure(UWorld* World, ETravelFailure::Type Type, const FString& Error)
{
	if (World != GetWorld() || !bRoomBusy)
		return;
	ClearRoomDelegates();
	IOnlineSessionPtr sessions = GetRoomSessions();
	if (sessions.IsValid() && sessions->GetNamedSession(NAME_GameSession))
	{
		if (RoomMenu)
			RoomMenu->SetStatus(TEXT("연결에 실패했습니다. 세션을 정리하고 있습니다..."), true);
		DestroyRoomHandle = sessions->AddOnDestroySessionCompleteDelegate_Handle(
			FOnDestroySessionCompleteDelegate::CreateUObject(this, &UCGameInstance::OnRoomDestroyed));
		if (sessions->DestroySession(NAME_GameSession))
			return;
		ClearRoomDelegates();
	}
	FinishRoomOperation(TEXT("서버 이동에 실패했습니다. 다시 시도하세요."));
}

void UCGameInstance::OnRoomDestroyed(FName SessionName, bool bSuccess)
{
	ClearRoomDelegates();
	FinishRoomOperation(bSuccess ? TEXT("서버 연결에 실패했습니다. 다시 검색해 주세요.")
		: TEXT("세션 정리에 실패했습니다. 게임을 다시 실행해 주세요."));
}

namespace
{
	UTextBlock* AddRoomText(UWidgetTree* Tree, UPanelWidget* Parent, const FName Name, const FString& Text, int32 FontSize = 16)
	{
		UTextBlock* label = Tree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), Name);
		label->SetText(FText::FromString(Text));
		FSlateFontInfo font = label->GetFont();
		font.Size = FontSize;
		label->SetFont(font);
		label->SetAutoWrapText(true);
		Parent->AddChild(label);
		return label;
	}

	UButton* AddRoomButton(UWidgetTree* Tree, UPanelWidget* Parent, const FName Name, const FString& Text)
	{
		USizeBox* size = Tree->ConstructWidget<USizeBox>();
		size->SetHeightOverride(46.0f);
		Parent->AddChild(size);
		UButton* button = Tree->ConstructWidget<UButton>(UButton::StaticClass(), Name);
		button->SetBackgroundColor(FLinearColor(0.10f, 0.25f, 0.38f, 1.0f));
		size->AddChild(button);
		UTextBlock* label = AddRoomText(Tree, button, NAME_None, Text, 18);
		label->SetAutoWrapText(false);
		return button;
	}
}

TSharedRef<SWidget> UCRoomMenuWidget::RebuildWidget()
{
	if (WidgetTree && !WidgetTree->RootWidget)
	{
		if (MenuType == 2)
		{
			UButton* button = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("SelectButton"));
			WidgetTree->RootWidget = button;
			AddRoomText(WidgetTree, button, TEXT("RoomLabel"), TEXT("방 이름     1 / 4명"), 18);
		}
		else
		{
			UCanvasPanel* canvas = WidgetTree->ConstructWidget<UCanvasPanel>();
			WidgetTree->RootWidget = canvas;
			UBorder* backdrop = WidgetTree->ConstructWidget<UBorder>();
			backdrop->SetBrushColor(FLinearColor(0, 0, 0, 0.80f));
			UCanvasPanelSlot* backSlot = canvas->AddChildToCanvas(backdrop);
			backSlot->SetAnchors(FAnchors(0, 0, 1, 1));
			backSlot->SetOffsets(FMargin(0));
			UBorder* card = WidgetTree->ConstructWidget<UBorder>();
			card->SetBrushColor(FLinearColor(0.018f, 0.026f, 0.043f, 1));
			card->SetPadding(FMargin(32));
			UCanvasPanelSlot* cardSlot = canvas->AddChildToCanvas(card);
			cardSlot->SetAnchors(FAnchors(0.5f));
			cardSlot->SetAlignment(FVector2D(0.5f));
			cardSlot->SetSize(FVector2D(MenuType == 0 ? 580 : 720, 690));
			UVerticalBox* content = WidgetTree->ConstructWidget<UVerticalBox>();
			card->AddChild(content);
			AddRoomText(WidgetTree, content, NAME_None, TEXT("LAN MULTIPLAYER"), 12);
			AddRoomText(WidgetTree, content, NAME_None, MenuType == 0 ? TEXT("방 만들기") : TEXT("방 참가하기"), 30);
			AddRoomText(WidgetTree, content, NAME_None, MenuType == 0
				? TEXT("함께 플레이할 방을 설정하세요.") : TEXT("같은 네트워크의 공개 방을 검색합니다."), 15);
			if (MenuType == 0)
			{
				AddRoomText(WidgetTree, content, NAME_None, TEXT("방 이름 (최대 32자)"));
				UEditableTextBox* name = WidgetTree->ConstructWidget<UEditableTextBox>(UEditableTextBox::StaticClass(), TEXT("RoomNameInput"));
				name->SetForegroundColor(FLinearColor(0.02f, 0.03f, 0.05f, 1));
				name->SetHintText(FText::FromString(TEXT("방 이름을 입력하세요")));
				content->AddChild(name);
				AddRoomText(WidgetTree, content, NAME_None, TEXT("최대 인원 (방장 포함 2~16명)"));
				USpinBox* capacity = WidgetTree->ConstructWidget<USpinBox>(USpinBox::StaticClass(), TEXT("CapacityInput"));
				capacity->SetForegroundColor(FSlateColor(FLinearColor(0.02f, 0.03f, 0.05f, 1)));
				capacity->SetMinValue(2);
				capacity->SetMaxValue(16);
				capacity->SetMinSliderValue(2);
				capacity->SetMaxSliderValue(16);
				capacity->SetDelta(1);
				capacity->SetMinFractionalDigits(0);
				capacity->SetMaxFractionalDigits(0);
				capacity->SetValue(4);
				content->AddChild(capacity);
				UCheckBox* publicity = WidgetTree->ConstructWidget<UCheckBox>(UCheckBox::StaticClass(), TEXT("PublicInput"));
				publicity->SetIsChecked(true);
				content->AddChild(publicity);
				AddRoomText(WidgetTree, publicity, NAME_None, TEXT("공개 방 — 검색 목록에 표시"));
				AddRoomText(WidgetTree, content, NAME_None, TEXT("비공개 방은 목록에 표시되지 않습니다. 초대·방 코드 접속은 지원하지 않습니다."), 14);
			}
			else
			{
				AddRoomText(WidgetTree, content, NAME_None, TEXT("방 이름                         현재 / 최대 인원"), 16);
				UScrollBox* list = WidgetTree->ConstructWidget<UScrollBox>(UScrollBox::StaticClass(), TEXT("RoomRows"));
				content->AddChild(list);
				Cast<UVerticalBoxSlot>(list->Slot)->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
				AddRoomButton(WidgetTree, content, TEXT("RefreshButton"), TEXT("새로고침"));
			}
			AddRoomText(WidgetTree, content, TEXT("StatusLabel"), TEXT(""), 15);
			AddRoomButton(WidgetTree, content, TEXT("ConfirmButton"), MenuType == 0 ? TEXT("생성") : TEXT("선택한 방 참가"));
			AddRoomButton(WidgetTree, content, TEXT("BackButton"), TEXT("닫기"));
			for (int32 i = 0; i < content->GetChildrenCount(); ++i)
			{
				UVerticalBoxSlot* slot = Cast<UVerticalBoxSlot>(content->GetChildAt(i)->Slot);
				if (slot)
					slot->SetPadding(FMargin(0, 7));
			}
		}
	}
	return Super::RebuildWidget();
}

void UCRoomMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (MenuType == 2)
	{
		if (UButton* button = Cast<UButton>(GetWidgetFromName(TEXT("SelectButton"))))
			button->OnClicked.AddUniqueDynamic(this, &UCRoomMenuWidget::SelectRoom);
		return;
	}
	if (UButton* button = Cast<UButton>(GetWidgetFromName(TEXT("ConfirmButton"))))
	{
		if (MenuType == 0)
			button->OnClicked.AddUniqueDynamic(this, &UCRoomMenuWidget::CreateRoom);
		else
			button->OnClicked.AddUniqueDynamic(this, &UCRoomMenuWidget::JoinRoom);
	}
	if (UButton* button = Cast<UButton>(GetWidgetFromName(TEXT("RefreshButton"))))
		button->OnClicked.AddUniqueDynamic(this, &UCRoomMenuWidget::SearchRooms);
	if (UButton* button = Cast<UButton>(GetWidgetFromName(TEXT("BackButton"))))
		button->OnClicked.AddUniqueDynamic(this, &UCRoomMenuWidget::CloseMenu);
	SetStatus(MenuType == 0 ? TEXT("방 설정을 입력한 뒤 생성 버튼을 누르세요.") : TEXT("검색을 준비하고 있습니다."), false);
}

void UCRoomMenuWidget::SetStatus(const FString& Message, bool bBusy)
{
	if (UTextBlock* status = Cast<UTextBlock>(GetWidgetFromName(TEXT("StatusLabel"))))
		status->SetText(FText::FromString(Message));
	UCGameInstance* game = GetGameInstance<UCGameInstance>();
	for (const FName name : { FName(TEXT("ConfirmButton")), FName(TEXT("RefreshButton")), FName(TEXT("BackButton")),
		FName(TEXT("RoomNameInput")), FName(TEXT("CapacityInput")), FName(TEXT("PublicInput")), FName(TEXT("RoomRows")) })
	{
		if (UWidget* widget = GetWidgetFromName(name))
			widget->SetIsEnabled(!bBusy);
	}
	if (MenuType == 1 && game)
		if (UWidget* button = GetWidgetFromName(TEXT("ConfirmButton")))
			button->SetIsEnabled(!bBusy && game->GetRooms().IsValidIndex(game->GetSelectedRoom())
				&& game->GetRooms()[game->GetSelectedRoom()].Players < game->GetRooms()[game->GetSelectedRoom()].Capacity);
}

void UCRoomMenuWidget::RefreshRooms()
{
	UCGameInstance* game = GetGameInstance<UCGameInstance>();
	UScrollBox* list = Cast<UScrollBox>(GetWidgetFromName(TEXT("RoomRows")));
	if (!game || !list)
		return;
	list->ClearChildren();
	UClass* rowClass = LoadClass<UCRoomMenuWidget>(nullptr, TEXT("/Game/Widgets/WB_RoomEntry.WB_RoomEntry_C"));
	if (!rowClass)
		return;
	for (int32 i = 0; i < game->GetRooms().Num(); ++i)
	{
		const FCRoomInfo& room = game->GetRooms()[i];
		UCRoomMenuWidget* row = CreateWidget<UCRoomMenuWidget>(GetOwningPlayer(), rowClass);
		if (!row)
			continue;
		row->ResultIndex = i;
		list->AddChild(row);
		row->TakeWidget();
		if (UTextBlock* label = Cast<UTextBlock>(row->GetWidgetFromName(TEXT("RoomLabel"))))
			label->SetText(FText::FromString(FString::Printf(TEXT("%s%s     %d / %d명%s"),
				i == game->GetSelectedRoom() ? TEXT("▶ ") : TEXT(""), *room.Name,
				room.Players, room.Capacity, room.Players >= room.Capacity ? TEXT(" (가득 참)") : TEXT(""))));
		if (UButton* button = Cast<UButton>(row->GetWidgetFromName(TEXT("SelectButton"))))
			button->SetBackgroundColor(i == game->GetSelectedRoom()
				? FLinearColor(0.1f, 0.5f, 0.65f, 1) : FLinearColor(0.08f, 0.15f, 0.23f, 1));
	}
}

void UCRoomMenuWidget::CreateRoom()
{
	UCGameInstance* game = GetGameInstance<UCGameInstance>();
	UEditableTextBox* name = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("RoomNameInput")));
	USpinBox* capacity = Cast<USpinBox>(GetWidgetFromName(TEXT("CapacityInput")));
	UCheckBox* publicity = Cast<UCheckBox>(GetWidgetFromName(TEXT("PublicInput")));
	if (game && name && capacity && publicity)
		game->CreateRoom(name->GetText().ToString(), FMath::RoundToInt(capacity->GetValue()), publicity->IsChecked());
}

void UCRoomMenuWidget::SearchRooms()
{
	if (UCGameInstance* game = GetGameInstance<UCGameInstance>())
		game->SearchRooms();
}

void UCRoomMenuWidget::JoinRoom()
{
	if (UCGameInstance* game = GetGameInstance<UCGameInstance>())
		game->JoinRoom(game->GetSelectedRoom());
}

void UCRoomMenuWidget::SelectRoom()
{
	if (UCGameInstance* game = GetGameInstance<UCGameInstance>())
		game->SelectRoom(ResultIndex);
}

void UCRoomMenuWidget::CloseMenu()
{
	if (UCGameInstance* game = GetGameInstance<UCGameInstance>())
		game->CloseRoomMenu();
}

void UCRoomPlayModeWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (UButton* button = Cast<UButton>(GetWidgetFromName(TEXT("HostButton"))))
	{
		button->OnClicked.RemoveAll(this);
		button->OnClicked.AddUniqueDynamic(this, &UCRoomPlayModeWidget::OpenSettings);
	}
	if (UButton* button = Cast<UButton>(GetWidgetFromName(TEXT("JoinButton"))))
	{
		button->OnClicked.RemoveAll(this);
		button->OnClicked.AddUniqueDynamic(this, &UCRoomPlayModeWidget::OpenRoomList);
	}
}

void UCRoomPlayModeWidget::OpenSettings()
{
	if (UCGameInstance* game = GetGameInstance<UCGameInstance>())
		game->ShowRoomMenu(false);
}

void UCRoomPlayModeWidget::OpenRoomList()
{
	if (UCGameInstance* game = GetGameInstance<UCGameInstance>())
		game->ShowRoomMenu(true);
}
