function [c,dict] = LZc_x(s,d,use_mex)

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% Lempl-Ziv complexity - extended version returns complexity at all sequence lengths
%
% INPUT
%
% s           input character string
% d           alphabet size (or empty for pessimistic memory usage)
% use_mex     use c version (default: MUCH faster)
%
% OUTPUT
%
% c      LZ complexity
% dict   The dictionary - cell string (optional)
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

assert(ischar(s) && isvector(s),"Input must be a character string");

if nargin < 2 || isempty(d)
	d = 0;
else
	assert(isscalar(d) && isnumeric(d) && d == floor(d),'Alphabet size must be a scalar integer');
end

if nargin < 3 || isempty(use_mex), use_mex = true; end

if use_mex

	if nargout > 1
		[c,dict] = LZc_mex_x(s,d);
	else
		c = LZc_mex_x(s,d);
	end

else

	dict = containers.Map;      % the dictionary
	nmax = length(s);           % length of string
	c  = zeros(nmax,1);         % the complexities
	cc = 0;                     % current complexity
	w  = [];                    % initialise current word
	for n = 1:nmax              % iterate through input characters
		w = [w s(n)];           % append current character to current word
		if ~isKey(dict,w)       % if current word not already in dictionary...
			dict(w) = true;     % add to dictionary
			w = [];             % and reinitialise
			cc = cc+1;          % increment current complexity
		end
		c(n) = cc;              % set to current
	end
	if nargout > 1
		dict = keys(dict)';     % cell string of dictionary words
	end

end
