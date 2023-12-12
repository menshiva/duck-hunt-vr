#pragma once

#include "Laser.generated.h"

class IHandVisualizationInterface;
class UNiagaraSystem;
class UPaperSprite;
class UNiagaraComponent;
class UPaperSpriteComponent;

UENUM(BlueprintType)
enum class ELaserType : uint8 {
	None, Crosshair, Laser
};

UCLASS(NotBlueprintable, NotBlueprintType, NotPlaceable)
class DUCKHUNTVR_API ULaserComponent : public USceneComponent {
	GENERATED_BODY()
public:
	ULaserComponent();

	void Init(IHandVisualizationInterface* Parent);
	FORCEINLINE void SetNewHandVisualizationParent(IHandVisualizationInterface* Parent) { ParentHandVisualization = Parent; }
	void UpdateType();
	void Destroy();

	virtual void TickComponent(float Dt, ELevelTick Tt, FActorComponentTickFunction* Tf) override;
private:
	void SetNiagaraLaserPosition(int Index, const FVector& Pos) const;

	ETraceTypeQuery TraceTypeQuery;

	UPROPERTY()
	TObjectPtr<UNiagaraSystem> NiagaraLaserAsset;

	UPROPERTY()
	TObjectPtr<UPaperSprite> CrosshairSpriteAsset;

	IHandVisualizationInterface* ParentHandVisualization = nullptr;

	UPROPERTY()
	TObjectPtr<UNiagaraComponent> NiagaraLaser = nullptr;

	UPROPERTY()
	TObjectPtr<UPaperSpriteComponent> CrosshairSprite = nullptr;

	FHitResult HitResult;
};
