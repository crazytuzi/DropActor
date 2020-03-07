// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "MyDropActor.generated.h"

UCLASS()
class DROPACTOR_API AMyDropActor : public ATriggerBox
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMyDropActor();

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;

public:
	// Called every frame
	void Tick(float DeltaTime) override;

public:
#if WITH_EDITOR	// º¯Êý

	UFUNCTION(CallInEditor, Category = "Editor")
	void SpawnOne();

	UFUNCTION(CallInEditor, Category = "Editor")
	void ClearSpawns();

	UFUNCTION(CallInEditor, Category = "Editor")
	void StartSimulate() const;

	UFUNCTION(BlueprintCallable, Category = "Editor")
	static void SaveSimulate();

#endif

#if WITH_EDITORONLY_DATA	// ÊôÐÔ
public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> SpawnTargetClass;

	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> SpawnActors;
#endif
};
