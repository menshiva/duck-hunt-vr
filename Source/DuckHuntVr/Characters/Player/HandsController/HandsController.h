#pragma once

#include "MotionController/HandMotionController.h"
#include "HandsController.generated.h"

class UControllerVisualizationBase;
class UTrackedVisualizationBase;

UCLASS(NotBlueprintable, NotBlueprintType, NotPlaceable, meta=(BlueprintSpawnableComponent))
class DUCKHUNTVR_API UHandsController : public USceneComponent {
	GENERATED_BODY()
public:
	UHandsController();

	virtual void TickComponent(float Dt, ELevelTick Tt, FActorComponentTickFunction* Tf) override;

	UFUNCTION(BlueprintCallable)
	void SetPrimaryHandType(const EControllerHand NewPrimaryHand);

	UFUNCTION(BlueprintPure)
	FORCEINLINE EControllerHand GetPrimaryHandType() const { return PrimaryHand; }
private:
	EVisualizationType GetNewVisualizationType() const;
	void UpdateControllersVisualizationIfNeeded();

	UPROPERTY(EditDefaultsOnly, Category="Init|Controller", DisplayName=Left, meta=(AllowPrivateAccess=true))
	TSubclassOf<UControllerVisualizationBase> LeftControllerVisualizationClass;

	UPROPERTY(EditDefaultsOnly, Category="Init|Controller", DisplayName=Right, meta=(AllowPrivateAccess=true))
	TSubclassOf<UControllerVisualizationBase> RightControllerVisualizationClass;

	UPROPERTY(EditDefaultsOnly, Category="Init|Tracked", DisplayName=Left, meta=(AllowPrivateAccess=true))
	TSubclassOf<UTrackedVisualizationBase> LeftTrackedVisualizationClass;

	UPROPERTY(EditDefaultsOnly, Category="Init|Tracked", DisplayName=Right, meta=(AllowPrivateAccess=true))
	TSubclassOf<UTrackedVisualizationBase> RightTrackedVisualizationClass;

	// TODO: Make this a config variable
	const bool AllowHandTracking = true;

	// TODO: Make this a config variable
	EControllerHand PrimaryHand = EControllerHand::Right;

	EVisualizationType CurrentVisualizationType = EVisualizationType::None;

	UPROPERTY()
	TObjectPtr<UHandMotionController> LeftController;

	UPROPERTY()
	TObjectPtr<UHandMotionController> RightController;

	TWeakObjectPtr<UHandMotionController> PrimaryController;
	TWeakObjectPtr<UHandMotionController> SecondaryController;
};
