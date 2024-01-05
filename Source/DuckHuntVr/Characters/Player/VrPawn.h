#pragma once

#include "HandsController/Hand/VisualizationType.h"
#include "Laser/LaserType.h"
#include "VrPawn.generated.h"

class UCameraComponent;
class UHandsController;
class UDhGameInstance;
class ADhGameStateBase;

UCLASS(NotBlueprintable, NotBlueprintType, Placeable)
class DUCKHUNTVR_API AVrPawn : public APawn {
	GENERATED_BODY()
public:
	AVrPawn();

	virtual void BeginPlay() override;

	// TODO: remove UFUNCTION
	UFUNCTION(BlueprintCallable)
	void SetPrimaryHand(EControllerHand NewPrimaryHand) const;

	// TODO: remove UFUNCTION
	UFUNCTION(BlueprintCallable)
	void SetLaserType(ELaserType NewLaserType) const;

	static void ResetOrientationAndPosition();

	float GetCameraRotationYaw() const;
	EVisualizationType GetVisualizationType() const;
	FORCEINLINE APlayerController* GetPlayerController() const { return PlayerController.Get(); }
	FORCEINLINE ADhGameStateBase* GetGameState() const { return GameState.Get(); }
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UHandsController> HandsController;
private:
	bool OnGunFired(void* TargetActor) const;
	void OnMenuPressed() const;
	void OnVisualizationTypeChanged(EVisualizationType NewType) const;

	TWeakObjectPtr<UDhGameInstance> GameInstance;
	TWeakObjectPtr<ADhGameStateBase> GameState;
	TWeakObjectPtr<APlayerController> PlayerController;
};
