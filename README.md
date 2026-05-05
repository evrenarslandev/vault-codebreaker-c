# Vault Codebreaker

A strategic logic game developed in **C** that challenges players to crack a secret code. The project emphasizes modular programming, file-based configuration, and robust user input validation.

## 🚀 Key Technical Features
- **Dual Operating Modes:** Includes an **Admin Mode** for real-time game rule configuration and a **Player Mode** for the main gameplay.
- **Persistent Configuration:** Game parameters (code length, digit range, duplicate rules, scoring) are stored in and loaded from `vault_config.txt`.
- **Customizable Difficulty:** Supports dynamic code lengths and toggleable rules for duplicate digits, allowing for a scalable difficulty level.
- **Advanced Scoring Engine:** Implements a feedback system (Correct, Misplaced, Wrong) with a point/penalty system that tracks the player's performance.
- **Session Logging:** Automatically logs game start times, secret codes, and every attempt with a timestamped header in `game_log.txt`.

## 🛠️ How to Compile and Run
1. **Compile** using gcc:
   gcc vault.c -o vault
2. Run the executable:
   ./vault

## 🎮 How to Play
1. **Admin Mode (a):** Set up the game rules (Length, Range, Max Attempts, etc.). These will be saved for future sessions.
2. **Player Mode (p):** Try to guess the secret code.
   - **C (Green):** Correct number in the correct place.
   - **M (Yellow):** Correct number but in the wrong place.
   - **W (Red):** Wrong number.
