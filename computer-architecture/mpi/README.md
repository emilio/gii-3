# MPI practice

Practice for Computer Architecture -- 2016

## Requirements

In debian-based linuxes:

```
# apt-get install openmpi-bin openmpi-common libopenmpi-dev
```

## Details

This is a MPI application for the course of Computer architecture.

It's a C++ wrapper over the MPI C API.

### Why not using MPI C++ bindings?

The API is basically equal to the C API (full of `void*` and all that stuff).

I wanted a more high-level API in order to ease development.
