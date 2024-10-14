#include "Weapons/SubActions/CSubAction_Sword.h"
#include "Global.h"
#include "Weapons/CDoAction.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"

UCSubAction_Sword::UCSubAction_Sword()
{
}

void UCSubAction_Sword::Pressed()
{
	CheckTrue(DoAction->GetInAction()); // �⺻ ���� �߿��� ��� �� �ϰ� ���´�.
	CheckTrue(GetInAction());	// ������ �׼� ���̸� ó�� 
	CheckFalse(State->IsIdleMode());

	Super::Pressed();

	if (DoSubActionDatas.Num() > 0)
	{
		CLog::Log("Sub Action Call");
		DoSubActionDatas[this->Index].DoAction(Owner);

		State->SetActionMode();
	}
}

void UCSubAction_Sword::Released()
{
	//Super::Released();
}
	
void UCSubAction_Sword::End_DoSubAction()
{
	bInAction = false;
}

void UCSubAction_Sword::GetSubActionDatas(const TArray<FDoActionData>& InDoSubActionDatas)
{
	CLog::Log("Sub Action Datas Set" + Owner->GetFName().ToString());
	DoSubActionDatas = InDoSubActionDatas;
}

void UCSubAction_Sword::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther)
{
	CheckNull(InOther);

	for (ACharacter* hitted : Hitted)
		CheckTrue(hitted == InOther);

	// ���� ����.


}
