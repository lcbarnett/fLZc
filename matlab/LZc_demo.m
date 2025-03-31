
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% LZc demo script demonstrating capabilities of the toolbox
%
% Default parameters (may be overriden on command line)

defvar('s', '000101000101111010001010100010101000000010000010'); % input sequence

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Alphabet size and sequence length

a = length(unique(s));
n = length(s);

% Display string

fprintf('\ninput sequence = %s\n',s);
fprintf('\nalphabet size   = %3d\n',a);
fprintf(  'sequence length = %3d\n',n);

% Calculate LZ76 and LZ78

c76 = LZc(s,76);
c78 = LZc(s,78);

fprintf('\nLZ76c = %3d\n',c76);
fprintf(  'LZ78c = %3d\n',c78);

% Calculate again, returning dictionaries (string cell vectors)

[c76,d76] = LZc(s,76);
[c78,d78] = LZc(s,78);

fprintf('\nLZ76c dictionary: %s\n',  d76);
fprintf(  'LZ78c dictionary: %s\n\n',d78);

% Calculate again, returning vector of "running" complexities

cr76 = LZc(s,76,true);
cr78 = LZc(s,78,true);

% Normalise running complexities by random mean
%
% NOTE: Normalisation is by mean LZc for random sequences of the given length
%       and given alphabet, rather than the maximum LZc. Thus normalised LZc
%       can be greater than 1, especially for short sequences.

alln = (1:n)'; % the sequence lengths up to the maximum

crn76 = cr76./LZc_normfac(alln,a,76);
crn78 = cr78./LZc_normfac(alln,a,78);

% Display running complexities

figure(1); clf

sgtitle(sprintf('LZ76c and LZ78c: alphabet size = %d, sequence length = %d\n',a,n));

subplot(2,1,1);
plot(alln,[cr76 cr78]);
xlim([0,n+1]);
title('Unnormalised');
xlabel('sequence length');
legend('LZ76c','LZ78c');
grid on

subplot(2,1,2);
alln = (1:n)';
plot(alln,[crn76 crn78]);
xlim([0,n+1]);
ylim([0,1.05*max([max(crn76);max(crn78)])]);
title('Normalised');
xlabel('sequence length');
legend('LZ76c','LZ78c');
grid on
