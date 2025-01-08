#pragma once

#include "CoreMinimal.h"

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

#define DYNAMIC_EVENT_CALL_TWO_PARAM(EventName, InParam1, InParam2)\
{\
	if(EventName.IsBound())\
	{\
		EventName.Broadcast(InParam1, InParam2); \
	}\
}

#define DYNAMIC_EVENT_CALL_THREE_PARAM(EventName, InParam1, InParam2, InParam3)\
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

	static void AttachTo(AActor* InActor, USceneComponent* InParent, FName InSocketName, EAttachmentRule InRule = EAttachmentRule::KeepRelative)
	{
		InActor->AttachToComponent(InParent, FAttachmentTransformRules(InRule, true), InSocketName);
	}

};