#pragma once

#include "MotionController/Visualization/Controller/HandController.h"
#include "MotionController/Visualization/Controller/Gun/Gun.h"
#include "HandsController.generated.h"

class UHandMotionController;
class UHandTracked;

UCLASS(NotBlueprintable, NotBlueprintType, NotPlaceable, meta=(BlueprintSpawnableComponent))
class DUCKHUNTVR_API UHandsController : public USceneComponent {
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category="Init|Controller", DisplayName=Left, meta=(AllowPrivateAccess=true))
	FHandControllerInitStatics LeftHandControllerInitStatics;

	UPROPERTY(EditDefaultsOnly, Category="Init|Controller", DisplayName=Right, meta=(AllowPrivateAccess=true))
	FHandControllerInitStatics RightHandControllerInitStatics;

	UPROPERTY(EditDefaultsOnly, Category="Init|Gun", DisplayName=Gun, meta=(AllowPrivateAccess=true))
	FGunInitStatics GunInitStatics;

	UPROPERTY(EditDefaultsOnly, Category="Init|Gun", DisplayName=Left, meta=(AllowPrivateAccess=true))
	FTransform LeftGunTransform;

	UPROPERTY(EditDefaultsOnly, Category="Init|Gun", DisplayName=Right, meta=(AllowPrivateAccess=true))
	FTransform RightGunTransform;

	UPROPERTY(EditDefaultsOnly, Category="Init|Tracked", DisplayName=Primary, meta=(AllowPrivateAccess=true))
	UMaterialInstance* PrimaryHandTrackedMaterial;

	UPROPERTY(EditDefaultsOnly, Category="Init|Tracked", DisplayName=Secondary, meta=(AllowPrivateAccess=true))
	UMaterialInstance* SecondaryHandTrackedMaterial;
public:
	UHandsController();

	virtual void TickComponent(float Dt, ELevelTick Tt, FActorComponentTickFunction* Tf) override;

	UFUNCTION(BlueprintCallable)
	void SetPrimaryHandType(const EControllerHand NewPrimaryHand);

	UFUNCTION(BlueprintPure)
	FORCEINLINE EControllerHand GetPrimaryHandType() const { return PrimaryHand; }
private:
	void DeterminePrimaryController(
		UHandController* LeftHandController, UHandController* RightHandController,
		UHandController*& PrimaryHandController, UHandController*& SecondaryHandController,
		const FTransform*& PrimaryGunTransformPtr, const FTransform*& SecondaryGunTransformPtr
	) const;

	void SetControllerVisualization();

	void SwapPrimaryControllerVisualization() const;

	void DeterminePrimaryHand(
		UHandTracked* LeftHandTracked, UHandTracked* RightHandTracked,
		UHandTracked*& PrimaryHandTracked, UHandTracked*& SecondaryHandTracked
	) const;

	void SetHandsVisualization() const;

	void SwapPrimaryHandsVisualization() const;

	enum class EControllerVisualizationType : uint8_t { None, Controller, Hands };

	EControllerVisualizationType GetNewVisualizationType() const;

	void UpdateControllersVisualizationIfNeeded();

	// TODO: Make this a config variable
	const bool AllowHandTracking = true;

	// TODO: Make this a config variable
	EControllerHand PrimaryHand = EControllerHand::Right;

	EControllerVisualizationType CurrentVisualizationType = EControllerVisualizationType::None;

	UPROPERTY()
	TObjectPtr<UHandMotionController> LeftController;

	UPROPERTY()
	TObjectPtr<UHandMotionController> RightController;
};
