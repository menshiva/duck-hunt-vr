#include "MainMenuWidget.h"
#include "DuckHuntVr/Maps/DhLevelScriptActorMenu.h"
#include "DuckHuntVr/UI/Widgets/ButtonWidget.h"

void UMainMenuWidget::NativeConstruct() {
	Super::NativeConstruct();
	const auto MenuLevelScript = Cast<ADhLevelScriptActorMenu>(GetWorld()->GetLevelScriptActor());

	GameAButton->SetOnClickEvent([MenuLevelScript] {
		MenuLevelScript->OpenDuckLevel(1);
	});

	GameBButton->SetOnClickEvent([MenuLevelScript] {
		MenuLevelScript->OpenDuckLevel(2);
	});

	GameCButton->SetOnClickEvent([MenuLevelScript] {
		MenuLevelScript->OpenClayLevel();
	});
}
