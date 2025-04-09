function [cm,ns,cv,cx] = LZc_normfac(n,a,ver,asymp)

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% LZc normalisation factors.
%
% Returns means and (optionally) variances of LZc for random sequences with the supplied
% lengths and alphabet size from data files, as available. Binary normalisation data may
% be downloaded from:
%
%    http://users.sussex.ac.uk/~lionelb/downloads/fLZc_data.zip
%
% and should be unzipped into fLZc_data_path (see startup.m). If requested values are
% not available, NaNs are returned.
%
% DEPRECATED AS NOT FIT FOR PURPOSE: If 'asymp' is true, then returns theoretical asymptotic
% upper bound values.
%
% INPUT
%
% n        vector of sequence lengths
% a        alphabet size
% ver      LZc version: 76 or 78
% asymp    DEPRECATED (flag for asymptotic vs. random normalisation)
%
% OUTPUT
%
% cm       random sequence mean LZc (or theoretical ceilings if asymp = false)
% ns       random sequence sample size
% cv       random sequence variances, if available
% cx       random sequence maxima, if available
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

assert(isvector(n) && isnumeric(n) && all(n == floor(n)) && all(n > 0),'String lengths must be a vector of positive integers');
assert(isscalar(a) && isnumeric(a) && a == floor(a)  && a > 1,'Alphabet size must be an integer > 1');
assert(ver == 76 || ver == 78,'Lempel-Ziv version must be 76 or 78');

if nargin > 3 && asymp
	error('\nAsymptotic bounds deprecated as not fit for purpose; they are unacceptably poor, especially for ''short'' (n < 10^5) sequences.');
end

global fLZc_data_path;
cmean = [];
cvar  = [];
cmax  = [];
fname = fullfile(fLZc_data_path,sprintf('LZ%dc_rand_a%02d.mat',ver,a));
try
	load(fname);
catch
	error('\n\t*** No LZ%2dc data found for alphabet size %d ***',ver,a);
end

% loaded:
%
% nmax     : max string length
% asize    : alphabet size
% nsamples : number of samples used for estimates
% rseed    : random seed
% cmean    : means     at each string length 1 ... nmax
% cvar     : variances at each string length 1 ... nmax (if available)
% cmax     : maxima    at each string length 1 ... nmax (if available)

iof = find(n <= nmax); % indices on file
nof = n(iof);          % values on file
numn = length(n);      % sequence lengths

cm = nan(numn,1);
cm(iof) = cmean(nof);
if nargout > 1
	ns = nsamples;
	if nargout > 2
		assert(~isempty(cvar),'Sorry, ''%s'' does not contain variances',fname);
		cv = nan(numn,1);
		cv(iof) = cvar(nof);
		if nargout > 3
			assert(~isempty(cmax),'Sorry, ''%s'' does not contain maxima',fname);
			cx = nan(numn,1);
			cx(iof) = cmax(nof);
		end
	end
end
