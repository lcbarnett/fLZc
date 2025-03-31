function smerged = merge_sequences(s)

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
% Merge multiple sequences (of same length). If the symbols along each row
% represent contemporaneous values, then to obtain a "global" LZc, the
% LZc(...) function should be called on the merged sequence.
%
% INPUT
%
% s        character matrix of symbols; each row represents a sequence.
%
% OUTPUT
%
% smerged  merged sequence; length = (number of rows of s) * (number of columns of s)
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

assert(ischar(s),'Sequence matrix should comprise character strings');

[m,n] = size(s); % (number of sequences) * (length of sequences )

smerged = reshape(s,1,m*n)
