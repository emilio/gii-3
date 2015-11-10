simulation_time = [0 20];
initial_population = 2000;
close all;

CONSTANTS = [0.1 0.2 0.6];

for index = 1:numel(CONSTANTS)
    figure(index);
    k = CONSTANTS(index);

    % We use a lambda to access the constant
    ydot = @(t, y) y * k - .2;

    [t, y] = ode45(ydot, simulation_time, initial_population);

    final_population = y(size(y, 1));
    disp('Initial population: ');
    disp(initial_population);
    disp('Final population: ');
    disp(final_population);

    plot(t, y);
end
