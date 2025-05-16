// EnemyCharacter.cpp
#include "EnemyCharacter.h"
#include "AIController.h" // Required for AAIController
#include "GameFramework/CharacterMovementComponent.h" // Required for character movement
#include "Kismet/GameplayStatics.h" // Required for getting player character
#include "Components/CapsuleComponent.h" // Include for UCapsuleComponent

// Potentially include "BehaviorTree/BlackboardComponent.h" if using Blackboard for AI
// Potentially include "Perception/PawnSensingComponent.h" for more advanced detection

AEnemyCharacter::AEnemyCharacter()
{
    PrimaryActorTick.bCanEverTick = true; // Allows this actor to tick
    MaxHealth = 100.0f;
    CurrentHealth = MaxHealth;

    // Configure character movement
    GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // Rotation speed
    GetCharacterMovement()->MaxWalkSpeed = 300.f; // Walking speed
}

void AEnemyCharacter::BeginPlay()
{
    Super::BeginPlay();
    CurrentHealth = MaxHealth; // Set current health to max health at start
}

void AEnemyCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    // AI movement logic is typically handled by the AIController and Behavior Tree
}

float AEnemyCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
    const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    if (ActualDamage > 0.f && !IsDead())
    {
        CurrentHealth -= ActualDamage;

        if (CurrentHealth <= 0.f)
        {
            Die();
        }
    }
    return ActualDamage;
}


void AEnemyCharacter::Die()
{
    // --- Essential cleanup before destruction ---
    GetCharacterMovement()->StopMovementImmediately();
    GetCharacterMovement()->DisableMovement();

    // Disable all collision on the capsule
    UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
    if (CapsuleComp)
    {
        CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        CapsuleComp->SetCollisionResponseToAllChannels(ECR_Ignore);
    }

    // Stop AI Controller
    AAIController* AIController = Cast<AAIController>(GetController());
    if (AIController)
    {
        AIController->StopMovement();
        // If using a Behavior Tree, you might want to stop it:
        // if (AIController->BrainComponent)
        // {
        //    AIController->BrainComponent->StopLogic("Died");
        // }
        // You might also want to unpossess the pawn if the controller could be reused.
        // AIController->UnPossess();
    }

    // --- INSTANTLY DESTROY THE ACTOR ---
    Destroy();
}

bool AEnemyCharacter::IsDead() const
{
    return CurrentHealth <= 0.f;
}