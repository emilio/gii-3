

# RIP

The RIP algorithm uses the multicast dir `224.0.0.9` (it subscribes to that
group using IGMP). It works at UDP at the port 520.

Two types of message:

 * `REQUEST`:  The router requests information. This is done mainly when RIP
     starts to run (multicast), or in diagnostics.
 * `RESPONSE`: The router sends updates with the routing tables, either as
     a periodic update (multicast) or as a response to another router
     (unicast).

It stores the cost of reaching a network in a table, where a cost of 16
indicates the route is unreachable.

When a router starts, it request entering the multicast group (with `TTL = 1`).

The routing info is sent periodically via multicast each 30 seconds
approximately.

Router will update the routes table when:

 * It receives info to reach a destiny at less distance.
 * Always it receives info for a route from the next router (even when the
     distance is greater). This allows the router at distance 1 to inform of
     changes in network topology.

## Solution for router fall-down

TODO:

 * Split horizon
 * Split Horizon + Poison Reverse
 * Triggered updates
 * Garbage Collect Timer

# OSPF

Multicast group at `224.0.0.5`. It doesn't use neither TCP nor UDP, **it uses
a custom protocol directly over IP**.

Each router constructs the graph of the whole network to get the shortest path
to a given route.

A OSPF router has a unique identifier.

LSDB: Link Status Database

## The protocol

The OSPF has the following type of messages:

Hello
: It's used as a greeting, to allow a router to discover adjacent routers.

Database Description
: They contain descriptions of the topology. Since this can be large, it can be
spread in multiple messages, requiring acknowledgements.

Link State Request
: This is used to request information to another router from a portion of its
LSDB.

Link State Update
: This messages provide information about the state of certain links on the
LSDB, they're sent as a response as a *Link State Request*.

Link State Acknowledgement
: This message is used to provide acknowledging receipt of a Link State Update.

## Areas

OSPF is divided in areas.
