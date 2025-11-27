#include "timer.h"
#include "raylib.h"
#include <sstream>
#include <iomanip>
#include <cmath>

using namespace std;

// Variáveis internas do módulo
static double g_timerAccum = 0.0;    // tempo acumulado quando parado
static double g_timerStart = 0.0;    // GetTime() quando iniciou/retomou
static bool   g_timerRunning = false; // se o timer está rodando

void StartTimer()
{
    if (!g_timerRunning) {
        g_timerStart = GetTime();
        g_timerRunning = true;
    }
}

void StopTimer()
{
    if (g_timerRunning) {
        g_timerAccum += (GetTime() - g_timerStart);
        g_timerRunning = false;
    }
}

void ResetTimer()
{
    g_timerAccum = 0.0;
    g_timerStart = 0.0;
    g_timerRunning = false;
}

double GetElapsedTime()
{
    if (g_timerRunning) return g_timerAccum + (GetTime() - g_timerStart);
    return g_timerAccum;
}

string FormatTime(double seconds)
{
    if (seconds < 0.0) seconds = 0.0;
    int totalMs = (int)std::round(seconds * 1000.0);
    int ms = totalMs % 1000;
    int totalSec = totalMs / 1000;
    int sec = totalSec % 60;
    int min = totalSec / 60;
    ostringstream ss;
    ss << setfill('0') << setw(2) << min << ":" << setw(2) << sec << "." << setw(3) << ms;
    return ss.str();
}

bool IsTimerRunning()
{
    return g_timerRunning;
}