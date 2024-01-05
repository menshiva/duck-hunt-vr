#include "DhGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"

void UDhGameInstance::OnStart() {
	Super::OnStart();
	UKismetSystemLibrary::ExecuteConsoleCommand(this, TEXT("r.ScreenPercentage 100"));
}

void UDhGameInstance::SavePrimaryHand(const EControllerHand NewPrimaryHand) {
	PrimaryHand = NewPrimaryHand;
	SaveConfig();
}

void UDhGameInstance::SaveLaserType(const ELaserType NewLaserType) {
	LaserType = NewLaserType;
	SaveConfig();
}
