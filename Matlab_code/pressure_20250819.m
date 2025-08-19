clear all; clc; close all;

pressures = readmatrix("20250819 test pressures.csv");
pressures = pressures(7000:end);

t = 1:length(pressures);

y = fft(pressures);
m = abs(y);
y(m<1e-6) = 0;
p = unwrap(angle(y));

f = (0:length(y)-1)*100/length(y);        % Frequency vector

subplot(2,1,1)
plot(f,m)
title('Magnitude')
ax = gca;
ax.XTick = [15 40 60 85];

subplot(2,1,2)
plot(f,p*180/pi)
title('Phase')
ax = gca;
ax.XTick = [15 40 60 85];

yspec = pressures(10000:10100);
figure(2);
plot(yspec)