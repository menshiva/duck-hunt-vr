#include "HandsControllerBase.h"
#include "OculusXRInputFunctionLibrary.h"
#include "Hand/HandVisualizationInterface.h"

UHandsControllerBase::UHandsControllerBase() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UHandsControllerBase::OnComponentCreated() {
	Super::OnComponentCreated();

	const auto NewMotionControllerObject = [this] (const TSubclassOf<UHandMotionControllerBase>& Class, const EControllerHand HandType) {
		if (Class) {
			const auto MotionController = NewObject<UHandMotionControllerBase>(this, Class);
			MotionController->SetHandType(HandType);
			MotionController->SetupAttachment(this);
			MotionController->RegisterComponent();
			return MotionController;
		}
		return static_cast<UHandMotionControllerBase*>(nullptr);
	};

	LeftMotionController = NewMotionControllerObject(LeftMotionControllerClass, EControllerHand::Left);
	RightMotionController = NewMotionControllerObject(RightMotionControllerClass, EControllerHand::Right);

	PrimaryMotionController = RightMotionController;
	SecondaryMotionController = LeftMotionController;

	check(PrimaryHand == EControllerHand::Right || PrimaryHand == EControllerHand::Left);
	if (PrimaryHand == EControllerHand::Left)
		Swap(PrimaryMotionController, SecondaryMotionController);
}

void UHandsControllerBase::OnComponentDestroyed(const bool bDestroyingHierarchy) {
	if (LeftMotionController) {
		LeftMotionController->DestroyComponent();
		LeftMotionController = nullptr;
	}

	if (RightMotionController) {
		RightMotionController->DestroyComponent();
		RightMotionController = nullptr;
	}

	Super::OnComponentDestroyed(bDestroyingHierarchy);
}

void UHandsControllerBase::TickComponent(const float Dt, const ELevelTick Tt, FActorComponentTickFunction* Tf) {
	Super::TickComponent(Dt, Tt, Tf);
	UpdateControllersVisualizationIfNeeded();
}

void UHandsControllerBase::SetPrimaryHand(const EControllerHand NewPrimaryHand) {
	if (PrimaryHand == NewPrimaryHand)
		return;
	PrimaryHand = NewPrimaryHand;
	if (CurrentVisualizationType != EVisualizationType::None)
		PrimaryMotionController->GetVisualizationComponent()->SwapPrimary(SecondaryMotionController->GetVisualizationComponent());
	Swap(PrimaryMotionController, SecondaryMotionController);
}

void UHandsControllerBase::SetLaserType(const ELaserType NewLaserType) {
	if (LaserType == NewLaserType)
		return;
	LaserType = NewLaserType;
	if (CurrentVisualizationType != EVisualizationType::None)
		PrimaryMotionController->GetVisualizationComponent()->UpdateLaserType();
}

void UHandsControllerBase::PlayFireEffects() const {
	if (const auto PrimaryControllerVis = PrimaryMotionController->GetVisualizationComponent())
		PrimaryControllerVis->PlayFireEffects();
}

EVisualizationType UHandsControllerBase::GetNewVisualizationType() const {
	if (AllowHandTracking && UOculusXRInputFunctionLibrary::IsHandTrackingEnabled())
		return EVisualizationType::Tracked;
	if (LeftMotionController->IsTracked() || RightMotionController->IsTracked())
		return EVisualizationType::Controller;
	return EVisualizationType::None;
}

void UHandsControllerBase::UpdateControllersVisualizationIfNeeded() {
	const auto NewVisualizationType = GetNewVisualizationType();
	if (NewVisualizationType != CurrentVisualizationType) {
		PrimaryMotionController->UpdateVisualization(NewVisualizationType, true);
		SecondaryMotionController->UpdateVisualization(NewVisualizationType, false);
		CurrentVisualizationType = NewVisualizationType;
	}
}
