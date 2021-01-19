#include <assert.h>
#include <sstream>
#include <iostream>
#include "LDgammaTracker.h"
#include "packet.h"


constexpr std::array<int, 4> LDgammaTracker::telemetry_t, LDgammaTracker::telemetry_delta;

uint64_t telemetryStatistics[40][28];

LDgammaTracker::LDgammaTracker(Flow *flow, int size) {
    this->size = size;
    this->flow = flow;
    this->last_arrived_packet = -1;
    this->num_packets = 0;
    this->R = 0;

    if (size < 8 * 33 + 1) {
        return;
    }


    for (int i = 0; i < 40; i++) {
        for (int j = 0; j < telemetry_t.size(); j++) {
            telemetryTrackers.emplace_back(5 + i, telemetry_t[j], telemetry_delta[j]);
        }
    }

}


void LDgammaTracker::notifyPacketEnque(Packet *p, Queue *queue) {
    if (queue != nullptr)
    {
        if (queue->src->type == HOST)
            p->packet_number_by_first_switch = num_packets++;
        return;
    }

    assert(p->packet_number_by_first_switch != last_arrived_packet);
    if (p->packet_number_by_first_switch > last_arrived_packet)
    {
        last_arrived_packet = p->packet_number_by_first_switch;
    }
    else
    {
        R = std::max(R, last_arrived_packet - p->packet_number_by_first_switch);
    }

    if (size < 8 * 33 + 1) {
        return;
    }


    for (auto& tracker : telemetryTrackers)
    {
        tracker.update(p->packet_number_by_first_switch);
    }

}

void LDgammaTracker::notifyPacketDrop(Packet *p, Queue *queue) {
    int insertion_pos = -1;
    for (int i = 0; i < dropped_packets.size(); i++) {
        if (dropped_packets[i].second + 1 == p->packet_number_by_first_switch) {
            dropped_packets[i].second++;
            insertion_pos = i;
            break;
        }
    }
    if (insertion_pos == -1) {
        insertion_pos = dropped_packets.size();
        dropped_packets.emplace_back(p->packet_number_by_first_switch, p->packet_number_by_first_switch);
    }

    for (int i = 0; i < dropped_packets.size(); i++) {
        if (dropped_packets[insertion_pos].second + 1 == dropped_packets[i].first) {
            dropped_packets[insertion_pos].second = dropped_packets[i].second;
            dropped_packets.erase(dropped_packets.begin() + i);
            break;
        }
    }
    p->flow->receive(p);

}

void LDgammaTracker::finalizeResults() {
    if (size < 8 * 33 + 1) {
        return;
    }


    int L = 0;
    for (const auto &seg : dropped_packets) {
        L = std::max(L, seg.second - seg.first + 1);
    }

    {   // Telemetry statistics
        for (int i = 0; i < telemetryTrackers.size(); i++) {
            int x = i / 4;
            int y = i % 4 * 7;

            telemetryStatistics[x][y + 6]++;
            bool isValid = (R <= (telemetry_delta[i % 4] - 1) * (5 + x)) &&
                           (R + L < ((1 << telemetry_t[i % 4])-telemetry_delta[i % 4] ) * (5 + x));
            if (!isValid) {
                telemetryStatistics[x][y + 4]++;
            }
            if (!telemetryTrackers[i].isCorrect()) {
                telemetryStatistics[x][y + 5]++;
            }

            if (!telemetryTrackers[i - i % 4].isCorrect()) {
                continue;
            }
            std::tuple<int, int, int, int> stat = telemetryTrackers[i].getDelayInfo();
            telemetryStatistics[x][y] += (u_int64_t) std::get<0>(stat);
            telemetryStatistics[x][y + 1] += (u_int64_t) std::get<1>(stat);
            telemetryStatistics[x][y + 2] += (u_int64_t) std::get<2>(stat);
            telemetryStatistics[x][y + 3] += (u_int64_t) std::get<3>(stat);

        }
    }

    if (num_packets != size) {
        while (true) {}
    }
}