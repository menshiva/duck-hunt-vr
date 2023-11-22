#include "TrackedVisualization.h"
#include "DuckHuntVr/Characters/Player/HandsController/MotionController/HandMotionController.h"

UTrackedVisualizationBase::UTrackedVisualizationBase() {
	PrimaryComponentTick.bAllowTickOnDedicatedServer = false;

	SetGenerateOverlapEvents(false);
	CanCharacterStepUpOn = ECB_No;
	UPrimitiveComponent::SetCollisionProfileName(TEXT("NoCollision"), false);

	SetCastShadow(false);

	bUpdateHandScale = true;
}

void UTrackedVisualizationBase::Init(UHandMotionController* Parent) {
	IHandVisualizationInterface::Init(Parent);
	RegisterComponent();
	AttachToComponent(Parent, FAttachmentTransformRules::KeepRelativeTransform);
}

void UTrackedVisualizationBase::Destroy() {
	DestroyComponent(true);
}

void UTrackedVisualizationBase::SetPrimary(const bool Primary) {
	IHandVisualizationInterface::SetPrimary(Primary);
	if (Primary) {
		MaterialOverride = PrimaryMaterial;
		bSkeletalMeshInitialized = false; // Force the mesh to be reinitialized
	}
}

void UTrackedVisualizationBase::SwapPrimary(IHandVisualizationInterface* OtherHandVisualization) {
	IHandVisualizationInterface::SwapPrimary(OtherHandVisualization);
	check(Cast<UTrackedVisualizationBase>(OtherHandVisualization) != nullptr);

	const auto Secondary = CastChecked<UTrackedVisualizationBase>(OtherHandVisualization);
	Swap(MaterialOverride, Secondary->MaterialOverride);

	// Force meshes reinitialization
	// (I can use SetMaterial(0, MaterialOverride) as in UOculusXRHandComponent::InitializeSkeletalMesh(), but this won't
	// update UOculusXRHandComponent::CachedBaseMaterial and this variable is private, so, as far as I can see, there is
	// no other way than to force UOculusXRHandComponent::Tick() to fully reinitialize the skeletal mesh with the new material...)
	bSkeletalMeshInitialized = false;
	Secondary->bSkeletalMeshInitialized = false;
}
