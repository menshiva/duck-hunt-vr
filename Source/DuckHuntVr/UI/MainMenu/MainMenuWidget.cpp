#include "MainMenuWidget.h"
#include "DuckHuntVr/Maps/DhLevelScriptActorMenu.h"
#include "DuckHuntVr/UI/Widgets/ButtonWidget.h"

void UMainMenuWidget::NativeConstruct() {
	Super::NativeConstruct();
	MenuLevelScript = Cast<ADhLevelScriptActorMenu>(GetWorld()->GetLevelScriptActor());

	FOnButtonClickedEvent OnGameAButtonClicked;
	OnGameAButtonClicked.AddDynamic(this, &UMainMenuWidget::OnGameAButtonClicked);
	GameAButton->SetOnClickEvent(OnGameAButtonClicked);

	FOnButtonClickedEvent OnGameBButtonClicked;
	OnGameBButtonClicked.AddDynamic(this, &UMainMenuWidget::OnGameBButtonClicked);
	GameBButton->SetOnClickEvent(OnGameBButtonClicked);

	FOnButtonClickedEvent OnGameCButtonClicked;
	OnGameCButtonClicked.AddDynamic(this, &UMainMenuWidget::OnGameCButtonClicked);
	GameCButton->SetOnClickEvent(OnGameCButtonClicked);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UMainMenuWidget::OnGameAButtonClicked() {
	MenuLevelScript->OpenDuckLevel(1);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UMainMenuWidget::OnGameBButtonClicked() {
	MenuLevelScript->OpenDuckLevel(2);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void UMainMenuWidget::OnGameCButtonClicked() {
	MenuLevelScript->OpenClayLevel();
}
