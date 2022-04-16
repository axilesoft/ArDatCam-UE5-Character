// Fill out your copyright notice in the Description page of Project Settings.


#include "ARDatCharacter.h"
#include "S:/AProj/VkUpDll/VeInterface.h"
#include "UpDllUtils.h"
// Sets default values
AARDatCharacter::AARDatCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// set our turn rates for input
	BaseTurnRate = 1;
	BaseLookUpRate = 1;

	if (!Mat) Mat = LoadObject<UMaterial>(nullptr, TEXT("Material'/ARDatPlayer/M_VIDEO_TEX.M_VIDEO_TEX'"));
}

// Called when the game starts or when spawned
void AARDatCharacter::BeginPlay()
{
	Super::BeginPlay();
	VeDecodedFrame Vdf{};
	dlSendCmd(95000, 0, 0, (wchar_t*)&Vdf);

	if (Mat) {
		DynMaterial = UMaterialInstanceDynamic::Create(Mat, GetTransientPackage() );
		
	}
	if (ScrMesh) ScrMesh->SetMaterial(0, DynMaterial);
}

// Called every frame
void AARDatCharacter::Tick(float DeltaTime)
{
 
	Super::Tick(DeltaTime);
	frameCount++;	
	VeDecodedFrame Vdf{};
 	int ret=dlSendCmd(95001, 0, 0, (wchar_t*)&Vdf,true);

	if (ret == 1 || ret == 2)
	{
		matf = *(FMatrix44f*)Vdf.mCam;
#if 1

		//FVector pos(Vdf.x, Vdf.y, Vdf.z);
		//this->SetActorLocation(pos);
#if 1
		//FRotator rt(Vdf.rx, Vdf.ry, Vdf.rz );
		//FQuat qt = rt.Quaternion();
		//this->SetActorRotation(qt);
	

#else
		FVector rt(Vdf.rx + 90, -Vdf.ry, Vdf.rz + 90);
		this->SetActorRotation(FRotator(rt.X, rt.Z, rt.Y).Quaternion());
#endif
#else
#if 0
		this->SetActorLocation(FVector(Vdf.y * Rm.X+Ra.X, Vdf.z * Rm.Y+Ra.Y, Vdf.x * Rm.Z+Ra.Z));

		FVector rt(Vdf.rx+90, -Vdf.ry, Vdf.rz+90);
#else
		this->SetActorLocation(FVector(Vdf.y , Vdf.z , Vdf.x ));

		FVector rt(Vdf.rx * Rm.X + Ra.X, Vdf.ry * Rm.Y +Ra.Y, Vdf.rz * Rm.Z + Ra.Z);
#endif
		this->SetActorRotation(FRotator(rt.Y, rt.Z, rt.X).Quaternion());
		
#endif

		if (ret == 1)
		{


			if (SizeX != (uint32_t)Vdf.w || SizeY != (uint32_t)Vdf.h)
			{
				SizeX = FMath::Clamp(Vdf.w, 256, 10240); SizeY = FMath::Clamp(Vdf.h, 256, 10240);
				Texture = UTexture2D::CreateTransient(SizeX, SizeY);
				DynMaterial->SetTextureParameterValue(FName(TEXT("Texture0")), Texture);
			}

			if (Texture)
			{
				FTexture2DMipMap& Mip = Texture->PlatformData->Mips[0];
				uint32* Data = (uint32*)Mip.BulkData.Lock(LOCK_READ_WRITE);
				//FMemory::Memcpy(Data, NewData, DataSize);

				//for (uint32 y = 0; y < SizeY; y++)for (uint32 x = 0; x < SizeX; x++)		Data[y * SizeX + x] = (y + x+ frameCount) | 0xFF000000;


				FMemory::Memcpy(Data, Vdf.pb, std::min((uint32)Vdf.size, (uint32)SizeY * SizeX * 4));
				Mip.BulkData.Unlock();
				Texture->UpdateResource();

			}
		}

		dlSendCmd(95002, 0, 0, (wchar_t*)&Vdf, true);
	}
	FTransform tf, tfr;
	tf.SetFromMatrix(FMatrix44d(matf));
	//tfr.SetRotation(FRotator(Ra.X, Ra.Y, Ra.Z).Quaternion().Inverse());
	//tf = tfr * tf;
 
	this->SetActorTransform(tf);
}

// Called to bind functionality to input
void AARDatCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	//PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AARDatCharacter::OnFire);

	// Enable touchscreen input
	//EnableTouchscreenMovement(PlayerInputComponent);

	//PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AARDatCharacter::OnResetVR);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AARDatCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AARDatCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AARDatCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AARDatCharacter::LookUpAtRate);
}


void AARDatCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AARDatCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AARDatCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AARDatCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}
