/**
 * @file xwindow.cc
 * @brief Interface for the XWindow class (X11 graphics window)
 * 
 * This file defines the XWindow class, which provides a simple interface for
 * drawing graphics using the X11 library. It uses the PIMPL (Pointer to
 * Implementation) pattern to hide X11 implementation details.
 */

export module xwindow;

import <string>;
import <memory>;

/**
 * @class XWindow
 * @brief Wrapper for X11 graphics window operations
 * 
 * XWindow provides a simple interface for drawing rectangles, strings, and
 * handling keyboard input using the X11 library. It uses the PIMPL pattern
 * to hide X11 implementation details from clients.
 * 
 * The window supports various colors and can handle keyboard events in a
 * non-blocking manner. Used by GraphicsObserver to render the game display.
 */
export class XWindow {
    struct XWindowImpl;                    ///< Forward declaration of implementation
    std::unique_ptr<XWindowImpl> pImpl;   ///< Pointer to implementation (PIMPL pattern)
    
public:
    /**
     * @brief Constructs a new XWindow
     * @param w Window width in pixels (default: 800)
     * @param h Window height in pixels (default: 600)
     * 
     * Opens an X11 display connection and creates a window. Throws an exception
     * if the display cannot be opened.
     */
    XWindow(int w = 800, int h = 600);
    
    /**
     * @brief Destructor
     * 
     * Closes the X11 display connection and frees resources.
     */
    ~XWindow();
    
    /**
     * @brief Draws a filled rectangle
     * @param x X coordinate of top-left corner
     * @param y Y coordinate of top-left corner
     * @param width Rectangle width
     * @param height Rectangle height
     * @param color Color index (default: 0 = White)
     * 
     * Fills a rectangle with the specified color. Coordinates are in pixels.
     */
    void fillRectangle(int x, int y, int width, int height, int color = 0);
    
    /**
     * @brief Draws a text string
     * @param x X coordinate of text start
     * @param y Y coordinate of text baseline
     * @param msg Text string to draw
     * @param color Color index (default: 0 = White)
     * 
     * Renders text at the specified position with the given color.
     */
    void drawString(int x, int y, const std::string& msg, int color = 0);
    
    /**
     * @brief Draws an outline rectangle
     * @param x X coordinate of top-left corner
     * @param y Y coordinate of top-left corner
     * @param width Rectangle width
     * @param height Rectangle height
     * @param color Color index (default: 0 = White)
     * 
     * Draws the outline of a rectangle with the specified color.
     */
    void drawRectangle(int x, int y, int width, int height, int color = 0);
    
    /**
     * @brief Checks for keyboard events (non-blocking)
     * @param key Output parameter: key string if event found
     * @return true if a key event was found, false otherwise
     * 
     * Checks for pending keyboard events and converts them to command strings.
     * Returns immediately (non-blocking). If an event is found, key is set
     * to a command string like "left", "right", "drop", "quit", etc.
     */
    bool checkEvent(std::string& key);
    
    /**
     * @enum Color
     * @brief Available colors for drawing
     */
    enum Color { 
        White = 0,      ///< White
        Black,          ///< Black
        Red,            ///< Red
        Green,          ///< Green
        Blue,           ///< Blue
        Cyan,           ///< Cyan
        Yellow,         ///< Yellow
        Magenta,        ///< Magenta
        Orange,         ///< Orange
        Brown,          ///< Brown
        DarkGray,       ///< Dark gray
        LightGray       ///< Light gray
    };
};

