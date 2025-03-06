function [c,dict] = LZc_x(s,use_mex)

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% Lempl-Ziv complexity - extended version returns complexity at all sequence lengths
%
% INPUT
%
% s           input character string% d           alphabet size (or empty for pessimistic memory usage)
% use_mex     use c version (default: MUCH faster)
%
% OUTPUT
%
% c      LZ complexity
% dict   The dictionary - cell string (optional)
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

assert(ischar(s) && isvector(s),"Input must be a character string");

if nargin < 2 || isempty(use_mex), use_mex = true; end

if use_mex

	if nargout > 1
		[c,dict] = LZc_mex_x(s);
	else
		c = LZc_mex_x(s);
	end

else

	dict = containers.Map;      % the dictionary
	nmax = length(s);           % length of string
	c  = zeros(nmax,1);         % the complexities
	cc = 0;                     % current complexity
	w  = [];                    % initialise current word
	for i = 1:nmax              % iterate through input characters
		w = [w s(i)];           % append current character to current word
		if ~isKey(dict,w)       % if current word not already in dictionary...
			dict(w) = true;     % add to dictionary
			w = [];             % and reinitialise
			cc = cc+1;          % increment current complexity
		end
		c(i) = cc;              % set to current
	end
	if nargout > 1
		dict = keys(dict)';     % cell string of dictionary words
	end

end
