#include "HandVisualizationInterface.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "HandMotionControllerBase.h"
#include "DuckHuntVr/Characters/Player/HandsController/HandsController.h"

static UEnhancedInputLocalPlayerSubsystem* GetEnhancedInputSubsystem(const APlayerController* PlayerController) {
	return ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
}

void IHandVisualizationInterface::Init(UHandMotionControllerBase* MotionController, const bool Primary) {
	ParentMotionController = MotionController;

	InitImpl(MotionController, Primary);

	if (const auto PlayerController = MotionController->GetParentHandsController()->GetPlayerController())
		if (const auto Subsystem = GetEnhancedInputSubsystem(PlayerController))
			if (const auto Component = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
				AddMappingContexts(Subsystem, Component);
}

void IHandVisualizationInterface::Destroy() {
	if (UnderlyingComponent.IsValid()) {
		if (const auto PlayerController = ParentMotionController->GetParentHandsController()->GetPlayerController())
			if (const auto Subsystem = GetEnhancedInputSubsystem(PlayerController))
				ClearMappingContexts(Subsystem);
		UnderlyingComponent->DestroyComponent();
	}
	UnderlyingComponent = nullptr;
}

void IHandVisualizationInterface::SwapPrimary(IHandVisualizationInterface* SecondaryHandVisualization) {
	check(this != SecondaryHandVisualization);
	check(IsPrimary());
	check(!SecondaryHandVisualization->IsPrimary());
}

void IHandVisualizationInterface::PlayFireEffects() {
	check(IsPrimary());
}

void IHandVisualizationInterface::UpdateLaserType(const ELaserType NewType) {
	check(IsPrimary());
}

EControllerHand IHandVisualizationInterface::GetHandType() const {
	return ParentMotionController->GetHandType();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void IHandVisualizationInterface::Fire() {
	if (IsPrimary())
		ParentMotionController->GetParentHandsController()->OnGunFired.Execute();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void IHandVisualizationInterface::Menu() {
	ParentMotionController->GetParentHandsController()->OnMenuPressed.Execute();
}
