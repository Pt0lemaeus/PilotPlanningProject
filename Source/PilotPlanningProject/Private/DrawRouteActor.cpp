// Copyright 2023 DaduInteractive, Inc. All Rights Reserved.


#include "DrawRouteActor.h"
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "GeoTransforms.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

//class WayPoint;
//int32 PointIndex = 0;

TURNTYPE ADrawRouteActor::ComputeTurnType(glm::dvec3& i0, glm::dvec3& i1, glm::dvec3& i2, glm::dvec3& norm)
{
	glm::dvec3 i01 = i1 - i0;
	glm::dvec3 i12 = i2 - i1;
	norm = glm::normalize(glm::cross(i01, i12));

	glm::dvec3 earthCenter = this->CesiumGeoreference->TransformLongitudeLatitudeHeightToUnreal(static_cast<glm::dvec3>(0,0,0));
	glm::dvec3 up = i1 - earthCenter;

	float alpha = glm::dot(up, norm);
	UE_LOG(
		LogTemp,
		Warning,
		TEXT("the angle of two vectors is %f "),
		alpha);
	if (alpha > 0)
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("TURNRIGHT"))
		return TURNTYPE::TURNRIGHT;
	}

	else if (alpha < 0)
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("TURNLEFT"))
		return TURNTYPE::TURNLEFT;
	}

	else
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("TURNNONE"))
		return TURNTYPE::NONE;
			}

}

// Sets default values
ADrawRouteActor::ADrawRouteActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Initialize the track
	SplineTrack = CreateDefaultSubobject<USplineComponent>(TEXT("SplineRoute"));
	// This lets us visualize the spline in Play mode
	SplineTrack->SetDrawDebug(true);
	// Set the color of the spline
	SplineTrack->SetUnselectedSplineSegmentColor(FLinearColor(1.f, 0.f, 0.f));

	
	
	//wayPointsList = &it0.wpList;

}

// Called when the game starts or when spawned
void ADrawRouteActor::BeginPlay()
{
	Super::BeginPlay();
	GEngine->GameViewport->AddViewportWidgetContent(
		SNew(MyWidgetPtr.ToSharedRef())
	);
}

// Called every frame
void ADrawRouteActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


//优先实现
int ADrawRouteActor::PointToCenterPoint(std::vector<WayPoint>::iterator it)
{
	//求i、i+1、i+2确定的平面法线
	//求向量ii+1和i+1i+2的夹角
	//根据夹角确定的方向求出进入点
	glm::dvec3 I0 = this->CesiumGeoreference->TransformLongitudeLatitudeHeightToUnreal(it->lonlathigh);
	glm::dvec3 I1 = this->CesiumGeoreference->TransformLongitudeLatitudeHeightToUnreal(std::next(it, 1)->lonlathigh);
	UE_LOG(
		LogTemp,
		Warning,
		TEXT("lon: %lf,lat: %lf,h: %lf"),std::next(it, 1)->lonlathigh.x, std::next(it, 2)->lonlathigh.y, std::next(it, 2)->lonlathigh.z)
	glm::dvec3 I2 = this->CesiumGeoreference->TransformLongitudeLatitudeHeightToUnreal(std::next(it, 2)->lonlathigh);
	UE_LOG(
		LogTemp,
		Warning,
		TEXT("lon: %lf,lat: %lf,h: %lf"), std::next(it, 2)->lonlathigh.x, std::next(it, 2)->lonlathigh.y, std::next(it, 2)->lonlathigh.z)
	glm::dvec3 norm;
	//按照转弯半径旋转法逐渐逼近进入点
	glm::dvec3 r = (static_cast<double>(std::next(it, 1)->radius)) * glm::normalize(I0 - I1);

	std::next(it, 1)->uType = ComputeTurnType(I0, I1, I2, norm);
	//if (ComputeTurnType(I0, I1, I2, norm) == TURNTYPE::TURNLEFT)
	//{
	//testCirclePath.push_back(I0);
		for(auto n =0; n<=90; n++)
		{
			glm::dvec3 p = I1 - I0 + r;
			if (glm::dot(r, p) > 0)
				break;
			r = glm::rotate(r, static_cast<double>(glm::radians(1.0f)), norm);
			//test
			//testCirclePath.push_back(I1 + r);
			//test
		}

	std::next(it, 1)->radEntryPoint = I1 + r;

	

	std::next(it, 1)->radCenter = this->CesiumGeoreference->TransformLongitudeLatitudeHeightToUnreal(std::next(it, 1)->lonlathigh);

	if (it->pType == POINTTYPE::STARTPOINT)
	{
		it->radCenter = it->radEntryPoint = it->radExitPoint = this->CesiumGeoreference->TransformLongitudeLatitudeHeightToUnreal(it->lonlathigh);
		it->radius = 0;
	}
	return 0;
}

