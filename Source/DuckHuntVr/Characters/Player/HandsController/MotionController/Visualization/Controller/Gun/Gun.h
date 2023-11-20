#pragma once

#include "Gun.generated.h"

class USoundCue;
class UHapticFeedbackEffect_Curve;

USTRUCT(NotBlueprintable, NotBlueprintType)
struct FGunInitStatics {
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* MeshAsset;

	UPROPERTY(EditDefaultsOnly)
	USoundCue* SoundAsset;

	UPROPERTY(EditDefaultsOnly)
	UHapticFeedbackEffect_Curve* HapticFeedbackEffect;
};

class UHandController;

UCLASS(NotBlueprintable, NotBlueprintType, NotPlaceable)
class DUCKHUNTVR_API UGun : public UStaticMeshComponent {
	GENERATED_BODY()
public:
	UGun();

	void Init(UHandController* Parent, const FTransform& Transform, const FGunInitStatics& InitData);

	void Reattach(UHandController* NewParent, const FTransform& NewTransform);

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
private:
	void InitFireMappingContext();
	void RemoveFireMappingContext() const;

	void Fire();

	UPROPERTY()
	UAudioComponent* FireAudioComponent;

	TWeakObjectPtr<UHandController> ParentHandController;

	TWeakObjectPtr<UHapticFeedbackEffect_Curve> HapticFeedbackEffect;
};
