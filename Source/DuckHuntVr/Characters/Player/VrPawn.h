#pragma once

#include "HandsController/Hand/VisualizationType.h"
#include "Laser/LaserType.h"
#include "VrPawn.generated.h"

class UCameraComponent;
class UHandsController;
class UDhGameInstance;
class ADhGameState;

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
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UHandsController> HandsController;
private:
	TWeakObjectPtr<UDhGameInstance> GameInstance;
	TWeakObjectPtr<ADhGameState> GameState;
	TWeakObjectPtr<APlayerController> PlayerController;
};
