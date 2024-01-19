#include "TrackedVisualizationBase.h"
#include "AudioComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "HandPoseRecognizer.h"
#include "DuckHuntVr/Characters/Player/Laser/LaserBase.h"

UTrackedVisualizationBase::UTrackedVisualizationBase() {
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.bTickEvenWhenPaused = true;
}

void UTrackedVisualizationBase::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	if (GunPoseRecognizer) {
		GunPoseRecognizer->DestroyComponent();
		GunPoseRecognizer = nullptr;
	}
	if (LaserComponent) {
		LaserComponent->DestroyComponent();
		LaserComponent = nullptr;
	}
	if (FireAudioComponent) {
		FireAudioComponent->DestroyComponent();
		FireAudioComponent = nullptr;
	}
	Super::EndPlay(EndPlayReason);
}

void UTrackedVisualizationBase::TickComponent(const float Dt, const ELevelTick Tt, FActorComponentTickFunction* Tf) {
	Super::TickComponent(Dt, Tt, Tf);

	if (!IsPrimary()) {
		CurrentPose = GunPose::None;
		CurrentFresnel = 0.0f;
		return;
	}

	if (UOculusXRInputFunctionLibrary::GetTrackingConfidence(MeshType) != EOculusXRTrackingConfidence::High) {
		if (LaserComponent->IsActive())
			LaserComponent->Deactivate();
		CurrentPose = GunPose::None;
		CurrentFresnel = 0.0f;
		return;
	}

	const auto NewHandPose = static_cast<GunPose>(GunPoseRecognizer->GetCurrentHandPoseIndex());

	UpdateMaterialFresnelIfNeeded(NewHandPose, Dt);
	UpdateLaserTransform(NewHandPose, Dt);
	ProcessGunPoseChangesIfNeeded(NewHandPose);

	CurrentPose = NewHandPose;
}

void UTrackedVisualizationBase::SwapPrimary(IHandVisualizationInterface* SecondaryHandVisualization) {
	IHandVisualizationInterface::SwapPrimary(SecondaryHandVisualization);
	const auto NewPrimary = CastChecked<UTrackedVisualizationBase>(SecondaryHandVisualization);

	GunPoseRecognizer->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
	Swap(GunPoseRecognizer, NewPrimary->GunPoseRecognizer);
	NewPrimary->GunPoseRecognizer->Reset();
	NewPrimary->GunPoseRecognizer->Side = NewPrimary->MeshType;
	NewPrimary->GunPoseRecognizer->Poses = NewPrimary->GunPoses;
	NewPrimary->GunPoseRecognizer->AttachToComponent(NewPrimary, FAttachmentTransformRules::KeepRelativeTransform);
	NewPrimary->GunPoseRecognizer->Decode();

	LaserComponent->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
	Swap(LaserComponent, NewPrimary->LaserComponent);
	NewPrimary->LaserComponent->AttachToComponent(NewPrimary, FAttachmentTransformRules::KeepRelativeTransform);

	FireAudioComponent->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
	Swap(FireAudioComponent, NewPrimary->FireAudioComponent);
	NewPrimary->FireAudioComponent->AttachToComponent(NewPrimary, FAttachmentTransformRules::KeepRelativeTransform);

	UpdateMaterialPrimaryParameter();
	NewPrimary->UpdateMaterialPrimaryParameter();
}

void UTrackedVisualizationBase::PlayFireEffects() {
	IHandVisualizationInterface::PlayFireEffects();
	FireAudioComponent->Play();
}

void UTrackedVisualizationBase::ForceStopFireEffects() {
	IHandVisualizationInterface::ForceStopFireEffects();
	FireAudioComponent->Stop();
}

void UTrackedVisualizationBase::UpdateLaserType(const ELaserType NewType) {
	IHandVisualizationInterface::UpdateLaserType(NewType);
	LaserComponent->UpdateType(NewType);
}

