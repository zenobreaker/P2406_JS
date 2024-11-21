#include "Characters/CGhostTrail.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/PoseableMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"

ACGhostTrail::ACGhostTrail()
{
	CHelpers::CreateComponent<UPoseableMeshComponent>(this, &Mesh, "Mesh");
}

void ACGhostTrail::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());

	UMaterialInstanceConstant* material;
	CHelpers::GetAssetDynamic<UMaterialInstanceConstant>(&material, "MaterialInstanceConstant'/Game/Materials/M_GhostTrail_Inst.M_GhostTrail_Inst'");

	Material = UMaterialInstanceDynamic::Create(material, this);
	Material->SetVectorParameterValue("Color", Color);
	Material->SetScalarParameterValue("Exponent", Exponent);

	Mesh->SetVisibility(false);
	Mesh->SetSkeletalMesh(OwnerCharacter->GetMesh()->GetSkeletalMeshAsset());

	Mesh->CopyPoseFromSkeletalComponent(OwnerCharacter->GetMesh()); // 캡쳐는 무조건 한번은 해야한다.
	Mesh->SetRelativeScale3D(Scale);


	for (int32 i = 0; i < OwnerCharacter->GetMesh()->GetSkeletalMeshAsset()->GetMaterials().Num(); i++)
		Mesh->SetMaterial(i, Material);

	SetSubMeshes(OwnerCharacter->GetMesh());

	FTimerDelegate timerDelegate;
	timerDelegate.BindLambda([this]()
	{
		if (Mesh->IsVisible() == false)
			Mesh->ToggleVisibility();

		SetActorLocation(OwnerCharacter->GetActorLocation() + FVector(0, 0, -90));
		SetActorRotation(OwnerCharacter->GetActorRotation() + FRotator(0, -90, 0));

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
			if (pose)
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
		}
	}
}


