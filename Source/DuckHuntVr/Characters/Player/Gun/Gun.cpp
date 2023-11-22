#include "Gun.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/AudioComponent.h"
#include "..\Hands\Controller\ControllerVisualization.h"
#include "Haptics/HapticFeedbackEffect_Curve.h"
#include "Kismet/GameplayStatics.h"

UGunComponentBase::UGunComponentBase() {
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bAllowTickOnDedicatedServer = false;

	UPrimitiveComponent::SetEnableGravity(false);
	bApplyImpulseOnDamage = false;
	bReplicatePhysicsToAutonomousProxy = false;

	SetGenerateOverlapEvents(false);
	CanCharacterStepUpOn = ECB_No;
	UStaticMeshComponent::SetCollisionProfileName(TEXT("NoCollision"), false);

	SetCastShadow(false);

	FireAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("FireAudioComponent"));
	FireAudioComponent->SetupAttachment(this);

	FireAudioComponent->PrimaryComponentTick.bStartWithTickEnabled = false;
	FireAudioComponent->PrimaryComponentTick.bCanEverTick = false;
	FireAudioComponent->PrimaryComponentTick.bAllowTickOnDedicatedServer = false;
}

void UGunComponentBase::Init(UControllerVisualizationBase* Parent, const bool ShouldRegister) {
	check(ParentControllerVisualizationComponent.IsExplicitlyNull());
	check(Parent);

	ParentControllerVisualizationComponent = Parent;
	const auto& HandInitData = GetHandInitDataBasedOnParent();

	SetRelativeTransform(HandInitData.Transform);

	if (ShouldRegister)
		RegisterComponent();
	else
		check(IsRegistered());

	AttachToComponent(Parent, FAttachmentTransformRules::KeepRelativeTransform);
	InitFireMappingContext(HandInitData);
}

void UGunComponentBase::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	RemoveFireMappingContext();
	Super::EndPlay(EndPlayReason);
}

void UGunComponentBase::SetNewParentControllerVisualization(UControllerVisualizationBase* NewParent) {
	check(!ParentControllerVisualizationComponent.IsExplicitlyNull());
	check(NewParent);
	check(NewParent != ParentControllerVisualizationComponent.Get());

	RemoveFireMappingContext();
	DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
	ParentControllerVisualizationComponent = nullptr;

	Init(NewParent, false);
}

const FGunInitPerHand& UGunComponentBase::GetHandInitDataBasedOnParent() const {
	return PerHandInitData.FindChecked(ParentControllerVisualizationComponent->GetHandType());
}

void UGunComponentBase::InitFireMappingContext(const FGunInitPerHand& HandInitData) {
	if (const auto PlayerController = UGameplayStatics::GetPlayerController(this, 0)) {
		if (const auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			Subsystem->AddMappingContext(HandInitData.FireMappingContext, 0);

		if (const auto EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
			EnhancedInputComponent->BindAction(HandInitData.FireAction, ETriggerEvent::Triggered, this, &UGunComponentBase::Fire);
	}
}

void UGunComponentBase::RemoveFireMappingContext() const {
	if (const auto PlayerController = UGameplayStatics::GetPlayerController(this, 0))
		if (const auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			Subsystem->RemoveMappingContext(GetHandInitDataBasedOnParent().FireMappingContext);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UGunComponentBase::Fire() {
	FireAudioComponent->Play();
	UGameplayStatics::GetPlayerController(this, 0)->PlayHapticEffect(
		FireHapticFeedbackEffect, ParentControllerVisualizationComponent->GetHandType()
	);
}
