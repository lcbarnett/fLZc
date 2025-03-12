function c = LZc(s)

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% Lempl-Ziv complexity
%
% INPUT
%
% s      input character string
%
% OUTPUT
%
% c      LZ complexity
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

assert(~isempty(s) && ischar(s) && isvector(s),"Input must be a non-empty character string");

c = LZc_mex(s);
