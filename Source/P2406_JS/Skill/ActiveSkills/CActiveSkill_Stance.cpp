#include "Skill/ActiveSkills/CActiveSkill_Stance.h"
#include "Global.h"
#include "GameFramework/Character.h"


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

	//StartNextPhase(); 
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

void UCActiveSkill_Stance::DefineSkillPhases()
{
	SetupDefaultSkillPhase();
}
