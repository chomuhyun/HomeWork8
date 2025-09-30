#include "ChasingMine.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
AChasingMine::AChasingMine()
{
	ExplosionDelay = 1.0f;
	ExplosionRadius = 170.0f;
	ExplosionDamage = 30.0f;
	ItemType = "Mine";
	bHasExploded = false;


	PrimaryActorTick.bCanEverTick = true;
	ExplosionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollision"));
	ExplosionCollision->InitSphereRadius(ExplosionRadius);
	ExplosionCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	ExplosionCollision->SetupAttachment(Scene);
}

void AChasingMine::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Player"), FoundActors);

	if (FoundActors.Num() > 0)
	{
		TargetActor = FoundActors[0];
	}
}

void AChasingMine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (TargetActor && !bHasExploded)
	{
		FVector Direction = (TargetActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		float Speed = 300.0f;

		FVector NewLocation = GetActorLocation() + Direction * Speed * DeltaTime;
		SetActorLocation(NewLocation);

		FRotator NewRotation = Direction.Rotation();
		SetActorRotation(NewRotation);
	}
}

void AChasingMine::ActivateItem(AActor* Activator)
{
	if (bHasExploded) return;
	Super::ActivateItem(Activator);

	GetWorld()->GetTimerManager().SetTimer(
		ExplosionTimerHandle,
		this,
		&AChasingMine::Explode,
		ExplosionDelay,
		false
	);

	bHasExploded = true;
}

void AChasingMine::Explode()
{
	UParticleSystemComponent* Particle = nullptr;

	if (ExplosionParticle)
	{
		Particle = UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			ExplosionParticle,
			GetActorLocation(),
			GetActorRotation(),
			false
		);
	}

	if (ExplosionSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(
			GetWorld(),
			ExplosionSound,
			GetActorLocation()
		);
	}

	TArray<AActor*> OverlappingActors;
	ExplosionCollision->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		if (Actor && Actor->ActorHasTag("Player"))
		{
			UGameplayStatics::ApplyDamage(
				Actor,
				ExplosionDamage,
				nullptr,
				this,
				UDamageType::StaticClass()
			);
		}
	}

	DestroyItem();

	if (Particle)
	{
		FTimerHandle DestroyParticleTimerHandle;

		GetWorld()->GetTimerManager().SetTimer(
			DestroyParticleTimerHandle,
			[Particle]()
			{
				Particle->DestroyComponent();
			},
			1.0f,
			false
		);
	}
}
