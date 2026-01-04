/**
 * @file subject.cc
 * @brief Interface for the Subject class (Observer Pattern)
 * 
 * This file defines the abstract Subject class, which is the core of the
 * Observer design pattern. Subject maintains a list of observers and
 * notifies them when its state changes. This is a pure abstract class.
 */

export module subject;

import <vector>;
import observer;

/**
 * @class Subject
 * @brief Abstract base class for objects that can be observed
 * 
 * The Subject class implements the subject side of the Observer pattern.
 * It maintains a collection of Observer pointers (non-owning) and provides
 * methods to attach, detach, and notify all registered observers.
 * 
 * This class must be inherited by concrete classes that need to be observed,
 * such as the Board class in Biquadris.
 */
export class Subject {
    std::vector<Observer*> observers;  ///< List of observers (non-owning pointers)
    
public:
    /**
     * @brief Pure virtual destructor
     * 
     * Makes this an abstract class. Derived classes must implement
     * their own destructors.
     */
    virtual ~Subject() = 0;
    
    /**
     * @brief Registers an observer to receive notifications
     * @param obs Pointer to the observer to attach (non-owning)
     * 
     * The observer will be added to the list and will receive
     * notifications when notifyObservers() is called.
     */
    void attach(Observer* obs);
    
    /**
     * @brief Unregisters an observer from receiving notifications
     * @param obs Pointer to the observer to detach
     * 
     * The observer will be removed from the list and will no longer
     * receive notifications.
     */
    void detach(Observer* obs);
    
    /**
     * @brief Notifies all registered observers of a state change
     * 
     * Calls the notify() method on each observer in the list.
     * This should be called by derived classes whenever their
     * state changes in a way that observers should know about.
     */
    void notifyObservers();
    
    /**
     * @brief Gets a copy of all registered observers
     * @return Vector of observer pointers
     * 
     * This is useful when wrapping a player with decorators
     * to preserve observers.
     */
    std::vector<Observer*> getObservers() const;
};


