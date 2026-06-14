%% set constants
BW = 128;

%% set intensity
x = linspace(0, 5, 500);

%% calculate performance
y = BW*ones(size(x));
mask = (x>=0) & (x<=1);
y(mask) = BW .* x(mask);

%% plot roofline model
plot(x, y, 'LineWidth', 2);
xlabel('Intensity (FLOPs/Byte)');
ylabel('Performance (FLOPs/Cycle)');
title('Roofline Model');
grid on;
hold on;

%% plot achieved performance - intensity point
achieved_I = 4.57;
achieved_P = 65536.0/1710.0;
plot(achieved_I, achieved_P, 'ro', 'MarkerSize', 8, 'DisplayName', 'Achieved');
legend show;
hold off;