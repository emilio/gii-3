% Simulacion del muelle con una masa
% G(s) = 1 / (m * s ^ 2 + bs + k)

F = 10;
m = input('Masa (m): ');
b = input('Coeficiente de rozamiento (b): ');
k = input('Constante de restitucion (k): ');

t = 0:.5:60;

numerator = 1;
denominator = [m, b, k];

% Simulating with the same force during t time
y = 10 * step(numerator, denominator, t);
figure(1);
plot(t, y);

disp('Enter...');
pause();

% Simulating using a signal with period = 30
u = gensig('square', 30, 60, 0.5);
y = lsim(numerator, denominator, u, t);

figure(2);
plot(t, y);
