#include "EnemyAIController.h"
#include "NavigationPath.h"       // Required for FNavigationPath
#include "NavigationData.h"    // Required for FPathFindingResult
#include "NavigationDataHandler.h" // Required for FNavLocation
#include "NavigationSystem.h" // Required for UNavigationSystemV1
#include "Kismet/GameplayStatics.h" // Required for UGameplayStatics
#include "EnemyCharacter.h" // The character this AI controls
#include "BehaviorTree/BehaviorTree.h" // Required for Behavior Tree
#include "BehaviorTree/BlackboardComponent.h" // Required for Blackboard
#include "BehaviorTree/BehaviorTreeComponent.h" // Required for BT Component
#include "Kismet/GameplayStatics.h" // For getting player character

AEnemyAIController::AEnemyAIController()
{
    PrimaryActorTick.bCanEverTick = true; // Allows this controller to tick

    BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
    BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));

    TargetPlayerKeyName = "TargetActor";
    TargetLocation = FVector::ZeroVector;
}

void AEnemyAIController::BeginPlay()
{
    Super::BeginPlay();
    // Find and set the player pawn as the initial target
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (PlayerPawn)
    {
        SetTargetPlayer(PlayerPawn);
    }
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    AEnemyCharacter* EnemyChar = Cast<AEnemyCharacter>(InPawn);
    if (EnemyChar && EnemyChar->BehaviorTree)
    {
        //BlackboardComponent->InitializeBlackboard(*(EnemyChar->BehaviorTree->BlackboardAsset));
        //BehaviorTreeComponent->StartTree(*(EnemyChar->BehaviorTree));
    }
}

void AEnemyAIController::Tick(float DeltaTime)
{

    Super::Tick(DeltaTime);
    // If not using a behavior tree for movement, chase logic would go here
}

void AEnemyAIController::SetTargetPlayer(APawn* PlayerPawn)
{
    if (BlackboardComponent && PlayerPawn)
    {
        //BlackboardComponent->SetValueAsObject(TargetPlayerKeyName, PlayerPawn);
    }
    else if (BlackboardComponent)
    {
        //BlackboardComponent->ClearValue(TargetPlayerKeyName); // Clear if no player
    }
}