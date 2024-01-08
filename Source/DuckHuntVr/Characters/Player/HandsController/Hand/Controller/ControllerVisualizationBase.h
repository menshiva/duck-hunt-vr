#pragma once

#include "DuckHuntVr/Characters/Player/HandsController/Hand/HandVisualizationInterface.h"
#include "ControllerVisualizationBase.generated.h"

class UInputMappingContext;
class UInputAction;
class UGunBase;
struct FInputActionValue;

UCLASS(Abstract, Blueprintable, BlueprintType)
class DUCKHUNTVR_API UControllerVisualizationBase : public USkeletalMeshComponent, public IHandVisualizationInterface {
	GENERATED_BODY()
public:
	UControllerVisualizationBase();

	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintPure)
	virtual bool IsPrimary() const override { return static_cast<bool>(GunComponent); }
	virtual void SwapPrimary(IHandVisualizationInterface* SecondaryHandVisualization) override;

	virtual void PlayFireEffects() override;
	virtual void ForceStopFireEffects() override;

	virtual void UpdateLaserType(ELaserType NewType) override;
protected:
	virtual void InitImpl(USceneComponent* AttachmentParent, bool Primary) override;
	virtual void AddMappingContexts(UEnhancedInputLocalPlayerSubsystem* Subsystem, UEnhancedInputComponent* Component) override;
	virtual void ClearMappingContexts(UEnhancedInputLocalPlayerSubsystem* Subsystem) const override;
	virtual const ULaserBase* GetLaser() const override;

	UPROPERTY(EditDefaultsOnly, Category="Init|Action", DisplayName=Context)
	TObjectPtr<UInputMappingContext> ActionMappingContext;

	UPROPERTY(EditDefaultsOnly, Category="Init|Action")
	TObjectPtr<UInputAction> FireAction;

	UPROPERTY(EditDefaultsOnly, Category="Init|Action")
	TObjectPtr<UInputAction> MenuAction;

	UPROPERTY(EditDefaultsOnly, Category="Init|Anim", DisplayName=Context)
	TObjectPtr<UInputMappingContext> AnimMappingContext;

	UPROPERTY(EditDefaultsOnly, Category="Init|Anim", DisplayName=PointTouchAction)
	TObjectPtr<UInputAction> AnimPointCapTouchAction;

	UPROPERTY(EditDefaultsOnly, Category="Init|Anim", DisplayName=TriggerAction)
	TObjectPtr<UInputAction> AnimTriggerAction;

	UPROPERTY(EditDefaultsOnly, Category="Init|Anim", DisplayName=ThumbTouchAction)
	TObjectPtr<UInputAction> AnimThumbCapTouchAction;

	UPROPERTY(EditDefaultsOnly, Category="Init|Anim", DisplayName=GripAction)
	TObjectPtr<UInputAction> AnimGripAction;

	UPROPERTY(EditDefaultsOnly, Category=Subcomponents)
	TSubclassOf<UGunBase> GunClass;

	UPROPERTY(BlueprintReadOnly)
	bool IsPointing = false;

	UPROPERTY(BlueprintReadOnly)
	float TriggerAxis = 0.0f;

	UPROPERTY(BlueprintReadOnly)
	bool IsThumbUp = false;

	UPROPERTY(BlueprintReadOnly)
	float GripAxis = 0.0f;
private:
	void AnimPointCapTouchActionEvent(const FInputActionValue& Value);
	void AnimTriggerActionEvent(const FInputActionValue& Value);
	void AnimThumbCapTouchActionEvent(const FInputActionValue& Value);
	void AnimGripActionEvent(const FInputActionValue& Value);

	UPROPERTY()
	TObjectPtr<UGunBase> GunComponent;
};
