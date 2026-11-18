% ng912
% 18/11/2026

time = run2(:,1);
setpoint = run2(:,2);
error = abs(run2(:,3));
actual = run2(:,4);

%defined region allows comparison of error value across runs
errorlower = 60;
errorupper = 180;
error([1:errorlower-1, errorupper+1:end]) = NaN;


%upper = sp + 0.05*abs(sp);
%lower = sp - 0.05*abs(sp);

hold on
%fill([t; flipud(t)], [upper; flipud(lower)], "b", ...
%    "FaceAlpha", 0.2, "EdgeColor", "none")
patch([time(errorlower) time(errorlower) time(errorupper) time(errorupper)],[0 360 360 0], [0.8 0.8 0.8], ...
    'FaceAlpha', 0.4, 'EdgeColor', 'none');
plot(time, setpoint, "k--")
plot(time, actual, "b")
plot(time, error, "r:")
hold off

title(['Run 2 (k_{p} = 5.0, k_{i} = 0.5, k_{d} = 0.05) \Sigma\epsilon =', num2str(sum(error,"omitNaN")), '\circ' ])
legend("Comparison region","Target angle", "Actual angle", "Error (\epsilon)")

set(gca, 'YGrid', 'on', 'XGrid', 'off')
xlabel("Time (ms)")
ylabel("Angle (\circ)")
yticks(0:30:360)
ylim([0 360])
xlim([2000 4000])
%text(6100,10,'ng912', "FontWeight","bold", "FontSize",15)

