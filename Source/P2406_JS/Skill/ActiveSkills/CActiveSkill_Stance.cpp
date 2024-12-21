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
	// 이 스킬은 캐스팅 애니메이션이 긑나면 다음 포지션으로 가기 위한 입력을 받는 로직이 포함되어 있다. 
	// 캐스팅 중엔 입력을 받아도 다음 페이즈로 가지 않도록 하는건 다음 문제이다
	// 
	// 키를 입력 받는 상태가 되면 입력 받는게 이 함수가 호출된다는건
	// 이 스킬이  슬롯으로 부터 호출됨을 의미하는 것 
	// 그러므로 다음 상태로 전이된다. 

	CheckNull(OwnerCharacter);

	UInputComponent* input = FHelpers::GetComponent<UInputComponent>(OwnerCharacter);
	CheckNull(input);


	UCWeaponComponent* weapon = FHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter);
	CheckNull(weapon)

	// 기존 Action 이벤트 제거 
	input->RemoveActionBinding("Action", EInputEvent::IE_Pressed);

	input->BindAction("Action", EInputEvent::IE_Pressed, weapon,
		&UCWeaponComponent::DoAction);

	StartNextPhase(); 
}

void UCActiveSkill_Stance::Begin_WaitInput()
{
	Super::Begin_WaitInput();
	// 입력을 대기한다. 특정 키 입력이 오면 다음 동작으로 간다.

	// 입력을 받을 동안 특정 모션을 실행한다. 

	// 여기서 이 함수 위에 함수를 호출하는게 좋을듯?

	// 스킬의 선행 동작이 끝날 때까지 공격 입력을 받아야 하니까 .. 
	CheckNull(OwnerCharacter); 

	UInputComponent* input = FHelpers::GetComponent<UInputComponent>(OwnerCharacter);
	CheckNull(input);

	// 기존 Action 이벤트 제거 
	input->RemoveActionBinding("Action", EInputEvent::IE_Pressed);

	// 서브 액션 중일 때 이벤트로 변경하기
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
