#pragma once

#include "HandPose.h"
#include "OculusXRHandComponent.h"
#include "DuckHuntVr/Characters/Player/HandsController/Hand/HandVisualizationInterface.h"
#include "TrackedVisualizationBase.generated.h"

class ULaserBase;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UHandPoseRecognizer;

UCLASS(Abstract, Blueprintable, NotBlueprintType)
class DUCKHUNTVR_API UTrackedVisualizationBase : public UOculusXRHandComponent, public IHandVisualizationInterface {
	GENERATED_BODY()
public:
	UTrackedVisualizationBase();

	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;
	virtual void TickComponent(float Dt, ELevelTick Tt, FActorComponentTickFunction* Tf) override;

	virtual bool IsPrimary() const override { return GunPoseRecognizer && LaserComponent && FireAudioComponent; }
	virtual void SwapPrimary(IHandVisualizationInterface* SecondaryHandVisualization) override;

	virtual void PlayFireEffects() override;
	virtual void ForceStopFireEffects() override;

	virtual void UpdateLaserType(ELaserType NewType) override;
protected:
	virtual void InitImpl(USceneComponent* AttachmentParent, bool Primary) override;
	virtual void AddMappingContexts(UEnhancedInputLocalPlayerSubsystem* Subsystem, UEnhancedInputComponent* Component) override;
	virtual void ClearMappingContexts(UEnhancedInputLocalPlayerSubsystem* Subsystem) const override;
	virtual const ULaserBase* GetLaser() const override { return LaserComponent; }

	UPROPERTY(EditDefaultsOnly, Category="Init|Action", DisplayName=Context)
	TObjectPtr<UInputMappingContext> ActionMappingContext;

	UPROPERTY(EditDefaultsOnly, Category="Init|Action")
	TObjectPtr<UInputAction> SystemAction;

	UPROPERTY(EditDefaultsOnly, Category="Init|Action")
	TObjectPtr<UInputAction> IndexPinchAction;

	UPROPERTY(EditDefaultsOnly, Category=Init)
	TArray<FHandPose> GunPoses;

	UPROPERTY(EditDefaultsOnly, Category=Init)
	TObjectPtr<USoundBase> FireSound;

	UPROPERTY(EditDefaultsOnly, Category=Subcomponents)
	TSubclassOf<ULaserBase> LaserClass;
private:
	enum class GunPose : int32 {
		None = INDEX_NONE,
		Gun = 0,
		GunShot = 1
	};

	FQuat GetBoneRotationMeshTypeBased(const FTransform& BoneTransform) const;

	void UpdateMaterialFresnelIfNeeded(GunPose NewPose, float Dt);
	void UpdateLaserTransform(GunPose NewPose, float Dt);
	void ProcessGunPoseChangesIfNeeded(GunPose NewPose);

	void OnMenuVisibilityChanged(const FInputActionValue& Value);
	void OnIndexPinched();

	FORCEINLINE void UpdateMaterialPrimaryParameter() const {
		DynamicHandMaterial->SetScalarParameterValue(TEXT("Primary"), IsPrimary());
	}
	FORCEINLINE void UpdateMaterialFresnelParameter() const {
		DynamicHandMaterial->SetScalarParameterValue(TEXT("FresnelIn"), CurrentFresnel);
	}

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynamicHandMaterial;

	UPROPERTY()
	TObjectPtr<UHandPoseRecognizer> GunPoseRecognizer;

	UPROPERTY()
	TObjectPtr<ULaserBase> LaserComponent;

	UPROPERTY()
	TObjectPtr<UAudioComponent> FireAudioComponent;

	GunPose CurrentPose = GunPose::None;
	float CurrentFresnel = 0.0f;

	int32 Index2BoneIdx = INDEX_NONE;
	FQuat CurrentLaserQuat;

	bool IsSystemMenuShown = false;
};
