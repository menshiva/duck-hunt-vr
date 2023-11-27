#include "Laser.h"
#include "NiagaraComponent.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

ULaser::ULaser() {
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bAllowTickOnDedicatedServer = false;

	TraceTypeQuery = UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1);

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LaserTrace"));
	NiagaraComponent->SetupAttachment(this);
}

void ULaser::BeginPlay() {
	Super::BeginPlay();

	NiagaraComponent->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
	NiagaraComponent->RegisterComponent();
}

void ULaser::TickComponent(const float Dt, const ELevelTick Tt, FActorComponentTickFunction* Tf) {
	Super::TickComponent(Dt, Tt, Tf);

	const auto Start = GetComponentLocation();
	const auto Fwd = GetForwardVector();
	auto End = Start + Fwd * 1000.0f;

    FHitResult HitResult;
    if (UKismetSystemLibrary::LineTraceSingle(
    	this,
    	Start, End,
    	TraceTypeQuery, false, {},
    	EDrawDebugTrace::None, HitResult, true,
    	FLinearColor::Black, FLinearColor::Red, 0
    )) {
		End = HitResult.ImpactPoint;
	}

	SetNiagaraComponentPosition(0, Start);
	SetNiagaraComponentPosition(1, End);
}

void ULaser::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	NiagaraComponent->DestroyComponent();
	NiagaraComponent = nullptr;

	Super::EndPlay(EndPlayReason);
}

void ULaser::SetNiagaraComponentPosition(const int Index, const FVector& Pos) const {
	UNiagaraDataInterfaceArrayFunctionLibrary::SetNiagaraArrayVectorValue(
		NiagaraComponent,
		"User.PointArray", Index,
		Pos, false
	);
}
