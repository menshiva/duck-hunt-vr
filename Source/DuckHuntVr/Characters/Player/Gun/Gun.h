#pragma once

#include "Gun.generated.h"

class UInputMappingContext;
class UInputAction;

USTRUCT(NotBlueprintable, NotBlueprintType)
struct FGunInitData {
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FTransform Transform;

	UPROPERTY(EditDefaultsOnly)
	UInputMappingContext* FireMappingContext;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* FireAction;
};

class UControllerVisualizationBase;
class UHapticFeedbackEffect_Curve;
class ULaserComponent;

UCLASS(NotBlueprintable, NotBlueprintType, NotPlaceable)
class DUCKHUNTVR_API UGunComponent : public UStaticMeshComponent {
	GENERATED_BODY()
public:
	UGunComponent();

	void Init(UControllerVisualizationBase* Parent);
	void SetNewParentControllerVisualization(UControllerVisualizationBase* NewParent);
	void UpdateLaserType() const;
	void Destroy();
private:
	void InitFireMappingContext(const FGunInitData& HandInitData);
	void RemoveFireMappingContext(const FGunInitData& HandInitData) const;

	void Fire();

	UPROPERTY()
	TObjectPtr<UAudioComponent> FireAudioComponent;

	UPROPERTY()
	TObjectPtr<UHapticFeedbackEffect_Curve> FireHapticFeedbackEffect;

	UPROPERTY()
	TObjectPtr<ULaserComponent> LaserComponent;

	TWeakObjectPtr<UControllerVisualizationBase> ParentControllerVisualizationComponent = nullptr;
};
