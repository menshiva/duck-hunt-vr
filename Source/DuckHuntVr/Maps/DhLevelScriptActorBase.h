#pragma once

#include "Engine/LevelScriptActor.h"
#include "DhLevelScriptActorBase.generated.h"

class AVrPawn;
class ALevelSequenceActor;
class UDhGameInstance;

UCLASS(Abstract, NotBlueprintable, NotBlueprintType)
class DUCKHUNTVR_API ADhLevelScriptActorBase : public ALevelScriptActor {
	GENERATED_BODY()
public:
	ADhLevelScriptActorBase();
	virtual void BeginPlay() override;
protected:
	void OpenLevel(const TSoftObjectPtr<UWorld>& Level);

	UPROPERTY(EditDefaultsOnly, Category=Subcomponents)
	TWeakObjectPtr<ALevelSequenceActor> FadeInSequence;

	UPROPERTY(EditDefaultsOnly, Category=Subcomponents)
	TWeakObjectPtr<ALevelSequenceActor> IntroSequence;

	UPROPERTY(EditDefaultsOnly, Category=Subcomponents)
	TWeakObjectPtr<AVrPawn> Pawn;

	TWeakObjectPtr<UDhGameInstance> GameInstance;
private:
	void OnAsyncMapLoadCompleted(const FName&, UPackage*, EAsyncLoadingResult::Type Result);

	UFUNCTION()
	void OnFadeInSequenceEnd();

	void OpenLevelImpl() const;

	FString LevelNameToOpen;
	bool AsyncMapLoadCompleted = false;
	bool FadeInSequenceCompleted = false;
};
