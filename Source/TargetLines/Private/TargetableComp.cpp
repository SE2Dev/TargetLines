#include "TargetLinesPrivatePCH.h"
#include "Engine.h"
#include "TargetableComp.h"

UTargetableComp::UTargetableComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}

UTargetableComp::UTargetableComp(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SceneRoot = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("SceneComponent"));

#if WITH_EDITORONLY_DATA
	// Make sure that the TEXT(...) here matches the name of the property being assigned to on the left
	// Note: The name "TargetableComp" doesn't work, but pretty much anything else will
	Targetable = ObjectInitializer.CreateDefaultSubobject<UTargetable>(this, TEXT("Targetable"));
	Targetable->PostPhysicsComponentTick.bCanEverTick = false;
	Targetable->SetupAttachment(SceneRoot);
	Targetable->Targets = &this->Targets;
#endif
}

// Called on game start
void UTargetableComp::BeginPlay()
{
	Super::BeginPlay();
}

// Called once per frame
void UTargetableComp::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
}

