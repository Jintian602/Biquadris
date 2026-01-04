/**
 * @file subject-impl.cc
 * @brief Implementation of the Subject class
 * 
 * This file contains the implementation of Subject methods for managing
 * the observer list and notifying observers of state changes.
 */

module subject;

import <vector>;
import observer;

Subject::~Subject() {}

void Subject::attach(Observer* obs) {
    observers.push_back(obs);
}

void Subject::detach(Observer* obs) {
    for (auto it = observers.begin(); it != observers.end(); ++it) {
        if (*it == obs) {
            observers.erase(it);
            break;
        }
    }
}

void Subject::notifyObservers() {
    for (auto obs : observers) {
        obs->notify();
    }
}

std::vector<Observer*> Subject::getObservers() const {
    return observers;
}


