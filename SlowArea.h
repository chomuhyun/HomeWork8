#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "SlowArea.generated.h"

class UCapsuleComponent;

UCLASS()
class MOVINGOBJECT_API ASlowArea : public ABaseItem
{
	GENERATED_BODY()

public:

	ASlowArea();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slow")
	USceneComponent* SceneComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slow")
	UCapsuleComponent* CapComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Slow")
	float SlowSpeed;

	FTimerHandle SlowTimerHandle;
	
protected:

	virtual void BeginPlay() override;
	virtual void ActivateItem(AActor* Activator) override;

private:
	AActor* TargetActor;
	
};
