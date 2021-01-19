#include <sstream>
#include <iostream>
#include "TelemetryTracker.h"

#define LEN 5

TelemetryTracker::TelemetryTracker(int G_, int t_, int delta_)
    : G(G_),
      t(t_),
      delta(delta_),
      groupI(0),
      is_wrong(false),
      numberOfNonEndingGroups(0),
      totalDelayOfNonEndingGroups(0),
      totalDelayOfOptimiziedNonEndingGroups(0),
      numberOfNonEndingOptimizedGroups(0),
      lag(LEN, 0),
      numArrivedPackets(LEN, 0)
      {};

void TelemetryTracker::update(int num)
{
    if (is_wrong) {
        return;
    }

    int mod = (1 << t) - 1;
    int type = (num / G) & mod;
    int diff = (type - (groupI & mod) + 3*mod + 3) & mod;

    int predicted_groupId = groupI+diff;
    if (predicted_groupId != num / G) {
        is_wrong = true;
        return;
    }

    while (diff >= delta) {
        updateGroupCounters(groupI);
        lag[groupI % LEN] = 0;
        numArrivedPackets[groupI % LEN] = 0;
        groupI++;
        diff--;
    }

    bool meet_non_zero = false;
    for (int i  = groupI+delta-1;  i >= groupI; i--) {
        meet_non_zero |= (numArrivedPackets[i%LEN] != 0);
        if (meet_non_zero) {
            lag[i % LEN]++;
        }
    }

    lag[predicted_groupId % LEN] = 0;
    numArrivedPackets[predicted_groupId % LEN]++;
}



void TelemetryTracker::updateGroupCounters(int i) {
   numberOfNonEndingGroups++;
   lag[i%LEN] += 1;
   totalDelayOfNonEndingGroups += lag[i % LEN];
   totalDelayOfOptimiziedNonEndingGroups += (numArrivedPackets[i%LEN] == G ? 0 : lag[i%LEN]);
   numberOfNonEndingOptimizedGroups += (numArrivedPackets[i%LEN] == G ? 0 : 1);
}

std::tuple<int, int, int, int> TelemetryTracker::getDelayInfo() {
    return std::tuple<int, int, int, int>(numberOfNonEndingGroups, totalDelayOfNonEndingGroups, totalDelayOfOptimiziedNonEndingGroups, numberOfNonEndingOptimizedGroups);
}

bool TelemetryTracker::isCorrect() {
    return !is_wrong;
}



