# ACE Architecture

This document provides a high-level overview of the ACE (A Chess Engine) codebase structure.

## Directory Structure

### `src/board`
Contains the board representation and move generation logic:
- Board state management using a 120-square array representation
- Bitboards for pawn structures
- Move making and unmaking functionality
- Position evaluation
- Move representation and manipulation

### `src/search`
Implements the search algorithms:
- Alpha-Beta pruning search
- Iterative deepening
- Move generation
- Move list management
- Search information and timing control

### `src/io`
Handles UCI protocol and input parsing:
- UCI (Universal Chess Interface) protocol implementation
- Command parsing
- Move notation parsing (algebraic notation)
- Communication with chess GUIs

### `src/utils`
Provides helper functions and utilities:
- Common utility functions
- Exception handling (`AceException` class)
- General-purpose helpers used across modules

### `src/main`
Entry point for the chess engine:
- Main game loop
- Command-line interface
- Integration of all modules
