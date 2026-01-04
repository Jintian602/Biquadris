# 🎮 Biquadris

A competitive two-player Tetris game implemented in C++20 with modern design patterns and enhanced features.

---

## 📋 Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Compilation](#compilation)
- [Running the Game](#running-the-game)
- [Game Rules](#game-rules)
- [Controls](#controls)
- [Command Line Arguments](#command-line-arguments)
- [Extra Features](#extra-features)
- [Project Structure](#project-structure)
- [Design Patterns](#design-patterns)

---

## 🎯 Overview

Biquadris is a two-player variant of Tetris where players compete side-by-side. The game features:
- **Dual boards**: Two players play simultaneously with independent boards
- **Turn-based gameplay**: Players take turns dropping blocks
- **Level progression**: 5 difficulty levels (0-4) with different block generation patterns
- **Special effects**: Heavy and blind effects to challenge opponents
- **Modern C++20**: Uses modules, smart pointers, and the Observer pattern

---

## ✨ Features

### Core Features
- ✅ Two-player competitive gameplay
- ✅ 7 classic Tetris blocks (I, J, L, O, S, T, Z)
- ✅ 5 difficulty levels with different generation rules
- ✅ Line clearing and scoring system
- ✅ Text and graphical display modes
- ✅ Special effects system
- ✅ Independent random generation for each player

### Enhanced Features (Bonus)
- 🎨 **Beautiful UI**: Modern graphical interface with color-coded blocks
- ⌨️ **Raw Key Inputs**: Real-time control with arrow keys, WASD, and shortcuts
- 👻 **Phantom Blocks**: Visual preview showing where blocks will land
- 📦 **Block Holding**: Save blocks for strategic use later
- 🎮 **Dual Control Schemes**: Both command-line and keyboard shortcuts
- 🎯 **Smart Turn System**: Blocks only appear when it's your turn

---

## 🔨 Compilation

### Requirements
- **Compiler**: g++-14 or later with C++20 module support
- **Libraries**: X11 development libraries (for graphics)
- **OS**: Linux (tested on Ubuntu 24.04)

### Install Dependencies
```bash
# Install X11 development libraries
sudo apt-get update
sudo apt-get install libx11-dev
```

### Build
```bash
# Navigate to project directory
cd /path/to/Biquadris

# Clean build (recommended)
make clean
make

# Quick rebuild (if only implementation changed)
make
```

### Troubleshooting
If you encounter module compilation errors:
```bash
# Force clean rebuild
make clean
rm -rf gcm.cache
make
```

---

## 🎮 Running the Game

### Basic Usage
```bash
# Default: Start at Level 0 with graphics
./biquadris

# Text-only mode
./biquadris -text

# Start at a specific level (0-4)
./biquadris -startlevel 3

# Use custom sequence files for Level 0
./biquadris -scriptfile1 data/my_sequence1.txt -scriptfile2 data/my_sequence2.txt

# Combine options
./biquadris -startlevel 2 -text
```

### Command Line Arguments

| Argument | Description | Example |
|----------|-------------|---------|
| `-text` | Run in text-only mode (no graphics) | `./biquadris -text` |
| `-startlevel n` | Start at level n (0-4) | `./biquadris -startlevel 3` |
| `-scriptfile1 file` | Custom block sequence for Player 1 | `./biquadris -scriptfile1 data/seq1.txt` |
| `-scriptfile2 file` | Custom block sequence for Player 2 | `./biquadris -scriptfile2 data/seq2.txt` |

---

## 📖 Game Rules

### Objective
Clear more lines than your opponent. The game ends when one player can't spawn a new block.

### Levels

| Level | Description | Block Generation |
|-------|-------------|------------------|
| **0** | Scripted | Reads from sequence file |
| **1** | Easy | S/Z: 1/12, Others: 1/6 each |
| **2** | Medium | All blocks equally likely (1/7) |
| **3** | Hard | S/Z: 2/9, Others: 1/9 each |
| **4** | Expert | Level 3 + drops 1×1 block every 5 turns without clearing |

### Scoring
Points earned when clearing rows:
```
score += (block_level + rows_cleared)²
```

### Special Effects
- **Heavy**: Block drops 2 extra rows after each left/right/rotate command
- **Blind**: Opponent's board is partially obscured

### Turn System
- Players take turns controlling one block at a time
- A turn ends when the block is dropped (manually or automatically)
- When a block touches the ground and can't move down, it locks immediately
- Blocks only appear on the board when it's that player's turn

---

## 🎮 Controls

### Keyboard Shortcuts (Graphics Mode)

#### Movement
| Key | Action |
|-----|--------|
| `←` / `A` | Move block left |
| `→` / `D` | Move block right |
| `↓` / `S` | Move block down (soft drop) |

#### Rotation
| Key | Action |
|-----|--------|
| `↑` / `W` | Rotate clockwise |
| `X` | Rotate clockwise |
| `Z` | Rotate counter-clockwise |

#### Actions
| Key | Action |
|-----|--------|
| `Space` | Hard drop (instant drop) |
| `Enter` | Hard drop (instant drop) |
| `H` | Hold current block |
| `C` | Hold current block (alternative) |

#### Level Control
| Key | Action |
|-----|--------|
| `+` / `=` | Level up |
| `-` / `_` | Level down |

#### Game Control
| Key | Action |
|-----|--------|
| `R` | Restart game |
| `Q` / `Esc` | Quit game |

### Command Line Interface

Type commands followed by Enter:

#### Basic Commands
```
left          Move block left
right         Move block right
down          Move block down
drop          Hard drop (instant drop)
clockwise     Rotate clockwise (can use "cw")
counterclockwise  Rotate counter-clockwise (can use "ccw")
hold          Hold/swap current block
```

#### Level Commands
```
levelup       Increase difficulty level
leveldown     Decrease difficulty level
```

#### Game Commands
```
restart       Restart the game
quit          Exit the game
```

#### Testing Commands
```
I, J, L, O, S, T, Z    Replace current block with specific type
```

#### Command Multipliers
You can prefix commands with a number:
```
3right        Move right 3 times
5down         Move down 5 times
2clockwise    Rotate clockwise twice
```

#### Command Abbreviation
Commands can be abbreviated to their shortest unique prefix:
```
lef     → left
ri      → right
d       → down
dr      → drop
cl      → clockwise
cou     → counterclockwise
h       → hold
```

---

## 🌟 Extra Features

### P2: Raw Key Inputs ⌨️
Real-time keyboard control without needing to press Enter.

**Implementation**:
- `XWindow::checkEvent()`: Polls X11 events for keyboard input
- Non-blocking input using `poll()` on stdin
- Event loop processes both keyboard shortcuts and command-line input

**Keyboard Mappings**:
- Arrow keys: ← → ↓ ↑
- WASD: A/D (left/right), S (down), W (rotate)
- Shortcuts: Z/X (rotate), H/C (hold), Space/Enter (drop)
- Game control: +/- (level), R (restart), Q/Esc (quit)

### P3: Phantom Blocks 👻
Visual preview showing where the current block will land.

**Implementation**:
- `GraphicsObserver::drawBoard()`: Calculates landing position
- Simulates moving block down until collision
- Draws semi-transparent preview at landing position
- Uses white fill with colored border for visibility

**Benefits**:
- Plan moves in advance
- See exactly where blocks will land
- Reduces mistakes and improves strategy

### P3: Block Holding 📦
Save the current block for later strategic use.

**Implementation**:
- `Player::hold()`: Swaps current block with held block
- `Player::heldBlock`: Stores the saved block
- `canHold` flag: Prevents multiple holds per turn
- UI display: Shows held block in dedicated box

**Rules**:
- Can only hold at spawn position (top row)
- Can only hold once per turn (resets after drop)
- First hold: Saves current, spawns next block
- Later holds: Swaps current with held block

**Strategy**:
- Save I-blocks for clearing 4 lines
- Hold blocks that don't fit current situation
- Strategic piece management for higher scores

---

## 📁 Project Structure

```
Biquadris/
├── src/
│   ├── blocks/           # Block implementations (I, J, L, O, S, T, Z)
│   │   ├── block.cc/block-impl.cc
│   │   ├── iblock.cc/iblock-impl.cc
│   │   └── ... (other blocks)
│   ├── levels/           # Level generation logic
│   │   ├── level.cc/level-impl.cc
│   │   ├── level0.cc/level0-impl.cc (scripted)
│   │   └── ... (levels 1-4)
│   ├── core/             # Core game logic
│   │   ├── board.cc/board-impl.cc
│   │   └── player.cc/player-impl.cc
│   ├── controllers/      # Game controller and command interpreter
│   │   ├── game.cc/game-impl.cc
│   │   └── commandinterpreter.cc/commandinterpreter-impl.cc
│   ├── observers/        # Observer pattern for UI updates
│   │   ├── observer.cc/observer-impl.cc
│   │   ├── subject.cc/subject-impl.cc
│   │   ├── textobserver.cc/textobserver-impl.cc
│   │   └── graphicsobserver.cc/graphicsobserver-impl.cc
│   ├── ui/               # User interface
│   │   └── xwindow.cc/xwindow-impl.cc
│   ├── utils/            # Utility classes
│   │   ├── position.cc/position-impl.cc
│   │   ├── cell.cc/cell-impl.cc
│   │   └── effect.cc/effect-impl.cc
│   └── main.cc           # Entry point
├── data/                 # Sequence files for Level 0
│   ├── biquadris_sequence1.txt
│   └── biquadris_sequence2.txt
├── Makefile
├── CONTROLS.txt
└── README.md
```

### Module Organization

The project uses C++20 modules with interface/implementation separation:
- **Interface files** (`.cc`): Exported module definitions
- **Implementation files** (`-impl.cc`): Module implementations
- **Naming convention**: `module modulename;` in implementation files

---

## 🏗️ Design Patterns

### Observer Pattern
**Used for**: UI updates

**Structure**:
- `Subject`: Board notifies observers when state changes
- `Observer`: TextObserver and GraphicsObserver react to changes
- **Benefit**: Decouples game logic from display logic

### Factory Pattern (implicit)
**Used for**: Block and Level creation

**Structure**:
- `Level::generateBlock()`: Creates block objects based on type
- `Player::levelUp()/levelDown()`: Creates appropriate Level objects
- **Benefit**: Encapsulates object creation logic

### MVC Pattern
**Used for**: Overall architecture

**Structure**:
- **Model**: Board, Player, Block, Level classes
- **View**: TextObserver, GraphicsObserver
- **Controller**: Game, CommandInterpreter
- **Benefit**: Separation of concerns, maintainability

### Smart Pointers
**Used throughout**: Memory management

**Types**:
- `std::shared_ptr`: Ownership semantics (e.g., Board, Level, Block)
- Raw pointers: Non-ownership references (e.g., Observer pointers)
- **Benefit**: Automatic memory management, no leaks

---

## 🎯 Game Tips

1. **Use Hold Strategically**: Save I-blocks for 4-line clears
2. **Watch Phantom Blocks**: Plan moves ahead of time
3. **Level Management**: Adjust difficulty to your skill level
4. **Multiple Controls**: Use keyboard shortcuts for faster gameplay
5. **Turn Planning**: Plan your move while opponent is playing
6. **Next Block Preview**: Always check what's coming next

---

## 🐛 Known Issues / Notes

- Auto-lock: Blocks lock immediately when they can't move down (standard Tetris behavior)
- Turn system: Players alternate after each drop (manual or automatic)
- Held blocks: Can only be used at spawn position
- Random generation: Each player has independent random sequences

---

## 👥 Credits

- **Course**: CS246 - Object-Oriented Software Development
- **Implementation**: C++20 with modules, smart pointers, and modern design patterns
- **Graphics**: X11 library
- **Design**: Observer pattern, MVC architecture

---

## 📝 License

Academic project for CS246. All rights reserved.

---

## 🚀 Quick Start Guide

```bash
# 1. Compile the game
make clean && make

# 2. Run with graphics (recommended for first time)
./biquadris

# 3. Try Level 3 for a challenge
./biquadris -startlevel 3

# 4. Learn the controls
#    - Arrow keys to move
#    - Space to drop
#    - H to hold
#    - Watch for phantom blocks (white preview)
#    - Check next block preview

# 5. Have fun! 🎮
```

---

**Enjoy playing Biquadris!** 🎉
