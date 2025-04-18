#pragma once

#include "CoreMinimal.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "Particles/ParticleSystemComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

#define CheckTrue(x) { if((x) == true) return; }
#define CheckTrueResult(x, y) { if((x) == true) return y; }

#define CheckFalse(x) { if(x == false) return;}
#define CheckFalseResult(x, y) { if((x) == false) return y;}

#define CheckNull(x) { if(x == nullptr) return; }
#define CheckNullResult(x, y) { if((x) == nullptr) return y;}

#define CreateTextRenderComponent()\
{\
	FHelpers::CreateComponent<UTextRenderComponent>(this, &Text, "Text", Root);\
	Text->SetRelativeLocation(FVector(0, 0, 100));\
	Text->SetRelativeRotation(FRotator(0, 180, 0));\
	Text->SetRelativeScale3D(FVector(2));\
	Text->TextRenderColor = FColor::Red;\
	Text->HorizontalAlignment = EHorizTextAligment::EHTA_Center;\
	Text->Text = FText::FromString(GetName().Replace(L"Default__", L""));\
}
// 이벤트 등록 매크로
#define REGISTER_EVENT(Component, EventName, Listener, Function)\
{ \
    if (Component && Listener) \
	{ \
        Component->EventName.AddDynamic(Listener, &Function); \
    }\
	else if(Component == nullptr)\
		FLog::Log(FString::Printf(TEXT("Component is Null. Tried to use: %s"), TEXT(#Component)));\
	else \
		FLog::Log(FString::Printf(TEXT("Listener is Null. Tried to use: %s"), TEXT(#Listener)));\
}

// 이벤트 해제 매크로
#define UNREGISTER_EVENT(Component, EventName, Listener, Function) \
{\
	if (Component && Listener) \
	{\
		Component->EventName.RemoveDynamic(Listener, &Function); \
	}\
	else if(Component == nullptr)\
		FLog::Log(FString::Printf(TEXT("Component is Null. Tried to use: %s"), TEXT(#Component)));\
	else \
		FLog::Log(FString::Printf(TEXT("Listener is Null. Tried to use: %s"), TEXT(#Listener)));\
}

// 이벤트 등록: 이미 등록된 리스너가 있으면 먼저 삭제하고 새로운 리스너를 등록
#define REGISTER_EVENT_WITH_REPLACE(Component, EventName, Listener, Function)\
{\
    if (Component && Listener)\
	 {\
        Component->EventName.RemoveDynamic(Listener, &Function);\
        Component->EventName.AddDynamic(Listener, &Function);\
    }\
	else if(Component == nullptr)\
		FLog::Log(FString::Printf(TEXT("Component is Null. Tried to use: %s"), TEXT(#Component)));\
	else \
		FLog::Log(FString::Printf(TEXT("Listener is Null. Tried to use: %s"), TEXT(#Listener)));\
}

// 다이나믹 이벤트 콜 : 이벤트를 검사한 후 있으면 호출
#define DYNAMIC_EVENT_CALL(EventName)\
{\
	if(EventName.IsBound())\
		EventName.Broadcast();\
}

#define DYNAMIC_EVENT_CALL_ONE_PARAM(EventName, InParam)\
{\
	if(EventName.IsBound())\
		EventName.Broadcast(InParam);\
}

#define DYNAMIC_EVENT_CALL_TWO_PARAMS(EventName, InParam1, InParam2)\
{\
	if(EventName.IsBound())\
	{\
		EventName.Broadcast(InParam1, InParam2); \
	}\
}

#define DYNAMIC_EVENT_CALL_THREE_PARAMS(EventName, InParam1, InParam2, InParam3)\
{\
	if(EventName.IsBound())\
	{\
		EventName.Broadcast(InParam1, InParam2, InParam3); \
	}\
}



class P2406_JS_API FHelpers
{
public:
	template<typename TValueType>
	static void CreateComponent(AActor* InActor, TValueType** OutComponent, FName InName, USceneComponent* InParent = nullptr)
	{
		*OutComponent = InActor->CreateDefaultSubobject<TValueType>(InName);

		if (!!InParent)
		{
			(*OutComponent)->SetupAttachment(InParent);

			return;
		}

		InActor->SetRootComponent(*OutComponent);
	}

	template<typename TValueType>
	static void CreateActorComponent(AActor* InActor, TValueType** OutComponent, FName InName)
	{
		*OutComponent = InActor->CreateDefaultSubobject<TValueType>(InName);
	}

	template<typename TValueType>
	static void CreateActorComponent(AActor* InActor, TObjectPtr<TValueType>* OutComponent, FName InName)
	{
		*OutComponent = InActor->CreateDefaultSubobject<TValueType>(InName);
	}

	template<typename TChild, typename TParent>
	static void CreateParentActorComponent(AActor* InActor, TParent** OutComponent, FName InName)
	{
		// 부모-자식 관계 검증 (컴파일 타임에서 오류 발생 가능)
		static_assert(std::is_base_of_v<TParent, TChild>, "TChild must be a subclass of TParent");

		// ActorComponent 생성
		TChild* NewComponent = InActor->CreateDefaultSubobject<TChild>(InName);

		// 부모 클래스 포인터에 저장
		*OutComponent = NewComponent;
	}

	template<typename TValueType>
	static void GetAsset(TValueType** OutObject, FString InPath)
	{
		ConstructorHelpers::FObjectFinder<TValueType> asset(*InPath);
		if (asset.Succeeded())
			*OutObject = asset.Object;
	}


	template<typename TValueType>
	static void GetAssetAsync(TValueType** OutObject, FString InPath, TFunction<void()> OnLoadedCallback = nullptr)
	{

		// 경로가 비어 있으면 기본 객체 생성 후 콜백 실행 
		if (InPath.IsEmpty())
		{
			*OutObject = NewObject<TValueType>();
			if (OnLoadedCallback)
				OnLoadedCallback();
			return; 
		}


		TSoftObjectPtr<TValueType> softAsset(InPath);

		if (softAsset.IsValid())
		{
			*OutObject = softAsset.Get();
			return;
		}

		FStreamableManager& streamable = UAssetManager::GetStreamableManager();
		streamable.RequestAsyncLoad(softAsset.ToSoftObjectPath(), [OutObject, softAsset, OnLoadedCallback]() mutable
			{
				if (softAsset.IsValid())
				{
					*OutObject = softAsset.Get();
					if (OnLoadedCallback)
						OnLoadedCallback();
				}
			});
	}

	template<typename TValueType>
	static void GetAssetDynamic(TValueType** OutObject, FString InPath)
	{
		*OutObject = Cast<TValueType>(StaticLoadObject(TValueType::StaticClass(), nullptr, *InPath));
	}

	template<typename TClassName>
	static void GetClass(TSubclassOf<TClassName>* OutClass, FString InPath)
	{
		ConstructorHelpers::FClassFinder<TClassName> asset(*InPath);
		*OutClass = asset.Class;
	}

	template<typename TFindType>
	static TFindType* FindActor(UWorld* InWorld)
	{
		for (AActor* actor : InWorld->GetCurrentLevel()->Actors)
		{
			if (!!actor && actor->IsA<TFindType>())
				return Cast<TFindType>(actor);
		}
		//
		//#ifdef DEBUG
		//		ensure 
		//#endif

		return nullptr;
	}

	template<typename T>
	static void FindActors(UWorld* InWorld, TArray<T*>& OutActors)
	{
		for (AActor* actor : InWorld->GetCurrentLevel()->Actors)
		{
			if (!!actor && actor->IsA<T>())
				OutActors.Add(Cast<T>(actor));
		}
	}

	template<typename T>
	static T* GetComponent(AActor* InActor)
	{
		return Cast<T>(InActor->GetComponentByClass(T::StaticClass()));
	}

	template<typename T, typename U>
	static T* GetComponent(TWeakObjectPtr<U> InActor)
	{
		if (!InActor.IsValid())
			return nullptr;
		
		return Cast<T>(InActor->GetComponentByClass(T::StaticClass()));
	}

	//template<typename T, typename U>
	//static TWeakObjectPtr<T> GetComponent(* InActor)
	//{
	//	if (!IsValid(InActor))
	//		return nullptr;

	//	T* Component = Cast<T>(InActor->GetComponentByClass(T::StaticClass()));
	//	return TWeakObjectPtr<T>(Component);
	//}

	template<typename T>
	static T* GetComponent(AActor* InActor, const FString& InName)
	{
		TArray<T*> components;
		InActor->GetComponents<T>(components);

		for (T* component : components)
		{
			if (component->GetName() == InName)
				return component;
		}

		return nullptr;
	}

	static void AttachTo(AActor* InActor, USceneComponent* InParent, FName InSocketName = "NAME_none", EAttachmentRule InRule = EAttachmentRule::KeepRelative)
	{
		InActor->AttachToComponent(InParent, FAttachmentTransformRules(InRule, true), InSocketName);
	}


	static void PlayEffect(UWorld* InWorld, UFXSystemAsset* InAsset, const FTransform& InTransform, USceneComponent* InAttach = nullptr, FName InSocketName = NAME_None)
	{
		UParticleSystem* particle = Cast<UParticleSystem>(InAsset);
		UNiagaraSystem* niagara = Cast<UNiagaraSystem>(InAsset);


		FVector location = InTransform.GetLocation();
		FRotator rotator = FRotator(InTransform.GetRotation());
		FVector scale = InTransform.GetScale3D();

		if (!!InAttach)
		{
			if (!!particle)
			{
				UGameplayStatics::SpawnEmitterAttached(particle, InAttach, InSocketName, location, rotator, scale);

				return;
			}

			if (!!niagara)
			{
				UNiagaraFunctionLibrary::SpawnSystemAttached(niagara, InAttach, InSocketName, location, rotator, scale, EAttachLocation::KeepRelativeOffset, true, ENCPoolMethod::None);

				return;
			}
		}

		if (!!particle)
		{
			UGameplayStatics::SpawnEmitterAtLocation(InWorld, particle, InTransform);

			return;
		}

		if (!!niagara)
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(InWorld, niagara, location, rotator, scale);
	}

};