void UTrackedVisualizationBase::InitImpl(USceneComponent* AttachmentParent, const bool Primary) {
	SetThisComponent(this);

	// We have to create a dynamic material instance based on the assigned MaterialOverride BEFORE skeletal mesh is
	// initialized (UOculusXRHandComponent::bSkeletalMeshInitialized) and then replace MaterialOverride with it so that
	// it will be initialized with this dynamic material and we will have the ability to control its parameters during runtime.
	// That's because UOculusXRHandComponent::InitializeSkeletalMesh() is called only once during BeginPlay() in shipping mode
	// and there is no specific function to update hand mesh material or the mesh itself (SetMaterial(0, ...) can't be used too
	// because it can be overwritten by UOculusXRHandComponent::SystemGestureMaterial during runtime), so this is kinda hacky.
	check(MaterialOverride != nullptr);
	DynamicHandMaterial = CreateDynamicMaterialInstance(0, MaterialOverride);
	MaterialOverride = DynamicHandMaterial.Get();

	SetupAttachment(AttachmentParent);
	RegisterComponent();

	if (Primary) {
		GunPoseRecognizer = NewObject<UHandPoseRecognizer>(this);
		GunPoseRecognizer->Side = MeshType;
		GunPoseRecognizer->Poses = GunPoses;
		GunPoseRecognizer->SetupAttachment(this);
		GunPoseRecognizer->RegisterComponent();

		LaserComponent = NewObject<ULaserBase>(this, LaserClass);
		LaserComponent->SetupAttachment(this);
		LaserComponent->RegisterComponent();
		LaserComponent->Deactivate();

		FireAudioComponent = NewObject<UAudioComponent>(this);
		FireAudioComponent->SetUISound(true); // allows to play sound even when game is paused
		FireAudioComponent->SetSound(FireSound);
		FireAudioComponent->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform); // fixes audio spatialization
		// FireAudioComponent->RegisterComponent(); // causes auto play
	}

	Index2BoneIdx = GetBoneIndex(FName(UOculusXRInputFunctionLibrary::GetBoneName(EOculusXRBone::Index_2)));

	UpdateMaterialPrimaryParameter();
}

void UTrackedVisualizationBase::AddMappingContexts(UEnhancedInputLocalPlayerSubsystem* Subsystem, UEnhancedInputComponent* Component) {
	if (ActionMappingContext)
		Subsystem->AddMappingContext(ActionMappingContext, 0);

	if (SystemAction) {
		Component->BindAction(SystemAction, ETriggerEvent::Started, this, &UTrackedVisualizationBase::OnMenuVisibilityChanged);
		Component->BindAction(SystemAction, ETriggerEvent::Completed, this, &UTrackedVisualizationBase::OnMenuVisibilityChanged);
	}

	if (IndexPinchAction)
		Component->BindAction(IndexPinchAction, ETriggerEvent::Triggered, this, &UTrackedVisualizationBase::OnIndexPinched);
}

void UTrackedVisualizationBase::ClearMappingContexts(UEnhancedInputLocalPlayerSubsystem* Subsystem) const {
	if (ActionMappingContext)
		Subsystem->RemoveMappingContext(ActionMappingContext);
}

FQuat UTrackedVisualizationBase::GetBoneRotationMeshTypeBased(const FTransform& BoneTransform) const {
	// fixes Oculus hand mesh weird rotation
	if (MeshType == EOculusXRHandType::HandRight)
		return BoneTransform.GetRotation() * FQuat(FVector::UpVector, PI);
	return BoneTransform.GetRotation() * FQuat(FVector::ForwardVector, PI);
}

void UTrackedVisualizationBase::UpdateMaterialFresnelIfNeeded(const GunPose NewPose, const float Dt) {
	float FresnelDelta = Dt * 2;
	if (NewPose == GunPose::None)
		FresnelDelta = -FresnelDelta;

	const float PrevValue = CurrentFresnel;
    CurrentFresnel = FMath::Clamp(CurrentFresnel + FresnelDelta, 0.0f, 1.0f);

    if (!FMath::IsNearlyEqual(PrevValue, CurrentFresnel))
    	UpdateMaterialFresnelParameter();
}

void UTrackedVisualizationBase::UpdateLaserTransform(const GunPose NewPose, const float Dt) {
	if (NewPose != GunPose::None) {
		const auto Index2Transform = GetBoneTransform(Index2BoneIdx);

		const auto Index2Pos = Index2Transform.GetLocation();
		const auto Index2Quat = GetBoneRotationMeshTypeBased(Index2Transform);

		if (CurrentPose != GunPose::None) {
			// laser quat stabilization
			const auto DistQuat = CurrentLaserQuat.AngularDistance(Index2Quat);
			CurrentLaserQuat = FQuat::Slerp(CurrentLaserQuat, Index2Quat, FMath::Clamp(DistQuat * Dt * 10, 0.0, 1.0));
		}
		else
			CurrentLaserQuat = Index2Quat;

		LaserComponent->SetWorldLocationAndRotation(Index2Pos, CurrentLaserQuat);
	}
}

void UTrackedVisualizationBase::ProcessGunPoseChangesIfNeeded(const GunPose NewPose) {
	if (CurrentPose != NewPose) {
		if (CurrentPose == GunPose::None) {
			if (!LaserComponent->IsActive())
				LaserComponent->Activate(false);
		}
		else if (NewPose == GunPose::None) {
			if (LaserComponent->IsActive())
				LaserComponent->Deactivate();
		}

		if (CurrentPose == GunPose::Gun && NewPose == GunPose::GunShot)
			Fire();
	}
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UTrackedVisualizationBase::OnMenuVisibilityChanged(const FInputActionValue& Value) {
	IsSystemMenuShown = Value.Get<bool>();
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UTrackedVisualizationBase::OnIndexPinched() {
	if (IsSystemMenuShown)
		Menu();
}
