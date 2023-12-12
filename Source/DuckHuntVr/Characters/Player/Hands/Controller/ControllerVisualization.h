#pragma once

#include "DuckHuntVr/Characters/Player/Gun/Gun.h"
#include "DuckHuntVr/Characters/Player/Hands/HandVisualizationInterface.h"
#include "ControllerVisualization.generated.h"

class UHandMotionController;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS(Abstract, Blueprintable, BlueprintType, NotPlaceable)
class DUCKHUNTVR_API UControllerVisualizationBase : public USkeletalMeshComponent, public IHandVisualizationInterface {
	GENERATED_BODY()
public:
	UControllerVisualizationBase();

	virtual void Init(UHandMotionController* Parent) override;
	virtual void SetPrimary(bool InitPrimary) override;
	virtual void SwapPrimary(IHandVisualizationInterface* OtherHandVisualization) override;
	virtual void UpdateLaserType() override;
	virtual void Destroy() override;

	UFUNCTION(BlueprintPure)
	virtual bool IsPrimary() const override { return static_cast<bool>(GunComponent); }

	FORCEINLINE const FGunInitData& GetGunInitData() const { return GunInitData; }
protected:
	UPROPERTY(EditDefaultsOnly, Category="Init|Input|Anim", DisplayName=Context)
	TObjectPtr<UInputMappingContext> AnimMappingContext;

	UPROPERTY(EditDefaultsOnly, Category="Init|Input|Anim", DisplayName=PointTouchAction)
	TObjectPtr<UInputAction> AnimPointCapTouchAction;

	UPROPERTY(EditDefaultsOnly, Category="Init|Input|Anim", DisplayName=TriggerAction)
	TObjectPtr<UInputAction> AnimTriggerAction;

	UPROPERTY(EditDefaultsOnly, Category="Init|Input|Anim", DisplayName=ThumbTouchAction)
	TObjectPtr<UInputAction> AnimThumbCapTouchAction;

	UPROPERTY(EditDefaultsOnly, Category="Init|Input|Anim", DisplayName=GripAction)
	TObjectPtr<UInputAction> AnimGripAction;

	UPROPERTY(EditDefaultsOnly, Category=Init, DisplayName=Gun)
	FGunInitData GunInitData;
private:
	void InitAnimMappingContext();
	void RemoveAnimMappingContext() const;

	void AnimPointCapTouchActionEvent(const FInputActionValue& Value);
	void AnimTriggerActionEvent(const FInputActionValue& Value);
	void AnimThumbCapTouchActionEvent(const FInputActionValue& Value);
	void AnimGripActionEvent(const FInputActionValue& Value);

	UPROPERTY()
	TObjectPtr<UGunComponent> GunComponent = nullptr;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	bool IsPointing = false;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	float TriggerAxis = 0.0f;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	bool IsThumbUp = false;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	float GripAxis = 0.0f;
};
