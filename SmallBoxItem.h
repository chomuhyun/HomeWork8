#pragma once

#include "CoreMinimal.h"
#include "BoxItem.h"
#include "SmallBoxItem.generated.h"

UCLASS()
class MOVINGOBJECT_API ASmallBoxItem : public ABoxItem
{
	GENERATED_BODY()

public:
	ASmallBoxItem();

	virtual void ActivateItem(AActor* Activator) override;
};
