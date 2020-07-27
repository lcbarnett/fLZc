function c = LZ76c_x(s,use_mex)

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% Lempl-Ziv complexity - extended version returns complexity at all sequence lengths
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

	c = LZ76c_mex_x(s);

else
	error('sorry, not implemented yet');
%{
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
%}
end
