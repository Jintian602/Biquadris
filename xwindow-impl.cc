// XWindow module - implementation
module;
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <unistd.h>

import <string>;
import <memory>;
import <cstring>;

module xwindow;

struct XWindow::XWindowImpl {
    Display* d;
    Window w;
    int s;
    GC gc;
    unsigned long colors[12];
    int width, height;
};

XWindow::XWindow(int w, int h) : pImpl{std::make_unique<XWindowImpl>()} {
    pImpl->width = w;
    pImpl->height = h;
    pImpl->d = XOpenDisplay(nullptr);
    if (!pImpl->d) throw "Cannot open display";
    
    pImpl->s = DefaultScreen(pImpl->d);
    Window root = DefaultRootWindow(pImpl->d);
    
    pImpl->w = XCreateSimpleWindow(pImpl->d, root, 10, 10, pImpl->width, pImpl->height, 1,
                                    BlackPixel(pImpl->d, pImpl->s), WhitePixel(pImpl->d, pImpl->s));
    
    XSelectInput(pImpl->d, pImpl->w, ExposureMask | KeyPressMask);
    XMapRaised(pImpl->d, pImpl->w);
    
    pImpl->gc = XCreateGC(pImpl->d, pImpl->w, 0, nullptr);
    
    pImpl->colors[White] = WhitePixel(pImpl->d, pImpl->s);
    pImpl->colors[Black] = BlackPixel(pImpl->d, pImpl->s);
    
    XColor xcolor, exact;
    Colormap cmap = DefaultColormap(pImpl->d, pImpl->s);
    
    XAllocNamedColor(pImpl->d, cmap, "red", &xcolor, &exact);
    pImpl->colors[Red] = xcolor.pixel;
    
    XAllocNamedColor(pImpl->d, cmap, "green", &xcolor, &exact);
    pImpl->colors[Green] = xcolor.pixel;
    
    XAllocNamedColor(pImpl->d, cmap, "blue", &xcolor, &exact);
    pImpl->colors[Blue] = xcolor.pixel;
    
    XAllocNamedColor(pImpl->d, cmap, "cyan", &xcolor, &exact);
    pImpl->colors[Cyan] = xcolor.pixel;
    
    XAllocNamedColor(pImpl->d, cmap, "yellow", &xcolor, &exact);
    pImpl->colors[Yellow] = xcolor.pixel;
    
    XAllocNamedColor(pImpl->d, cmap, "magenta", &xcolor, &exact);
    pImpl->colors[Magenta] = xcolor.pixel;
    
    XAllocNamedColor(pImpl->d, cmap, "orange", &xcolor, &exact);
    pImpl->colors[Orange] = xcolor.pixel;
    
    XAllocNamedColor(pImpl->d, cmap, "brown", &xcolor, &exact);
    pImpl->colors[Brown] = xcolor.pixel;
    
    XAllocNamedColor(pImpl->d, cmap, "gray25", &xcolor, &exact);
    pImpl->colors[DarkGray] = xcolor.pixel;
    
    XAllocNamedColor(pImpl->d, cmap, "gray85", &xcolor, &exact);
    pImpl->colors[LightGray] = xcolor.pixel;
    
    XFlush(pImpl->d);
    XSync(pImpl->d, False);
    
    usleep(50000);
}

XWindow::~XWindow() {
    if (pImpl) {
        XFreeGC(pImpl->d, pImpl->gc);
        XCloseDisplay(pImpl->d);
    }
    // pImpl automatically deleted by unique_ptr
}

void XWindow::fillRectangle(int x, int y, int w, int h, int color) {
    XSetForeground(pImpl->d, pImpl->gc, pImpl->colors[color]);
    XFillRectangle(pImpl->d, pImpl->w, pImpl->gc, x, y, w, h);
    XFlush(pImpl->d);
}

void XWindow::drawString(int x, int y, const std::string& msg, int color) {
    XSetForeground(pImpl->d, pImpl->gc, pImpl->colors[color]);
    XDrawString(pImpl->d, pImpl->w, pImpl->gc, x, y, msg.c_str(), msg.length());
    XFlush(pImpl->d);
}

void XWindow::drawRectangle(int x, int y, int w, int h, int color) {
    XSetForeground(pImpl->d, pImpl->gc, pImpl->colors[color]);
    XDrawRectangle(pImpl->d, pImpl->w, pImpl->gc, x, y, w, h);
    XFlush(pImpl->d);
}

bool XWindow::checkEvent(std::string& key) {
    XEvent event;
    if (XPending(pImpl->d) > 0) {
        XNextEvent(pImpl->d, &event);
        if (event.type == KeyPress) {
            KeySym keysym = XLookupKeysym(&event.xkey, 0);
            
            // Arrow keys
            if (keysym == XK_Left) { key = "left"; return true; }
            if (keysym == XK_Right) { key = "right"; return true; }
            if (keysym == XK_Down) { key = "down"; return true; }
            if (keysym == XK_Up) { key = "cw"; return true; }  // Up = rotate clockwise
            
            // Special keys
            if (keysym == XK_Return || keysym == XK_KP_Enter) { 
                key = "drop"; 
                return true; 
            }
            if (keysym == XK_space) { 
                key = "drop"; 
                return true; 
            }
            if (keysym == XK_Escape) { 
                key = "quit"; 
                return true; 
            }
            
            // Regular keys (a-z, 0-9)
            if (keysym >= 0x20 && keysym <= 0x7E) {
                char c = (char)keysym;
                // Convert to lowercase
                if (c >= 'A' && c <= 'Z') c += ('a' - 'A');
                
                // Map common shortcuts
                if (c == 'h') { key = "hold"; return true; }
                if (c == 'z') { key = "ccw"; return true; }  // Z = counter-clockwise
                if (c == 'x') { key = "cw"; return true; }   // X = clockwise
                if (c == 'c') { key = "hold"; return true; } // C = hold (alternative)
                if (c == 'a') { key = "left"; return true; }
                if (c == 'd') { key = "right"; return true; }
                if (c == 's') { key = "down"; return true; }
                if (c == 'w') { key = "cw"; return true; }
                if (c == 'q') { key = "quit"; return true; }
                if (c == 'r') { key = "restart"; return true; }
                if (c == 'p') { key = "phantom"; return true; } // P = toggle phantom block display
                if (c == '+' || c == '=') { key = "levelup"; return true; }
                if (c == '-' || c == '_') { key = "leveldown"; return true; }
                
                // Otherwise return the character as-is for commands
                key = std::string(1, c);
                return true;
            }
        }
    }
    return false;
}

