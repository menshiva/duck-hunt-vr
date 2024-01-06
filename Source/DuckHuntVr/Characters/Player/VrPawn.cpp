#include "VrPawn.h"
#include "Camera/CameraComponent.h"
#include "HandsController/HandsController.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "OculusXRFunctionLibrary.h"
#include "DuckHuntVr/GameInstance/DhGameInstance.h"
#include "DuckHuntVr/GameStates/DhGameState.h"

AVrPawn::AVrPawn() {
	PrimaryActorTick.bCanEverTick = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(DefaultSceneRoot);

	HandsController = CreateDefaultSubobject<UHandsController>(TEXT("HandsController"));
	HandsController->SetupAttachment(DefaultSceneRoot);
}

void AVrPawn::BeginPlay() {
	GameInstance = CastChecked<UDhGameInstance>(GetGameInstance());
	GameState = Cast<ADhGameState>(GetWorld()->GetGameState());
	PlayerController = CastChecked<APlayerController>(Controller);

	if (!GameState.IsExplicitlyNull()) {
		HandsController->OnGunFired.BindUObject(GameState.Get(), &ADhGameState::OnGunFired);
		HandsController->OnMenuPressed.BindUObject(GameState.Get(), &ADhGameState::TogglePause);
		HandsController->OnVisualizationTypeChanged.BindUObject(GameState.Get(), &ADhGameState::OnVisualizationTypeChanged);
	}

	HandsController->Init(
		this,
		GameInstance->GetPrimaryHand(),
		GameInstance->GetGameVisualizationType(),
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

bool AVrPawn::IsHmdWorn() {
	return UHeadMountedDisplayFunctionLibrary::GetHMDWornState() == EHMDWornState::Worn;
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
