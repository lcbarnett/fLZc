function c = LZc_x(s)

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% Lempl-Ziv complexity - extended version returns complexity at all sequence lengths
%
% INPUT
%
% s      input character string% d           alphabet size (or empty for pessimistic memory usage)
%
% OUTPUT
%
% c      vector of LZ complexities at all sequence lengths
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

assert(~isempty(s) && ischar(s) && isvector(s),"Input must be a non-empty character string");

c = LZc_mex_x(s);
