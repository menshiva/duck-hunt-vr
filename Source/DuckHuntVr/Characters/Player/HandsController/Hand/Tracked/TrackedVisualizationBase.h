#pragma once

#include "OculusXRHandComponent.h"
#include "DuckHuntVr/Characters/Player/HandsController/Hand/HandVisualizationInterface.h"
#include "TrackedVisualizationBase.generated.h"

class UInputMappingContext;
class UInputAction;
class ULaserBase;

UCLASS(Abstract, Blueprintable, NotBlueprintType, NotPlaceable)
class DUCKHUNTVR_API UTrackedVisualizationBase : public UOculusXRHandComponent, public IHandVisualizationInterface {
	GENERATED_BODY()
public:
	UTrackedVisualizationBase();

	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

	virtual bool IsPrimary() const override { return static_cast<bool>(LaserComponent); }
	virtual void SwapPrimary(IHandVisualizationInterface* SecondaryHandVisualization) override;

	virtual void PlayFireEffects() override;
	virtual void UpdateLaserType() override;

	virtual void TickComponent(float Dt, ELevelTick Tt, FActorComponentTickFunction* Tf) override;
protected:
	virtual void InitImpl(USceneComponent* AttachmentParent, bool Primary) override;

	virtual void AddMappingContexts(UEnhancedInputLocalPlayerSubsystem* Subsystem, UEnhancedInputComponent* Component) override;
	virtual void ClearMappingContexts(UEnhancedInputLocalPlayerSubsystem* Subsystem) const override;

	UPROPERTY(EditDefaultsOnly, Category="Init|Action", DisplayName=Context)
	TObjectPtr<UInputMappingContext> ActionMappingContext;

	UPROPERTY(EditDefaultsOnly, Category="Init|Action")
	TObjectPtr<UInputAction> SystemAction;

	UPROPERTY(EditDefaultsOnly, Category="Init|Action")
	TObjectPtr<UInputAction> IndexPinchAction;

	UPROPERTY(EditDefaultsOnly, Category=Init)
	TObjectPtr<USoundBase> FireSound;

	UPROPERTY(EditDefaultsOnly, Category=Subcomponents)
	TSubclassOf<ULaserBase> LaserClass;
private:
	void OnMenuVisibilityChanged(const FInputActionValue& Value);
	void OnIndexPinched();

	FORCEINLINE void UpdateHandMaterialColor() const {
		DynamicHandMaterial->SetScalarParameterValue(TEXT("Primary"), IsPrimary());
	}

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynamicHandMaterial;

	UPROPERTY()
	TObjectPtr<ULaserBase> LaserComponent;

	bool IsSystemMenuShown = false;
};
