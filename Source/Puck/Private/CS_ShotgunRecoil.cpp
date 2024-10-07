// Fill out your copyright notice in the Description page of Project Settings.


#include "CS_ShotgunRecoil.h"

UCS_ShotgunRecoil::UCS_ShotgunRecoil()
{
	OscillationDuration = 0.3f;
	OscillationBlendInTime = 0.1f;
	OscillationBlendOutTime = 0.1f;
	
	RotOscillation.Pitch.Amplitude = 5.f;
	RotOscillation.Pitch.Frequency = 15.f;
	RotOscillation.Pitch.InitialOffset = EInitialOscillatorOffset::EOO_OffsetZero;

	RotOscillation.Yaw.Amplitude = FMath::RandRange(0.0, 5.0);
	RotOscillation.Yaw.Frequency = 15.f;
	RotOscillation.Yaw.InitialOffset = EInitialOscillatorOffset::EOO_OffsetZero;
}
