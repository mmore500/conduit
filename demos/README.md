Example mini-projects using Conduit.
Source code can be found in `demos/`.
Several are used as diagnostic benchmarks in [our publications](../docs/publications.md).

## Channel Selection Demo

Simulation components communicate with neighbors using a distributed algorithm inspired by WLAN channel selection to solve a graph coloring problem.

> Leith, Douglas J., et al. "WLAN channel selection without communication." Computer Networks 56.4 (2012): 1424-1441.

## Consensus Demo

In this demo, simulation components start out with a unique initial state.
Components broadcast their state to neighbors.
Simulation components then listen for state broadcast from neighbors then update their state to match the plurality of neighbor state.
If there is no plurality among neighbor states, simulation components update to an arbitrary neighbors' state.

## Diffusion Demo

In this demo, simulation components exchange a continuous-valued substance.
Simulation component 0 is fixed to a substance quantity of 1.0.
Simulation component `n-1` is fixed to a substance quantity of 0.0.
Each update, simulation components dispatch a fraction of their resource to neighbors.

## SignalGP Demo

Simulation components each house a SignalGP virtual CPU with a randomly-generated program.
Virtual CPUs are executed and allowed to exchange messages with CPUs in neighbor simulation components.

> Lalejini, Alexander, and Charles Ofria. "Evolving event-driven programs with SignalGP." Proceedings of the Genetic and Evolutionary Computation Conference. 2018.
