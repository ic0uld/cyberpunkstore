// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "GameFramework/Character.h"
#include "AICharacter.generated.h"

class UAIInteractionComp;

UCLASS()
class SUPERMARKET_API AAICharacter : public ACharacter, public ISGameplayInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAICharacter();

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAIInteractionComp> InteractionComp;
	
	//virtual void Interact_Implementation(APawn* InstigatorPawn) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
