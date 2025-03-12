function [cm,cs,n] = LZc_crand(slen,d)

% Load random string mean complexities (and optionally standard deviations) for alphabet of size d
% and string lengths slen from file of values estimated by Monte Carlo simulation.

if isscalar(slen)
	assert(isnumeric(slen) && slen == floor(slen) && slen > 1,'Maximum length must be a positive');
	slen = 1:slen;
else
	assert(isvector(slen) && isnumeric(slen) && all(slen == floor(slen)) && all(slen > 0),'String lengths must be a vector of positive integers');
end
assert(isscalar(d) && isnumeric(d) && d == floor(d)  && d > 1,'Alphabet size must be an integer > 1');

global fLZc_data_path;
load(fullfile(fLZc_data_path,sprintf('LZc_rand_d%02d.mat',d)));

% loaded:
%
% n     : max string length
% d     : alphabet size
% N     : number of samples used for estimates
% cmean : means               at each string length 1 ... n
% csdev : standard deviations at each string length 1 ... n

iof = find(slen <= n); % indices on file
nof = slen(iof);       % values on file

numn = length(slen);

cm = nan(numn,1);
cm(iof) = cmean(nof);

if nargout > 1
	cs = nan(numn,1);
	cs(iof) = csdev(nof);
end
