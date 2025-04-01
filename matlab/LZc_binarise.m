function s = LZc_binarise(x,usemean)

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% Binarise data sequence x (column vector)
%
% s returns the binarised symbol string (0s and 1s)
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

assert(isvector(x),'Input must be a vector');
n = length(x);

if nargin < 2 || isempty(usemean), usemean = false; end % default: binarise around the median

if usemean
	xmid = mean(x);
else
	xmid = median(x);
end

% Binarise

z = zeros(1,length(x)); % row vector
z(x > xmid) = 1;
s = char(48+z); % 0 or 1
