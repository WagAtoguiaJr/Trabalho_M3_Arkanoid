#pragma once

#include <string>

using namespace std;

// Inicia ou retoma o timer (se já estiver rodando, não faz nada)
void StartTimer();

// Para o timer e acumula o tempo decorrido
void StopTimer();

// Reseta o timer para zero e deixa parado
void ResetTimer();

// Retorna o tempo total acumulado em segundos (inclui tempo corrente se estiver rodando)
double GetElapsedTime();

// Retorna string formatada MM:SS.mmm para exibição
string FormatTime(double seconds);

// Retorna se o timer está rodando
bool IsTimerRunning();