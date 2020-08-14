function [cm,cs,ixf,N,cdesc] = LZ76c_crand(n,d,asymp)

% Load random string mean complexities (and optionally standard deviations) for alphabet of size d
% and string lengthslength n from file of values estimated by Monte Carlo simulation. For string
% sizes larger than the largest on file, the asymptotic value n/log_d(n) is returned.

if nargin < 3 || isempty(asymp), asymp = 0; end

assert(isvector(n) && isnumeric(n) && all(n == floor(n)) && all(n > 0),'String lengths must be a vector of positive integers');
assert(isscalar(d) && isnumeric(d) && d == floor(d),'Alphabet size must be a scalar integer');

numn = length(n);

if asymp > 1 % asymp == 2 : return asymptotic for every value
	n = n(:);
	cm = n.*(log(d)./log(n));
	if nargout > 1
		cs    = nan(numn,1);
		if nargout > 2
			ixf   = [];
			N     = [];
			cdesc = [];
		end
	end
	return
end

global fLZc_data_path;
load(fullfile(fLZc_data_path,sprintf('LZ76c_rand_A%02d.mat',d)));

of = (n <= N)';    % on file
iof = find(of);    % indices on file
nof = n(iof);      % values on file
ixf = find(~of);   % indices not on file

cm = nan(numn,1);
cm(iof) = c76mean(nof);
if asymp > 0 % asymp == 1 : return asymptotic for out of range only
	nxf = n(ixf);      % values not on file
	cm(ixf) = nxf.*(log(d)./log(nxf));
else
	%          asymp == 0 : return NaNs in out of range
end

if nargout > 1
	cs      = nan(numn,1);
	cs(iof) = c76sdev(nof);
	% we don't know the asymptotic standard deviations; leave as NaNs
end
