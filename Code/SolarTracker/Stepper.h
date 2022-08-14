#pragma once

void SetupStepper();

void DeactivateStepperLR();
void DeactivateStepperUD();
void DeactivateStepper();
void ReactivateStepper();

void ExecuteStep_lr(int direction);
void ExecuteStep_ud(int direction);
void MoveBothSteppers(int lr, int ud);

void SetStepperTiming(int speed);

#define DIR_LEFT 1
#define DIR_RIGHT -1
#define DIR_UP 1
#define DIR_DOWN -1

void InitializeStepperPosition();