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
	// �� ��ų�� ĳ���� �ִϸ��̼��� �P���� ���� ���������� ���� ���� �Է��� �޴� ������ ���ԵǾ� �ִ�. 
	// ĳ���� �߿� �Է��� �޾Ƶ� ���� ������� ���� �ʵ��� �ϴ°� ���� �����̴�
	// 
	// Ű�� �Է� �޴� ���°� �Ǹ� �Է� �޴°� �� �Լ��� ȣ��ȴٴ°�
	// �� ��ų��  �������� ���� ȣ����� �ǹ��ϴ� �� 
	// �׷��Ƿ� ���� ���·� ���̵ȴ�. 

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

	// �÷��̾�� �÷��̾��� �Է� ���� ����� ���� �ش� Ű�� �������� �˻� 
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

	// �÷��̾�� �÷��̾��� �Է� ���� ����� ���� �ش� Ű�� �������� �˻� 
	player->SetCanInput(true);

	if (CheckReleasedSetKey())
	{
		FLog::Log("Input_Anykey - Release Input ");
		bCanInput = false;

		OnReleasedKey();
	}
}
