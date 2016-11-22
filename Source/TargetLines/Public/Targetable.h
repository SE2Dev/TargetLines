#pragma once

#include "Components/PrimitiveComponent.h"
#include "Targetable.generated.h"

UCLASS()
class TARGETLINES_API UTargetable : public UPrimitiveComponent
{
	GENERATED_BODY()

public:
	// UPROPERTY() // Does this even need to be here?
	TArray<AActor*>* Targets;

	UTargetable(const class FObjectInitializer& ObjectInitializer);

	virtual FPrimitiveSceneProxy* CreateSceneProxy() override;

	virtual bool ShouldRecreateProxyOnUpdateTransform() const override;

	virtual FBoxSphereBounds CalcBounds(const FTransform &LocalToWorld) const override;
	
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
};
