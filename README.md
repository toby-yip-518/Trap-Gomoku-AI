# Gomoku AI Agent

Heuristic-based Gomoku (Five-in-a-Row) playing agent in C, ranked 
top 10 out of approximately 100 submissions in a class competition 
based on head-to-head win rate against classmate-designed agents.

## Approach

The agent evaluates each legal move along 4 directional axes 
(horizontal, vertical, two diagonals), detecting multiple threat 
patterns at each position:

- **Consecutive runs** — two, three, or four pieces in a row
- **Empty-end states** — whether each end of a run is open or 
  blocked, distinguishing live threats (open) from dead ones (closed)
- **Gap-skip patterns** — `X _ X` and `X X _ X` shapes that 
  represent latent threats requiring response
- **Double-space threats** — separated runs that combine into 
  attacks on the same axis

Each candidate move is scored as the sum of an **offensive component** 
(value of completing the agent's own patterns) and a **defensive 
component** (value of blocking the opponent's patterns). Score 
multipliers reflect Gomoku's first-move-advantage asymmetry.

## Heuristic Weights

Pattern weights and threat thresholds were tuned empirically through 
repeated matches against baseline agents. **Specific weight values are 
omitted from the public source** as the assignment is reused across 
academic terms; the structural logic remains visible.

## Build

```bash
# requires the assignment's `battle_base.h` header (course-provided framework)
gcc -O2 -o gomoku battle.c
./gomoku
```

## Architecture

```
play_games(step)
    │
    ├── enumerate legal moves
    │
    └── for each candidate position (x, y):
            │
            ├── evaluate_direction()  ── 4 axes × 2 polarities
            │       │
            │       └── count consecutive / empty-ends / gap patterns
            │
            ├── calculate_score()  ── offensive vs defensive weighting
            │
            └── aggregate threat counters across all directions
                
        select max-score move
```

## Future Work

A natural extension would be adding **minimax with alpha-beta pruning** 
on top of the current heuristic for deeper lookahead — the existing 
heuristic would serve as the leaf evaluation function. Other directions 
include transposition tables for repeated board states and iterative 
deepening to handle variable per-move time budgets.