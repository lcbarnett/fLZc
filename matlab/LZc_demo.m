% LZc demo script: calculate LZ-complexity at all sequence lengths for a
% subsampled stationary Ornstein-Uhlenbeck process.
%
% Default parameters (may be overriden on command line)

defvar('T',       600       ); % length of process (seconds)
defvar('fs',      200       ); % sampling frequency (Hz)
defvar('oudec',   0.1       ); % OU process decay parameter (> 0); set to Inf for white noise
defvar('sig',     1         ); % OU process noise std. dev.
defvar('a',       2         ); % LZc alphabet size (a = 2 for binarisation around median)
defvar('cnorm',   1         ); % LZc normalisation: 0 - none; 1 - random mean; 2 asymptotic upper bound

% Generate subsampled Ornstein-Uhlenbeck time series data

fprintf('\ngenerating stationary OU time series... ');
[x,t] = ouproc(oudec,sig,fs,T);
maxn = length(x);
alln = (1:maxn)';
fprintf('done (%d observations)\n\n',maxn);

% Discretise time series (quantise)

[s,qtiles] = LZc_quantise(x,a-1); % quantise noise sequence by q = a-1 quantiles; store quantiles

% Calculate complexities (we calculate "running" LZ complexity (i.e., for all sequence lengths to maximum))

fprintf('calculating LZ76c... ');
st = tic;
c76 = LZc_x(s,76);
et = toc(st);
fprintf('done (%g seconds)\n',et);

fprintf('calculating LZ78c... ');
st = tic;
c78 = LZc_x(s,78);
et = toc(st);
fprintf('done (%g seconds)\n\n',et);

% Normalise as requested

switch cnorm
	case 0, % do nothing
		normstr = 'unnormalised';
	case 1
		normstr = 'normalised by random sequence complexity';
		cm76 = LZc_normfac(alln,a,76,false);
		cm78 = LZc_normfac(alln,a,78,false);
		c76 = c76./cm76;
		c78 = c78./cm78;
	case 2
		normstr = 'normalised by asymptotic upper bound';
		cm76 = LZc_normfac(alln,a,76,true);
		cm78 = LZc_normfac(alln,a,78,true);
		c76 = c76./cm76;
		c78 = c78./cm78;
	otherwise, error('Bad normalisation specification: ''cnorm'' must be 0, 1 or 2');
end
if cnorm > 0
	if isnan(c76(end)), fprintf(2,'WARNING: sequence rather long - couldn''t normalise LZ76c for all lengths\n\n'); end
	if isnan(c78(end)), fprintf(2,'WARNING: sequence rather long - couldn''t normalise LZ78c for all lengths\n\n'); end
end

figure(1); clf

% Display time series with quantiles

subplot(2,1,1);
xmax = 1.05*max(abs(x));
plot(t,x);
for k = 1:a-1, yline(qtiles(k),'color','r'); end
xlim([0,T]);
ylim([-xmax xmax]);
xlabel('Time (seconds)');
title(sprintf('Ornstein-Uhlenbeck process (sampling frequency = %gHz)\n',fs));
grid on

% Display complexity vs time (sequence length)

subplot(2,1,2);
if cnorm > 0
	semilogx(t,[c76 c78]);
	ylim([0 1.2]);
	yline(1,'color','k');
	ylabel('Complexity');
else
	loglog(t,[c76 c78]);
	ylabel('Complexity (log-scale)');
end
legend('LZ76c','LZ78c');
xlim([1/fs,T]);
title(sprintf('LZ76c and LZ78c (%s)\n\nalphabet size = %d, %d observations\n',normstr,a,maxn));
xlabel('Time (seconds; log-scale)');
grid on
