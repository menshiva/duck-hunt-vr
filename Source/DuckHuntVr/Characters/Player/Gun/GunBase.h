#pragma once

#include "GunBase.generated.h"

class UHapticFeedbackEffect_Curve;
class ULaserBase;

UCLASS(Abstract, Blueprintable, NotBlueprintType, NotPlaceable)
class DUCKHUNTVR_API UGunBase : public UStaticMeshComponent {
	GENERATED_BODY()
public:
	UGunBase();

	virtual void OnComponentCreated() override;
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

	void PlayFireEffects(EControllerHand HandType) const;
	void UpdateLaserType() const;
protected:
	UPROPERTY(EditDefaultsOnly, Category=Init)
	TObjectPtr<USoundBase> FireSound;

	UPROPERTY(EditDefaultsOnly, Category=Init)
	TObjectPtr<UHapticFeedbackEffect_Curve> FireHapticFeedbackEffect;

	UPROPERTY(EditDefaultsOnly, Category=Subcomponents)
	TSubclassOf<ULaserBase> LaserClass;
private:
	UPROPERTY()
	TObjectPtr<UAudioComponent> FireAudioComponent;

	UPROPERTY()
	TObjectPtr<ULaserBase> LaserComponent;
};
