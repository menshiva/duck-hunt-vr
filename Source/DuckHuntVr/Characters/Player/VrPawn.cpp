#include "VrPawn.h"
#include "Camera/CameraComponent.h"
#include "HandsController/HandsController.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "DuckHuntVr/GameInstance/DhGameInstance.h"
#include "DuckHuntVr/GameStates/DhGameStateBase.h"

AVrPawn::AVrPawn() {
	PrimaryActorTick.bCanEverTick = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(DefaultSceneRoot);

	HandsController = CreateDefaultSubobject<UHandsController>(TEXT("HandsController"));
	HandsController->OnGunFired.BindUObject(this, &AVrPawn::OnGunFired);
	HandsController->OnMenuPressed.BindUObject(this, &AVrPawn::OnMenuPressed);
	HandsController->OnVisualizationTypeChanged.BindUObject(this, &AVrPawn::OnVisualizationTypeChanged);
	HandsController->SetupAttachment(DefaultSceneRoot);
}

void AVrPawn::BeginPlay() {
	GameInstance = CastChecked<UDhGameInstance>(GetGameInstance());
	GameState = CastChecked<ADhGameStateBase>(GetWorld()->GetGameState());
	PlayerController = CastChecked<APlayerController>(Controller);

	HandsController->Init(
		this,
		GameInstance->GetPrimaryHand(),
		GameInstance->GetVisualizationTypeGameStartedWith(),
		GameInstance->GetLaserType()
	);

	Super::BeginPlay();

	UHeadMountedDisplayFunctionLibrary::EnableHMD(true);
	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Floor);
}

void AVrPawn::SetPrimaryHand(const EControllerHand NewPrimaryHand) const {
	GameInstance->SavePrimaryHand(NewPrimaryHand);
	HandsController->SetPrimaryHand(NewPrimaryHand);
}

void AVrPawn::SetLaserType(const ELaserType NewLaserType) const {
	GameInstance->SaveLaserType(NewLaserType);
	HandsController->SetLaserType(NewLaserType);
}

void AVrPawn::ResetOrientationAndPosition() {
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

float AVrPawn::GetCameraRotationYaw() const {
	return Camera->GetRelativeRotation().Yaw;
}

EVisualizationType AVrPawn::GetVisualizationType() const {
	return HandsController->GetVisualizationType();
}

void AVrPawn::OnGunFired(const bool IsUI) const {
	HandsController->PlayFireEffects();
	// TODO: decrease bullet count only if not UI
}

void AVrPawn::OnMenuPressed() const {
	GameState->TogglePause();
}

void AVrPawn::OnVisualizationTypeChanged(const EVisualizationType NewType) const {
	GameState->OnVisualizationTypeChanged(NewType);
}
