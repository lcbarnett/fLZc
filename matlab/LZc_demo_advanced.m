
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% LZc demo script: calculate and plot LZ76c and LZ78c at all sequence lengths
% for a subsampled stationary zero-mean Ornstein-Uhlenbeck process.
%
%
% Default parameters (may be overriden on command line)

defvar('T',       600       ); % length of process (seconds)
defvar('fs',      200       ); % sampling frequency (Hz)
defvar('oudec',   0.1       ); % OU process decay parameter (> 0); set to Inf for white noise
defvar('a',       2         ); % LZc alphabet size (a = 2 for binarisation around median or "mean" for binarisation around the mean)
defvar('cnorm',   1         ); % LZc normalisation: 0 - none; 1 - random mean; 2 asymptotic upper bound
defvar('logs',    false     ); % display sequence length on a log scale?

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Generate subsampled Ornstein-Uhlenbeck time series data

fprintf('\ngenerating stationary OU time series... ');
[x,t] = ouproc(oudec,1,fs,T);
maxn = length(x);
alln = (1:maxn)';
fprintf('done (%d observations)\n\n',maxn);

% Discretise time series (quantise)

if strcmpi(a,'mean') % binarise around the mean
	a = 2;
	q = mean(x);
	[s,qtiles] = LZc_quantise(x,q,false); % binarise around the mean
else
	q = a-1;
	[s,qtiles] = LZc_quantise(x,q,true);  % quantise sequence by q quantiles (q = 1 for binarisation around the median)
end

% Calculate complexities (we calculate "running" LZ complexity (i.e., for all sequence lengths to maximum))

fprintf('calculating LZ76c... ');
st = tic;
c76 = LZc(s,76,true); % vector of all LZc from beginning of string
et = toc(st);
fprintf('done (%g seconds)\n',et);

fprintf('calculating LZ78c... ');
st = tic;
c78 = LZc(s,78,true); % vector of all LZc from beginning of string
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
cmax = max([max(c76);max(c78)]);
plot(t,[c76 c78]);
ylim([0 1.05*cmax]);
xlim([0,T]);
title(sprintf('LZ76c and LZ78c (%s)\n\nalphabet size = %d, %d observations\n',normstr,a,maxn));
if logs
	xscale('log');
	xlabel('Time (seconds; log-scale)');
else
	xlabel('Time (seconds)');
end
if cnorm > 0
	yline(1,'color','k');
	ylabel('Complexity');
else
	yscale('log');
	ylabel('Complexity (log-scale)');
end
legend('LZ76c','LZ78c');
grid on
