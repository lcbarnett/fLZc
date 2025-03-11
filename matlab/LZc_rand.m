function [cmean,csdev] = LZc_rand(n,d,N,allvals,s)

% Estimate means and (optionally) standard deviations of LZc for random strings
% with alphabet size d of length n, based on a sample of size N. If the flag
% 'allvals' evaluates as true, then means and standard deviations are calculated
% for all lengths up to and including n.

assert(isscalar(n) && isnumeric(n) && n == floor(n) && n > 0,'string length must be a positive integer');
assert(isscalar(d) && isnumeric(n) && d == floor(d) && d > 1,'alphabet size must be an integer > 1');

if nargout > 1
	assert(isscalar(N) && isnumeric(n) && N == floor(N) && N > 1,'sample size must be an integer > 1');
else
	assert(isscalar(N) && isnumeric(n) && N == floor(N) && N > 0,'sample size must be a nonnegative integer');
end

if nargin < 4 || isempty(allvals)
	allvals = false;
else
	assert(isscalar(allvals),'''allvals'' must evaluate to a scalar logical');
end

if nargin < 5 || isempty(s)
	s = 0;
else
	assert(isscalar(s) && isnumeric(n) && s == floor(s) && s >= 0,'seed must be a non-negative integer');
end

if allvals
	if nargout > 1
		[cmean,csdev] = LZc_rand_x_mex(n,d,N,s);
	else
		cmean = LZc_rand_x_mex(n,d,N,s);
	end
else
	if nargout > 1
		[cmean,csdev] = LZc_rand_mex(n,d,N,s);
	else
		cmean = LZc_rand_mex(n,d,N,s);
	end
end
