dy1dt = @(t, y1t, y2t) y2t;
dy2dt = @(t, y2t, y1t, mu) mu * (1 - y1t^2) * y2t - y1t;
state = @(t, y, mu) [dy1dt(t, y(1), y(2)); dy2dt(t, y(2), y(1), mu)];

mu = .1;
simulation_time = [0 20];
initial_conditions = [2; 3];

[t, y] = ode45(@(t, y) state(t, y, mu), simulation_time, initial_conditions);

plot(t, y(:, 1));
