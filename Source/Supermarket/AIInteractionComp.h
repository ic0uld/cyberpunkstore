// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AIInteractionComp.generated.h"

class APlayerCharacter;
class AAICharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SUPERMARKET_API UAIInteractionComp : public UActorComponent
{
	GENERATED_BODY()

public:

	UAIInteractionComp();

	void PrimaryInteract(AAICharacter* InFocus);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	TObjectPtr<AAICharacter> FocusedAI;

	UPROPERTY()
	TObjectPtr<APlayerCharacter> PlayerCharatcer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
	TEnumAsByte<ECollisionChannel> CollisionChannel;

	
protected:

	void FindBestInteractable();
	
	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float TraceDistance;

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float TraceRadius;

	UPROPERTY(EditAnywhere)
	FName TraceStartBone;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void BeginPlay() override;

		
};