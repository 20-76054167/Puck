// Fill out your copyright notice in the Description page of Project Settings.


#include "CS_RifleRecoil.h"

UCS_RifleRecoil::UCS_RifleRecoil()
{
	OscillationDuration = 0.2f;
	OscillationBlendInTime = 0.1f;
	OscillationBlendOutTime = 0.2f;
	
	RotOscillation.Pitch.Amplitude = 2.f;
	RotOscillation.Pitch.Frequency = 2.f;
	RotOscillation.Pitch.InitialOffset = EInitialOscillatorOffset::EOO_OffsetRandom;

	RotOscillation.Yaw.Amplitude = 2.f;
	RotOscillation.Yaw.Frequency = 2.f;
	RotOscillation.Yaw.InitialOffset = EInitialOscillatorOffset::EOO_OffsetRandom;
	
	LocOscillation.Y.Amplitude = 15.f;
	LocOscillation.Y.Frequency = 1.f;
	LocOscillation.Y.InitialOffset = EInitialOscillatorOffset::EOO_OffsetZero;

	LocOscillation.Z.Amplitude = 5.f;
	LocOscillation.Z.Frequency = 15.f;
	LocOscillation.Z.InitialOffset = EInitialOscillatorOffset::EOO_OffsetZero;
}
