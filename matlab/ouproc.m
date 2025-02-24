function [x,t] = ouproc(a,s,fs,T)

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% Simulate stationary univariate Ornstein-Uhlenbeck process of length T (seconds),
% with coefficient a and noise std. dev. s, sampled at frequency fs (Hz).
%
% The pU rocess simulated is:
%
%     dx(t) = -ax(t)*dt + s*dw(t)
%
% where w(t) is a standard Wiener process.
%
% The subsampled process is AR(1), which is what we simulate.
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

assert(a >= 0,'The AR coefficient must be positive, or the process will be explosive');

A    = exp(-a/fs);          % AR(1) autoregression coefficient
omg  = s/sqrt(2*a);         % process std. dev.
sig  = sqrt(1-A^2)*omg;     % AR(1) residuals std. dev.
n    = round(fs*T)+1;       % number of samples (observations)
x    = sig*randn(n,1);      % initialise residuals (Gaussian white noise)
x(1) = omg*randn;           % initial value (t = 0)
for i = 2:n;
	x(i) = x(i) + A*x(i-1); % generate AR(1) process
end
t = (0:(n-1))'*(T/(n-1));   % time stamps
