% Circuit simulation
% G(s) = C*s / (R*C*s + 1)

C = input('Capacity (C): ');
R = input('Resistance (R): ');

t = 0:.5:60;

numerator = [C, 0];
denominator = [R * C, 1];

% Simulating with the same force during t time
y = step(numerator, denominator, t);
figure(1);
plot(t, y);

disp('Enter...');
pause();


% Simulating using a signal with period = 30
u = gensig('square', 30, 60, 0.5);
y = lsim(numerator, denominator, u, t);

figure(2);
plot(t, y);
