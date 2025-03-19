% LZc demo script: calculate LZ-complexity at all sequence lengths for a
% subsampled stationary Ornstein-Uhlenbeck process.
%
% Default parameters (may be overriden on command line)

defvar('lzcver',  76        ); % LZc version: 76 or 78
defvar('T',       400       ); % length of process
defvar('a',       2         ); % alphabet size (a = 2 for binarisation around median)
defvar('fs',      200       ); % sampling frequency (Hz)
defvar('oudec',   0.1       ); % OU process decay parameter (> 0); smaller oudec gives "smoother" process
defvar('sig',     1         ); % OU process noise std. dev.
defvar('lzcn',    1         ); % LZc normalisation: 0 - none; 1 - random mean; 2 asymptotic upper bound

switch lzcver
	case 76, if lzcn > 0, algostr = 'LZ76c (normalised)'; else, algostr = 'LZ76c'; end
	case 78, if lzcn > 0, algostr = 'LZ78c (normalised)'; else, algostr = 'LZ78c'; end
	otherwise, error('LZc version must be 76 or 78');
end

% Generate subsampled Ornstein-Uhlenbeck time series data

fprintf('\ngenerating stationary OU time series... ');
[x,t] = ouproc(oudec,sig,fs,T);
fprintf('done\n\n');
maxn = length(x);

% Calculate complexities and load random string complexities

fprintf('calculating %s... ',algostr);
st = tic;
[s,qtiles] = LZc_quantise(x,a-1); % quantise noise sequence by q = a-1 quantiles; store quantiles
c = LZc_x(s,lzcver);              % calculate "running" LZ complexity (i.e., for all sequence lengths to maximum)
switch lzcn
	case 0, % do nothing
	case 1, cm = LZc_normfac((1:maxn)',a,lzcver,false); c = c./cm;
	case 2, cm = LZc_normfac((1:maxn)',a,lzcver,true ); c = c./cm;
	otherwise, error('Bad normalisation specification: ''lzcn'' must be 0, 1 or 2');
end
et = toc(st);
fprintf('done (%g seconds)\n\n',et);

if lzcn && isnan(c(end))
	fprintf(2,'WARNING: sequence rather long - couldn''t normalise for all lengths\n\n');
end

% Display time series with quantiles

figure(1); clf

subplot(3,1,1);
xmax = 1.05*max(abs(x));
plot(t,x);
for k = 1:a-1
	yline(qtiles(k),'color','r');
end
xlim([0,T]);
ylim([-xmax xmax]);
xlabel('Time (secs)');
title(sprintf('Subsampled OU process (alphabet size = %d)',a));
grid on

% Power spectral density (PSD)

[S,f] = pwelch(x,[],[],[],fs);                    % estimated PSD (Welch method)
ST    = abs(1./(1-exp(-(oudec+2*pi*1i*f)/fs))/fs).^2; % theoretical PSD

% Display PSD and normalised complexities

subplot(3,1,2);
semilogx(f,log([S ST]));
xlim([f(2) fs/2]); % to Nyqvist frequency
title(sprintf('Power spectral density (f_s = %gHz)',fs));
legend('estimated','theoretical');
xlabel('Frequency (Hz; log-scale)');
ylabel('Log-power (dB)');
set(gca,'XTickLabel',num2str(get(gca,'XTick')')); % ridiculous faff to force sensible tick labels
grid on

subplot(3,1,3);
if lzcn > 0
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
