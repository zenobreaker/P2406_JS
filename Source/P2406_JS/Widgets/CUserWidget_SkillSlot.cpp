#include "Widgets/CUserWidget_SkillSlot.h"
#include "Global.h"
#include "Skill/CActiveSkill.h"


void UCUserWidget_SkillSlot::NativeConstruct()
{
    Super::NativeConstruct();

    // C++ �ʱ�ȭ �ڵ�
    //UE_LOG(LogTemp, Warning, TEXT("NativeConstruct called!"));

}

void UCUserWidget_SkillSlot::OnSlotClear()
{
    // nullptr�����ؼ� ���ο��� �̿�Ұ� ó�� �������� ó���ϰ� �غ��� 
    OnSkillSlot(nullptr);
}
