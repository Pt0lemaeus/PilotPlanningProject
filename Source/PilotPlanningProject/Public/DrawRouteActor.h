// Copyright 2023 DaduInteractive, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <vector>
#include <glm/vec3.hpp>
#include <glm/gtc/constants.hpp >
#include "CesiumGeoreference.h"
#include "CesiumGeospatial/Cartographic.h"
#include "Components/SplineComponent.h"
#include "CesiumGeospatial/Ellipsoid.h"
#include "WayPointsList.h"
#include "DrawDebugHelpers.h"
#include "SDrawRouteCompoundWidget.h"
#include "DrawRouteActor.generated.h"

struct WayPoint;
enum class TURNTYPE;
//class WayPointsList;

UCLASS()
class PILOTPLANNINGPROJECT_API ADrawRouteActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADrawRouteActor();

	// Spline variable to represent the plane track
	UPROPERTY(BlueprintReadOnly, Category = "WayPoint")
		USplineComponent* SplineTrack;
	UFUNCTION(BlueprintCallable, Category = "WayPoint")
		void DoDataThings();
	// Function to parse the data table and create the spline track
	UFUNCTION(BlueprintCallable, Category = "WayPoint")
		void LoadSplineRoutePoints();
	UFUNCTION(BlueprintCallable, Category = "WayPoint")
		void DoEntryExitPointCompute();
	UFUNCTION(BlueprintCallable, Category = "WayPoint")
		void ShowEntryExitPoint();
	//UPROPERTY(BlueprintReadOnly, Category = "WayPoint")
	WayPointsList it0;
	std::vector<WayPoint>* wayPointsList;
	std::vector<glm::dvec3> testCirclePath;
	UFUNCTION(BlueprintCallable, Category = "WayPoint")
	void ShowTestPoint();
	SDrawRouteCompoundWidget MyWidgetPtr;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, Category = "WayPoint")
	ACesiumGeoreference* CesiumGeoreference;

private:
    int PointToCenterPoint(std::vector<WayPoint>::iterator it);
	int HeadPointToCenterPoint(std::vector<WayPoint>::iterator it);
	int RearPointToCenterPoint(std::vector<WayPoint>::iterator it);
	int HeadPointToHeadPoint(std::vector<WayPoint>::iterator it);
	int HeadPointToRearPoint(std::vector<WayPoint>::iterator it);
	int CenterPointToHeadPoint(std::vector<WayPoint>::iterator it);
	int CenterPointToCenterPoint(std::vector<WayPoint>::iterator it);
	int CenterPointToRearPoint(std::vector<WayPoint>::iterator it);
	int RearPointToHeadPoint(std::vector<WayPoint>::iterator it);
	int RearPointToRearPoint(std::vector<WayPoint>::iterator it);

protected:
    TURNTYPE ComputeTurnType(glm::dvec3&, glm::dvec3&, glm::dvec3&, glm::dvec3&);
public:
	int CenterPointToPoint(std::vector<WayPoint>::iterator it);
};
