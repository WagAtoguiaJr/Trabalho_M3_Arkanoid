#pragma once

#include <string>
#include <vector>

using namespace std;

struct RankEntry {
    string date; // YYYY-MM-DD
    string name;
    int score;
    int difficulty; // 0,1,2
    double timeSeconds;
};

static bool RankComparator(const RankEntry &a, const RankEntry &b);
void DrawRankingScreen();

void LoadRankingCSV(const char* filename, vector<RankEntry> &outList);
void AppendRankingCSV(const char* filename, const RankEntry &entry);