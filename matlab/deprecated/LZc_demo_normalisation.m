% LZc demo script: calculate LZ-complexity at all sequence lengths for a
% subsampled stationary Ornstein-Uhlenbeck process.
%
% Default parameters (may be overriden on command line)

defvar('maxn',    10000     ); % length of sequence
defvar('a',       2         ); % LZc alphabet size (a = 2 for binarisation around median)
defvar('logs',    false      ); % display sequence length on a log scale?

alln = (1:maxn)';

cm76r = LZc_normfac(alln,a,76,false);
cm78r = LZc_normfac(alln,a,78,false);

cm76a = LZc_normfac(alln,a,76,true);
cm78a = LZc_normfac(alln,a,78,true);

c76 = cm76r./cm76a;
c78 = cm78r./cm78a;

% Display complexity vs time (sequence length)

figure(1); clf
cmax = max([max(c76);max(c78)]);
plot(alln,[c76 c78]);
ylim([0 1.05*cmax]);
yline(1,'color','k');
xlim([1,maxn]);
title(sprintf('LZ76c and LZ78c normalisation:\n\nalphabet size = %d, %d observations\n',a,maxn));
if logs
	xscale('log');
	xlabel('Sequence length (log-scale)');
else
	xlabel('Sequence length');
end
ylabel('Complexity');
legend('LZ76c','LZ78c');
grid on
