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

end
