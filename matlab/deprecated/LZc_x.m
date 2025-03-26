function c = LZc_x(s,ver,dstatic)

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% Lempl-Ziv complexity at all sequence lengths
%
% INPUT
%
% s        input character string
% ver      LZc version: 76 or 78
% dstatic  static dictionary? (LZ78c only)
%
% OUTPUT
%
% c        vector of LZ complexities at all sequence lengths along s
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

assert(nargin > 1,'Must supply Lempel-Ziv version (76 or 78)');
assert(~isempty(s) && ischar(s) && isvector(s),"Input must be a non-empty character string");

switch ver

	case 76
		c = LZ76c_x_mex(s);

	case 78
		if nargin < 3 || isempty(dstatic)
			dstatic = false; % default: use dynamic dictionary
		else
			assert(isscalar(dstatic),'Static/dynamic flag must be a scalar that resolves to logical true/false');
			dstatic = logical(dstatic);
		end
		c = LZ78c_x_mex(s,dstatic);

	otherwise, error('Lempel-Ziv version must be 76 or 78');

end
