#include "InterfaceTestActor.h"

#include "Components/StaticMeshComponent.h"

AInterfaceTestActor::AInterfaceTestActor() {
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	
}

void AInterfaceTestActor::BeginPlay() {
	Super::BeginPlay();
}

void AInterfaceTestActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AInterfaceTestActor::BeginFocus() {
	if(Mesh) {
		Mesh->SetRenderCustomDepth(true);
	}
}

void AInterfaceTestActor::EndFocus() {
	if(Mesh) {
		Mesh->SetRenderCustomDepth(false);
	}
}

void AInterfaceTestActor::BeginInteract() {
	UE_LOG(LogTemp, Warning, TEXT("BeginInteract"));
}

void AInterfaceTestActor::EndInteract() {
	UE_LOG(LogTemp, Warning, TEXT("EndInteract"));
}

void AInterfaceTestActor::Interact(AMegaCharacter* PlayerCharacter) {
	UE_LOG(LogTemp, Warning, TEXT("Interact"));
}
