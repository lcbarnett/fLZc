function cmax = LZc_cmax_x(nmax,d,use_mex) % max complexity for strings of length n, alphabet size d

assert(isscalar(nmax) && isnumeric(nmax) && nmax == floor(nmax),'Maximum string length must be a scalar integer');
assert(isscalar(d)    && isnumeric(d)    && d    == floor(d),   'Alphabet size must be a scalar integer');

if nargin < 3 || isempty(use_mex), use_mex = true; end

if use_mex

	cmax = LZc_mex_cmax_x(nmax,d);

else

	cmax = zeros(nmax,1);
	for n = 1:d
		if n > nmax, return; end
		cmax(n) = n;
	end
	g  = 0; % g_k(d)     = d + 2*d^2 + ... + k*d^k
	f  = 0; % f_{k-1}(d) = d + d^2 + ... + d^{k-1}
	dk = 1; % d^k
	k  = 1;
	while (1)
		dk = dk*d;
		f  = f+dk;
		g  = g+k*dk;
		k = k+1;
		for D = 0:((d*dk)-1)
			c = f + D;
			for n = g + k*D : g + k*(D+1)
				if n > nmax, return; end
				cmax(n) = c;
			end
		end
	end

end
