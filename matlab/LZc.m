function [c,dict] = LZc(s,ver,allc)

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% Lempl-Ziv complexity
%
% INPUT
%
% s        input character string
% ver      LZc version: 76 or 78
% allc     flag: if true, return a vector of complexities for all substrings starting from
%          the beginning of the input string; else just scalar complexity of whole string
% OUTPUT
%
% c        LZ complexity
% dict     the dictionary (optional)
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

assert(nargin > 1,'Must supply Lempel-Ziv version (76 or 78)');
assert(~isempty(s) && ischar(s) && isvector(s),'Input must be a non-empty character string');

if nargin < 3 || isempty(allc)
	allc = false;
else
	assert(isscalar(allc) && (isnumeric(allc) | islogical(allc)),'''all complexities'' flag must be a scalar that resolves to logical true/false');
	allc = logical(allc);
end

switch ver

	case 76
		if nargout > 1
			[c,dict] = LZ76c_mex(s,allc);
		else
			c = LZ76c_mex(s,allc);
			dict = [];
		end

	case 78
		if nargout > 1
			[c,dict] = LZ78c_mex(s,allc);
		else
			c = LZ78c_mex(s,allc);
			dict = [];
		end

	otherwise, error('Lempel-Ziv version must be 76 or 78');

end
