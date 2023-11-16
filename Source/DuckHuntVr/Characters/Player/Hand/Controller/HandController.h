#pragma once

#include "HandController.generated.h"

class UGun;

UCLASS(NotBlueprintable, NotPlaceable, meta=(BlueprintSpawnableComponent))
class DUCKHUNTVR_API UHandController : public USkeletalMeshComponent {
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category=Type, meta=(AllowPrivateAccess=true))
	EControllerHand Hand;
public:
	UHandController();

	UFUNCTION(BlueprintPure)
	bool HasGun(UGun*& Gun) const;

	UFUNCTION(BlueprintCallable)
	void ShootIfHasGun() const;
private:
	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	bool IsPointing = false;

	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	float TriggerAxis = 0.0f;

	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	bool IsThumbUp = false;

	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	float GripAxis = 0.0f;
};
