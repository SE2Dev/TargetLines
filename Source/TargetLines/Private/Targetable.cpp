#include "TargetLinesPrivatePCH.h"
#include "Engine.h"
#include "Targetable.h"

#if !UE_SERVER

class TARGETLINES_API UTargetableProxy : public FPrimitiveSceneProxy
{
private:
	FVector TargetableLocation;
	TArray<AActor*>* Targets;

public:

	UTargetableProxy(const UPrimitiveComponent* InComponent) : FPrimitiveSceneProxy(InComponent)
	{
		const UTargetable *Targetable = Cast<UTargetable>(InComponent);
		if (Targetable != NULL)
		{
			this->Targets = Targetable->Targets;
			this->TargetableLocation = (InComponent->GetOwner() == NULL) ? FVector(0, 0, 0) : InComponent->GetOwner()->GetActorLocation();
		}
		else
		{
			this->Targets = NULL;
		}
	}

	virtual void GetDynamicMeshElements(const TArray<const FSceneView*>& Views, const FSceneViewFamily& ViewFamily, uint32 VisibilityMap, FMeshElementCollector& Collector) const override
	{
		const FVector Start = TargetableLocation;

		const FColor LINE_COLOR = FColor(0, 0, 255);
		const float LINE_THICKNESS = 4;

		for (int32 ViewIndex = 0; ViewIndex < Views.Num(); ViewIndex++)
		{
			if (!(VisibilityMap & (1 << ViewIndex)) || this->Targets == NULL)
			{
				continue;
			}

			FPrimitiveDrawInterface* PDI = Collector.GetPDI(ViewIndex);

			for (int i = 0; i < this->Targets->Num(); i++)
			{
				AActor* Target = (*this->Targets)[i];
				if (Target == NULL)
				{
					continue;
				}

				FVector End = (*this->Targets)[i]->GetActorLocation();
				
				PDI->DrawLine(Start, End, LINE_COLOR, 0, LINE_THICKNESS, 0, true);
			}
		}
	}

	virtual uint32 GetMemoryFootprint(void) const
	{
		return(sizeof(*this));
	}

	FPrimitiveViewRelevance GetViewRelevance(const FSceneView* View) const
	{
		FPrimitiveViewRelevance Result;
		Result.bDrawRelevance = true; // IsShown(View) && (IsSelected() || View->Family->EngineShowFlags.Navigation);
		Result.bDynamicRelevance = true;
		Result.bNormalTranslucencyRelevance = true; //  IsShown(View);
		Result.bShadowRelevance = IsShadowCast(View);
		Result.bEditorPrimitiveRelevance = UseEditorCompositing(View);
		return Result;
	}
};

#endif


UTargetable::UTargetable(const class FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Allows updating in game, while optimizing rendering for the case that it is not modified
	Mobility = EComponentMobility::Stationary;

	BodyInstance.SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);

	AlwaysLoadOnClient = false;
	AlwaysLoadOnServer = false;
	bHiddenInGame = true;
	bGenerateOverlapEvents = false;

	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.bTickEvenWhenPaused = true;
	bTickInEditor = true;
}

FPrimitiveSceneProxy* UTargetable::CreateSceneProxy()
{
#if UE_SERVER
	return nullptr;
#else
	return new UTargetableProxy(this);
#endif
}

// Should recreate proxy once every update
bool UTargetable::ShouldRecreateProxyOnUpdateTransform() const
{
	return true;
}

FBoxSphereBounds UTargetable::CalcBounds(const FTransform & LocalToWorld) const
{
	FBox Bounds(0);

	if (GExitPurge || HasAnyFlags(RF_BeginDestroyed) || GetWorld() == nullptr ||
		this->Targets == NULL || this->Targets->Num() < 1 ||
		this->GetOwner() == NULL)
	{
		return FBoxSphereBounds(Bounds);
	}

	FVector Location = this->GetOwner()->GetActorLocation();
	Bounds = FBox(&Location, 1);

	for (int i = 0; i < this->Targets->Num(); i++)
	{
		AActor* Target = (*this->Targets)[i];
		if (Target == NULL)
		{
			continue;
		}

		FVector offset = Target->GetActorLocation();
		Bounds.Min = Bounds.Min.ComponentMin(offset);
		Bounds.Max = Bounds.Max.ComponentMax(offset);
	}

	//
	// Old method of calculating bounds, had several bugs relating to the world origin
	// (Is only here for reference)
	//
	/*FVector min_extent = (Location - Bounds.Min).GetAbs();
	FVector max_extent = (Location - Bounds.Max).GetAbs();
	
	FVector Extent = min_extent.ComponentMax(max_extent);

	return FBoxSphereBounds(Location, Extent, 1);*/

	return FBoxSphereBounds(Bounds);
}

void UTargetable::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
