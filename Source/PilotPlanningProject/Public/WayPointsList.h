// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <vector>
#include <glm/vec3.hpp>

//using namespace std;

enum class POINTTYPE
{
	NONE,
	STARTPOINT,
	CENTERPOINT,
	HEADPOINT,
	REARPOINT,
	ENDPOINT,
	ENTRYPOINT,
	EXITPOINT
};

enum class TURNTYPE
{
	NONE,
	TURNLEFT,
    TURNRIGHT
};

struct WayPoint
{
	glm::dvec3 lonlathigh;
	float radius;
	POINTTYPE pType;
	TURNTYPE uType;
	glm::dvec3 radCenter;
	glm::dvec3 radEntryPoint;
	glm::dvec3 radExitPoint;
};

/**
 * 
 */
class PILOTPLANNINGPROJECT_API WayPointsList
{
public:
	WayPointsList();
	~WayPointsList();

	std::vector<WayPoint> wpList;

};
