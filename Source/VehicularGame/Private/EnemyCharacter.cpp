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
    if (IsDead()) return; // Already dead

    CurrentHealth = 0.f; // Ensure health is zero

    // Disable further movement and collision
    GetCharacterMovement()->StopMovementImmediately();
    GetCharacterMovement()->DisableMovement();
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // Stop AI Controller
    AAIController* AIController = Cast<AAIController>(GetController());
    if (AIController)
    {
        AIController->StopMovement();
        // If using Behavior Tree, stop it
        // AIController->BrainComponent->StopLogic("Died");
    }

    // Example: Play death animation (montage)
    // UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
    // if (AnimInstance && DeathMontage) { AnimInstance->Montage_Play(DeathMontage); }

    // Destroy actor after a delay
    SetLifeSpan(5.0f); // Actor will be destroyed after 5 seconds

    // You could also broadcast an event here for other systems to react to death
}

bool AEnemyCharacter::IsDead() const
{
    return CurrentHealth <= 0.f;
}