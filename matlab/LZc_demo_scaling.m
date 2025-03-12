% Demonstrate scaling with sequence length of maximum complexity strings and
% complexity of random string against the rough asymptotic log(d)*(maxn/log(maxn)
%
% Default parameters (may be overriden on command line)

defvar('n',       10000      ); % sequence length
defvar('d',       3          ); % alphabet size

s = char(randi(d,1,n)-1+'0'); % random string of length n with alphabet size d

c = LZc_normalise(LZc_x(s),d);

nn = (1:n)';

figure(1); clf
semilogx(nn,c);
title(sprintf('Asymptotic normalisation of random sequence\n'));
xlabel('sequence length (log-scale)');
ylabel('normalised complexity');
grid on
