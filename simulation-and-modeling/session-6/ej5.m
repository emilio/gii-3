hdot = @(t, x, A, Q, K) ((1 / A) * (Q - K * sqrt(x)));

simulation_interval = [0 1000];
initial_height = 2;

A = input('Base area (A):');
Q = input('Flow (Q):');
K = input('K:');

[t, h] = ode45(@(t, x) hdot(t, x, A, Q, K), simulation_interval, initial_height);

plot(t, h);