//绕点到绕点，优先实现，尽快出画全是绕点的功能
int ADrawRouteActor::CenterPointToCenterPoint(std::vector<WayPoint>::iterator it)
{
	glm::dvec3 I0 = this->CesiumGeoreference->TransformLongitudeLatitudeHeightToUnreal(it->lonlathigh);
	glm::dvec3 I1 = this->CesiumGeoreference->TransformLongitudeLatitudeHeightToUnreal(std::next(it, 1)->lonlathigh);
	glm::dvec3 I2 = this->CesiumGeoreference->TransformLongitudeLatitudeHeightToUnreal(std::next(it, 2)->lonlathigh);
	glm::dvec3 norm;

	glm::dvec3 r1Minr0 = (static_cast<double>(std::next(it, 1)->radius - it->radius)) * glm::normalize(I0 - I1);
	glm::dvec3 r1Plusr0 = (static_cast<double>(std::next(it, 1)->radius + it->radius)) * glm::normalize(I0 - I1);
	std::next(it, 1)->uType = ComputeTurnType(I0, I1, I2, norm);
	if (TURNTYPE::NONE == it->uType)
	{
		UE_LOG(
			LogTemp,
			Error,
			TEXT("Turn type is none"))
	}

	if (it->uType == std::next(it, 1)->uType)
	{
		for (auto n = 0; n <= 90; n++)
		{
			glm::dvec3 p = I1 - I0 + r1Minr0;
			if (glm::dot(r1Minr0, p) > 0)
				break;
			r1Minr0 = glm::rotate(r1Minr0, static_cast<double>(glm::radians(1.0f)), norm);

			glm::dvec3 rNC = glm::normalize(r1Minr0);

			//testCirclePath.push_back(I0 + static_cast<double>(it->radius) * rNC);
			//testCirclePath.push_back(I1 + static_cast<double>(std::next(it, 1)->radius) * rNC);
		}
		glm::dvec3 rNC = glm::normalize(r1Minr0);
		it->radExitPoint = I0 + static_cast<double>(it->radius) * rNC;
		std::next(it, 1)->radEntryPoint = I1 + static_cast<double>(std::next(it, 1)->radius) * rNC;
	}
	else
	{
		for (auto n = 0; n <= 90; n++)
		{
			glm::dvec3 p = I1 - I0 + r1Plusr0;
			if (glm::dot(r1Plusr0, p) > 0)
				break;
			r1Plusr0 = glm::rotate(r1Plusr0, static_cast<double>(glm::radians(1.0f)), norm);
			glm::dvec3 rNC = glm::normalize(r1Plusr0);

			//testCirclePath.push_back(I0 + static_cast<double>(it->radius) * rNC);
			//testCirclePath.push_back(I1 + static_cast<double>(std::next(it, 1)->radius) * rNC);
		}
		glm::dvec3 rNC = glm::normalize(r1Plusr0);
		it->radExitPoint = I0 - static_cast<double>(it->radius) * rNC;
		std::next(it, 1)->radEntryPoint = I1 + static_cast<double>(std::next(it, 1)->radius) * rNC;
	}
	it->radCenter = this->CesiumGeoreference->TransformLongitudeLatitudeHeightToUnreal(it->lonlathigh);

	return 0;
}

int ADrawRouteActor::CenterPointToPoint(std::vector<WayPoint>::iterator it)
{
	glm::dvec3 I1 = this->CesiumGeoreference->TransformLongitudeLatitudeHeightToUnreal(it->lonlathigh);
	glm::dvec3 I0 = this->CesiumGeoreference->TransformLongitudeLatitudeHeightToUnreal(std::next(it, 1)->lonlathigh);
	glm::dvec3 I2 = this->CesiumGeoreference->TransformLongitudeLatitudeHeightToUnreal(std::prev(it, 1)->lonlathigh);
	glm::dvec3 norm;
	//按照转弯半径旋转法逐渐逼近进入点
	glm::dvec3 r = (static_cast<double>(it->radius)) * glm::normalize(I0 - I1);

	ComputeTurnType(I0, I1, I2, norm);
	//if (ComputeTurnType(I0, I1, I2, norm) == TURNTYPE::TURNLEFT)
	//{
	for (auto n = 0; n <= 90; n++)
	{
		glm::dvec3 p = I1 - I0 + r;
		if (glm::dot(r, p) > 0)
			break;
		r = glm::rotate(r, static_cast<double>(glm::radians(1.0f)), norm);
		testCirclePath.push_back(I1 + r);
	}

	it->radExitPoint = I1 + r;
	it->radCenter = this->CesiumGeoreference->TransformLongitudeLatitudeHeightToUnreal(it->lonlathigh);

	if (std::next(it,1)->pType == POINTTYPE::ENDPOINT)
	{
		std::next(it, 1)->radCenter = std::next(it, 1)->radEntryPoint = std::next(it, 1)->radExitPoint = this->CesiumGeoreference->TransformLongitudeLatitudeHeightToUnreal(std::next(it, 1)->lonlathigh);
		std::next(it, 1)->radius = 0;
	}
	
	return 0;
}

