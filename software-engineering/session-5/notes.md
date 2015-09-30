# Activity diagram

An **activity** is what we've been calling a use case.

An **action** is each step taken to fullfill the action.

In an activity diagram each rounded square is an action, and each square is data
(probably an instance of a class).

## Uses

* Capture the actions realize.
* Capture the internal work of an object.
* Show how a set of related actions can be made and how they affect the objects.
* ...

## A bit more specific

### Actions

An action is an **executable node**, which is indivisible.

An action can have control **flow arrows** to specify data from and to other
nodes.

The termination of the execution of an action can allow the execution of nodes
after it.

They can have **pre and post-conditions**, which are specified inside a note
near the action.

## Type of nodes in an Activity diagram

### Object nodes

They are **data nodes**. With sharp edges.

### Control nodes

They manage the **control flow and objects**.

### Executable nodes

The're the actions, which can be:

* Primitive functions (calculate something or accessing the memory).
* Invocation of behavior.
* Signals and communication
* Data manipulation

## Moar detail

### Tokens

Vehicles to move information and eventsacross the system. They can transport
objects or values.

### Activity arcs

They are connections between activity node.

Tokens flow across them.

They are represented as an arrow with an optional name. There's extra notation
for weight and interruption representation.

### Decision node

One arc in, multiple out.
Each token that arrives to a decision node can only flow across one of the out
arcs.

The guard conditions in the out arcs determine which arc must be used.

### Merge node

Control node with multiple arcs in.

### Fork node

Control node which divides a flow in multiple concurrent flows.

For each token that arrives to a fork node, a copy is passed to every out arrow
of the node.

### Join node

Waits for multiple concurrent flows and has only an out arc.

### Initial node

Self-descriptive

### Final node

// TODO

### Object nodes

Runtime values of an object node in a determinate state.

// TODO (*set of*, *signal node*,...)

### Pin nodes

They represent an input or an output of an action.

They can have the restrictions `{stream}` or `{nonStream}`.

**Note**: They should be specified always that relevant data is passed to the
action.

### Parameter nodes

They're used to use to provide inputs and output to an activity..

When an activity is invoked the input values are stored as token in the input
value nodes.
