#include "VrPawnBase.h"
#include "Camera/CameraComponent.h"
#include "HandsController/HandsController.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Components/WidgetComponent.h"
#include "DuckHuntVr/Gamemodes/DhGameModeBase.h"
#include "DuckHuntVr/UI/InGame/InGameWidget.h"
#include "Kismet/KismetSystemLibrary.h"

AVrPawnBase::AVrPawnBase() {
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(DefaultSceneRoot);

	HandsController = CreateDefaultSubobject<UHandsController>(TEXT("HandsController"));
	HandsController->Init(this, EControllerHand::Right, ELaserType::Laser); // TODO: get from settings (or game instance)
	HandsController->OnGunFired.BindUObject(this, &AVrPawnBase::OnGunFired);
	HandsController->OnMenuPressed.BindUObject(this, &AVrPawnBase::OnMenuPressed);
	HandsController->OnVisualizationTypeChanged.BindUObject(this, &AVrPawnBase::OnVisualizationTypeChanged);
	HandsController->SetupAttachment(DefaultSceneRoot);

	InGameWidgetHolder = CreateDefaultSubobject<UWidgetComponent>(TEXT("InGameWidgetHolder"));
	InGameWidgetHolder->SetupAttachment(DefaultSceneRoot);
}

void AVrPawnBase::BeginPlay() {
	Super::BeginPlay();

	InGameWidget = CastChecked<UInGameWidget>(InGameWidgetHolder->GetUserWidgetObject());

	// TODO: move it somewhere
	UHeadMountedDisplayFunctionLibrary::EnableHMD(true);
	UKismetSystemLibrary::ExecuteConsoleCommand(this, TEXT("r.ScreenPercentage 100"));
	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Floor);
}

void AVrPawnBase::Tick(const float DeltaSeconds) {
	Super::Tick(DeltaSeconds);

	static double CurrentWidgetRotationYaw = GetActorRotation().Yaw;

	// TODO: do it if 180 or 360 degree mode is enabled
	const double NewCameraRotationYaw = Camera->GetRelativeRotation().Yaw;
	const double CameraRotationYawOffset = NewCameraRotationYaw - CurrentWidgetRotationYaw;
	const double Angle = FMath::Abs(CameraRotationYawOffset);

	if (Angle > 60.0) {
		const double NewWidgetRotationYaw = CurrentWidgetRotationYaw + (CameraRotationYawOffset > 0.0 ? 90.0 : -90.0);
		const auto DiffTransform = FTransform(FRotator(0.0, NewWidgetRotationYaw, 0.0) - FRotator(0.0, CurrentWidgetRotationYaw, 0.0));
		InGameWidgetHolder->SetRelativeTransform(InGameWidgetHolder->GetRelativeTransform() * DiffTransform);
		CurrentWidgetRotationYaw = NewWidgetRotationYaw;
	}
}

void AVrPawnBase::SetPrimaryHand(const EControllerHand NewPrimaryHand) const {
	HandsController->SetPrimaryHand(NewPrimaryHand);
}

void AVrPawnBase::SetLaserType(const ELaserType NewLaserType) const {
	HandsController->SetLaserType(NewLaserType);
}

void AVrPawnBase::ResetOrientationAndPosition() {
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AVrPawnBase::OnGunFired() const {
	HandsController->PlayFireEffects();
	UKismetSystemLibrary::PrintString(this, TEXT("Gun fired"), true, true, FLinearColor::Red);
}

void AVrPawnBase::OnMenuPressed() const {
	const auto Gm = CastChecked<ADhGameModeBase>(GetWorld()->GetAuthGameMode());
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
	}
}

void AVrPawnBase::OnVisualizationTypeChanged() const {
	// TODO: call gamestate callback and pass previous visualization type to it
}
