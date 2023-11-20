#include "HandsController.h"
#include "OculusXRInputFunctionLibrary.h"
#include "MotionController/HandMotionController.h"
#include "MotionController/Visualization/Tracked/HandTracked.h"

UHandsController::UHandsController() {
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bAllowTickOnDedicatedServer = false;

	USceneComponent::SetMobility(EComponentMobility::Static);

	UActorComponent::SetAutoActivate(true);

	LeftController = CreateDefaultSubobject<UHandMotionController>(TEXT("LeftMotionController"));
	LeftController->Init(EControllerHand::Left);
	LeftController->SetupAttachment(this);

	RightController = CreateDefaultSubobject<UHandMotionController>(TEXT("RightMotionController"));
	RightController->Init(EControllerHand::Right);
	RightController->SetupAttachment(this);
}

void UHandsController::TickComponent(const float Dt, const ELevelTick Tt, FActorComponentTickFunction* Tf) {
	Super::TickComponent(Dt, Tt, Tf);
	UpdateControllersVisualizationIfNeeded();
}

void UHandsController::SetPrimaryHandType(const EControllerHand NewPrimaryHand) {
	if (PrimaryHand == NewPrimaryHand)
		return;

	if (CurrentVisualizationType == EControllerVisualizationType::Controller)
		SwapPrimaryControllerVisualization();
	else if (CurrentVisualizationType == EControllerVisualizationType::Hands)
		SwapPrimaryHandsVisualization();
	else
		check(CurrentVisualizationType == EControllerVisualizationType::None);

	PrimaryHand = NewPrimaryHand;
}

void UHandsController::DeterminePrimaryController(
	UHandController* LeftHandController, UHandController* RightHandController,
	UHandController*& PrimaryHandController, UHandController*& SecondaryHandController,
	const FTransform*& PrimaryGunTransformPtr, const FTransform*& SecondaryGunTransformPtr
) const {
	PrimaryHandController = LeftHandController;
	SecondaryHandController = RightHandController;

	PrimaryGunTransformPtr = &LeftGunTransform;
	SecondaryGunTransformPtr = &RightGunTransform;

	check(PrimaryHand == EControllerHand::Left || PrimaryHand == EControllerHand::Right);
	if (PrimaryHand == EControllerHand::Right) {
		Swap(PrimaryHandController, SecondaryHandController);
		Swap(PrimaryGunTransformPtr, SecondaryGunTransformPtr);
	}
}

void UHandsController::SetControllerVisualization() {
	UHandController* PrimaryController, *SecondaryController;
	const FTransform* PrimaryGunTransformPtr, *SecondaryGunTransformPtr;

	DeterminePrimaryController(
		LeftController->SetControllerVisualization(LeftHandControllerInitStatics),
		RightController->SetControllerVisualization(RightHandControllerInitStatics),
		PrimaryController, SecondaryController,
		PrimaryGunTransformPtr, SecondaryGunTransformPtr
	);

	const auto Gun = NewObject<UGun>(this);
	Gun->Init(PrimaryController, *PrimaryGunTransformPtr, GunInitStatics);
	PrimaryController->SetGun(Gun);
}

void UHandsController::SwapPrimaryControllerVisualization() const {
	UHandController* PrimaryController, *SecondaryController;
	const FTransform* PrimaryGunTransformPtr, *SecondaryGunTransformPtr;

	DeterminePrimaryController(
		CastChecked<UHandController>(LeftController->GetHandMesh()),
		CastChecked<UHandController>(RightController->GetHandMesh()),
		PrimaryController, SecondaryController,
		PrimaryGunTransformPtr, SecondaryGunTransformPtr
	);

	check(PrimaryController->IsPrimary());
	check(!SecondaryController->IsPrimary());

	const auto Gun = PrimaryController->GetGun();
	PrimaryController->SetGun(nullptr);
	Gun->Reattach(SecondaryController, *SecondaryGunTransformPtr);
	SecondaryController->SetGun(Gun);
}

void UHandsController::DeterminePrimaryHand(
	UHandTracked* LeftHandTracked, UHandTracked* RightHandTracked,
	UHandTracked*& PrimaryHandTracked, UHandTracked*& SecondaryHandTracked
) const {
	PrimaryHandTracked = LeftHandTracked;
	SecondaryHandTracked = RightHandTracked;

	check(PrimaryHand == EControllerHand::Left || PrimaryHand == EControllerHand::Right);
	if (PrimaryHand == EControllerHand::Right)
		Swap(PrimaryHandTracked, SecondaryHandTracked);
}

void UHandsController::SetHandsVisualization() const {
	UHandTracked* PrimaryHandTracked, *SecondaryHandTracked;

	DeterminePrimaryHand(
		LeftController->SetHandsVisualization(),
		RightController->SetHandsVisualization(),
		PrimaryHandTracked, SecondaryHandTracked
	);

	PrimaryHandTracked->SetPrimary(true, PrimaryHandTrackedMaterial);
	SecondaryHandTracked->SetPrimary(false, SecondaryHandTrackedMaterial);
}

void UHandsController::SwapPrimaryHandsVisualization() const {
	UHandTracked* PrimaryHandTracked, *SecondaryHandTracked;

	DeterminePrimaryHand(
		CastChecked<UHandTracked>(LeftController->GetHandMesh()),
		CastChecked<UHandTracked>(RightController->GetHandMesh()),
		PrimaryHandTracked, SecondaryHandTracked
	);

	check(PrimaryHandTracked->IsPrimary());
	check(!SecondaryHandTracked->IsPrimary());

	SecondaryHandTracked->SetPrimary(true, PrimaryHandTrackedMaterial);
	PrimaryHandTracked->SetPrimary(false, SecondaryHandTrackedMaterial);
}

UHandsController::EControllerVisualizationType UHandsController::GetNewVisualizationType() const {
	if (AllowHandTracking && UOculusXRInputFunctionLibrary::IsHandTrackingEnabled())
		return EControllerVisualizationType::Hands;
	if (LeftController->IsTracked() || RightController->IsTracked())
		return EControllerVisualizationType::Controller;
	return EControllerVisualizationType::None;
}

void UHandsController::UpdateControllersVisualizationIfNeeded() {
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
