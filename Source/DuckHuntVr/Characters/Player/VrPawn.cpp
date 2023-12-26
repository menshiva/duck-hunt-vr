#include "VrPawn.h"
#include "Camera/CameraComponent.h"
#include "HandsController/HandsController.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

AVrPawn::AVrPawn() {
	PrimaryActorTick.bCanEverTick = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(DefaultSceneRoot);

	HandsController = CreateDefaultSubobject<UHandsController>(TEXT("HandsController"));
	HandsController->Init(this, EControllerHand::Right, ELaserType::Laser); // TODO: get from settings (or game instance)
	HandsController->OnGunFired.BindUObject(this, &AVrPawn::OnGunFired);
	HandsController->OnMenuPressed.BindUObject(this, &AVrPawn::OnMenuPressed);
	HandsController->OnVisualizationTypeChanged.BindUObject(this, &AVrPawn::OnVisualizationTypeChanged);
	HandsController->SetupAttachment(DefaultSceneRoot);
}

void AVrPawn::BeginPlay() {
	Super::BeginPlay();

	// TODO: move it somewhere
	UHeadMountedDisplayFunctionLibrary::EnableHMD(true);
	UKismetSystemLibrary::ExecuteConsoleCommand(this, TEXT("r.ScreenPercentage 100"));
	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Floor);
}

void AVrPawn::SetPrimaryHand(const EControllerHand NewPrimaryHand) const {
	HandsController->SetPrimaryHand(NewPrimaryHand);
}

void AVrPawn::SetLaserType(const ELaserType NewLaserType) const {
	HandsController->SetLaserType(NewLaserType);
}

void AVrPawn::ResetOrientationAndPosition() {
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

float AVrPawn::GetCameraRotationYaw() const {
	return Camera->GetRelativeRotation().Yaw;
}

void AVrPawn::OnGunFired() const {
	HandsController->PlayFireEffects();
	UKismetSystemLibrary::PrintString(this, TEXT("Gun fired"), true, true, FLinearColor::Red);
}

void AVrPawn::OnMenuPressed() const {
	// TODO
	/*const auto Gm = CastChecked<ADhGameModeBase>(GetWorld()->GetAuthGameMode());
	if (!Gm->AllowPausing())
		return;

	const auto WillBePaused = !Gm->IsPaused();

	static EVisualizationType PrevVisualizationType = EVisualizationType::None;
	const EVisualizationType NewisualizationType = HandsController->GetVisualizationType();

	if (WillBePaused)
		PrevVisualizationType = NewisualizationType;

	if (PrevVisualizationType == NewisualizationType) {
		Gm->SetPause(GetLocalViewingPlayerController(), WillBePaused);
		UKismetSystemLibrary::PrintString(this, WillBePaused ? TEXT("Paused") : TEXT("Unpaused"), true, true, FLinearColor::Red);
	}
	else {
		// TODO: create notification
		UKismetSystemLibrary::PrintString(
			this, TEXT("Please return to the controller type you started the game with"),
			true, true, FLinearColor::Green
		);
	}*/
}

void AVrPawn::OnVisualizationTypeChanged() const {
	// TODO: call gamestate callback and pass previous visualization type to it
}
