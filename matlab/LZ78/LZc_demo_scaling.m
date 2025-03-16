% Demonstrate scaling with sequence length of maximum complexity strings and
% complexity of random string against the rough asymptotic log(d)*(maxn/log(maxn)
%
% Default parameters (may be overriden on command line)

defvar('maxn',       1000       ); % maximum sequence length
defvar('d',          2          ); % alphabet size

cmax = LZc_cmax_x(maxn,d);
crnd = LZc_crand(1:maxn,d);

n = (2:maxn)';
csca = log(d)*(n./log(n));
cmax = cmax(2:end)./csca;
crnd = crnd(2:end)./csca;

figure(1); clf
semilogx(n,[cmax crnd]);
title(sprintf('Scaling of maximum complexity and random\nstring complexity by log(d)*n/log(n)\n'));
xlabel('sequence length (log-scale)');
ylabel('scaled complexity');
leg = legend({'maximum','random'},'location','southeast');
leg.Title.Visible = 'on';
title(leg,sprintf('d = %d',d));
grid on
