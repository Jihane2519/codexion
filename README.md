*This project has been created as part of the 42 curriculum by jbounou.*

# Codexion

## Description

Codexion simulates coders competing for shared USB dongles to compile quantum code.
Each coder repeatedly compiles (requires two dongles), debugs, and refactors.
The simulation stops when a coder burns out or all coders finish their required compilations.

---

## Instructions

```bash
make
./codexion <nb_coders> <burnout_ms> <compile_ms> <debug_ms> <refactor_ms> <nb_compiles> <cooldown_ms> <fifo|edf>
```

Example:
```bash
./codexion 4 800 200 100 100 3 50 fifo
```

---

## Blocking cases handled

- **Deadlock prevention**: even coders take right dongle first, odd coders take left first + 1ms delay at startup.
- **Starvation prevention**: FIFO and EDF schedulers ensure fair dongle access via a priority heap.
- **Cooldown handling**: dongles check their release timestamp before being granted.
- **Burnout detection**: monitor checks every 1ms and logs within 10ms of actual burnout.
- **Log serialization**: `pause_print` mutex prevents interleaved output.

---

## Thread synchronization mechanisms

- `pause_dongle`: protects each dongle's state (is_taken, release, queue).
- `pause_print`: serializes all printf calls.
- `pause`: protects `simulation_running` and coder state (last_compile, done).

---

## Resources

- [POSIX Threads](https://hpc-tutorials.llnl.gov/posix/)
- [Dining Philosophers Problem](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
- AI was used to explain concurrency concepts, and review code logic.