//优先实现
int ADrawRouteActor::HeadPointToCenterPoint(std::vector<WayPoint>::iterator it)
{
	// TODO: Add your implementation code here.
	return 0;
}

//优先实现
int ADrawRouteActor::RearPointToCenterPoint(std::vector<WayPoint>::iterator it)
{
	// TODO: Add your implementation code here.
	return 0;
}


int ADrawRouteActor::HeadPointToHeadPoint(std::vector<WayPoint>::iterator it)
{
	// TODO: Add your implementation code here.
	return 0;
}


int ADrawRouteActor::HeadPointToRearPoint(std::vector<WayPoint>::iterator it)
{
	// TODO: Add your implementation code here.
	return 0;
}


int ADrawRouteActor::CenterPointToHeadPoint(std::vector<WayPoint>::iterator it)
{
	// TODO: Add your implementation code here.
	return 0;
}




int ADrawRouteActor::CenterPointToRearPoint(std::vector<WayPoint>::iterator it)
{
	// TODO: Add your implementation code here.
	return 0;
}


int ADrawRouteActor::RearPointToHeadPoint(std::vector<WayPoint>::iterator it)
{
	// TODO: Add your implementation code here.
	return 0;
}


int ADrawRouteActor::RearPointToRearPoint(std::vector<WayPoint>::iterator it)
{
	// TODO: Add your implementation code here.
	return 0;
}

void ADrawRouteActor::DoDataThings()
{
	//WayPointsList it0;
	//wayPointsList = &it0.wpList;
}

void ADrawRouteActor::LoadSplineRoutePoints()
{
	if (/*wayPointsList != nullptr &&*/ this->CesiumGeoreference != nullptr)
	{
		int32 PointIndex = 0;
		for (auto &row : it0.wpList)
		//for (auto& row = wayPointsList->begin(); row != wayPointsList->end(); row++)
		{
		    // Get row data point in lat/long/alt and transform it into UE4 points
			double PointLatitude = row.lonlathigh.y;
			double PointLongitude = row.lonlathigh.x;
			double PointHeight = row.lonlathigh.z;

			// Compute the position in UE coordinates
			glm::dvec3 UECoords = this->CesiumGeoreference->TransformLongitudeLatitudeHeightToUnreal(glm::dvec3(PointLongitude, PointLatitude, PointHeight));
			FVector SplinePointPosition = FVector(UECoords.x, UECoords.y, UECoords.z);
			this->SplineTrack->AddSplinePointAtIndex(SplinePointPosition, PointIndex, ESplineCoordinateSpace::World, false);

			// Get the up vector at the position to orient the aircraft
			const CesiumGeospatial::Ellipsoid& Ellipsoid = CesiumGeospatial::Ellipsoid::WGS84;
			glm::dvec3 upVector = Ellipsoid.geodeticSurfaceNormal(CesiumGeospatial::Cartographic(FMath::DegreesToRadians(PointLongitude), FMath::DegreesToRadians(PointLatitude), FMath::DegreesToRadians(PointHeight)));

			// Compute the up vector at each point to correctly orient the plane
			glm::dvec4 ecefUp(upVector, 0.0);
			const GeoTransforms& geoTransforms = this->CesiumGeoreference->GetGeoTransforms();
			const glm::dmat4& ecefToUnreal = geoTransforms.GetEllipsoidCenteredToAbsoluteUnrealWorldTransform();
			glm::dvec4 unrealUp = ecefToUnreal * ecefUp;
			this->SplineTrack->SetUpVectorAtSplinePoint(PointIndex, FVector(unrealUp.x, unrealUp.y, unrealUp.z), ESplineCoordinateSpace::World, false);

			PointIndex++;
		}
		this->SplineTrack->UpdateSpline();
	}
}

