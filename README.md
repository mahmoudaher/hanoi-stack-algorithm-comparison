# Towers of Hanoi: Recursive vs Iterative

A C++ console project for solving and comparing the Towers of Hanoi using:

- Recursive decomposition.
- Iterative movement with `std::stack`.
- Move counting and high-resolution execution timing.

## Projects

- `Rekürsif/Rekürsif Yöntem/Rekürsif Yöntem.sln`
  - Menu option 3 runs both algorithms for the same disk count and prints a comparison table.
  - Menu options 1 and 2 display the recursive solution step by step.
- `Iteratif/Iteratif.sln`
  - Runs the iterative solution and prints its move count and measured time.

## Measurement rules

Benchmark runs do not print individual moves. This keeps console I/O from dominating the algorithm measurement. Both algorithms must perform exactly `2^n - 1` moves.

The accepted disk range is 1 to 20. Larger values create a very large number of moves and are intentionally rejected.

## Build

Open either solution in Visual Studio 2022 and select `Debug` or `Release` with the `x64` platform.

The recursive project file points to its actual source file, `Rekürsif Yöntem.cpp`.

## Suggested repository name

`hanoi-stack-algorithm-comparison`
