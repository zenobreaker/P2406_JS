#include "Skill/ActiveSkills/CActiveSkill_Stance.h"
#include "Global.h"
#include "GameFramework/Character.h"

#include "Characters/CPlayer.h"


void UCActiveSkill_Stance::BeginPlay_ActiveSkill(ACharacter* InOwner, FSkillFlowData InFlowData)
{
	Super::BeginPlay_ActiveSkill(InOwner, InFlowData);

	CheckNull(InOwner);

	PlayerController = Cast<APlayerController>(InOwner->GetController());
}


void UCActiveSkill_Stance::SetInputKey(FKey InTargetKey)
{
	InputKeyTable.Emplace(InTargetKey, false);
}


void UCActiveSkill_Stance::Input_AnyKey()
{
	// 이 스킬은 캐스팅 애니메이션이 긑나면 다음 포지션으로 가기 위한 입력을 받는 로직이 포함되어 있다. 
	// 캐스팅 중엔 입력을 받아도 다음 페이즈로 가지 않도록 하는건 다음 문제이다
	// 
	// 키를 입력 받는 상태가 되면 입력 받는게 이 함수가 호출된다는건
	// 이 스킬이  슬롯으로 부터 호출됨을 의미하는 것 
	// 그러므로 다음 상태로 전이된다. 

	PressKey();
}


void UCActiveSkill_Stance::DefineSkillPhases()
{
	SetupDefaultSkillPhase();
}

bool UCActiveSkill_Stance::CheckPressedSetKey(FKey InKey)
{
	CheckNullResult(PlayerController, false);
	
	if (PlayerController->IsInputKeyDown(InKey) == true)
		return true;

	return false; 
}

bool UCActiveSkill_Stance::CheckPressedSetKey()
{
	CheckNullResult(OwnerCharacter, false);

	
	for (auto& keyPair : InputKeyTable)
	{
		if (CheckPressedSetKey(keyPair.Key) == true)
			return true; 
	}

	return false;
}

bool UCActiveSkill_Stance::CheckReleasedSetKey(FKey InKey)
{
	CheckNullResult(PlayerController, false);

	if(PlayerController->WasInputKeyJustReleased(InKey) == true)
		return true;

	return false; 
}

bool UCActiveSkill_Stance::CheckReleasedSetKey()
{
	CheckNullResult(OwnerCharacter, false);

	for (auto& keyPair : InputKeyTable)
	{
		if (CheckReleasedSetKey(keyPair.Key) == true)
			return true; 
	}

	return false;
}

void UCActiveSkill_Stance::PressKey()
{
	CheckNull(OwnerCharacter);

	ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
	CheckNull(player);

	// 플레이어면 플레이어의 입력 동작 기능을 막고 해당 키가 눌렀는지 검사 
	player->SetCanInput(false);

	if (CheckPressedSetKey())
	{
		FLog::Log("Input_Anykey - Do Input ");
		bCanInput = true;
		
		OnPressedKey();
	}
}

void UCActiveSkill_Stance::ReleaseKey()
{
	CheckNull(OwnerCharacter);

	ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
	CheckNull(player);

	// 플레이어면 플레이어의 입력 동작 기능을 막고 해당 키가 눌렀는지 검사 
	player->SetCanInput(true);

	if (CheckReleasedSetKey())
	{
		FLog::Log("Input_Anykey - Release Input ");
		bCanInput = false;

		OnReleasedKey();
	}
}
