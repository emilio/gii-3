function [first_plus_last_row, sum_of_last_row] = ej3(in)
	[m, n] = size(in);

	if n < 3 || m < 3 % || n ~= m
		error('Matrix not valid, must be at least 3x3');
	end

	% NOTE: Output is for debugging
	disp('Matrix: ');
	in

	disp('First plus last row: ');
	first_plus_last_row = in(1, :) + in(n, :)

	% NOTE: It's easier to use:
	% sum_of_last_row = sum(in(n, :))
	%
	% But we're not allowed to do it, so...
	sum_of_last_row = 0;
	for i = 1:m
		sum_of_last_row = sum_of_last_row + in(n, i);
	end

	disp('Sum of the last row: ');
	sum_of_last_row
end
