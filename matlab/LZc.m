function [c,dict] = LZc(s,ver,dstatic)

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% Lempl-Ziv complexity
%
% INPUT
%
% s        input character string
% ver      LZc version: 76 or 78
% dstatic  static dictionary? (LZ78c only)
%
% OUTPUT
%
% c        LZ complexity
% dict     the dictionary
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

assert(nargin > 1,"Must supply Lempel-Ziv version (76 or 78)');
assert(~isempty(s) && ischar(s) && isvector(s),"Input must be a non-empty character string");

switch ver

	case 76
		if nargout > 1
			[c,dict] = LZ76c_mex(s);
		else
			c = LZ76c_mex(s);
			dict = [];
		end

	case 78
		if nargin < 3 || isempty(dstatic)
			dstatic = false; % default: use dynamic dictionary
		else
			assert(isscalar(dstatic),'Static/dynamic flag must be a scalar that resolves to logical true/false');
			dstatic = logical(dstatic);
		end
		if nargout > 1
			[c,dict] = LZ78c_mex(s,dstatic);
		else
			c = LZ78c_mex(s,dstatic);
			dict = [];
		end

	otherwise, error('Lempel-Ziv version must be 76 or 78');

end
