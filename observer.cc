/**
 * @file observer.cc
 * @brief Interface for the Observer class (Observer Pattern)
 * 
 * This file defines the abstract Observer interface, which is the core
 * of the Observer design pattern. Observers implement the notify() method
 * to respond to changes in subjects they are observing.
 */

export module observer;

/**
 * @class Observer
 * @brief Abstract interface for objects that observe subjects
 * 
 * The Observer class defines the interface that all concrete observers
 * must implement. It is part of the Observer design pattern, where
 * observers register with subjects and are notified of state changes.
 * 
 * Concrete observer classes (like TextObserver and GraphicsObserver)
 * inherit from this class and implement the notify() method to handle
 * updates from their subjects.
 */
export class Observer {
public:
    /**
     * @brief Virtual destructor for proper cleanup of derived classes
     */
    virtual ~Observer() = default;
    
    /**
     * @brief Pure virtual method called when subject state changes
     * 
     * This method must be implemented by all concrete observer classes.
     * It is called by the subject when notifyObservers() is invoked,
     * allowing the observer to update its display or internal state.
     */
    virtual void notify() = 0;
};


