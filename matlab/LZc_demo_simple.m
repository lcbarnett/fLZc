% LZc demo script: calculate LZ-complexity at all sequence lengths for a
% subsampled stationary Ornstein-Uhlenbeck process.
%
% Default parameters (may be overriden on command line)

defvar('lzcver',  76        ); % LZc version: 76 or 78
defvar('T',       400       ); % length of process
defvar('a',       2         ); % alphabet size (a = 2 for binarisation around median)
defvar('fs',      200       ); % sampling frequency (Hz)
defvar('oudec',   0.1       ); % OU process decay parameter (> 0); set to Inf for white noise
defvar('sig',     1         ); % OU process noise std. dev.
defvar('cnorm',   1         ); % LZc normalisation: 0 - none; 1 - random mean; 2 asymptotic upper bound

% Which LZc version?

switch lzcver
	case 76, if cnorm > 0, algostr = 'LZ76c (normalised)'; else, algostr = 'LZ76c'; end
	case 78, if cnorm > 0, algostr = 'LZ78c (normalised)'; else, algostr = 'LZ78c'; end
	otherwise, error('LZc version must be 76 or 78');
end

% Generate subsampled Ornstein-Uhlenbeck time series data

fprintf('\ngenerating stationary OU time series... ');
[x,t] = ouproc(oudec,sig,fs,T);
maxn = length(x);
fprintf('done (%d observations)\n\n',maxn);

% Discretise time series (quantise)

[s,qtiles] = LZc_quantise(x,a-1); % quantise noise sequence by q = a-1 quantiles; store quantiles

% Calculate complexities

fprintf('calculating %s... ',algostr);
st = tic;
c = LZc_x(s,lzcver);              % calculate "running" LZ complexity (i.e., for all sequence lengths to maximum)
et = toc(st);
fprintf('done (%g seconds)\n\n',et);

% Normalise as requested

switch cnorm
	case 0, % do nothing
	case 1, cm = LZc_normfac((1:maxn)',a,lzcver,false); c = c./cm;
	case 2, cm = LZc_normfac((1:maxn)',a,lzcver,true ); c = c./cm;
	otherwise, error('Bad normalisation specification: ''cnorm'' must be 0, 1 or 2');
end
if cnorm > 0 && isnan(c(end))
	fprintf(2,'WARNING: sequence rather long - couldn''t normalise for all lengths\n\n');
end

figure(1); clf

% Display time series with quantiles

subplot(2,1,1);
xmax = 1.05*max(abs(x));
plot(t,x);
for k = 1:a-1, yline(qtiles(k),'color','r'); end
xlim([0,T]);
ylim([-xmax xmax]);
xlabel('Time (secs)');
title(sprintf('Subsampled OU process (alphabet size = %d)',a));
grid on

% Display complexity vs time (sequence length)

subplot(2,1,2);
if cnorm > 0
	semilogx(t,c);
	ylim([0 1.2]);
	yline(1,'color','k');
	ylabel('Complexity');
else
	loglog(t,c);
	ylabel('Complexity (log-scale)');
end
xlim([1/fs,T]);
title(algostr);
xlabel('Time (seconds; log-scale)');
grid on
