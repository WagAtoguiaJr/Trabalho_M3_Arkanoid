#pragma once

#include <string>

using namespace std;


void StartTimer();

void StopTimer();

void ResetTimer();

double GetElapsedTime();

string FormatTime(double seconds);

bool IsTimerRunning();