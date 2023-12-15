#include "TrackedVisualizationBase.h"
#include "DuckHuntVr/Characters/Player/Laser/LaserBase.h"

UTrackedVisualizationBase::UTrackedVisualizationBase() {
	PrimaryComponentTick.bStartWithTickEnabled = true;
}

void UTrackedVisualizationBase::OnComponentDestroyed(const bool bDestroyingHierarchy) {
	if (LaserComponent) {
		LaserComponent->DestroyComponent();
		LaserComponent = nullptr;
	}
	Super::OnComponentDestroyed(bDestroyingHierarchy);
}

void UTrackedVisualizationBase::SwapPrimary(IHandVisualizationInterface* SecondaryHandVisualization) {
	IHandVisualizationInterface::SwapPrimary(SecondaryHandVisualization);
	const auto Secondary = CastChecked<UTrackedVisualizationBase>(SecondaryHandVisualization);

	LaserComponent->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
	Swap(LaserComponent, Secondary->LaserComponent);
	Secondary->LaserComponent->AttachToComponent(Secondary, FAttachmentTransformRules::KeepRelativeTransform);

	UpdateHandMaterialColor();
	Secondary->UpdateHandMaterialColor();
}

void UTrackedVisualizationBase::UpdateLaserType() {
	IHandVisualizationInterface::UpdateLaserType();
	LaserComponent->UpdateType();
}

// TODO
void UTrackedVisualizationBase::TickComponent(const float Dt, const ELevelTick Tt, FActorComponentTickFunction* Tf) {
	Super::TickComponent(Dt, Tt, Tf);

	// const auto T = UOculusXRInputFunctionLibrary::GetTrackingConfidence(MeshType);
	// UKismetSystemLibrary::PrintString(
	// 	this, UEnum::GetValueAsString(MeshType) + FString::Printf(TEXT(" Tracking confidence: %d"), T),
	// 	true, false, FLinearColor::Red, 0.0f
	// );

	/*const auto K = UOculusXRInputFunctionLibrary::IsHandPositionValid(MeshType);
	UKismetSystemLibrary::PrintString(
		this, UEnum::GetValueAsString(MeshType) + FString::Printf(TEXT(" Hand position valid: %d"), K),
		true, false, FLinearColor::Black, 0.0f
	);

	const auto L = UOculusXRInputFunctionLibrary::IsPointerPoseValid(MeshType);
	UKismetSystemLibrary::PrintString(
		this, UEnum::GetValueAsString(MeshType) + FString::Printf(TEXT(" Pointer pose valid: %d"), L),
		true, false, FLinearColor::Black, 0.0f
	);*/

	/*if (T == EOculusXRTrackingConfidence::High) {
		// const auto PointerTransform = UOculusXRInputFunctionLibrary::GetPointerPose(MeshType);

		// const auto CompLoc = GetRelativeLocation();
		// const auto TransformedLoc = PointerTransform.TransformPositionNoScale(CompLoc);

		// auto Loc = PointerTransform.GetLocation();
		// Loc.Z = GetComponentLocation().Z;

		// const auto Fwd = PointerTransform.GetRotation().GetForwardVector();
		// UKismetSystemLibrary::DrawDebugArrow(
		// 	this, Loc, Loc + Fwd * 100.0f,
		// 	10.0f, FLinearColor::Green
		// );

		// UKismetSystemLibrary::DrawDebugArrow(
		// 	this, TransformedLoc, TransformedLoc + Fwd * 100.0f,
		// 	10.0f, FLinearColor::Black
		// );

		const FName BoneName1(UOculusXRInputFunctionLibrary::GetBoneName(EOculusXRBone::Index_1));
		const FName BoneName2(UOculusXRInputFunctionLibrary::GetBoneName(EOculusXRBone::Index_1));
		const FName BoneName3(UOculusXRInputFunctionLibrary::GetBoneName(EOculusXRBone::Index_1));
		const FName BoneName4(UOculusXRInputFunctionLibrary::GetBoneName(EOculusXRBone::Index_1));

		const auto Loc1 = GetBoneLocation(BoneName1, EBoneSpaces::WorldSpace);
		const auto Loc2 = GetBoneLocation(BoneName2, EBoneSpaces::WorldSpace);
		const auto Loc3 = GetBoneLocation(BoneName3, EBoneSpaces::WorldSpace);
		const auto Loc4 = GetBoneLocation(BoneName4, EBoneSpaces::WorldSpace);

		const auto Rot1 = UOculusXRInputFunctionLibrary::GetBoneRotation(MeshType, EOculusXRBone::Index_1).GetForwardVector();
		const auto Rot2 = UOculusXRInputFunctionLibrary::GetBoneRotation(MeshType, EOculusXRBone::Index_2).GetForwardVector();
		const auto Rot3 = UOculusXRInputFunctionLibrary::GetBoneRotation(MeshType, EOculusXRBone::Index_3).GetForwardVector();
		const auto Rot4 = UOculusXRInputFunctionLibrary::GetBoneRotation(MeshType, EOculusXRBone::Index_Tip).GetForwardVector();

		UKismetSystemLibrary::DrawDebugArrow(
			this, Loc1, Loc1 + Rot1 * 50.0f,
			10.0f, FLinearColor::Red
		);

		UKismetSystemLibrary::DrawDebugArrow(
			this, Loc2, Loc2 + Rot2 * 50.0f,
			10.0f, FLinearColor::Green
		);

		UKismetSystemLibrary::DrawDebugArrow(
			this, Loc3, Loc3 + Rot3 * 50.0f,
			10.0f, FLinearColor::Blue
		);

		UKismetSystemLibrary::DrawDebugArrow(
			this, Loc4, Loc4 + Rot4 * 50.0f,
			10.0f, FLinearColor::Yellow
		);
	}

	const auto CompLoc = GetComponentLocation();
	auto CompRot = GetComponentQuat();

	UKismetSystemLibrary::DrawDebugArrow(
		this, CompLoc, CompLoc + CompRot.GetRightVector() * 50.0f,
		10.0f, FLinearColor::Black
	);*/
}

void UTrackedVisualizationBase::InitImpl(USceneComponent* AttachmentParent, const bool Primary) {
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
		LaserComponent = NewObject<ULaserBase>(this, LaserClass);
		LaserComponent->UpdateType();
		LaserComponent->Deactivate(); // TODO
		LaserComponent->SetupAttachment(this);
		LaserComponent->RegisterComponent();
	}

	UpdateHandMaterialColor();
}
