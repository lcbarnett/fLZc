function c = LZ76c(s,use_mex)

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% Lempl-Ziv '76 complexity
%
% INPUT
%
% s           input character string
% use_mex     use c version (default: MUCH faster)
%
% OUTPUT
%
% c      LZ complexity
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

assert(ischar(s) && isvector(s),"Input must be a character string");

if nargin < 2 || isempty(use_mex), use_mex = true; end

if use_mex

	c = LZ76c_mex(s);

else
	error('sorry, not implemented yet');
%{
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
%}
end
