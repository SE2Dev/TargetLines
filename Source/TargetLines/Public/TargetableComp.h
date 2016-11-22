#pragma once

#include "Targetable.h"
#include "Components/ActorComponent.h"
#include "TargetableComp.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TARGETLINES_API UTargetableComp : public UActorComponent
{
	GENERATED_BODY()

protected:
#if WITH_EDITORONLY_DATA
	UPROPERTY()
		class UTargetable* Targetable;
#endif

public:	
	//
	// Array of Targets - Can be modified in the editor
	//
	UPROPERTY(EditAnywhere)
		TArray<AActor*> Targets;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Targetable)
		USceneComponent* SceneRoot;

	UTargetableComp();
	UTargetableComp(const class FObjectInitializer& ObjectInitializer);

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;
};
