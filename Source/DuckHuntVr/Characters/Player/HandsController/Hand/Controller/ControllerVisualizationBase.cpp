#include "ControllerVisualizationBase.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DuckHuntVr/Characters/Player/Gun/GunBase.h"
#include "DuckHuntVr/Characters/Player/Laser/LaserBase.h"

UControllerVisualizationBase::UControllerVisualizationBase() {
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bTickEvenWhenPaused = true;
}

void UControllerVisualizationBase::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	if (GunComponent) {
		GunComponent->DestroyComponent();
		GunComponent = nullptr;
	}
	Super::EndPlay(EndPlayReason);
}

void UControllerVisualizationBase::SwapPrimary(IHandVisualizationInterface* SecondaryHandVisualization) {
	IHandVisualizationInterface::SwapPrimary(SecondaryHandVisualization);
	const auto Secondary = CastChecked<UControllerVisualizationBase>(SecondaryHandVisualization);
	GunComponent->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
	Swap(GunComponent, Secondary->GunComponent);
	Secondary->GunComponent->AttachToComponent(Secondary, FAttachmentTransformRules::KeepRelativeTransform, TEXT("GunSocket"));
}

void UControllerVisualizationBase::PlayFireEffects() {
	IHandVisualizationInterface::PlayFireEffects();
	GunComponent->PlayFireEffects(GetHandType());
}

void UControllerVisualizationBase::UpdateLaserType(const ELaserType NewType) {
	IHandVisualizationInterface::UpdateLaserType(NewType);
	GunComponent->GetLaser()->UpdateType(NewType);
}

void UControllerVisualizationBase::InitImpl(USceneComponent* AttachmentParent, const bool Primary) {
	SetThisComponent(this);

	SetupAttachment(AttachmentParent);
	RegisterComponent();

	if (Primary) {
		GunComponent = NewObject<UGunBase>(this, GunClass);
		GunComponent->SetupAttachment(this, TEXT("GunSocket"));
		GunComponent->RegisterComponent();
	}
}

void UControllerVisualizationBase::AddMappingContexts(UEnhancedInputLocalPlayerSubsystem* Subsystem, UEnhancedInputComponent* Component) {
	Subsystem->AddMappingContext(ActionMappingContext, 0);
	Subsystem->AddMappingContext(AnimMappingContext, 0);

	Component->BindAction(FireAction, ETriggerEvent::Triggered, this, &UControllerVisualizationBase::Fire);
	if (MenuAction)
		Component->BindAction(MenuAction, ETriggerEvent::Triggered, this, &UControllerVisualizationBase::Menu);

	// don't know why, but binding with for example ETriggerEvent::Started | ETriggerEvent::Completed doesn't work

	Component->BindAction(AnimPointCapTouchAction, ETriggerEvent::Started, this, &UControllerVisualizationBase::AnimPointCapTouchActionEvent);
	Component->BindAction(AnimPointCapTouchAction, ETriggerEvent::Completed, this, &UControllerVisualizationBase::AnimPointCapTouchActionEvent);

	Component->BindAction(AnimTriggerAction, ETriggerEvent::Triggered, this, &UControllerVisualizationBase::AnimTriggerActionEvent);
	Component->BindAction(AnimTriggerAction, ETriggerEvent::Completed, this, &UControllerVisualizationBase::AnimTriggerActionEvent);

	Component->BindAction(AnimThumbCapTouchAction, ETriggerEvent::Started, this, &UControllerVisualizationBase::AnimThumbCapTouchActionEvent);
	Component->BindAction(AnimThumbCapTouchAction, ETriggerEvent::Completed, this, &UControllerVisualizationBase::AnimThumbCapTouchActionEvent);

	Component->BindAction(AnimGripAction, ETriggerEvent::Triggered, this, &UControllerVisualizationBase::AnimGripActionEvent);
	Component->BindAction(AnimGripAction, ETriggerEvent::Completed, this, &UControllerVisualizationBase::AnimGripActionEvent);
}

void UControllerVisualizationBase::ClearMappingContexts(UEnhancedInputLocalPlayerSubsystem* Subsystem) const {
	Subsystem->RemoveMappingContext(ActionMappingContext);
	Subsystem->RemoveMappingContext(AnimMappingContext);
}

const ULaserBase* UControllerVisualizationBase::GetLaser() const {
	return GunComponent->GetLaser();
}

void UControllerVisualizationBase::AnimPointCapTouchActionEvent(const FInputActionValue& Value) {
	IsPointing = Value.Get<bool>();
}

void UControllerVisualizationBase::AnimTriggerActionEvent(const FInputActionValue& Value) {
	TriggerAxis = Value.Get<float>();
}

void UControllerVisualizationBase::AnimThumbCapTouchActionEvent(const FInputActionValue& Value) {
	IsThumbUp = Value.Get<bool>();
}

void UControllerVisualizationBase::AnimGripActionEvent(const FInputActionValue& Value) {
	GripAxis = Value.Get<float>();
}
