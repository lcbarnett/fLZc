function cmax = LZc_cmax(n,d,use_mex) % max complexity for strings of length n, alphabet size d

assert(isscalar(n) && isnumeric(n) && n == floor(n),'String length must be a scalar integer');
assert(isscalar(d) && isnumeric(d) && d == floor(d),'Alphabet size must be a scalar integer');


if nargin < 3 || isempty(use_mex)
	use_mex = true;
end

if use_mex

	cmax = LZc_mex_cmax(n,d);

else

	if nargout > 1
		g  = 0; % g_k(d)     = d + 2*d^2 + ... + k*d^k
		f  = 0; % f_{k-1}(d) = d + d^2 + ... + d^{k-1}
		dk = 1; % d^k
		k  = 1;
		while true
			dk = dk*d;
			n0 = g;              % the D = k' = 0 string length
			g  = g+k*dk;
			if g > n, break; end % overshot (note k has also overshot!)
			f = f+dk;
			k = k+1;
		end
		cmax = f + floor((n-n0)/k); % f_{k-1}(d) + D, D = floor((n-g_{k-1}(d))/k)
		%k = k-1; % overshot, so decrement k
	end

end
