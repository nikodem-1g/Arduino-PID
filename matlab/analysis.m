% ng912
% 18/11/2026

time = run25(:,1);
setpoint = run25(:,2);
error = abs(run25(:,3));
actual = run25(:,4);

%defined region allows comparison of error value across runs
errorlower = 77;
errorupper = 277;
error([1:errorlower-1, errorupper+1:end]) = NaN;


%upper = sp + 0.05*abs(sp);
%lower = sp - 0.05*abs(sp);

hold on
%fill([t; flipud(t)], [upper; flipud(lower)], "b", ...
%    "FaceAlpha", 0.2, "EdgeColor", "none")
plot(time, setpoint, "k--")
plot(time, actual, "b", "MarkerSize",2)
plot(time, error, "r:")
patch([time(105) time(105) time(306) time(306)],[setpoint(105)+9 setpoint(105) setpoint(306) setpoint(306)+9], [0.8 0.8 0.8], ...
    'FaceAlpha', 0.4, 'EdgeColor', 'none');
plot(time, setpoint, "k--")
plot(time, actual, "b")
plot(time, error, "r:")
hold off

%title(['180\circ step test (K_{p} = 75.0, K_{i} = 2.5, K_{d} = 0.05) \Sigma\epsilon = ', num2str(sum(error,"omitNaN")), '\circ' ], "FontSize",18)
legend({"Target position", "Actual position", "Error (\epsilon)", "+5% step overshoot band"})

set(gca, 'YGrid', 'on', 'XGrid', 'off')
set(gca,'XMinorTick','on','YMinorTick','off')
xlabel("Time (ms)","FontSize",14)
ylabel("Motor shaft position (\circ)", "FontSize",14)
yticks(0:30:360)
ylim([0 360])
xlim([2000 4500])
text(2010,7.5,'ng912', "FontWeight","bold", "FontSize",14)

