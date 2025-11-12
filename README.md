# Bank Management System

## Overview
This is a simple CLI-based **Bank Management System** project in C. It demonstrates basic concepts like modular programming, function calls, and user input handling. The system currently has two modules:

1. **Administration** – Handles admin login and related functionality.
2. **Client** – Placeholder for client-related operations (can be extended later).

---

## Changes in Code Runner Settings (`settings.json`)

To ensure that all C files compile and run correctly in **VS Code** using Code Runner, the executor for C was updated:

### Original setting
```json
"c": "cd $dir && gcc $fileName -o $fileNameWithoutExt && $dir$fileNameWithoutExt"
