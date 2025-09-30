#include "SmallBoxItem.h"

ASmallBoxItem::ASmallBoxItem()
{
	PointValue = 10;
	ItemType = "SmallBox";
}

void ASmallBoxItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
}