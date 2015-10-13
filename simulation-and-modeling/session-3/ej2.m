
range = input('Introduce el rango a representar: ');

% TODO: Error checking, or allowing a vector directly

x = -range:range;

y = x.^3;

plot(x, y);
