function cmean = LZc_rand(n,d,N,s)


assert(isscalar(n) && isnumeric(n) && n == floor(n) && n > 0,'n must be a positive integer');
assert(isscalar(d) && isnumeric(n) && d == floor(d) && d > 1,'d must be an integer > 1');
assert(isscalar(N) && isnumeric(n) && N == floor(N) && N > 0,'N must be a positive integer');
if nargin < 4 || isempty(s)
	s = 0;
else
	assert(isscalar(s) && isnumeric(n) && s == floor(s) && s >= 0,'s must be a non-negative integer');
end

cmean = LZc_rand_mex(n,d,N,s);
