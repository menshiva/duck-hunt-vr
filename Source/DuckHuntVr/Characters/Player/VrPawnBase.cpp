#include "VrPawnBase.h"
#include "Camera/CameraComponent.h"
#include "HandsController/HandsControllerBase.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

AVrPawnBase::AVrPawnBase() {
	PrimaryActorTick.bCanEverTick = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	SetRootComponent(DefaultSceneRoot);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(DefaultSceneRoot);
}

void AVrPawnBase::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);
	if (HandsControllerClass) {
		HandsController = NewObject<UHandsControllerBase>(this, HandsControllerClass);
		HandsController->SetupAttachment(DefaultSceneRoot);
		HandsController->RegisterComponent();
	}
}

void AVrPawnBase::BeginPlay() {
	Super::BeginPlay();
	UHeadMountedDisplayFunctionLibrary::EnableHMD(true);
	UKismetSystemLibrary::ExecuteConsoleCommand(this, TEXT("r.ScreenPercentage 100"));
	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Floor);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void AVrPawnBase::OnGunFire() {
	HandsController->PlayFireEffects();
	UKismetSystemLibrary::PrintString(this, TEXT("Gun fired"), true, true, FLinearColor::Red);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void AVrPawnBase::OnMenuPressed() {
	const auto WillBePaused = !UGameplayStatics::IsGamePaused(this);

	static EVisualizationType PrevVisualizationType = EVisualizationType::None;
	const EVisualizationType NewisualizationType = HandsController->GetVisualizationType();

	if (WillBePaused)
		PrevVisualizationType = NewisualizationType;

	if (PrevVisualizationType == NewisualizationType) {
		UGameplayStatics::SetGamePaused(this, WillBePaused);
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
