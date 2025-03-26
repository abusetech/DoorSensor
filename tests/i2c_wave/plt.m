raw = csvread("out.csv");
cols = raw(2:end, :);
x = cols(:, 1);
scl = cols(:, 6);
sda = cols(:, 5);
plot(x, scl, "-", "linewidth", 3, x, sda * 0.8, "-x");
title("I2C Master Output");
pause;