#include "HandsController.h"
#include "OculusXRInputFunctionLibrary.h"
#include "DuckHuntVr/Characters/Player/Hands/HandVisualizationInterface.h"

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

	PrimaryController = RightController;
	SecondaryController = LeftController;

	check(PrimaryHand == EControllerHand::Left || PrimaryHand == EControllerHand::Right);
	if (PrimaryHand == EControllerHand::Left)
		Swap(PrimaryController, SecondaryController);
}

void UHandsController::SetPrimaryHandType(const EControllerHand NewPrimaryHand) {
	if (PrimaryHand == NewPrimaryHand)
		return;
	PrimaryHand = NewPrimaryHand;
	if (CurrentVisualizationType != EVisualizationType::None)
		PrimaryController->GetVisualizationComponent()->SwapPrimary(SecondaryController->GetVisualizationComponent());
	Swap(PrimaryController, SecondaryController);
}

void UHandsController::SetLaserType(const ELaserType NewLaserType) {
	if (LaserType == NewLaserType)
		return;
	LaserType = NewLaserType;
	if (CurrentVisualizationType != EVisualizationType::None)
		PrimaryController->GetVisualizationComponent()->UpdateLaserType();
}

void UHandsController::TickComponent(const float Dt, const ELevelTick Tt, FActorComponentTickFunction* Tf) {
	Super::TickComponent(Dt, Tt, Tf);
	UpdateControllersVisualizationIfNeeded();
}

EVisualizationType UHandsController::GetNewVisualizationType() const {
	if (AllowHandTracking && UOculusXRInputFunctionLibrary::IsHandTrackingEnabled())
		return EVisualizationType::Tracked;
	if (LeftController->IsTracked() || RightController->IsTracked())
		return EVisualizationType::Controller;
	return EVisualizationType::None;
}

void UHandsController::UpdateControllersVisualizationIfNeeded() {
	const auto NewVisualizationType = GetNewVisualizationType();
	if (NewVisualizationType != CurrentVisualizationType) {
		LeftController->ClearVisualization();
		RightController->ClearVisualization();

		switch (NewVisualizationType) {
			case EVisualizationType::Controller:
				LeftController->SetControllerVisualization(LeftControllerVisualizationClass);
				RightController->SetControllerVisualization(RightControllerVisualizationClass);
				break;
			case EVisualizationType::Tracked:
				LeftController->SetTrackedVisualization(LeftTrackedVisualizationClass);
				RightController->SetTrackedVisualization(RightTrackedVisualizationClass);
				break;
			case EVisualizationType::None:
				break;
			default:
				check(false);
		}

		if (NewVisualizationType != EVisualizationType::None) {
			PrimaryController->GetVisualizationComponent()->SetPrimary(true);
			SecondaryController->GetVisualizationComponent()->SetPrimary(false);
		}

		CurrentVisualizationType = NewVisualizationType;
	}
}
