#pragma once

#include "GunBase.generated.h"

class UHapticFeedbackEffect_Curve;
class ULaserBase;

UCLASS(Abstract, Blueprintable, NotBlueprintType)
class DUCKHUNTVR_API UGunBase : public UStaticMeshComponent {
	GENERATED_BODY()
public:
	UGunBase();

	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	void PlayFireEffects(EControllerHand HandType) const;

	FORCEINLINE ULaserBase* GetLaser() const { return LaserComponent; }
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
