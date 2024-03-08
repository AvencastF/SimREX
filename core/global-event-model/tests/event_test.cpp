//
// Created by Yulei on 2024/3/8.
//

#include "global-event-model/event.h"
#include <iostream>

int main() {
    SimREX::GEM::event e;

    // Test 1: Check initial run number
    {
        if (e.getRunNumber() != 0) {
            std::cerr << "Test 1 failed: Initial run number is not 0\n";
            return 1;
        }
    }

    // Test 2: Check initial event number
    {
        if (e.getEventNumber() != 0) {
            std::cerr << "Test 2 failed: Initial event number is not 0\n";
            return 1;
        }
    }

    // Test 3: Check initial event weight
    {
        if (e.getEventWeight() != 1.0) {
            std::cerr << "Test 3 failed: Initial event weight is not 1.0\n";
            return 1;
        }
    }

    // Test 4: Register a hit collection and check if it exists
    {
        e.registerHitCollection("testCollection");
        if (e.getHits("testCollection") == nullptr) {
            std::cerr << "Test 4 failed: Hit collection 'testCollection' was not registered\n";
            return 1;
        }
    }

    // Test 5: Remove a hit collection and check if it still exists
    {
        e.removeHitCollection("testCollection");
        if (e.getHits("testCollection") != nullptr) {
            std::cerr << "Test 5 failed: Hit collection 'testCollection' was not removed\n";
            return 1;
        }
    }

    // Test 6: Check if getHits returns nullptr for a non-existent collection
    {
        if (e.getHits("nonExistentCollection") != nullptr) {
            std::cerr << "Test 6 failed: getHits did not return nullptr for a non-existent collection\n";
            return 1;
        }
    }

    // Test 7: Check if initialization resets the event
    {
        e.setRunNumber(1);
        e.setEventNumber(1);
        e.setEventWeight(2.0);
        e.registerHitCollection("testCollection");
        e.initialization();
        if (e.getRunNumber() != 0 || e.getEventNumber() != 0 || e.getEventWeight() != 1.0 ||
            !e.getHits("testCollection")->empty()) {
            std::cerr << "Test 7 failed: Initialization did not reset the event\n";
            return 1;
        }
    }

    // Test 8: Check if addMCParticle adds a particle to the event
    {
        // Generate some dummy particles and add them to the event
        for (int i = 0; i < 10; ++i) {
            auto *p = new SimREX::GEM::particle();
            p->setId(i);
            e.getMCParticles()->push_back(p);
        }
        // Test the searchParticle function
        for (int i = 0; i < 10; ++i) {
            SimREX::GEM::particle *p = e.searchParticle(i);
            if (p == nullptr || p->getId() != i) {
                std::cerr << "Test failed: searchParticle did not find the correct particle\n";
                return 1;
            }
        }
        // Test the searchParticle function with an ID that does not exist
        if (e.searchParticle(11) != nullptr) {
            std::cerr << "Test failed: searchParticle found a particle that does not exist\n";
            return 1;
        }
    }

    // All tests passed
    std::cout << "All tests passed\n";
    return 0;
}