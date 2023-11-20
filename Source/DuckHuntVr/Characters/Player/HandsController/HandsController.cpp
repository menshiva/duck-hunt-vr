#include "HandsController.h"
#include "OculusXRInputFunctionLibrary.h"
#include "MotionController/HandMotionController.h"

UHandsController::UHandsController() {
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bAllowTickOnDedicatedServer = false;

	USceneComponent::SetMobility(EComponentMobility::Static);

	UActorComponent::SetAutoActivate(true);

	LeftController = CreateDefaultSubobject<UHandMotionController>(TEXT("LeftMotionController"));
	LeftController->Init(this, EControllerHand::Left);
	LeftController->SetupAttachment(this);

	RightController = CreateDefaultSubobject<UHandMotionController>(TEXT("RightMotionController"));
	RightController->Init(this, EControllerHand::Right);
	RightController->SetupAttachment(this);
}

void UHandsController::TickComponent(const float Dt, const ELevelTick Tt, FActorComponentTickFunction* Tf) {
	Super::TickComponent(Dt, Tt, Tf);
	UpdateControllersVisualizationIfNeeded();
}

void UHandsController::SetPrimaryHandType(const EControllerHand NewPrimaryHand) {
	PrimaryHand = NewPrimaryHand;
	// TODO
}

UHandsController::EControllerVisualizationType UHandsController::GetNewVisualizationType() const {
	if (AllowHandTracking && UOculusXRInputFunctionLibrary::IsHandTrackingEnabled())
		return EControllerVisualizationType::Hands;
	if (LeftController->IsTracked() || RightController->IsTracked())
		return EControllerVisualizationType::Controller;
	return EControllerVisualizationType::None;
}

void UHandsController::SetControllerVisualization() {
	auto PrimaryController = LeftController->SetControllerVisualization(LeftHandControllerInitStatics);
	auto PrimaryGunTransformPtr = &LeftGunTransform;

	auto SecondaryController = RightController->SetControllerVisualization(RightHandControllerInitStatics);
	auto SecondaryGunTransformPtr = &RightGunTransform;

	check(PrimaryHand == EControllerHand::Left || PrimaryHand == EControllerHand::Right);
	if (PrimaryHand == EControllerHand::Right) {
		Swap(PrimaryController, SecondaryController);
		Swap(PrimaryGunTransformPtr, SecondaryGunTransformPtr);
	}

	const auto Gun = NewObject<UGun>(this);
	Gun->Init(PrimaryController, *PrimaryGunTransformPtr, GunInitStatics);
	PrimaryController->SetGun(Gun);
}

void UHandsController::SetHandsVisualization() const {
	LeftController->SetHandsVisualization();
	RightController->SetHandsVisualization();
}

void UHandsController::UpdateControllersVisualizationIfNeeded() {
	static auto CurrentVisualizationType = EControllerVisualizationType::None;
	const auto NewVisualizationType = GetNewVisualizationType();

	if (NewVisualizationType != CurrentVisualizationType) {
		LeftController->ClearVisualization();
		RightController->ClearVisualization();

		if (NewVisualizationType == EControllerVisualizationType::Controller)
			SetControllerVisualization();
		else if (NewVisualizationType == EControllerVisualizationType::Hands)
			SetHandsVisualization();
		else
			check(NewVisualizationType == EControllerVisualizationType::None);

		CurrentVisualizationType = NewVisualizationType;
	}
}
