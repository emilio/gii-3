close all;

dx1dt = @(t, x1t) 2 * x1t - .3;
dx2dt = @(t, x2t, x1t) x2t - 3 * x1t -1;
state = @(t, x) [dx1dt(t, x(1)); dx2dt(t, x(2), x(1))];

x_0 = [2; 7];
simulation_time = [0 10];

[t, x] = ode45(state, simulation_time, x_0);
plot(t, x(:, 1), t, x(:, 2));
