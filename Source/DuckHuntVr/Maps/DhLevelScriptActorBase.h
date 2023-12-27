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

	FORCEINLINE const AVrPawn* GetPawn() const { return Pawn.Get(); }
	FORCEINLINE UDhGameInstance* GetDhGameInstance() const { return GameInstance.Get(); }
protected:
	void OpenLevel(const TSoftObjectPtr<UWorld>& Level);

	UPROPERTY(EditDefaultsOnly, Category=Subcomponents)
	TWeakObjectPtr<ALevelSequenceActor> FadeInSequence;

	UPROPERTY(EditDefaultsOnly, Category=Subcomponents)
	TWeakObjectPtr<AVrPawn> Pawn;

	TWeakObjectPtr<UDhGameInstance> GameInstance;
private:
	UFUNCTION()
	void OnFadeInSequenceEnd();

	const TSoftObjectPtr<UWorld>* LevelToOpen = nullptr;
};
