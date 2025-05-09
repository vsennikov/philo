# Philosophers Dining Problem

## Overview

This project is an implementation of the classic **Dining Philosophers Problem**, a concurrency problem that illustrates synchronization issues and solutions in multithreaded programming. The program simulates philosophers sitting at a table, alternating between eating, thinking, and sleeping, while sharing forks (resources). The goal is to ensure that no philosopher starves and that the program handles synchronization correctly.

The project is written in C and uses **POSIX threads (pthreads)** for multithreading and mutexes for synchronization.

---

## Features

- Simulates philosophers' behavior (eating, thinking, sleeping).
- Handles synchronization using mutexes to avoid deadlocks and race conditions.
- Supports configurable parameters such as the number of philosophers, time to eat, time to sleep, and more.
- Includes error handling for invalid inputs and resource allocation failures.

---

## How to Build and Run

### 1. Clone the Repository

```bash
git clone https://github.com/yourusername/philo.git
cd philo
```

### 2. Build the Project
Use the provided Makefile to compile the program:

```bash
make
```
This will generate an executable named philo.

### 3. Run the Program
The program accepts the following arguments:
```bash
./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]
```

<number_of_philosophers>: Number of philosophers (and forks).
<time_to_die>: Time (in milliseconds) a philosopher can live without eating.
<time_to_eat>: Time (in milliseconds) a philosopher takes to eat.
<time_to_sleep>: Time (in milliseconds) a philosopher takes to sleep.
[<number_of_times_each_philosopher_must_eat>] (optional): If provided, the simulation ends when each philosopher has eaten this many times.

### Example:
```bash
./philo 5 800 200 200
```
This runs the simulation with 5 philosophers, where each philosopher has:

800ms to eat before dying,
200ms to eat,
200ms to sleep.

## Code Highlights
Key Functions
**check_during_eat**:
Ensures a philosopher can continue eating or stops if the simulation is finished.
Uses custom_usleep to simulate eating time.

**philo_died**:
Checks if a philosopher has died due to starvation.
Handles synchronization using multiple mutex locks to update shared states safely.

**print_state**:
Prints the current state of a philosopher (e.g., eating, thinking, sleeping).
Ensures thread-safe output using mutex locks.

**update_id_delay**:
Updates the delay mechanism for philosophers to avoid simultaneous actions.

**set_start_time**:
Sets the start time for a philosopher's activity.
