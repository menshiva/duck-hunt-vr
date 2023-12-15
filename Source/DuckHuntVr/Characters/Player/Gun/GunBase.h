#pragma once

#include "GunBase.generated.h"

class UControllerVisualizationBase;
class UHapticFeedbackEffect_Curve;
class UInputMappingContext;
class UInputAction;
class ULaserBase;

UCLASS(Abstract, Blueprintable, NotBlueprintType, NotPlaceable)
class DUCKHUNTVR_API UGunBase : public UStaticMeshComponent {
	GENERATED_BODY()
public:
	UGunBase();

	virtual void OnComponentCreated() override;
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

	void Init(UControllerVisualizationBase* Parent);
	void UpdateLaserType() const;
protected:
	UPROPERTY(EditDefaultsOnly, Category=Init)
	TObjectPtr<USoundBase> FireSound;

	UPROPERTY(EditDefaultsOnly, Category=Init)
	TObjectPtr<UHapticFeedbackEffect_Curve> FireHapticFeedbackEffect;

	UPROPERTY(EditDefaultsOnly, Category=Subcomponents)
	TSubclassOf<ULaserBase> LaserClass;
private:
	void InitFireMappingContext(const UInputMappingContext* FireMappingContext, const UInputAction* FireAction);
	void RemoveFireMappingContext(const UInputMappingContext* FireMappingContext) const;

	void Fire();

	UPROPERTY()
	TObjectPtr<UAudioComponent> FireAudioComponent;

	UPROPERTY()
	TObjectPtr<ULaserBase> LaserComponent;

	TWeakObjectPtr<UControllerVisualizationBase> ParentControllerVisualizationComponent;
};
