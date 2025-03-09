function [cmean,csdev] = LZc_rand(n,d,N,s)

% Estimate means and (optionally) standard deviations of LZc for random strings
% with alphabet size d up to length n, based on a sample of size N.

assert(isscalar(n) && isnumeric(n) && n == floor(n) && n > 1,'n must be an integer > 1');
assert(isscalar(d) && isnumeric(n) && d == floor(d) && d > 1,'d must be an integer > 1');
if nargout > 1
	assert(isscalar(N) && isnumeric(n) && N == floor(N) && N > 1,'N must be an integer > 1');
else
	assert(isscalar(N) && isnumeric(n) && N == floor(N) && N > 0,'N must be an integer');
end
if nargin < 4 || isempty(s)
	s = 0;
else
	assert(isscalar(s) && isnumeric(n) && s == floor(s) && s >= 0,'s must be a non-negative integer');
end

if nargout > 1
	[cmean,csdev] = LZc_rand_mex(n,d,N,s);
else
	cmean = LZc_rand_mex(n,d,N,s);
end
