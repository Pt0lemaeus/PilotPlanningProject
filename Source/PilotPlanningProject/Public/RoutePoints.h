// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <list>
#include "RoutePoints.generated.h"

enum class POINTTYPE;
struct RoutePoint
{
	double lat;
	double lon;
	float high;
	float radius;
	POINTTYPE type;
};

UCLASS()
class PILOTPLANNINGPROJECT_API ARoutePoints : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoutePoints();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	std::list<RoutePoint> rpList;

};
