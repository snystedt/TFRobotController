function servo_angles_3

theta = 0:1:120;
phi = 10:1:180;

L1 = 27;
L2 = 27;
h0 = 10;
y_claw = 8.5;

XY = @(theta,phi) [L1*cosd(theta) - L2*cosd(theta+phi);...
    L1*sind(theta) - L2*sind(theta+phi) + h0];

points = zeros(4, length(theta)*length(phi));

i = 1;
for t = theta
    for p = phi
        points(1:2,i) = XY(t, p);
        points(3,i) = t;
        points(4,i) = p;
        i = i + 1;
    end
end

figure(1);
clf;
hold on;
rectangle('Position', [20, 0, 20, 30]);
plot(points(1,:), points(2,:), 'r.');