function [cm,cs,ixf,N,cdesc] = LZc_crand(n,d)

% Load random string mean complexities (and optionally standard deviations) for alphabet of size d
% and string lengths n from file of values estimated by Monte Carlo simulation.

if isscalar(n)
	assert(isnumeric(n) && n == floor(n) && n > 0,'Maximum length must be a positive integer');
	n = 1:n;
else
	assert(isvector(n) && isnumeric(n) && all(n == floor(n)) && all(n > 0),'String lengths must be a vector of positive integers');
end
assert(isscalar(d) && isnumeric(d) &&     d == floor(d)  && d > 0 ,    'Alphabet size must be a positive integer');

numn = length(n);

global fLZc_data_path;
load(fullfile(fLZc_data_path,sprintf('LZc_rand_A%02d.mat',d)));

of = (n <= N)';    % on file
iof = find(of);    % indices on file
nof = n(iof);      % values on file

cm = nan(numn,1);
cm(iof) = cmean(nof);
% we don't know the asymptotic means; leave as NaNs

if nargout > 1
	cs = nan(numn,1);
	cs(iof) = csdev(nof);
	% we don't know the asymptotic standard deviations; leave as NaNs
end
