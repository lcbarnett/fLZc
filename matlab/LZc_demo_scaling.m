% Demonstrate scaling with sequence length of maximum complexity strings and
% complexity of random string against the rough asymptotic log(d)*(maxn/log(maxn)
%
% Supply:
%
% maxn     % maximum sequence length
% d        % alphabet size
%
% e.g. (cut & paste)
%{
maxn = 100000; d = 5; LZc_demo_scaling
%}

cmax = LZc_cmax_x(maxn,d);
crnd = LZc_crand(maxn,d);

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
