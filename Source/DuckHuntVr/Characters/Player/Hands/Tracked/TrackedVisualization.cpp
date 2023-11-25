#include "TrackedVisualization.h"
#include "DuckHuntVr/Characters/Player/HandsController/MotionController/HandMotionController.h"

UTrackedVisualizationBase::UTrackedVisualizationBase() {
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bAllowTickOnDedicatedServer = false;

	SetGenerateOverlapEvents(false);
	CanCharacterStepUpOn = ECB_No;
	UPrimitiveComponent::SetCollisionProfileName(TEXT("NoCollision"), false);

	SetCastShadow(false);

	bUpdateHandScale = true;
}

void UTrackedVisualizationBase::Init(UHandMotionController* Parent) {
	IHandVisualizationInterface::Init(Parent);

	// We have to create a dynamic material instance based on the assigned MaterialOverride BEFORE skeletal mesh is
	// initialized (UOculusXRHandComponent::bSkeletalMeshInitialized) and then replace MaterialOverride with it so that
	// it will be initialized with this dynamic material and we will have the ability to control its parameters during runtime.
	// That's because UOculusXRHandComponent::InitializeSkeletalMesh() is called only once during BeginPlay() in shipping mode
	// and there is no specific function to update hand mesh material or the mesh itself (SetMaterial(0, ...) can't be used too
	// because it can be overwritten by UOculusXRHandComponent::SystemGestureMaterial during runtime), so this is kinda hacky.
	check(MaterialOverride != nullptr);
	DynamicHandMaterial = CreateDynamicMaterialInstance(0, MaterialOverride);
	MaterialOverride = DynamicHandMaterial.Get();

	RegisterComponent();
	AttachToComponent(Parent, FAttachmentTransformRules::KeepRelativeTransform);
}

void UTrackedVisualizationBase::Destroy() {
	DestroyComponent(true);
}

void UTrackedVisualizationBase::SetPrimary(const bool InitPrimary) {
	IHandVisualizationInterface::SetPrimary(InitPrimary);
	Primary = InitPrimary;
	UpdateHandMaterialColor();
}

void UTrackedVisualizationBase::SwapPrimary(IHandVisualizationInterface* OtherHandVisualization) {
	IHandVisualizationInterface::SwapPrimary(OtherHandVisualization);
	check(Cast<UTrackedVisualizationBase>(OtherHandVisualization) != nullptr);

	const auto Secondary = CastChecked<UTrackedVisualizationBase>(OtherHandVisualization);
	Swap(Primary, Secondary->Primary);

	UpdateHandMaterialColor();
	Secondary->UpdateHandMaterialColor();
}
