#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

struct PhysVehicle3D;

#define MAX_ACCELERATION 1000.0f
#define TURN_DEGREES 15.0f * DEGTORAD
#define BRAKE_POWER 1000.0f

struct VehicleProperties {
	float turn;
	float acceleration;
	float brake;
};


class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	//Vehicle info
	VehicleInfo DefineVehickleInfo();

	//Update cars
	void UpdateCar01();
	void UpdateCar02();

public:

	PhysVehicle3D* car01;
	PhysVehicle3D* car02;
	VehicleProperties car01Prop;
	VehicleProperties car02Prop;
	
};