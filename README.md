
# 🏓 Ping Pong — Raylib C++ Game

A feature-rich Ping Pong game built in C++ using the [Raylib](https://www.raylib.com/) graphics library. Supports three game modes: Single Player, Two Player (Local Multiplayer), and AI opponent with selectable difficulty. Developed as an OOP course project.

---

## 📸 Preview

![Game Preview](Resources/home%203.png)

---

## 🎮 Game Modes

| Mode | Description |
|------|-------------|
| **Single Player** | Control a horizontal paddle with arrow keys. Ball speeds up as your score increases. 3 health points. Name entry + high score leaderboard. |
| **Two Player** | Two vertical paddles face each other. Left player uses `W`/`S`, right player uses `↑`/`↓`. Choose a target score of 5, 10, or 15. |
| **vs AI** | Play against a computer-controlled paddle. Choose from Easy, Medium, or Extreme difficulty. First to 5 points wins. |

---

## 🕹️ Controls

### Single Player
| Key | Action |
|-----|--------|
| `←` / `→` | Move paddle left / right |

### Two Player (Multiplayer)
| Key | Action |
|-----|--------|
| `W` / `S` | Move **left** paddle up / down |
| `↑` / `↓` | Move **right** paddle up / down |

### vs AI
| Key | Action |
|-----|--------|
| `W` / `S` | Move your (left) paddle up / down |

---

## ✨ Features

- 3 distinct game modes with separate game loops
- Animated main menu with interactive hover effects
- Countdown timer before each game starts
- Dynamic ball speed scaling in single player (score-based difficulty)
- AI paddle with 3 difficulty levels (Easy / Medium / Extreme)
- High score system — saves top 3 scores to `Resources/highscores.txt`
- Player name entry in single player mode
- Sound effects for hit, score, damage, game over, and start
- Custom font rendering via Raylib's `DrawTextEx`
- Rounded paddle and button rendering
- Restart / Replay button after game over

---

## 📁 Project Structure

```
ping-pong/
│
├── main.cpp                  # All game source code
├── Makefile                  # Cross-platform build script (raylib template)
├── libgcc_s_dw2-1.dll        # MinGW runtime DLL (Windows)
├── libstdc++-6.dll           # MinGW runtime DLL (Windows)
├── preview.jpg               # Screenshot for README
├── LICENSE.txt               # MIT License
├── README.md                 # This file
│
├── Resources/                # All game assets (you must provide these)
│   ├── Namaku.ttf            # Custom font
│   ├── highscores.txt        # Auto-created on first game over
│   │
│   ├── home.png              # Main menu background
│   ├── home 1.png            # Menu hover state (Single Player)
│   ├── home 2.png            # Menu hover state (Multiplayer)
│   ├── home 3.png            # Menu hover state (vs AI)
│   ├── gamescreen.png        # Single player game background
│   ├── gamescreenmulti.png   # Multiplayer game background
│   ├── gamescreenmulti2.png  # AI mode game background
│   ├── one.png               # Countdown image "1"
│   ├── two.png               # Countdown image "2"
│   ├── three.png             # Countdown image "3"
│   ├── restart.png           # Restart button icon
│   │
│   ├── hit.wav               # Ball hit sound
│   ├── score.wav             # Point scored sound
│   ├── damage.wav            # Health lost sound
│   ├── gameover.wav          # Game over sound
│   └── start.wav             # Game start sound
│
└── .vscode/                  # VS Code workspace config (optional)
    ├── c_cpp_properties.json
    ├── launch.json
    ├── tasks.json
    └── settings.json
```

> **Note:** The `Resources/` folder and its contents are not included in this repo due to asset size. You must supply your own assets or download the original template assets.

---

## 🛠️ Build Instructions

### Prerequisites

- **Windows:** Install [w64devkit](https://github.com/skeeto/w64devkit) and [Raylib 4.5](https://www.raylib.com/) to `C:/raylib/`
- **Linux:** Install raylib via your package manager or build from source
- **macOS:** Install raylib via Homebrew: `brew install raylib`

### Build on Windows

```bash
cd path/to/ping-pong
mingw32-make RAYLIB_PATH=C:/raylib/raylib PROJECT_NAME=main OBJS=*.cpp
```

Or press `Ctrl+Shift+B` in VS Code to run the default build task.

### Build on Linux

```bash
make PROJECT_NAME=main OBJS=*.cpp DESTDIR=/home/linuxbrew/.linuxbrew RAYLIB_LIBTYPE=SHARED
```

### Build on macOS

```bash
make PROJECT_NAME=main OBJS=*.cpp
```

### Run

```bash
./main        # Linux / macOS
main.exe      # Windows
```

> Make sure the `Resources/` folder is in the **same directory** as the executable.

---

## 🧱 OOP Design

The project uses two primary classes:

### `Ball`
Encapsulates the ball's position, radius, and velocity. Exposes three draw/update methods:
- `DrawSingle()` — single player logic with score-based speed scaling and health deduction
- `DrawMulti()` — multiplayer/AI logic with left/right boundary scoring
- `DrawIntro()` — animated bouncing ball for the main menu

### `Paddle`
Encapsulates paddle position, size, and speed. Exposes four movement methods:
- `Draw()` — horizontal movement (arrow keys, single player)
- `DrawLeft()` — vertical movement with `W`/`S` keys
- `DrawRight()` — vertical movement with `↑`/`↓` keys
- `DrawAI(int ball_y)` — auto-tracks ball Y position at configured speed

Global instances: `Ball ball`, `Paddle user, RightUser, LeftUser, AI`

---

## 🐛 Known Issues / Limitations

- `#include <conio.h>` is Windows-only; remove it if building on Linux/macOS (it is unused in the code).
- `DrawRestartButton` reloads `restart.png` from disk on every frame — this should be moved to a one-time load for performance.
- Game functions (`PlayGameSingle`, `PlayGameMulti`, `PlayGameAI`) call `InitWindow` internally and use recursion for restart — a full refactor to a state machine would be cleaner.
- High scores are stored as plaintext and are not validated on load.

---

## 📜 License

This project is licensed under the **MIT License** — see [LICENSE.txt](LICENSE.txt) for details.  
Raylib template originally by [Nick Koumaris](https://www.youtube.com/@educ8s).

---
