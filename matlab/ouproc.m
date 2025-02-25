function [x,t] = ouproc(a,s,fs,T,x0)

% Generate time series sampled from a univariate Ornstein-Uhlenbeck process
%
% The OU process is:
%
%     dx(t) = -ax(t)*dt + s*dw(t)
%
% where w(t) is a standard Wiener process.
%
% a    OU decay parameter (a > 0 for stability)
% s    OU residual noise std. dev.
% fs   sampling rate (Hz)
% T    simulation time (seconds)
% x0   initial value (empty to sample from stationary distribution)
%
% x    time series
% t    time stamps for time series (note: times run from 0 .. T)

assert(a >= 0,'The OU decay parameter must be positive, or the process will blow up!');

% A strooboscopically subsampled OU process is AR(1), which is what we generate.

A    = exp(-a/fs);           % AR(1) autoregression coefficient
sdp  = s/sqrt(2*a);          % AR(1) process std. dev. at stationarity
sdr  = sqrt(1-A^2)*sdp;      % AR(1) residuals std. dev.
n    = round(fs*T)+1;        % number of samples (observations)
x    = sdr*randn(n,1);       % initialise residuals (Gaussian white noise)
if nargin < 5 || isempty(x0) % no initial value supplied - draw from stationary distribution
	x(1) = sdp*randn;        % initial value (t = 0)
else
	x(1) = x0;               % initial value (t = 0)
end
for i = 2:n;
	x(i) = x(i) + A*x(i-1);  % generate AR(1) process
end
t = (0:(n-1))'*(T/(n-1));    % time stamps
