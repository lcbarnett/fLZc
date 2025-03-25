
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% LZc demo script demonstrating capabilities of the toolbox
%
%
% Default parameters (may be overriden on command line)

defvar('s',      '000101000101111010001010100010101000000010000010'); % input sequence
defvar('cnorm',   1         ); % LZc normalisation: 0 - none; 1 - random mean; 2 asymptotic upper bound

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% alphabet size

a = length(unique(s));


% Display string
fprintf('\ninput sequence = %s\n',s);
fprintf('\nalphabet size = %d\n',a);

% Calculate LZ76 and LZ78

c76 = LZc(s,76);
c78 = LZc(s,78);

fprintf('\nLZ76c = %3d\n',c76);
fprintf(  'LZ78c = %3d\n',c78);

% Calculate again, return dictionaries (string cell vectors)

[c76,d76] = LZc(s,76);
[c78,d78] = LZc(s,78);

fprintf('\nLZ76c dictionary:\n'); disp(d76);
fprintf(  'LZ78c dictionary:\n'); disp(d78);

% Calculate again, return vector of "running" complexities

cr76 = LZc(s,76,true);
cr78 = LZc(s,78,true);

fprintf('LZ76c (running) = '); disp(cr76');
fprintf('LZ78c (running) = '); disp(cr78');

% Normalise (running) complexities by random mean

crn76 = cr76./LZc_normfac(cr76,a,76);
crn78 = cr78./LZc_normfac(cr78,a,78);

fprintf('LZ76c (normalised) = '); disp(crn76');
fprintf('LZ78c (normalised) = '); disp(crn78');
