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
	float YSpeed; // �̵� �ӷ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Box|Properties")
	float FwdSpeed; // �̵� �ӷ�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Box|Properties")
	float MaxDistance = 300.0f; //��ǥ �Ÿ�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Box|Properties")
	float CurrentDistance = 0.0f; // �̵� �Ÿ� ����

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Box|Properties")
	float CurrentDistance_Fwd = 0.0f; // �̵� �Ÿ� ����

	float Reverse = 1.0f; // ���� ����
	float Reverse_Fwd = 1.0f;
	FTimerHandle StartTimer;
	FTimerHandle StopTimer;

	void StartForwardMove();
	void StopForwardMove();

	bool MoveForWard = false;
};
