function [c,dict] = LZc(s,d,use_mex)

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% Lempl-Ziv complexity
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
		[c,dict] = LZc_mex(s,d);
	else
		c = LZc_mex(s,d);
	end

else

	dict = containers.Map;      % the dictionary
	w = [];                     % initialise current word
	for ch = s                  % iterate through input characters
		w = [w ch];             % append current character to current word
		if ~isKey(dict,w)       % if current word not already in dictionary...
			dict(w) = true;     % add to dictionary
			w = [];             % and reinitialise
		end
	end
	c = length(dict);           % LZ complexity is size of dictionary
	if nargout > 1
		dict = keys(dict)';     % cell string of dictionary words
	end

end
