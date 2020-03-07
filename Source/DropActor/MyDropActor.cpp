// Fill out your copyright notice in the Description page of Project Settings.


#include "MyDropActor.h"
#include "Components/BoxComponent.h"

#if WITH_EDITOR
#include "Editor.h"
#include "UnrealEd.h"
#include "LevelEditor.h"
#include "ModuleManager.h"
#endif


// Sets default values
AMyDropActor::AMyDropActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMyDropActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMyDropActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

#if WITH_EDITOR

void AMyDropActor::SpawnOne()
{
	if (SpawnTargetClass == nullptr)
	{
		return;
	}

	auto EditorWorld = GEditor->GetEditorWorldContext().World();

	const auto BoxComponent = CastChecked<UBoxComponent>(GetRootComponent());

	const auto Origin = GetActorLocation();

	const auto BoxExtent = BoxComponent->GetScaledBoxExtent();

	const auto Box = FBox::BuildAABB(Origin, BoxExtent);

	auto Pos = FMath::RandPointInBox(Box);
	Pos.Z = Box.Max.Z;

	const auto NewActor = EditorWorld->SpawnActor(SpawnTargetClass, &Pos);
	SpawnActors.Add(NewActor);
}

void AMyDropActor::ClearSpawns()
{
	for (auto SpawnActor : SpawnActors)
	{
		SpawnActor->Destroy();
	}

	SpawnActors.Empty();
}

void AMyDropActor::StartSimulate() const
{
	// Is a simulation session already running?  If so, do nothing
	if (GEditor->PlayWorld != nullptr && GUnrealEd->bIsSimulatingInEditor)
	{
		return;
	}

	auto& LevelEditorModule = FModuleManager::GetModuleChecked<FLevelEditorModule>(TEXT("LevelEditor"));

	const auto ActiveLevelViewport = LevelEditorModule.GetFirstActiveViewport();
	if (ActiveLevelViewport.IsValid())
	{
		// Start a new simulation session!
		if (GEditor->PlayWorld == nullptr)
		{
			auto PlaySettings = GetMutableDefault<ULevelEditorPlaySettings>();
			PlaySettings->LastExecutedPlayModeType = PlayMode_Simulate;

			FPropertyChangedEvent PropChangeEvent(
				ULevelEditorPlaySettings::StaticClass()->FindPropertyByName(
					GET_MEMBER_NAME_CHECKED(ULevelEditorPlaySettings, LastExecutedPlayModeType)));
			PlaySettings->PostEditChangeProperty(PropChangeEvent);

			PlaySettings->SaveConfig();

			GUnrealEd->RequestPlaySession(false, ActiveLevelViewport, true/*bSimulateInEditor*/, nullptr, nullptr, -1,
			                              false);
		}
	}
}

void AMyDropActor::SaveSimulate()
{
	const auto EditorWorld = GEditor->GetEditorWorldContext().World();

	TArray<AActor*> OutDropActors;
	UGameplayStatics::GetAllActorsOfClass(EditorWorld, StaticClass(), OutDropActors);

	for (auto Elem : OutDropActors)
	{
		auto DropActor = Cast<AMyDropActor>(Elem);

		for (auto SpawnActor : DropActor->SpawnActors)
		{
			const auto SimWorldActor = EditorUtilities::GetSimWorldCounterpartActor(SpawnActor);

			if (SimWorldActor != nullptr)
			{
				const auto CopyOptions = static_cast<EditorUtilities::ECopyOptions::Type>(EditorUtilities::ECopyOptions
					::
					CallPostEditChangeProperty |
					EditorUtilities::ECopyOptions::CallPostEditMove |
					EditorUtilities::ECopyOptions::OnlyCopyEditOrInterpProperties |
					EditorUtilities::ECopyOptions::FilterBlueprintReadOnly);
				EditorUtilities::CopyActorProperties(
					SimWorldActor, SpawnActor, CopyOptions);
			}
		}
	}
}

#endif
