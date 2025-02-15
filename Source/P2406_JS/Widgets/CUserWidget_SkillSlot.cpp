#include "Widgets/CUserWidget_SkillSlot.h"
#include "Global.h"
#include "Skill/CActiveSkill.h"


void UCUserWidget_SkillSlot::NativeConstruct()
{
    Super::NativeConstruct();

    // C++ 초기화 코드
    //UE_LOG(LogTemp, Warning, TEXT("NativeConstruct called!"));

}

void UCUserWidget_SkillSlot::OnSlotClear()
{
    // nullptr전달해서 내부에서 이용불가 처리 로직으로 처리하게 해보기 
    OnSkillSlot(nullptr);
}
