#include "Characters/CGhostTrail.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/PoseableMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"

ACGhostTrail::ACGhostTrail()
{
	FHelpers::CreateComponent<UPoseableMeshComponent>(this, &Mesh, "Mesh");
}

void ACGhostTrail::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());

	UMaterialInstanceConstant* material;
	FHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&material, "MaterialInstanceConstant'/Game/Materials/M_GhostTrail_Inst.M_GhostTrail_Inst'");

	Material = UMaterialInstanceDynamic::Create(material, this);
	Material->SetVectorParameterValue("Color", Color);
	Material->SetScalarParameterValue("Exponent", Exponent);

	Mesh->SetVisibility(false);
	Mesh->SetSkeletalMesh(OwnerCharacter->GetMesh()->GetSkeletalMeshAsset());

	Mesh->CopyPoseFromSkeletalComponent(OwnerCharacter->GetMesh()); // 캡쳐는 무조건 한번은 해야한다.
	Mesh->SetRelativeScale3D(Scale);
	Mesh->SetComponentTickEnabled(true); // 생성 후 업데이트 막기 


	for (int32 i = 0; i < OwnerCharacter->GetMesh()->GetSkeletalMeshAsset()->GetMaterials().Num(); i++)
		Mesh->SetMaterial(i, Material);

	SetSubMeshes(OwnerCharacter->GetMesh());

	// 초기 위치 고정 
	FVector initialLocaton = GetActorLocation();
	FRotator initialRotator = GetActorRotation();

	FTimerDelegate timerDelegate;
	timerDelegate.BindLambda([this, initialLocaton, initialRotator]()
		{
			if (Mesh->IsVisible() == false)
				Mesh->ToggleVisibility();

			SetActorLocation(initialLocaton);
			// 액터는 회전을 따라가고, Mesh는 초기 회전 유지
			SetActorRotation(OwnerCharacter->GetActorRotation());
			Mesh->SetWorldRotation(OwnerCharacter->GetActorRotation());
			Mesh->SetComponentTickEnabled(false); // 생성 후 업데이트 막기 

			// 포즈 유지 (위치는 변동 없음)
			Mesh->CopyPoseFromSkeletalComponent(OwnerCharacter->GetMesh());
			CopySubMeshes(OwnerCharacter->GetMesh());
		});

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, timerDelegate, Interval, true, StartDelay);
}

void ACGhostTrail::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

//TODO: 오브젝트 풀링을 한다면 이 함수는 따로 처리해야함
void ACGhostTrail::DestroyTrail()
{
	// 서브 메쉬들 있으면 삭제
	if (SubMeshes.Num() > 0)
	{
		for (int i = SubMeshes.Num() - 1; i >= 0; i--)
		{
			SubMeshes[i]->DestroyComponent();
		}
	}

	// 메인 메쉬 삭제 
	Mesh->DestroyComponent();

	// 이 클래스 삭제 
	//this->Destroy(); 
}

void ACGhostTrail::SetSubMeshes(USkeletalMeshComponent* InParentMesh)
{
	CheckNull(Mesh);

	// 캐릭터의 스켈레탈에서 자식들 가져오기 
	const int32 childCount = InParentMesh->GetNumChildrenComponents();

	for (int32 i = 0; i < childCount; i++)
	{
		// 자식 컴포넌트 
		USceneComponent* child = InParentMesh->GetChildComponent(i);

		// 자식이 SkeletalMeshComponent라면 포즈 복사
		if (USkeletalMeshComponent* childMesh = Cast<USkeletalMeshComponent>(child))
		{
			UPoseableMeshComponent* pose = NewObject<UPoseableMeshComponent>(this);
			if (pose != nullptr)
			{
				pose->AttachToComponent(Mesh, FAttachmentTransformRules::KeepRelativeTransform);
				pose->RegisterComponent();

				pose->SetVisibility(false);
				pose->SetSkeletalMesh(childMesh->GetSkeletalMeshAsset());
				// 자식의 포즈는 부모에서 처리
				pose->CopyPoseFromSkeletalComponent(InParentMesh);
				pose->SetRelativeScale3D(Scale);

				if (!!childMesh->GetSkeletalMeshAsset())
				{
					for (int32 j = 0; j < childMesh->GetSkeletalMeshAsset()->GetMaterials().Num(); j++)
						pose->SetMaterial(j, Material);
				}


				SubMeshes.Add(pose);
			}
		}
	}
}

void ACGhostTrail::CopySubMeshes(USkeletalMeshComponent* InParentMesh)
{
	CheckNull(InParentMesh);


	for (int32 i = 0; i < SubMeshes.Num(); i++)
	{
		if (SubMeshes[i]->IsVisible() == false)
			SubMeshes[i]->ToggleVisibility();

		// 자식 컴포넌트 
		USceneComponent* child = InParentMesh->GetChildComponent(i);
		if (USkeletalMeshComponent* childMesh = Cast<USkeletalMeshComponent>(child))
		{
			// 자식의 포즈는 부모에서 처리
			SubMeshes[i]->CopyPoseFromSkeletalComponent(InParentMesh);
			SubMeshes[i]->SetComponentTickEnabled(false);
		}
	}
}


