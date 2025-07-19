// Fill out your copyright notice in the Description page of Project Settings.


#include "AIInteractionComp.h"

#include "AICharacter.h"
#include "SGameplayInterface.h"
#include "EngineUtils.h"
#include "PlayerCharacter.h"



static TAutoConsoleVariable<bool> CVarDebugInteraction(TEXT("su.InteractionDebug"), false, TEXT("Enable Debug Interact Component."), ECVF_Cheat);

UAIInteractionComp::UAIInteractionComp()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	TraceRadius = 2.0f;
	TraceDistance = 200.0f;
	CollisionChannel = ECC_WorldDynamic;

}

void UAIInteractionComp::PrimaryInteract(AAICharacter* InFocus)
{
	if (InFocus == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "No Focus Actor to interact.");
		return;
	}
	APawn* MyPawn = CastChecked<APawn>(GetOwner());
	ISGameplayInterface::Execute_Interact(InFocus, MyPawn);
}


void UAIInteractionComp::FindBestInteractable()
{
	APawn* MyOwner = CastChecked<APawn>(GetOwner());
	
	bool bDebugDraw = CVarDebugInteraction.GetValueOnGameThread();

	
	FCollisionObjectQueryParams ObjectQueryParams;
	FCollisionQueryParams queryParams;
	
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);

	queryParams.AddIgnoredActor(PlayerCharatcer);
	
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	FVector StartEyeLocation;
	FRotator EyeDirection;
    
	PlayerController->GetPlayerViewPoint(StartEyeLocation, EyeDirection);

	// X birim ileri konum hesapla
	float Distance = 100.f; // X birim mesafe (örn. 100 birim)
	FVector EyeLocation = StartEyeLocation + (EyeDirection.Vector() * Distance);
	
	FVector TraceEnd = EyeLocation + (EyeDirection.Vector() * TraceDistance);

	TArray<FHitResult> hitResult;

	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);
	
	
	FCollisionShape Shape;
	Shape.SetSphere(TraceRadius);
	
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(hitResult, EyeLocation, TraceEnd, FQuat::Identity, ObjectQueryParams, Shape);

	FColor LineColor = bBlockingHit ? FColor::Yellow : FColor::White;
	
	FocusedAI = nullptr;

	for (FHitResult Hit : hitResult)
	{
		if (bDebugDraw)
		{
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, TraceRadius, 32, LineColor, false, 0.0f);
		}
		

		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			if (HitActor->Implements<USGameplayInterface>())
			{
				FocusedAI = Cast<AAICharacter>(HitActor);
				break;
			}
		}
	}

	if (FocusedAI)
	{
		
		AAICharacter* InteractibleActor = Cast<AAICharacter>(FocusedAI);
    
		if (InteractibleActor)
		{
			
			USkeletalMeshComponent* BaseMeshComponent = InteractibleActor->GetMesh(); 
        
			if (BaseMeshComponent)
			{
				BaseMeshComponent->SetRenderCustomDepth(true);
			}
		}
		
	}
	
	else
	{ 

		for (TActorIterator<AAICharacter> It(GetWorld()); It; ++It)
		{
			AAICharacter* NonInteractibleActor = *It;
			USkeletalMeshComponent* BaseMeshComponent = NonInteractibleActor->GetMesh();
			if (BaseMeshComponent)
			{
				BaseMeshComponent->SetRenderCustomDepth(false);
			}
		}
	}


	if (bDebugDraw)
	{
		DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, LineColor, false, 0.0f, 0, 0.1f);
	}
	
	
}


void UAIInteractionComp::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	PlayerCharatcer = Cast<APlayerCharacter>(PlayerController);
}

void UAIInteractionComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FindBestInteractable();
	
	
}


