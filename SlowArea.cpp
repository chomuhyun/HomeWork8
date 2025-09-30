#include "SlowArea.h"
#include "Components/CapsuleComponent.h"
#include "DroneCharacter.h"
#include "GameFramework/FloatingPawnMovement.h"

ASlowArea::ASlowArea()
{
	ItemType = "Slow";

	// Ä¸½¶ ÄÄÆ÷³ÍÆ®¸¦ Scene¿¡ ºÙÀÓ
	CapComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	CapComp->SetupAttachment(Scene);
	CapComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
}

void ASlowArea::BeginPlay()
{
	Super::BeginPlay();
}

void ASlowArea::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

    ADroneCharacter* Drone = Cast<ADroneCharacter>(Activator);
    if (Drone && Drone->MovementComp)
    {
        Drone->MovementComp->MaxSpeed *= 0.25f;

        FTimerHandle TimerHandle;
        GetWorld()->GetTimerManager().SetTimer(
            
            TimerHandle, [Drone]()
            {
                if (Drone && Drone->MovementComp)
                {
                    Drone->MovementComp->MaxSpeed *= 4.0f;
                }
            }, 
            2.0f, 
            false
        );
    }
}
