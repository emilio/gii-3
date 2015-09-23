NOTE: part missing here.

# BDI: Beliefs, Desires and Intentions
Relevant links:
* [Belief–desire–intention software model on Wikipedia](https://en.wikipedia.org/wiki/Belief%E2%80%93desire%E2%80%93intention_software_model)

## Beliefs
A belief is an **environment representation**:
* Variable values.
* Relational databases.
* Symbolic expressions (`p -> q`).

It's **different from knowledge** (a belief may be false).

Need to store state.

## Desires
A *desire* is a **final state** that is our objective.

## Intentions
Planned sequence of actions that allows the agent to reach a whished state.
They can be interrupted if the environment changes.

# Case Based Reasoning
You **apply or not an action depending of the previous experiences** stored.

A value judgement is needed to determine if the result of an experience
was positive or negative.

A *case* is defined as a tuple `<P, S(p), R>` where:
* `P` is the initial state.
* `S(P)` is a sequence of `<action, state>` that descrive the steps taken.
* `R` is the result (positive or negative, maybe with intermediate values).

Example steps:
* Retrieve: Get previous results similar to the current problem.
* Reuse: Obtain a solution based on the previous results.
* Revise: See if the solution is appropiated ti our problem.
* Retain: Learn from the experience (retain the result).

