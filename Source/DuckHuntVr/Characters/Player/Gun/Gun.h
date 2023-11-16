#pragma once

#include "Gun.generated.h"

class UHapticFeedbackEffect_Curve;

UCLASS(NotBlueprintable, NotBlueprintType, NotPlaceable, meta=(BlueprintSpawnableComponent))
class DUCKHUNTVR_API UGun : public UStaticMeshComponent {
	GENERATED_BODY()
public:
	UGun();

	UFUNCTION(BlueprintCallable)
	void Shoot(EControllerHand Hand) const;
private:
	UPROPERTY()
	UAudioComponent* ShootSound;

	UPROPERTY()
	UHapticFeedbackEffect_Curve* HapticFeedbackEffect;
};
