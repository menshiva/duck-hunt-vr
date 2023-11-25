#include "Laser.h"
#include "Kismet/KismetSystemLibrary.h"

ULaser::ULaser() {
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bAllowTickOnDedicatedServer = false;

	TraceTypeQuery = UEngineTypes::ConvertToTraceType(ECC_GameTraceChannel1);
}

void ULaser::TickComponent(const float Dt, const ELevelTick Tt, FActorComponentTickFunction* Tf) {
	Super::TickComponent(Dt, Tt, Tf);

	const auto Start = GetComponentLocation();
	const auto Fwd = GetForwardVector();

    FHitResult HitResult;
    UKismetSystemLibrary::LineTraceSingle(
    	this,
    	Start, Start + Fwd * 1000.0f,
    	TraceTypeQuery, false, {},
    	EDrawDebugTrace::ForOneFrame, HitResult, true,
    	FLinearColor::Black, FLinearColor::Red, 0
    );
}
