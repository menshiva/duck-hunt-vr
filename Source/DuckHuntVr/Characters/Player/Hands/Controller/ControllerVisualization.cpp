#include "ControllerVisualization.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "DuckHuntVr/Characters/Player/Gun/Gun.h"
#include "DuckHuntVr/Characters/Player/HandsController/MotionController/HandMotionController.h"
#include "Kismet/GameplayStatics.h"

UControllerVisualizationBase::UControllerVisualizationBase() {
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bAllowTickOnDedicatedServer = false;

	SetAllowClothActors(false);
	bResetAfterTeleport = false;

	USkeletalMeshComponent::SetEnableGravity(false);
	bApplyImpulseOnDamage = false;
	bReplicatePhysicsToAutonomousProxy = false;

	SetGenerateOverlapEvents(false);
	CanCharacterStepUpOn = ECB_No;
	UPrimitiveComponent::SetCollisionProfileName(TEXT("NoCollision"), false);

	SetCastShadow(false);
}

void UControllerVisualizationBase::Init(UHandMotionController* Parent) {
	IHandVisualizationInterface::Init(Parent);

	AttachToComponent(Parent, FAttachmentTransformRules::KeepRelativeTransform);
	RegisterComponent();

	InitAnimMappingContext();
}

void UControllerVisualizationBase::Destroy() {
	if (GunComponent) {
		GunComponent->DestroyComponent();
		GunComponent = nullptr;
	}

	RemoveAnimMappingContext();
	DestroyComponent(true);
}

void UControllerVisualizationBase::SetPrimary(const bool InitPrimary) {
	IHandVisualizationInterface::SetPrimary(InitPrimary);
	if (InitPrimary) {
		GunComponent = NewObject<UGunComponentBase>(this, GunClass);
		GunComponent->Init(this);
	}
}

void UControllerVisualizationBase::SwapPrimary(IHandVisualizationInterface* OtherHandVisualization) {
	IHandVisualizationInterface::SwapPrimary(OtherHandVisualization);
	check(Cast<UControllerVisualizationBase>(OtherHandVisualization) != nullptr);
	const auto Secondary = CastChecked<UControllerVisualizationBase>(OtherHandVisualization);
	Swap(GunComponent, Secondary->GunComponent);
	Secondary->GunComponent->SetNewParentControllerVisualization(Secondary);
}

void UControllerVisualizationBase::InitAnimMappingContext() {
	if (const auto PlayerController = UGameplayStatics::GetPlayerController(this, 0)) {
		if (const auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			Subsystem->AddMappingContext(AnimMappingContext, 0);

		if (const auto EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent)) {
			// don't know why, but binding with for example ETriggerEvent::Started | ETriggerEvent::Completed doesn't work

			EnhancedInputComponent->BindAction(AnimPointCapTouchAction, ETriggerEvent::Started, this, &UControllerVisualizationBase::AnimPointCapTouchActionEvent);
			EnhancedInputComponent->BindAction(AnimPointCapTouchAction, ETriggerEvent::Completed, this, &UControllerVisualizationBase::AnimPointCapTouchActionEvent);

			EnhancedInputComponent->BindAction(AnimTriggerAction, ETriggerEvent::Triggered, this, &UControllerVisualizationBase::AnimTriggerActionEvent);
			EnhancedInputComponent->BindAction(AnimTriggerAction, ETriggerEvent::Completed, this, &UControllerVisualizationBase::AnimTriggerActionEvent);

			EnhancedInputComponent->BindAction(AnimThumbCapTouchAction, ETriggerEvent::Started, this, &UControllerVisualizationBase::AnimThumbCapTouchActionEvent);
			EnhancedInputComponent->BindAction(AnimThumbCapTouchAction, ETriggerEvent::Completed, this, &UControllerVisualizationBase::AnimThumbCapTouchActionEvent);

			EnhancedInputComponent->BindAction(AnimGripAction, ETriggerEvent::Triggered, this, &UControllerVisualizationBase::AnimGripActionEvent);
			EnhancedInputComponent->BindAction(AnimGripAction, ETriggerEvent::Completed, this, &UControllerVisualizationBase::AnimGripActionEvent);
		}
	}
}

void UControllerVisualizationBase::RemoveAnimMappingContext() const {
	if (const auto PlayerController = UGameplayStatics::GetPlayerController(this, 0))
		if (const auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			Subsystem->RemoveMappingContext(AnimMappingContext);
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
