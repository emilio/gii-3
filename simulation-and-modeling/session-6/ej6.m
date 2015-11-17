dtdt = @(t, T, F, Fj, V, Ti, Tj, UA, rhocp) ((F/V) * (Ti - T) + UA * (Tj - T) / (V * rhocp));
dtjdt = @(t, Tj, F, V, Vj, T, Tji, UA, rhocpj) ((Fj/Vj) * (Tji - Tj) - UA * (Tj - T) / (Vj * rhocpj));

tdot = @(t, x, V, Vj, F, Fj, Ti, Tji, UA, rhocp, rhocpj) [dtdt(t, x(1), F, Fj, V, Ti, x(2), UA, rhocp); dtjdt(t, x(2), F, V, Vj, x(1), Tji, UA, rhocpj)];

F = 1;
Fjs = 1.5;
delFj = 0;
Fj = Fjs + delFj;
Ti = 50;
Tji = 200;
V = 10;
Vj = 1;
rhocp = 61.3;
rhocpj = 61.3;
UA = 183.9;

simulation_interval = [0 5];
initial_state = [124.6525; 154.9880];


[t, T] = ode45(@(t, x) tdot(t, x, V, Vj, F, Fj, Ti, Tji, UA, rhocp, rhocpj), simulation_interval, initial_state);

plot(t, T(:, 1));
