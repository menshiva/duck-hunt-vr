#pragma once

#include "LaserType.h"
#include "LaserBase.generated.h"

class UNiagaraSystem;
class UPaperSprite;
class UWidgetInteractionComponent;
class UNiagaraComponent;
class UPaperSpriteComponent;

UCLASS(Abstract, Blueprintable, NotBlueprintType)
class DUCKHUNTVR_API ULaserBase : public USceneComponent {
	GENERATED_BODY()
public:
	ULaserBase();

	virtual void Activate(bool bReset) override;
	virtual void Deactivate() override;

	virtual void BeginPlay() override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	void UpdateType(ELaserType NewType);
	void ClickUI() const;

	virtual void TickComponent(float Dt, ELevelTick Tt, FActorComponentTickFunction* Tf) override;

	FORCEINLINE bool IsUiHit() const { return UiHit; }
	FORCEINLINE const FHitResult& GetHitResult() const { return HitResult; }
protected:
	UPROPERTY(EditDefaultsOnly, Category=Init)
	TObjectPtr<UNiagaraSystem> NiagaraLaserAsset;

	UPROPERTY(EditDefaultsOnly, Category=Init)
	TObjectPtr<UPaperSprite> CrosshairSpriteAsset;
private:
	constexpr static float MaxLaserDistance = 2000.0f;

	ETraceTypeQuery TargetTraceTypeQuery;

	ELaserType Type = ELaserType::None;

	UPROPERTY()
	TObjectPtr<UWidgetInteractionComponent> WidgetInteractor;

	UPROPERTY()
	TObjectPtr<UNiagaraComponent> NiagaraLaser;

	UPROPERTY()
	TObjectPtr<UPaperSpriteComponent> CrosshairSprite;

	bool UiHit = false;
	FHitResult HitResult;
};
