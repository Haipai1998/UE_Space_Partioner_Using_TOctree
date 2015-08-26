// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GenericOctree.h"

#include "GameFramework/Actor.h"
#include "SpacePartioner.generated.h"

USTRUCT()
struct FOctreeElement
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Octree Element Struct")
	AActor* MyActor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Octree Element Struct")
	FBoxSphereBounds BoxSphereBounds;

	FOctreeElement()
	{
		MyActor = nullptr;
		BoxSphereBounds = FBoxSphereBounds(FVector(0.0f, 0.0f, 0.0f), FVector(1.0f, 1.0f, 1.0f), 2.0f);
	}
};

struct FOctreeSematics
{
	enum { MaxElementsPerLeaf = 16 };
	enum { MinInclusiveElementsPerNode = 7 };
	enum { MaxNodeDepth = 12 };

	typedef TInlineAllocator<MaxElementsPerLeaf> ElementAllocator;

	/**
	* Get the bounding box of the provided octree element. In this case, the box
	* is merely the point specified by the element.
	*
	* @param	Element	Octree element to get the bounding box for
	*
	* @return	Bounding box of the provided octree element
	*/
	FORCEINLINE static FBoxSphereBounds GetBoundingBox(const FOctreeElement& Element)
	{
		return Element.BoxSphereBounds;
	}

	FORCEINLINE static bool AreElementsEqual(const FOctreeElement& A, const FOctreeElement& B)
	{
		return A.MyActor == B.MyActor;
	}
	
	FORCEINLINE static void SetElementId(const FOctreeElement& Element, FOctreeElementId Id)
	{
	}

	FORCEINLINE static void ApplyOffset(FOctreeElement& Element, FVector Offset)
	{
		FVector NewPostion = Element.MyActor->GetActorLocation() + Offset;
		Element.MyActor->SetActorLocation(NewPostion);
		Element.BoxSphereBounds.Origin = NewPostion;
	}

};

typedef TOctree<FOctreeElement, FOctreeSematics> FSimpleOctree;

UCLASS()
class OCTREETESTSTATION_API ASpacePartioner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpacePartioner(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void Initialize(const FBox& NewBounds);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	/**
	* Adds a spefic element to the Octree
	* @param NewOctreeElement	FOctreeElement to be added.
	*/
	UFUNCTION(BlueprintCallable, Category = Octree)
	void AddOctreeElement(const FOctreeElement& NewOctreeElement);

	void ApplyWorldOffset(const FVector& InOffset);


private:

	FSimpleOctree* OctreeData;
	bool bInitialized;
	FBox Bounds;
	
};
