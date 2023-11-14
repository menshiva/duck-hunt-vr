#pragma once

#include "Gun.generated.h"

class UHapticFeedbackEffect_Curve;

UCLASS(meta=(BlueprintSpawnableComponent))
class DUCKHUNTVR_API UGun : public UStaticMeshComponent {
	GENERATED_BODY()
public:
	UGun();

	UFUNCTION(BlueprintCallable)
	void Shoot(EControllerHand Hand) const;
private:
	UPROPERTY()
	UAudioComponent* m_pShootSound;

	UPROPERTY()
	UHapticFeedbackEffect_Curve* m_pHapticFeedbackEffect;
};
