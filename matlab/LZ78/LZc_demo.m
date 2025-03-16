% LZc demo script: calculate LZ-complexity at all sequence lengths for a
% subsampled stationary Ornstein-Uhlenbeck process.
%
% Default parameters (may be overriden on command line)

defvar('T',       400       ); % length of process
defvar('q',       1         ); % quantisation (1 = binarise around median)
defvar('fs',      200       ); % sampling frequency (Hz)
defvar('a',       0.1       ); % OU process decay parameter (> 0); smaller a gives "smoother" process
defvar('sig',     1         ); % OU process noise std. dev.
defvar('nrmlz',   true      ); % Normalise LZc by random sequence mean?

if nrmlz, algostr = 'LZc (normalised)'; else, algostr = 'LZc';   end

% Generate subsampled Ornstein-Uhlenbeck time series data

fprintf('\ngenerating stationary OU time series... ');
[x,t] = ouproc(a,sig,fs,T);
fprintf('done\n\n');
maxn = length(x);

% Calculate complexities for different quantisation levels, and load random string complexities


fprintf('calculating %s... ',algostr);
st = tic;
d = q+1;                          % alphabet size = number of quantiles + 1
[s,qtiles] = LZc_quantise(x,q);   % quantise noise sequence by q quantiles; store quantiles
c = LZc_x(s);                     % calculate "running" LZ complexity (i.e., for all sequence lengths to maximum)
if nrmlz
	c = c./LZc_crand(maxn,d);     % scale by random string mean complexities
end
et = toc(st);
fprintf('done (%g seconds)\n\n',et);

if nrmlz && isnan(c(end))
	fprintf(2,'WARNING: sequence rather long - couldn''t normalise for all lengths\n\n');
end

% Display time series with quantiles

figure(1); clf

subplot(3,1,1);
xmax = 1.05*max(abs(x));
plot(t,x);
for k = 1:q
	yline(qtiles(k),'color','r');
end
xlim([0,T]);
ylim([-xmax xmax]);
xlabel('Time (secs)');
title(sprintf('Subsampled OU process (%d quantiles)',q));
grid on

% Power spectral density (PSD)

[S,f] = pwelch(x,[],[],[],fs);                    % estimated PSD (Welch method)
ST    = abs(1./(1-exp(-(a+2*pi*1i*f)/fs))/fs).^2; % theoretical PSD

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
if nrmlz
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
