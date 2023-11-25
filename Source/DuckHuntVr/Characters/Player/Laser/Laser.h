#pragma once

#include "Laser.generated.h"

UCLASS(NotBlueprintable, NotBlueprintType, NotPlaceable)
class DUCKHUNTVR_API ULaser : public USceneComponent {
	GENERATED_BODY()
public:
	ULaser();
	virtual void TickComponent(float Dt, ELevelTick Tt, FActorComponentTickFunction* Tf) override;
private:
	ETraceTypeQuery TraceTypeQuery;
};