void ADrawRouteActor::DoEntryExitPointCompute()
{
	for (auto i = it0.wpList.begin(); i < it0.wpList.end(); i++ )
	{
		switch (i->pType)
		{
			case POINTTYPE::STARTPOINT:
			{
				switch ((std::next(i, 1))->pType)
				{
					case POINTTYPE::CENTERPOINT:
					{
						PointToCenterPoint(i);
						break;
					}
					default:break;
				}
				break;
			}
			case POINTTYPE::CENTERPOINT:
			{
				switch ((std::next(i, 1))->pType)
				{
					case POINTTYPE::CENTERPOINT:
					{
						CenterPointToCenterPoint(i);
						break;
					}
					case POINTTYPE::ENDPOINT:
					{
						CenterPointToPoint(i);
						break;
					}
					default:break;
				}
				break;
			}
			default:break;
		}



		
	}
}

void ADrawRouteActor::ShowEntryExitPoint()
{
	std::vector<glm::dvec3> pList;

	for (auto i : it0.wpList)
	{
		if (i.pType == POINTTYPE::STARTPOINT || i.pType == POINTTYPE::ENDPOINT)
		{
			pList.push_back(i.radEntryPoint);
		}
		else
		{
			pList.push_back(i.radEntryPoint);
			pList.push_back(i.radExitPoint);
		}
		
		//pList.push_back(i.radCenter);
		
	}
	if (/*wayPointsList != nullptr &&*/ this->CesiumGeoreference != nullptr)
	{
		int32 PointIndex = 0;
		for (auto& row : pList)
			//for (auto& row = wayPointsList->begin(); row != wayPointsList->end(); row++)
		{
			// Get row data point in lat/long/alt and transform it into UE4 points
			//double PointLatitude = row.y;
			//double PointLongitude = row.x;
			//double PointHeight = row.z;

			// Compute the position in UE coordinates
			//glm::dvec3 UECoords = this->CesiumGeoreference->TransformLongitudeLatitudeHeightToUnreal(glm::dvec3(PointLongitude, PointLatitude, PointHeight));
			FVector SplinePointPosition = FVector(row.x, row.y, row.z);
			this->SplineTrack->AddSplinePointAtIndex(SplinePointPosition, PointIndex, ESplineCoordinateSpace::World, false);
			
			// Get the up vector at the position to orient the aircraft
			const CesiumGeospatial::Ellipsoid& Ellipsoid = CesiumGeospatial::Ellipsoid::WGS84;
			//glm::dvec3 upVector = Ellipsoid.geodeticSurfaceNormal(CesiumGeospatial::Cartographic(FMath::DegreesToRadians(PointLongitude), FMath::DegreesToRadians(PointLatitude), FMath::DegreesToRadians(PointHeight)));

			// Compute the up vector at each point to correctly orient the plane
			//glm::dvec4 ecefUp(upVector, 0.0);
			const GeoTransforms& geoTransforms = this->CesiumGeoreference->GetGeoTransforms();
			const glm::dmat4& ecefToUnreal = geoTransforms.GetEllipsoidCenteredToAbsoluteUnrealWorldTransform();
			//glm::dvec4 unrealUp = ecefToUnreal * ecefUp;
			//this->SplineTrack->SetUpVectorAtSplinePoint(PointIndex, FVector(unrealUp.x, unrealUp.y, unrealUp.z), ESplineCoordinateSpace::World, false);

			PointIndex++;
		}
		this->SplineTrack->UpdateSpline();
	}
}

void ADrawRouteActor::ShowTestPoint()
{

	if (/*wayPointsList != nullptr &&*/ this->CesiumGeoreference != nullptr)
	{
		int32 PointIndex = 0;
		for (auto& row : testCirclePath)
		{

			FVector SplinePointPosition = FVector(row.x, row.y, row.z);
			this->SplineTrack->AddSplinePointAtIndex(SplinePointPosition, PointIndex, ESplineCoordinateSpace::World, false);

			// Get the up vector at the position to orient the aircraft
			const CesiumGeospatial::Ellipsoid& Ellipsoid = CesiumGeospatial::Ellipsoid::WGS84;
			//glm::dvec3 upVector = Ellipsoid.geodeticSurfaceNormal(CesiumGeospatial::Cartographic(FMath::DegreesToRadians(PointLongitude), FMath::DegreesToRadians(PointLatitude), FMath::DegreesToRadians(PointHeight)));

			// Compute the up vector at each point to correctly orient the plane
			//glm::dvec4 ecefUp(upVector, 0.0);
			const GeoTransforms& geoTransforms = this->CesiumGeoreference->GetGeoTransforms();
			const glm::dmat4& ecefToUnreal = geoTransforms.GetEllipsoidCenteredToAbsoluteUnrealWorldTransform();

			PointIndex++;
		}
		this->SplineTrack->UpdateSpline();
	}
}
