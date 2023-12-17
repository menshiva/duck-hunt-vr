#pragma once

#include "DuckHuntVr/Characters/Player/Laser/LaserBase.h"
#include "Hand/HandMotionControllerBase.h"
#include "HandsControllerBase.generated.h"

class UHapticFeedbackEffect_Curve;

UCLASS(Abstract, Blueprintable, NotBlueprintType, NotPlaceable)
class DUCKHUNTVR_API UHandsControllerBase : public USceneComponent {
	GENERATED_BODY()
public:
	UHandsControllerBase();

	virtual void OnComponentCreated() override;
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

	virtual void TickComponent(float Dt, ELevelTick Tt, FActorComponentTickFunction* Tf) override;

	UFUNCTION(BlueprintCallable)
	void SetPrimaryHand(EControllerHand NewPrimaryHand);

	UFUNCTION(BlueprintCallable)
	void SetLaserType(ELaserType NewLaserType);

	FORCEINLINE ELaserType GetLaserType() const { return LaserType; }

	void PlayFireEffects() const;
protected:
	UPROPERTY(EditDefaultsOnly, Category=Subcomponents)
	TSubclassOf<UHandMotionControllerBase> LeftMotionControllerClass;

	UPROPERTY(EditDefaultsOnly, Category=Subcomponents)
	TSubclassOf<UHandMotionControllerBase> RightMotionControllerClass;
private:
	EVisualizationType GetNewVisualizationType() const;
	void UpdateControllersVisualizationIfNeeded();

	// TODO: Make this a config variable
	const bool AllowHandTracking = true;

	// TODO: Make this a config variable
	EControllerHand PrimaryHand = EControllerHand::Right;

	// TODO: Make this a config variable
	ELaserType LaserType = ELaserType::Laser;

	EVisualizationType CurrentVisualizationType = EVisualizationType::None;

	UPROPERTY()
	TObjectPtr<UHandMotionControllerBase> LeftMotionController;

	UPROPERTY()
	TObjectPtr<UHandMotionControllerBase> RightMotionController;

	TWeakObjectPtr<UHandMotionControllerBase> PrimaryMotionController;
	TWeakObjectPtr<UHandMotionControllerBase> SecondaryMotionController;
};
