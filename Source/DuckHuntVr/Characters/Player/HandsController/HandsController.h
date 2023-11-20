#pragma once

#include "MotionController/Visualization/Controller/HandController.h"
#include "MotionController/Visualization/Controller/Gun/Gun.h"
#include "HandsController.generated.h"

class UHandMotionController;

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
public:
	UHandsController();

	UFUNCTION(BlueprintCallable)
	void Init(APawn* Parent) { ParentCharacter = Parent; }

	virtual void TickComponent(float Dt, ELevelTick Tt, FActorComponentTickFunction* Tf) override;

	UFUNCTION(BlueprintCallable)
	void SetPrimaryHandType(const EControllerHand NewPrimaryHand);

	UFUNCTION(BlueprintPure)
	FORCEINLINE EControllerHand GetPrimaryHandType() const { return PrimaryHand; }

	FORCEINLINE const APawn* GetCharacter() const { return ParentCharacter.Get(); }
private:
	enum class EControllerVisualizationType : uint8_t { None, Controller, Hands };
	EControllerVisualizationType GetNewVisualizationType() const;

	void SetControllerVisualization();
	void SetHandsVisualization() const;
	void UpdateControllersVisualizationIfNeeded();

	// TODO
	const bool AllowHandTracking = true;
	EControllerHand PrimaryHand = EControllerHand::Right;

	TWeakObjectPtr<APawn> ParentCharacter = nullptr;

	UPROPERTY()
	TObjectPtr<UHandMotionController> LeftController;

	UPROPERTY()
	TObjectPtr<UHandMotionController> RightController;
};
