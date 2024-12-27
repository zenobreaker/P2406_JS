#pragma once

#include "CoreMinimal.h"

#define CheckTrue(x) { if(x == true) return; }
#define CheckTrueResult(x, y) { if(x == true) return y; }

#define CheckFalse(x) { if(x == false) return;}
#define CheckFalseResult(x, y) { if(x == false) return y;}

#define CheckNull(x) { if(x == nullptr) return; }
#define CheckNullResult(x, y) { if(x == nullptr) return y;}

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
    if (Component) \
	{ \
        Component->EventName.AddDynamic(Listener, &Function); \
    }\
}

// 이벤트 해제 매크로
#define UNREGISTER_EVENT(Component, EventName, Listener, Function) \
{\
	if (Component) \
	{\
		Component->EventName.RemoveDynamic(Listener, &Function); \
	}\
}

// 이벤트 등록: 이미 등록된 리스너가 있으면 먼저 삭제하고 새로운 리스너를 등록
#define REGISTER_EVENT_WITH_REPLACE(Component, EventName, Listener, Function)\
{\
    if (Component)\
	 {\
        Component->EventName.RemoveDynamic(Listener, &Function);  \
        Component->EventName.AddDynamic(Listener, &Function);     \
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

	template<typename TValueType>
	static void GetAsset(TValueType** OutObject, FString InPath)
	{
		ConstructorHelpers::FObjectFinder<TValueType> asset(*InPath);
		if(asset.Succeeded())
			*OutObject = asset.Object;
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
};