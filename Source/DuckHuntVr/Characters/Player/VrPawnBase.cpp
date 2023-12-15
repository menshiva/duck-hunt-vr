#include "VrPawnBase.h"
#include "Camera/CameraComponent.h"
#include "HandsController/HandsControllerBase.h"
#include "HeadMountedDisplayFunctionLibrary.h"
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
