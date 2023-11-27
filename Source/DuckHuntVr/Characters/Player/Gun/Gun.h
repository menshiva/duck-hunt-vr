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
class ULaser;

UCLASS(Abstract, Blueprintable, NotBlueprintType, NotPlaceable)
class DUCKHUNTVR_API UGunComponentBase : public UStaticMeshComponent {
	GENERATED_BODY()
public:
	UGunComponentBase();

	void Init(UControllerVisualizationBase* Parent, bool CalledFirstTime = true);
	void SetNewParentControllerVisualization(UControllerVisualizationBase* NewParent);

	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;
protected:
	UPROPERTY(EditDefaultsOnly, Category="Init|Fire", DisplayName=Audio)
	TObjectPtr<UAudioComponent> FireAudioComponent;

	UPROPERTY(EditDefaultsOnly, Category="Init|Fire", DisplayName=HapticEffect)
	TObjectPtr<UHapticFeedbackEffect_Curve> FireHapticFeedbackEffect;

	UPROPERTY(EditDefaultsOnly, Category=Init, DisplayName=PerHand)
	TMap<EControllerHand, FGunInitPerHand> PerHandInitData;

	UPROPERTY(EditDefaultsOnly, Category=Init, DisplayName=Laser)
	TSubclassOf<ULaser> LaserClass;
private:
	const FGunInitPerHand& GetHandInitDataBasedOnParent() const;

	void InitFireMappingContext(const FGunInitPerHand& HandInitData);
	void RemoveFireMappingContext() const;

	void Fire();

	UPROPERTY()
	TObjectPtr<ULaser> LaserComponent = nullptr;

	TWeakObjectPtr<UControllerVisualizationBase> ParentControllerVisualizationComponent = nullptr;
};
