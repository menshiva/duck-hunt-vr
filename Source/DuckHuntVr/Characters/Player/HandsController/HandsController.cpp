#include "HandsController.h"
#include "OculusXRFunctionLibrary.h"
#include "OculusXRInputFunctionLibrary.h"
#include "DuckHuntVr/Characters/Player/VrPawn.h"
#include "Hand/HandMotionControllerBase.h"
#include "Hand/HandVisualizationInterface.h"

UHandsController::UHandsController() {
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bTickEvenWhenPaused = true;
}

void UHandsController::Init(
	AVrPawn* VrPawn,
	const EControllerHand DefaultPrimaryHand, const EVisualizationType DefaultVisType, const ELaserType DefaultLaserType
) {
	ParentVrPawn = VrPawn;
	PrimaryHand = DefaultPrimaryHand;
	CurrentVisualizationType = DefaultVisType;
	LaserType = DefaultLaserType;
}

void UHandsController::BeginPlay() {
	Super::BeginPlay();

	const auto NewMotionControllerObject = [this] (const TSubclassOf<UHandMotionControllerBase>& Class, const EControllerHand HandType) {
		if (Class) {
			const auto MotionController = NewObject<UHandMotionControllerBase>(this, Class);
			MotionController->Init(this, HandType);
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

	if (CurrentVisualizationType != EVisualizationType::None)
		UpdateVisualizationType(CurrentVisualizationType);
}

void UHandsController::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	if (LeftMotionController) {
		LeftMotionController->DestroyComponent();
		LeftMotionController = nullptr;
	}
	if (RightMotionController) {
		RightMotionController->DestroyComponent();
		RightMotionController = nullptr;
	}
	Super::EndPlay(EndPlayReason);
}

void UHandsController::TickComponent(const float Dt, const ELevelTick Tt, FActorComponentTickFunction* Tf) {
	Super::TickComponent(Dt, Tt, Tf);
	const auto NewVisualizationType = GetNewVisualizationType();
	if (NewVisualizationType != CurrentVisualizationType) {
		OnVisualizationTypeChanged.Execute(NewVisualizationType);
		UpdateVisualizationType(NewVisualizationType);
	}
}

void UHandsController::SetPrimaryHand(const EControllerHand NewPrimaryHand) {
	if (PrimaryHand != NewPrimaryHand) {
		PrimaryHand = NewPrimaryHand;
		if (CurrentVisualizationType != EVisualizationType::None)
			PrimaryMotionController->GetVisualizationComponent()->SwapPrimary(SecondaryMotionController->GetVisualizationComponent());
		Swap(PrimaryMotionController, SecondaryMotionController);
	}
}

void UHandsController::SetLaserType(const ELaserType NewLaserType) {
	if (LaserType != NewLaserType) {
		LaserType = NewLaserType;
		if (const auto PrimaryControllerVis = PrimaryMotionController->GetVisualizationComponent())
			PrimaryControllerVis->UpdateLaserType(LaserType);
	}
}

void UHandsController::PlayFireEffects() const {
	if (const auto PrimaryControllerVis = PrimaryMotionController->GetVisualizationComponent())
		PrimaryControllerVis->PlayFireEffects();
}

const APlayerController* UHandsController::GetPlayerController() const {
	if (ParentVrPawn.IsValid())
		return ParentVrPawn->GetPlayerController();
	return nullptr;
}

EVisualizationType UHandsController::GetNewVisualizationType() {
	if (UOculusXRInputFunctionLibrary::IsHandTrackingEnabled())
		return EVisualizationType::Tracked;
	if (UOculusXRFunctionLibrary::IsDeviceTracked(EOculusXRTrackedDeviceType::LTouch)
		|| UOculusXRFunctionLibrary::IsDeviceTracked(EOculusXRTrackedDeviceType::RTouch))
			return EVisualizationType::Controller;
	return EVisualizationType::None;
}

void UHandsController::UpdateVisualizationType(const EVisualizationType NewType) {
	PrimaryMotionController->UpdateVisualization(NewType, true, LaserType);
	SecondaryMotionController->UpdateVisualization(NewType, false, LaserType);
	CurrentVisualizationType = NewType;
}
