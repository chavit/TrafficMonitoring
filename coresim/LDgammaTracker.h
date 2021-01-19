#ifndef SIMULATOR_LDGAMMATRACKER_H
#define SIMULATOR_LDGAMMATRACKER_H

#include <vector>
#include "TelemetryTracker.h"
#include <array>

class Flow;
class Packet;
class Queue;


extern u_int64_t telemetryStatistics[40][28];


//extern u_int64_t arrDel[15][28];



class LDgammaTracker {

public:

    LDgammaTracker(Flow* flow, int size);
    void notifyPacketEnque(Packet* p, Queue* queue);
    void notifyPacketDrop(Packet* p, Queue* queue);
    void finalizeResults();


    constexpr static std::array<int, 4> telemetry_t = {2, 3, 3, 3};
    constexpr static std::array<int, 4> telemetry_delta = {2, 2, 3, 4};


private:
    int num_packets,size;
    int last_arrived_packet;
    Flow* flow;
    std::vector<std::pair<int, int>> dropped_packets;
    int R;

    std::vector<TelemetryTracker> telemetryTrackers;
};


#endif //SIMULATOR_LDGAMMATRACKER_H
