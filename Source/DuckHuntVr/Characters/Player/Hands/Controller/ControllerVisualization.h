#pragma once

#include "DuckHuntVr/Characters/Player/Hands/HandVisualizationInterface.h"
#include "ControllerVisualization.generated.h"

class UHandMotionController;
class UInputMappingContext;
class UInputAction;
class UGunComponentBase;
struct FInputActionValue;

UCLASS(Abstract, Blueprintable, BlueprintType, NotPlaceable)
class DUCKHUNTVR_API UControllerVisualizationBase : public USkeletalMeshComponent, public IHandVisualizationInterface {
	GENERATED_BODY()
public:
	UControllerVisualizationBase();

	virtual void Init(UHandMotionController* Parent) override;
	virtual void Destroy() override;

	virtual void SetPrimary(bool Primary) override;
	virtual void SwapPrimary(IHandVisualizationInterface* OtherHandVisualization) override;

	UFUNCTION(BlueprintPure)
	virtual bool IsPrimary() const override { return static_cast<bool>(GunComponent); }
protected:
	UPROPERTY(EditDefaultsOnly, Category="Init|Input|Anim", DisplayName=Context)
	UInputMappingContext* AnimMappingContext;

	UPROPERTY(EditDefaultsOnly, Category="Init|Input|Anim", DisplayName=PointTouchAction)
	UInputAction* AnimPointCapTouchAction;

	UPROPERTY(EditDefaultsOnly, Category="Init|Input|Anim", DisplayName=TriggerAction)
	UInputAction* AnimTriggerAction;

	UPROPERTY(EditDefaultsOnly, Category="Init|Input|Anim", DisplayName=ThumbTouchAction)
	UInputAction* AnimThumbCapTouchAction;

	UPROPERTY(EditDefaultsOnly, Category="Init|Input|Anim", DisplayName=GripAction)
	UInputAction* AnimGripAction;

	UPROPERTY(EditDefaultsOnly, Category=Init, DisplayName=Gun)
	TSubclassOf<UGunComponentBase> GunClass;
private:
	void InitAnimMappingContext();
	void RemoveAnimMappingContext() const;

	void AnimPointCapTouchActionEvent(const FInputActionValue& Value);
	void AnimTriggerActionEvent(const FInputActionValue& Value);
	void AnimThumbCapTouchActionEvent(const FInputActionValue& Value);
	void AnimGripActionEvent(const FInputActionValue& Value);

	UPROPERTY()
	TObjectPtr<UGunComponentBase> GunComponent = nullptr;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	bool IsPointing = false;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	float TriggerAxis = 0.0f;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	bool IsThumbUp = false;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	float GripAxis = 0.0f;
};
