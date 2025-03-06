function [c,dict] = LZc(s,use_mex)

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% Lempl-Ziv complexity
%
% INPUT
%
% s           input character string
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
		[c,dict] = LZc_mex(s);
	else
		c = LZc_mex(s);
	end

else

	dict = containers.Map;      % the dictionary
	w = [];                     % initialise current word
	for i = s                   % iterate through input characters
		w = [w i];              % append current character to current word
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
