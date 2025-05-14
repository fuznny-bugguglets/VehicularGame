#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h" // Changed from ActorComponent to SceneComponent
#include "TurretRotationComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VEHICULARGAME_API UTurretRotationComponent : public USceneComponent // Changed base class
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UTurretRotationComponent();

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

public:
    // Called every frame if bCanEverTick is true in constructor
    // virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    /**
     * Rotates this component (and its children, like a turret head mesh) towards the point
     * where a raycast from the camera hits, or matches the camera's rotation if no hit occurs.
     * @param CameraTransform The current transform of the camera.
     * @param DeltaTime The time since the last frame.
     * @param TraceDistance The maximum distance for the raycast.
     * @param InterpolationSpeed How quickly the component rotates to the target.
     */
    UFUNCTION(BlueprintCallable, Category = "Turret Control")
    void RotateTurretHead(const FTransform& CameraTransform, float DeltaTime, float TraceDistance = 10000.0f, float InterpolationSpeed = 5.0f);

private:
    // Helper function to get the owning actor (if needed, though less critical for self-rotation)
    AActor* GetOwningActor() const;
};