# Modeling of representation

## Parametric identification

We experiment with a set of real data to get the values from the equation.

We usually don't experiment with the whole set, but just with half of it (for
example), and check the values with the other half to know the amount of error
we've been using.

## Transference function

See the notes on `theory-2` to see how to apply it. Basically you apply the
Laplace transform to everything, simplify (given that the initial state is equal
to 0), and get `G(s)`, which is equal to `Y(s)/U(s)`, which is the transference
function in terms of `s`.

