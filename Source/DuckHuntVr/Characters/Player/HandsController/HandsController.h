#pragma once

#include "DuckHuntVr/Characters/Player/Laser/LaserBase.h"
#include "Hand/HandMotionControllerBase.h"
#include "HandsController.generated.h"

class UHapticFeedbackEffect_Curve;

UCLASS(Blueprintable, NotBlueprintType, NotPlaceable)
class DUCKHUNTVR_API UHandsController : public USceneComponent {
	GENERATED_BODY()
public:
	UHandsController();

	virtual void OnComponentCreated() override;
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

	virtual void TickComponent(float Dt, ELevelTick Tt, FActorComponentTickFunction* Tf) override;

	UFUNCTION(BlueprintCallable)
	void SetPrimaryHand(EControllerHand NewPrimaryHand);

	UFUNCTION(BlueprintCallable)
	void SetLaserType(ELaserType NewLaserType);

	void PlayFireEffects() const;

	FORCEINLINE ELaserType GetLaserType() const { return LaserType; }
	FORCEINLINE EVisualizationType GetVisualizationType() const { return CurrentVisualizationType; }
protected:
	UPROPERTY(EditDefaultsOnly, Category=Subcomponents)
	TSubclassOf<UHandMotionControllerBase> LeftMotionControllerClass;

	UPROPERTY(EditDefaultsOnly, Category=Subcomponents)
	TSubclassOf<UHandMotionControllerBase> RightMotionControllerClass;
private:
	EVisualizationType GetNewVisualizationType() const;

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
