// Fill out your copyright notice in the Description page of Project Settings.

/*
 * Much of this code is derived from Epic's Action RPG example project that utilizes the
 * Gameplay Abilities system.
 */

#pragma once

#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "DungeonAttributeSet.generated.h"

// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
		GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/** This class holds all of the attributes used by abilities. An instance of this should be instantiated on every character. */
UCLASS()
class DUNGEONDEATHMATCH_API UDungeonAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	/** Current Health, when 0 we expect owner to die. Capped by MaxHealth */
	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UDungeonAttributeSet, Health)

	/** MaxHealth is its own attribute, since GameplayEffects may modify it */
	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UDungeonAttributeSet, MaxHealth)

	/** The amount of health regenerated per second */
	UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_HealthRegen)
	FGameplayAttributeData HealthRegen;
	ATTRIBUTE_ACCESSORS(UDungeonAttributeSet, HealthRegen)

	/** Current Mana, used to execute special abilities. Capped by MaxMana */
	UPROPERTY(BlueprintReadOnly, Category = "Mana", ReplicatedUsing = OnRep_Mana)
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UDungeonAttributeSet, Mana)

	/** MaxMana is its own attribute, since GameplayEffects may modify it */
	UPROPERTY(BlueprintReadOnly, Category = "Mana", ReplicatedUsing = OnRep_MaxMana)
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UDungeonAttributeSet, MaxMana)

	/** The amount of mana regenerated per second */
	UPROPERTY(BlueprintReadOnly, Category = "Mana", ReplicatedUsing = OnRep_ManaRegen)
	FGameplayAttributeData ManaRegen;
	ATTRIBUTE_ACCESSORS(UDungeonAttributeSet, ManaRegen)

	/** Current Stamina, used to execute special abilities. Capped by MaxStamina */
	UPROPERTY(BlueprintReadOnly, Category = "Stamina", ReplicatedUsing = OnRep_Stamina)
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UDungeonAttributeSet, Stamina)

	/** MaxStamina is its own attribute, since GameplayEffects may modify it */
	UPROPERTY(BlueprintReadOnly, Category = "Stamina", ReplicatedUsing = OnRep_MaxStamina)
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UDungeonAttributeSet, MaxStamina)

	/** The amount of stamina regenerated per second */
	UPROPERTY(BlueprintReadOnly, Category = "Stamina", ReplicatedUsing = OnRep_StaminaRegen)
	FGameplayAttributeData StaminaRegen;
	ATTRIBUTE_ACCESSORS(UDungeonAttributeSet, StaminaRegen)

	/** Strength determines a player's base health and stamina, and augments strength weapons and skills. */
	UPROPERTY(BlueprintReadOnly, Category = "Strength", ReplicatedUsing = OnRep_Strength)
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UDungeonAttributeSet, Strength)

	/** Finesse determines a player's speed, and augments finesse weapons and skills. */
	UPROPERTY(BlueprintReadOnly, Category = "Finnese", ReplicatedUsing = OnRep_Finesse)
	FGameplayAttributeData Finnese;
	ATTRIBUTE_ACCESSORS(UDungeonAttributeSet, Finnese)

	/** Spirit determines a player's base mana, and augments spirit weapons and skills. */
	UPROPERTY(BlueprintReadOnly, Category = "Spirit", ReplicatedUsing = OnRep_Spirit)
	FGameplayAttributeData Spirit;
	ATTRIBUTE_ACCESSORS(UDungeonAttributeSet, Spirit)

	/** Base defense power used in mitigating damage, increased by armor and skills */
	UPROPERTY(BlueprintReadOnly, Category = "Damage", ReplicatedUsing = OnRep_DefensePower)
	FGameplayAttributeData DefensePower;
	ATTRIBUTE_ACCESSORS(UDungeonAttributeSet, DefensePower)

	/** MovementSpeed affects how fast characters can move */
	UPROPERTY(BlueprintReadOnly, Category = "MovementSpeed", ReplicatedUsing = OnRep_MovementSpeed)
	FGameplayAttributeData MovementSpeed;
	ATTRIBUTE_ACCESSORS(UDungeonAttributeSet, MovementSpeed)

	/** MovementSpeedMultiplier is used to increase movement speed by a multiple of itself for actions like sprniting and rolling */
	UPROPERTY(BlueprintReadOnly, Category = "MovementSpeed", ReplicatedUsing = OnRep_MovementSpeedMultiplier)
	FGameplayAttributeData MovementSpeedMultiplier;
	ATTRIBUTE_ACCESSORS(UDungeonAttributeSet, MovementSpeedMultiplier)

	/** The weight of all items currently equipped or stored in inventory */
	UPROPERTY(BlueprintReadOnly, Category = "Weight", ReplicatedUsing = OnRep_CarryingWeight)
	FGameplayAttributeData CarryingWeight;
	ATTRIBUTE_ACCESSORS(UDungeonAttributeSet, CarryingWeight)
	
	/** The maximum amount of weight that can be carried */
	UPROPERTY(BlueprintReadOnly, Category = "Weight", ReplicatedUsing = OnRep_MaxCarryingWeight)
	FGameplayAttributeData MaxCarryingWeight;
	ATTRIBUTE_ACCESSORS(UDungeonAttributeSet, MaxCarryingWeight)

	/** Damage is a 'temporary' attribute used by the DamageExecution to calculate final damage, which then turns into -Health */
	UPROPERTY(BlueprintReadOnly, Category = "Damage", meta = (HideFromLevelInfos))
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UDungeonAttributeSet, Damage)

public:
	// Constructor and overrides
	UDungeonAttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

protected:
	/** Helper function to proportionally adjust the value of an attribute when it's associated max attribute changes. (i.e. When MaxHealth increases, Health increases by an amount that maintains the same percentage as before) */
	void AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty);

	// These OnRep functions exist to make sure that the ability system internal representations are synchronized properly during replication
	UFUNCTION()
	virtual void OnRep_Health();

	UFUNCTION()
	virtual void OnRep_MaxHealth();

	UFUNCTION()
	virtual void OnRep_HealthRegen();

	UFUNCTION()
	virtual void OnRep_Mana();

	UFUNCTION()
	virtual void OnRep_MaxMana();

	UFUNCTION()
	virtual void OnRep_ManaRegen();

	UFUNCTION()
	virtual void OnRep_Stamina();

	UFUNCTION()
	virtual void OnRep_MaxStamina();

	UFUNCTION()
	virtual void OnRep_StaminaRegen();

	UFUNCTION()
	virtual void OnRep_Strength();

	UFUNCTION()
	virtual void OnRep_Finesse();

	UFUNCTION()
	virtual void OnRep_Spirit();

	UFUNCTION()
	virtual void OnRep_DefensePower();

	UFUNCTION()
	virtual void OnRep_MovementSpeed();

	UFUNCTION()
	virtual void OnRep_MovementSpeedMultiplier();

	UFUNCTION()
	virtual void OnRep_CarryingWeight();

	UFUNCTION()
	virtual void OnRep_MaxCarryingWeight();
};
