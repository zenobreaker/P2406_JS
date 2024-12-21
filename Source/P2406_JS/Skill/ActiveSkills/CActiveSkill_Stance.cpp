#include "Skill/ActiveSkills/CActiveSkill_Stance.h"
#include "Global.h"
#include "GameFramework/Character.h"

void UCActiveSkill_Stance::BeginPlay(ACharacter* InOwner, const TArray<FSkillActionData>& InDoActionDatas, const TArray<FSkillHitData>& InHitDatas)
{
	OwnerCharacter = InOwner;
	DoActionDatas = InDoActionDatas;
	HitDatas = InHitDatas;

	FLog::Print("Skill Create!");

	RegisterSkillPhase_Stance();
}

void UCActiveSkill_Stance::Tick(float InDeltaTime)
{
	Super::Tick(InDeltaTime);

	 
}

void UCActiveSkill_Stance::SetSkillKey(EKeys InTargetKey)
{
	SkillKey = InTargetKey; 
}


void UCActiveSkill_Stance::Input_AnyKey()
{
	// �� ��ų�� ĳ���� �ִϸ��̼��� �P���� ���� ���������� ���� ���� �Է��� �޴� ������ ���ԵǾ� �ִ�. 
	// ĳ���� �߿� �Է��� �޾Ƶ� ���� ������� ���� �ʵ��� �ϴ°� ���� �����̴�
	// 
	// Ű�� �Է� �޴� ���°� �Ǹ� �Է� �޴°� �� �Լ��� ȣ��ȴٴ°�
	// �� ��ų��  �������� ���� ȣ����� �ǹ��ϴ� �� 
	// �׷��Ƿ� ���� ���·� ���̵ȴ�. 

	CheckNull(OwnerCharacter);

	UInputComponent* input = FHelpers::GetComponent<UInputComponent>(OwnerCharacter);
	CheckNull(input);


	UCWeaponComponent* weapon = FHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter);
	CheckNull(weapon)

	// ���� Action �̺�Ʈ ���� 
	input->RemoveActionBinding("Action", EInputEvent::IE_Pressed);

	input->BindAction("Action", EInputEvent::IE_Pressed, weapon,
		&UCWeaponComponent::DoAction);

	StartNextPhase(); 
}

void UCActiveSkill_Stance::Begin_WaitInput()
{
	Super::Begin_WaitInput();
	// �Է��� ����Ѵ�. Ư�� Ű �Է��� ���� ���� �������� ����.

	// �Է��� ���� ���� Ư�� ����� �����Ѵ�. 

	// ���⼭ �� �Լ� ���� �Լ��� ȣ���ϴ°� ������?

	// ��ų�� ���� ������ ���� ������ ���� �Է��� �޾ƾ� �ϴϱ� .. 
	CheckNull(OwnerCharacter); 

	UInputComponent* input = FHelpers::GetComponent<UInputComponent>(OwnerCharacter);
	CheckNull(input);

	// ���� Action �̺�Ʈ ���� 
	input->RemoveActionBinding("Action", EInputEvent::IE_Pressed);

	// ���� �׼� ���� �� �̺�Ʈ�� �����ϱ�
	input->BindAction("Action", EInputEvent::IE_Pressed, this,
		&UCActiveSkill_Stance::Input_AnyKey);

	//StartNextPhase();
}

void UCActiveSkill_Stance::RegisterSkillPhase_Stance()
{
	SkillPhaseTable.Empty();

	RegisterSkillPhase(ESkillPhase::Begin_Casting,
		TDelegate<void()>::CreateUObject(this, &UCActiveSkill_Stance::Begin_Casting));

	RegisterSkillPhase(ESkillPhase::Casting,
		TDelegate<void()>::CreateUObject(this, &UCActiveSkill_Stance::DoCasting));

	RegisterSkillPhase(ESkillPhase::End_Casting,
		TDelegate<void()>::CreateUObject(this, &UCActiveSkill_Stance::End_Casting));

	RegisterSkillPhase(ESkillPhase::Begin_Skill,
		TDelegate<void()>::CreateUObject(this, &UCActiveSkill::Begin_Skill));

	RegisterSkillPhase(ESkillPhase::WaitingForInput,
		TDelegate<void()>::CreateUObject(this, &UCActiveSkill::Begin_WaitInput));

	RegisterSkillPhase(ESkillPhase::DoAction_Skill,
		TDelegate<void()>::CreateUObject(this, &UCActiveSkill::DoAction_Skill));

	RegisterSkillPhase(ESkillPhase::End_Skill,
		TDelegate<void()>::CreateUObject(this, &UCActiveSkill::End_Skill));
}
