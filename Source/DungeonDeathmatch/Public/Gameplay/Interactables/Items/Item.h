// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "InteractableInterface.h"
#include "InventoryGlobals.h"
#include "Item.generated.h"

class ADungeonCharacter;
class UImage;

/**
 * Enum representation of the different quality levels for items. Higher quality items are generally more rare and valuable.
 * Used for UI and post process elements.
 */
UENUM(BlueprintType) enum class EItemQualityTier : uint8 {
	Normal		UMETA(DisplayName = "Normal"),
	Uncommon	UMETA(DisplayName = "Uncommon"),
	Rare		UMETA(DisplayName = "Rare"),
	Epic		UMETA(DisplayName = "Epic"),
	Legendary	UMETA(DisplayName = "Legendary")
};

/**
 * Base class for all items in the game.
 */
UCLASS()
class DUNGEONDEATHMATCH_API AItem : public AActor, public IInteractableInterface
{
	GENERATED_BODY()

protected:
	/* The root mesh component for this item that all physics simulation operations are performed on */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* RootMeshComponent;

	/* Widget used to display tooltips on interact focus. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UWidgetComponent* WidgetComponent;

	/* The name of this item. Used by UI classes. */
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	FText ItemName;

	/* The amount of slots this item occupies in an inventory grid. */
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	FInventoryGridPair GridSize;

	/* The quality of this item. Higher quality items are generally more rare and valuable.*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	EItemQualityTier QualityTier;

	/* Optional descriptive text about the item */
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	FText FlavorText;

	/* Icon texture for this item to represent it in the UI */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	UTexture2D* Icon;

	/* Sound to play when this item is added or removed from the player's inventory */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	USoundCue* InteractionSound;

	/* The color corresponding to the quality of the item that is used for UI and post process elements */
	FLinearColor QualityTierColor;

	/* The color corresponding to the quality of the item that is used for UI Text */
	FLinearColor QualityTierTextColor;

	/* The stencil value to use when rendering the post process outline for this item, based on its quality */
	uint8 QualityTierStencilValue;

	/* Flag for toggling ability to interact. Replicated to all clients. */
	UPROPERTY(Replicated, EditAnywhere, Category = "Interaction")
	bool bCanInteract;

	/* The prompt text that should appear on screen when this item is focused by the player */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
	FText InteractionPromptText;

	/* The text to display for the use prompt on the item tooltip. Ex. "Use", "Equip", etc. */
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	FText InventoryUseTooltipText;

public:	
	// Sets default values for this actor's properties
	AItem(const FObjectInitializer& ObjectInitializer);

	virtual ~AItem();

protected:
	virtual void PreInitializeComponents() override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	/**
	 * Gets the root mesh component for the item, item subclasses may have additional meshes, but all root physics simulation operations should be applied to this mesh.
	 */
	UStaticMeshComponent* GetRootMeshComponent();

	/**
	 * Gets the display name of the item; different from the name of the instanced object
	 */
	UFUNCTION(BlueprintPure, Category = "Item")
	FText GetItemName();

	/**
	 * Gets the grid size of the item
	 */
	UFUNCTION(BlueprintPure, Category = "Item")
	FInventoryGridPair GetGridSize();

	/**
	 * Gets the grid size vector of the item, used for setting the size of items in the UI
	 */
	UFUNCTION(BlueprintPure, Category = "Item")
	FVector2D GetGridSizeVector();

	/**
	 * Gets the optional descriptive text for the item, if any
	 */
	UFUNCTION(BlueprintPure, Category = "Item")
	FText GetFlavorText();

	/**
	 * Gets the icon texture for the item
	 */
	UFUNCTION(BlueprintPure, Category = "Item")
	UTexture2D* GetIcon();

	/* Sound to play when this item is added or removed from the player's inventory */
	UFUNCTION(BlueprintPure, Category = "Item")
	USoundCue* GetInteractionSound();

	/**
	 * Gets the quality color of the item; used by UI elements
	 */
	UFUNCTION(BlueprintPure, Category = "Item")
	FLinearColor GetQualityTierColor();

	/**
	 * Gets the quality text color of the item; used by UI elements
	 */
	UFUNCTION(BlueprintPure, Category = "Item")
	FLinearColor GetQualityTierTextColor();

	/**
	 * Gets the tooltip text for the item to use for interaction prompts
	 */
	UFUNCTION(BlueprintPure, Category = "Item")
	virtual FText GetInventoryUseTooltipText();

	virtual void OnInteract_Implementation(ADungeonCharacter* InteractingCharacter) override;
	virtual void OnFocused_Implementation() override;
	virtual void OnUnfocused_Implementation() override;
	virtual bool GetCanInteract_Implementation() override;
	virtual void SetCanInteract_Implementation(bool CanInteract) override;
	virtual FText GetInteractionPromptText_Implementation() override;
	virtual FText GetInteractableName_Implementation() override;

	/** Server side function to set if an interactable can be interacted with */
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetCanInteract(bool CanInteract);

	/** Server side function that "despawns" the item by hiding its mesh(es), disabling physics and collision, and moving it to the world origin */
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerDespawn();

	/** Server side function that "spawns" the item at the specified location; showing all meshes and enabling physics and collision.
	 *  An optional ejection force vector can be used, which may be desired for things like ejecting loot from a chest 
	 */
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSpawnAtLocation(const FVector Location, const FVector EjectionForce = FVector(0, 0, 0));

protected:
	/**
	 * Sets the mesh stencil value based on item type and quality; used for drawing post process item outlines
	 */
	void SetMeshStencilValue();

	/** Multicast function that "despawns" the item by hiding its mesh(es), disabling physics and collision, and moving it to the world origin */
	UFUNCTION(NetMulticast, Reliable)
	void MulticastDespawn();

	/** Multicast function that "spawns" the item at the specified location; showing all meshes and enabling physics and collision.
	 *  An optional ejection force vector can be used, which may be desired for things like ejecting loot from a chest
	 */
	UFUNCTION(NetMulticast, Reliable)
	void MulticastSpawnAtLocation(const FVector Location, const FVector EjectionForce = FVector(0, 0, 0));

};
