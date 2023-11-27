#pragma once

#include "Laser.generated.h"

class UNiagaraComponent;

UCLASS(Abstract, Blueprintable, NotBlueprintType, NotPlaceable)
class DUCKHUNTVR_API ULaser : public USceneComponent {
	GENERATED_BODY()
public:
	ULaser();

	virtual void BeginPlay() override;
	virtual void TickComponent(float Dt, ELevelTick Tt, FActorComponentTickFunction* Tf) override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;
protected:
	UPROPERTY(EditDefaultsOnly, Category=Init, DisplayName=TraceVFX)
	TObjectPtr<UNiagaraComponent> NiagaraComponent;
private:
	void SetNiagaraComponentPosition(int Index, const FVector& Pos) const;

	ETraceTypeQuery TraceTypeQuery;
};
