clear; close all; clc;

% fitting a (hopefully linear) function to the voltage vs analog data from the anemometer

voltages = [0.4 0.53 0.572 0.62 0.55 0.629 0.634 0.684 0.629 0.705 0.68 0.76 0.739 0.892 0.878 1.048 1.09 1.06];
analog_output = [85 114 122 133 118 135 137 147 135 152 147 163 159 193 188 227 233 230];

% plotting the raw data
subplot(2,1,1)
plot(voltages,analog_output,'.r','MarkerSize',20)
hold on

% fitting the data to a linear function
p = polyfit(voltages,analog_output,1);
a_output = @(v) p(1) * v + p(2);

% plotting the new function over the domain of the sensor (from 0.4 volts to 2.0 volts)
fplot(a_output, [0.4 2], 'b')
xlabel('v (Volts)')
ylabel('analog output')
legend('measured data', 'model')
hold on

% now creating a function to convert a_outputs to wind speeds (in m/s)
% 85 to 433 (348 subintervals) for analog_outputs corresponding to voltages from 0.4 to 2 volts
% 0 to 32.4 m/s wind speeds corresponding to voltages from 0.4 to 2 volts
% each output for wind speed should be something like 
% wind_speed(analog_data) = (32.4/348)*(analog_data-85)
wind_speed = @(analog_data) (32.4/348) * (analog_data-85);

% plot the wind speed over the interval of possible analog_output values
subplot(2,1,2)
fplot(wind_speed,[85 433])
xlabel('analog outputs')
ylabel('wind speed (m/s)')




