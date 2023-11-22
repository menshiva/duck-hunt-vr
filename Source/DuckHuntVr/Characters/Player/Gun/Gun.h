#pragma once

#include "Gun.generated.h"

class UInputMappingContext;
class UInputAction;

USTRUCT(NotBlueprintable, NotBlueprintType)
struct FGunInitPerHand {
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

UCLASS(Abstract, Blueprintable, NotBlueprintType, NotPlaceable)
class DUCKHUNTVR_API UGunComponentBase : public UStaticMeshComponent {
	GENERATED_BODY()
public:
	UGunComponentBase();
	void Init(UControllerVisualizationBase* Parent, bool ShouldRegister = true);
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	void SetNewParentControllerVisualization(UControllerVisualizationBase* NewParent);
protected:
	UPROPERTY(EditDefaultsOnly, Category="Init|Fire", DisplayName=Audio)
	UAudioComponent* FireAudioComponent;

	UPROPERTY(EditDefaultsOnly, Category="Init|Fire", DisplayName=HapticEffect)
	UHapticFeedbackEffect_Curve* FireHapticFeedbackEffect;

	UPROPERTY(EditDefaultsOnly, Category=Init, DisplayName=PerHand)
	TMap<EControllerHand, FGunInitPerHand> PerHandInitData;
private:
	const FGunInitPerHand& GetHandInitDataBasedOnParent() const;

	void InitFireMappingContext(const FGunInitPerHand& HandInitData);
	void RemoveFireMappingContext() const;

	void Fire();

	TWeakObjectPtr<UControllerVisualizationBase> ParentControllerVisualizationComponent = nullptr;
};
