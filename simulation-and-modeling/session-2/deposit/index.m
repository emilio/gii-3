t0 = 0;
tf = 1000;
h0 = 0;

[t, h] = ode45('dhdt', [t0, tf], h0);

plot(t, h);
