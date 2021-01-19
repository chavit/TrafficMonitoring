#ifndef SIMULATOR_TelemetryTracker_H
#define SIMULATOR_TelemetryTracker_H


#include <vector>
#include <string>
#include <tuple>

class TelemetryTracker {
public:
    TelemetryTracker(int G_, int t_, int delta_);
    void update(int num);
    bool isCorrect();
    std::tuple<int, int, int, int> getDelayInfo();

private:
    bool is_wrong;
    int G, t, delta;
    int groupI;

    int numberOfNonEndingGroups;
    int totalDelayOfNonEndingGroups;
    int totalDelayOfOptimiziedNonEndingGroups;
    int numberOfNonEndingOptimizedGroups;

    std::vector<int> lag;
    std::vector<int> numArrivedPackets;

    void updateGroupCounters(int i);
};


#endif //SIMULATOR_TelemetryTracker_H
