
DIM = 3;

m = zeros(DIM, DIM);

for i = 1:DIM
	for j = 1:DIM
		m(i, j) = input([ 'm[' num2str(i) '][' num2str(j) ']: ']);
	end
end

disp('Matrix:');

m

disp('Inverse:');

inv(m)

disp('Squared:');

m * m

disp('Second row:');
m(2, :)

