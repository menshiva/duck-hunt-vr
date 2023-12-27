#pragma once

#include "LaserType.h"
#include "LaserBase.generated.h"

class UNiagaraSystem;
class UPaperSprite;
class UNiagaraComponent;
class UPaperSpriteComponent;

UCLASS(Abstract, Blueprintable, NotBlueprintType)
class DUCKHUNTVR_API ULaserBase : public USceneComponent {
	GENERATED_BODY()
public:
	ULaserBase();

	virtual void Activate(bool bReset) override;
	virtual void Deactivate() override;

	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	void UpdateType(ELaserType NewType);

	virtual void TickComponent(float Dt, ELevelTick Tt, FActorComponentTickFunction* Tf) override;
protected:
	UPROPERTY(EditDefaultsOnly, Category=Init)
	TObjectPtr<UNiagaraSystem> NiagaraLaserAsset;

	UPROPERTY(EditDefaultsOnly, Category=Init)
	TObjectPtr<UPaperSprite> CrosshairSpriteAsset;
private:
	constexpr static float MaxLaserDistance = 2000.0f;

	ETraceTypeQuery TraceTypeQuery;

	UPROPERTY()
	TObjectPtr<UNiagaraComponent> NiagaraLaser;

	UPROPERTY()
	TObjectPtr<UPaperSpriteComponent> CrosshairSprite;

	FHitResult HitResult;
};
