#!/bin/tcsh
foreach x (`seq 100000 100000 1000000`) 
	generator $x > input_bob$x
end
