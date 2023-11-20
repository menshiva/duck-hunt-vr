#pragma once

#include "HandController.generated.h"

class UInputMappingContext;
class UInputAction;

USTRUCT(NotBlueprintable, NotBlueprintType)
struct FHandControllerInitStatics {
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FTransform Transform;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAnimInstance> AnimClass;

	UPROPERTY(EditDefaultsOnly)
	USkeletalMesh* SkeletalMeshAsset;

	UPROPERTY(EditDefaultsOnly)
	UInputMappingContext* AnimMappingContext;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* AnimPointCapTouchAction;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* AnimTriggerAction;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* AnimThumbCapTouchAction;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* AnimGripAction;

	UPROPERTY(EditDefaultsOnly)
	UInputMappingContext* FireMappingContext;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* FireAction;
};

class UHandMotionController;
class UGun;
struct FInputActionValue;

UCLASS(NotBlueprintable, NotBlueprintType, NotPlaceable)
class DUCKHUNTVR_API UHandController : public USkeletalMeshComponent {
	GENERATED_BODY()
public:
	UHandController();

	void Init(UHandMotionController* Parent, const FHandControllerInitStatics& InitData);

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	EControllerHand GetHandType() const;
	FORCEINLINE UInputMappingContext* GetFireMappingContext() const { return FireMappingContext.Get(); }
	FORCEINLINE UInputAction* GetFireAction() const { return FireAction.Get(); }

	FORCEINLINE void SetGun(UGun* NewGun) { Gun = NewGun; }
	FORCEINLINE UGun* GetGun() const { return Gun.Get(); }

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsPrimary() const { return static_cast<bool>(Gun); }
private:
	void InitAnimMappingContext(const FHandControllerInitStatics& InitData);
	void RemoveAnimMappingContext() const;

	void AnimPointCapTouchActionEvent(const FInputActionValue& Value);
	void AnimTriggerActionEvent(const FInputActionValue& Value);
	void AnimThumbCapTouchActionEvent(const FInputActionValue& Value);
	void AnimGripActionEvent(const FInputActionValue& Value);

	TWeakObjectPtr<UHandMotionController> ParentMotionController = nullptr;

	TWeakObjectPtr<UInputMappingContext> AnimMappingContext = nullptr;

	TWeakObjectPtr<UInputMappingContext> FireMappingContext = nullptr;

	TWeakObjectPtr<UInputAction> FireAction = nullptr;

	UPROPERTY()
	TObjectPtr<UGun> Gun = nullptr;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	bool IsPointing = false;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	float TriggerAxis = 0.0f;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	bool IsThumbUp = false;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess=true))
	float GripAxis = 0.0f;
};
