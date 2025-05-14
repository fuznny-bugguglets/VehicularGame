#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "TurretRotationComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VEHICULARGAME_API UTurretRotationComponent : public USceneComponent
{
    GENERATED_BODY()

public:
    // Default component properties.
    UTurretRotationComponent();

protected:
    // Initializes the component.
    virtual void BeginPlay() override;

public:
    /**
     * Rotates component towards camera raycast hit or camera rotation.
     * @param CameraTransform Current transform of the camera.
     * @param DeltaTime Time since the last frame.
     * @param TraceDistance Maximum distance for the raycast.
     * @param InterpolationSpeed How quickly the component rotates to the target.
     */
    UFUNCTION(BlueprintCallable, Category = "Turret Control")
    void RotateTurretHead(const FTransform& CameraTransform, float DeltaTime, float TraceDistance = 10000.0f, float InterpolationSpeed = 5.0f);

private:
    AActor* GetOwningActor() const;
};