// Fill out your copyright notice in the Description page of Project Settings.


#include "WayPointsList.h"

WayPointsList::WayPointsList()
{
	//暂时写成这样，测试方便,注意转弯半径暂时不能相等,而且下一个半径要大于上一个
	
	wpList.push_back({ {120.24932178538717, 30.337186349935937,  3000.0}, 0,  POINTTYPE::STARTPOINT });
	wpList.push_back({ {120.67639249723811, 30.50210199062304,   3000.0}, 250000.0,  POINTTYPE::CENTERPOINT });//5
	wpList.push_back({ {121.90136804209884, 30.11085031775529,   3000.0}, 260000.0,  POINTTYPE::CENTERPOINT });//4

	wpList.push_back({ {121.86566247688988, 31.27519612572878,   3000.0}, 270000.0,  POINTTYPE::CENTERPOINT });//3

	wpList.push_back({ {120.47314543374102, 31.249370545932432,  3000.0}, 280000.0,  POINTTYPE::CENTERPOINT });//2
	wpList.push_back({ {120.52234206687034, 30.6219323960438,    3000.0}, 290000.0,  POINTTYPE::CENTERPOINT });//1

	
	wpList.push_back({ {120.24941120919881, 30.33706082649912,   3000.0}, 0,  POINTTYPE::ENDPOINT });
	
}

WayPointsList::~WayPointsList()
{
}
