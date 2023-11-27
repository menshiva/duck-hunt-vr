#include "Gun.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/AudioComponent.h"
#include "DuckHuntVr/Characters/Player/Hands/Controller/ControllerVisualization.h"
#include "DuckHuntVr/Characters/Player/Laser/Laser.h"
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
}

void UGunComponentBase::Init(UControllerVisualizationBase* Parent, const bool CalledFirstTime) {
	check(ParentControllerVisualizationComponent.IsExplicitlyNull());
	check(Parent);

	ParentControllerVisualizationComponent = Parent;
	const auto& HandInitData = GetHandInitDataBasedOnParent();

	SetRelativeTransform(HandInitData.Transform);

	AttachToComponent(Parent, FAttachmentTransformRules::KeepRelativeTransform);
	if (CalledFirstTime)
		RegisterComponent();
	else
		check(IsRegistered());

	InitFireMappingContext(HandInitData);
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

void UGunComponentBase::BeginPlay() {
	Super::BeginPlay();

	FireAudioComponent->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	// FireAudioComponent->RegisterComponent(); // causes auto-play when gun is created

	LaserComponent = NewObject<ULaser>(this, LaserClass);
	LaserComponent->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform, TEXT("LaserSocket"));
	LaserComponent->RegisterComponent();
}

void UGunComponentBase::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	FireAudioComponent->DestroyComponent();
	FireAudioComponent = nullptr;

	LaserComponent->DestroyComponent();
	LaserComponent = nullptr;

	RemoveFireMappingContext();
	Super::EndPlay(EndPlayReason);
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
