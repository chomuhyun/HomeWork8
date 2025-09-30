#pragma once

#include "CoreMinimal.h"
#include "BoxItem.h"
#include "MovingSmallBoxItem.generated.h"


UCLASS()
class MOVINGOBJECT_API AMovingSmallBoxItem : public ABoxItem
{
	GENERATED_BODY()

public:
	AMovingSmallBoxItem();

	virtual void ActivateItem(AActor* Activator) override;

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Box|Properties")
	float YSpeed; // 이동 속력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Box|Properties")
	float FwdSpeed; // 이동 속력

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Box|Properties")
	float MaxDistance = 300.0f; //목표 거리

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Box|Properties")
	float CurrentDistance = 0.0f; // 이동 거리 측정

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Box|Properties")
	float CurrentDistance_Fwd = 0.0f; // 이동 거리 측정

	float Reverse = 1.0f; // 방향 반전
	float Reverse_Fwd = 1.0f;
	FTimerHandle StartTimer;
	FTimerHandle StopTimer;

	void StartForwardMove();
	void StopForwardMove();

	bool MoveForWard = false;
};
