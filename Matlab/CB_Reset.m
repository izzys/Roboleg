%Reset
function CB_Reset()
global    h

delete(h.Sine_PLOT);
delete(h.Furier_PLOT);

h.Sine_PLOT = axes('Position', [0.05 0.2 0.4 0.5]);
h.Furier_PLOT = axes('Position', [0.55 0.2 0.4 0.5]);
grid(h.Sine_PLOT,'on');
grid(h.Furier_PLOT,'on');
hold(h.Sine_PLOT,'on');
hold(h.Furier_PLOT,'on');