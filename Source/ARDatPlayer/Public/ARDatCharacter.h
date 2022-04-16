// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/StaticMeshComponent.h"
#include "CineCameraComponent.h"
#include "ARDatCharacter.generated.h"

UCLASS()
class ARDATPLAYER_API AARDatCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AARDatCharacter();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ARDat")
		FVector Rm=FVector(0.0,-1.0,0.0);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ARDat")
		FVector Ra=FVector( 90.0,0.0,90.0 );
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ARDat")
		UTexture2D* Texture;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ARDat")
		UStaticMeshComponent* ScrMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ARDat")
		UCineCameraComponent * Cam;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ARDat")
		UMaterial* Mat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ARDat")
		UMaterialInstanceDynamic* DynMaterial;
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


private:
	uint32_t SizeX=0, SizeY=0;
	int frameCount = 0;
	FTransform Uti,Ut;
	FMatrix44f matf;
};
