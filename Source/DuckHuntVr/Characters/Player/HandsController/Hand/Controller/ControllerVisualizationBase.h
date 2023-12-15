#pragma once

#include "DuckHuntVr/Characters/Player/HandsController/Hand/HandVisualizationInterface.h"
#include "ControllerVisualizationBase.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UGunBase;

UCLASS(Abstract, Blueprintable, BlueprintType, NotPlaceable)
class DUCKHUNTVR_API UControllerVisualizationBase : public USkeletalMeshComponent, public IHandVisualizationInterface {
	GENERATED_BODY()
public:
	UControllerVisualizationBase();

	virtual void Destroy() override { DestroyComponent(); }
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

	UFUNCTION(BlueprintPure)
	virtual bool IsPrimary() const override { return static_cast<bool>(GunComponent); }

	virtual void SwapPrimary(IHandVisualizationInterface* SecondaryHandVisualization) override;
	virtual void UpdateLaserType() override;

	FORCEINLINE const FTransform& GetGunTransform() const { return GunTransform; }
	FORCEINLINE const UInputMappingContext* GetGunFireMappingContext() const { return GunFireMappingContext; }
	FORCEINLINE const UInputAction* GetGunFireAction() const { return GunFireAction; }
protected:
	virtual void InitImpl(USceneComponent* AttachmentParent, bool Primary) override;

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

	UPROPERTY(EditDefaultsOnly, Category="Init|Gun", DisplayName=Transform)
	FTransform GunTransform;

	UPROPERTY(EditDefaultsOnly, Category="Init|Gun", DisplayName=FireContext)
	TObjectPtr<UInputMappingContext> GunFireMappingContext;

	UPROPERTY(EditDefaultsOnly, Category="Init|Gun", DisplayName=FireAction)
	TObjectPtr<UInputAction> GunFireAction;

	UPROPERTY(BlueprintReadOnly)
	bool IsPointing = false;

	UPROPERTY(BlueprintReadOnly)
	float TriggerAxis = 0.0f;

	UPROPERTY(BlueprintReadOnly)
	bool IsThumbUp = false;

	UPROPERTY(BlueprintReadOnly)
	float GripAxis = 0.0f;
private:
	void InitAnimMappingContext();
	void RemoveAnimMappingContext() const;

	void AnimPointCapTouchActionEvent(const FInputActionValue& Value);
	void AnimTriggerActionEvent(const FInputActionValue& Value);
	void AnimThumbCapTouchActionEvent(const FInputActionValue& Value);
	void AnimGripActionEvent(const FInputActionValue& Value);

	UPROPERTY()
	TObjectPtr<UGunBase> GunComponent;
};
