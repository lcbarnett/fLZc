function cnorm = LZc_normalise(c,n,d,ver,asymp)

% Load random string mean complexities (and optionally standard deviations) for alphabet of size d
% and string lengths slen from file of values estimated by Monte Carlo simulation.

if nargin < 4 || isempty(asymp), asymp = true; end

assert(iscolumn(c),'LZc must be a column vector');
assert(isscalar(d) && isnumeric(d) && d == floor(d) && d > 1,'alphabet size must be an integer > 1');

if asymp
	n = length(c);
	nn = (1:n)';
	cnorm = c.*(log(nn)/log(d))./nn;
else
	error('Sorry, not implemented yet\n');
end
