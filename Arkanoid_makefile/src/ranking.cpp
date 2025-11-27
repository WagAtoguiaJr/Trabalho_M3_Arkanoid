#include "ranking.h"
#include "raylib.h"
#include "timer.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>

using namespace std;


// Formato CSV: date;name;score;difficulty
void LoadRankingCSV(const char* filename, vector<RankEntry> &outList)
{
    outList.clear();
    ifstream in(filename);
    if (!in.is_open()) return;
    string line;
    while (getline(in, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string date, name, scoreStr, timeStr, diffStr;
        if (getline(ss, date, ';') &&
            getline(ss, name, ';') &&
            getline(ss, scoreStr, ';') &&
            getline(ss, timeStr, ';') &&
            getline(ss, diffStr))
        {
            RankEntry e;
            e.date = date;
            e.name = name;
            e.score = stoi(scoreStr);
            e.timeSeconds = stod(timeStr);
            e.difficulty = stoi(diffStr);
            outList.push_back(e);
        }
    }
    in.close();
}

void AppendRankingCSV(const char* filename, const RankEntry &entry)
{
    ofstream out(filename, ios::app);
    if (!out.is_open()) {
        // tenta criar o arquivo
        out.open(filename, ios::out);
        if (!out.is_open()) return;
    }
    out << entry.date << ";" << entry.name << ";" << entry.score << ";" << entry.timeSeconds << ";" << entry.difficulty << "\n";
    out.close();
}

// comparator: maior score primeiro, se empate maior difficulty primeiro, se empate data mais antiga primeiro
static bool RankComparator(const RankEntry &a, const RankEntry &b)
{
    if (a.score != b.score) return a.score > b.score;
    if (a.difficulty != b.difficulty) return a.difficulty > b.difficulty;
    if (a.timeSeconds != b.timeSeconds) return a.timeSeconds < b.timeSeconds;
    // data mais antiga primeiro -> string YYYY-MM-DD, lexicographic funciona
    return a.date < b.date;
}


void DrawRankingScreen()
{
    vector<RankEntry> list;
    LoadRankingCSV("rank/ranking.csv", list);
    sort(list.begin(), list.end(), RankComparator);

    BeginDrawing();
    ClearBackground(BLACK);
    DrawText("RANKING - TOP 10", GetScreenWidth()/2 - MeasureText("RANKING - TOP 10", 30)/2, 20, 30, RAYWHITE);

    int y = 80;
    int maxShow = min((int)list.size(), 10);
    for (int i = 0; i < maxShow; ++i) {
        const RankEntry &e = list[i];
        const char* diffText = (e.difficulty == 0) ? "Fácil" : (e.difficulty == 1) ? "Médio" : "Difícil";
        string line = to_string(i+1) + ". " + e.name + " | " + to_string(e.score) + " | " + FormatTime(e.timeSeconds) + " | " + diffText + " | " + e.date + " | ";
        DrawText(line.c_str(), 40, y + i*28, 20, WHITE);
    }

    DrawText("Pressione B para voltar", 10, GetScreenHeight() - 30, 18, LIGHTGRAY);
    EndDrawing();
}